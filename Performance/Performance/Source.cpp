#include <i3d/image3d.h>
#include <i3d/filters.h>
#include <i3d/metrics.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui_c.h>

#include <iostream>
#include <fstream>
#include <vector>

#include "myTime.h"
#include "ObjectConvertor.h"
#include "myGaussFIR.h"

#define INTERATION 100

using namespace cv;
using namespace i3d;



const int COUNT = 4;

double runOpenCV(Mat& src, Mat& dst, float sigma, int iterations = INTERATION);
double runOpenCV2(Mat& src, Mat& dst, float sigma, int iterations = INTERATION);

double runi3DlibsTest1(Image3d<float> &src, Image3d<float> &out, float sigma, int iterations = INTERATION);
double runi3DlibsTest2(Image3d<float> & img, float sigma, int iterations = INTERATION);

int main(int argc, char** argv)
{
	const float SIGMAS[] = { 0.5, 1, 1.5, 2 };

	std::vector<std::string> list;
	std::ifstream in_stream;
	std::string line;
	in_stream.open(argv[1]);

	while (!in_stream.eof())
	{
		in_stream >> line;
		list.push_back(line);
	}
	in_stream.close();

	std::ofstream cvfile;
	cvfile.open("cv_results.csv");
	cvfile << "Knihovna;Cesta;Sigma;filter2D;GaussianBlur" << std::endl;
	std::ofstream i3dfile;
	i3dfile.open("i3d_results.csv");
	i3dfile << "Knihovna;Cesta;Sigma;GaussFIR;GaussIIR" << std::endl;

	std::ofstream files;
	files.open("../Images/outputFilesList.txt");

	for (auto iter = list.begin(); iter != list.end(); iter++)
	{
		i3d::Image3d<float>* img = new i3d::Image3d<float>();
		img->ReadImage((*iter).c_str());
		i3d::Image3d<float> img2, img3;
		//std::cout << "Picture" << (*iter) << "  resolution: " << img.GetSize().x << "x" << img.GetSize().y << std::endl;

		img2 = *img;
		img3 = *img;

		Mat image, dst, dst2;
		image = image3DToMat(img);
		/*
		Image3d<float>* img_in;
		img_in = MatToImage3D<float>(image);
		img_in->SaveImage("cv_in.ics");*/

		auto size = image.size();

		if (!image.data)
		{
			std::cout << "Could not open or find the image" << std::endl;
			return -1;
		}

		std::cout << "Picture" << (*iter) << "  resolution: " << image.size().width << "x" << image.size().height << std::endl;


		for (size_t i = 0; i < COUNT; i++)
		{

			std::cout << "Sigma " << SIGMAS[i] << " :" << std::endl;

			double time_cv_1, time_cv_2, time_3d_1, time_3d_2;

			time_3d_1 = runi3DlibsTest1(*img, img2, SIGMAS[i]);
			time_3d_2 = runi3DlibsTest2(img3, SIGMAS[i]);

			time_cv_1 = runOpenCV(image, dst, SIGMAS[i]);
			time_cv_2 = runOpenCV2(image, dst2, SIGMAS[i]);

			std::cout << "i3dLibs GaussFIR	: " << time_3d_1 << " ms" << std::endl;
			std::cout << "i3dLibs GaussIIR	: " << time_3d_2 << " ms" << std::endl;
			std::cout << "OpenCV filter2D		: " << time_cv_1 << " ms" << std::endl;
			std::cout << "OpenCV GaussianBlur	: " << time_cv_2 << " ms" << std::endl;


			cvfile << "opencv;" << *iter << ";" << SIGMAS[i] << ";" << time_cv_1 << ";" << time_cv_2 << std::endl;
			i3dfile << "i3dlib;" << *iter << ";" << SIGMAS[i] << ";" << time_3d_1 << ";" << time_3d_2 << std::endl;
			
			
			for (size_t i = 0; i < size.width; i++)
			{
				std::cout << image.at<float>(0, i) << " x " << dst.at<float>(0, i) << " x " << dst2.at<float>(0, i) << std::endl;
				std::cout << img->GetVoxel(0, i, 0) << " x " << img2.GetVoxel(0, i, 0) << " x " << img3.GetVoxel(0, i, 0) << std::endl;
			}
			


			std::string str = (*iter).substr((*iter).find_last_of('/'));

			std::string str_cv = "../Images/out/opencv/";
			str_cv.append(std::to_string(SIGMAS[i]), 0, 3);
			str_cv.append(str);

			std::string str_i3d = "../Images/out/i3dlib/";
			str_i3d.append(std::to_string(SIGMAS[i]), 0, 3);
			str_i3d.append(str);

			files << str_cv << std::endl;
			files << str_i3d << std::endl;

			Image3d<float>* img_out;
			img_out = MatToImage3D<float>(dst);

			img2.SaveImage(str_i3d.c_str());
			img_out->SaveImage(str_cv.c_str());
			
		}

	}

	cvfile.close();
	i3dfile.close();
	files.close();
}


double runOpenCV(Mat& src, Mat& dst, float sigma, int iterations)
{
	Mat dummy;

	timeval t1, t2;

	Mat kernel;
	Point anchor;
	double delta;
	int ddepth;
	int kernel_size;

	/// Initialize arguments for the filter
	anchor = Point(-1, -1);
	delta = 0;
	ddepth = -1;
	kernel_size = sigma * 6;
	kernel_size |= 1;

	kernel = getGaussianKernel(kernel_size, sigma);

	filter2D(src, dummy, ddepth, kernel, anchor, delta, BORDER_REPLICATE);

	gettimeofday(&t1, NULL);
	for (int i = 0; i < iterations; i++)
	{
		/// Apply filter
		filter2D(src, dst, ddepth, kernel, anchor, delta, BORDER_REPLICATE);
	}
	gettimeofday(&t2, NULL);

	double d = TimeDiff(t1, t2);

	return d;
}

double runOpenCV2(Mat& src, Mat& dst, float sigma, int iterations)
{
	// Create a destination Mat object
	Mat dummy;

	timeval t1, t2;

	GaussianBlur(src, dummy, Size(3, 3), sigma, 0);

	gettimeofday(&t1, NULL);
	for (int i = 0; i < iterations; i++) {
		GaussianBlur(src, dst, Size(0, 0), sigma, 0);
	}
	gettimeofday(&t2, NULL);

	double d = TimeDiff(t1, t2);

	return d;
}


double runi3DlibsTest1(Image3d<float> &src, Image3d<float> &out, float sigma, int iterations)
{

	Image3d<float> img_dummy = src;
	
	timeval t1, t2;

	GaussFIR<float>(src, img_dummy, sigma);

	gettimeofday(&t1, NULL);
	for (int i = 0; i < iterations; i++)
	{
		GaussFIR_repeatBoundary<float>(src, out, sigma, 3);
	}
	gettimeofday(&t2, NULL);

	double d = TimeDiff(t1, t2);

	return d;
}

double runi3DlibsTest2(Image3d<float> &img, float sigma, int iterations)
{

	Image3d<float> img_dummy = img;

	timeval t1, t2;

	GaussIIR<float>(img_dummy, sigma);

	gettimeofday(&t1, NULL);
	for (int i = 0; i < iterations; i++)
	{
		GaussIIR<float>(img, sigma);
	}
	gettimeofday(&t2, NULL);

	double d = TimeDiff(t1, t2);

	return d;
}