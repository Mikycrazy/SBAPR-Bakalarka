#include <iostream>
#include <i3d/image3d.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "ObjectConvertor.h";

using namespace cv;

unsigned char* getUnsignedCharArray(i3d::Image3d<i3d::RGB> img)
{
	std::valarray<i3d::RGB> data = img.GetVoxelData();
	unsigned char *array = new unsigned char(data.size() * 3);


	for (size_t i = 0; i < data.size(); i++)
	{
		array[i * 3] = data[i].red;
		array[i * 3 + 1] = data[i].green;
		array[i * 3 + 2] = data[i].blue;
	}

	return array;
}

Mat convertImage3DToMat(i3d::Image3d<i3d::RGB> img)
{
	auto array = getUnsignedCharArray(img);

	return 	Mat(img.GetSizeX(), img.GetSizeY(), CV_8UC3, array).clone();
}

int main()
{
	i3d::Image3d<i3d::RGB16> img("Test-Picture-10x10.png");
	Mat image;
	image = imread("Test-Picture-10x10.png", IMREAD_COLOR); // Read the file

	i3d::Image3d<i3d::RGB>* img2(MatToImage3D<i3d::RGB>(image));


	Mat image2;


	int a;

	/*
	for (int i = 0; i < 100; i++)
	{
		std::cout << i << ": " << static_cast<int>(input[i]) << std::endl;
	}
	system("PAUSE");
	
	
	int i, j, r, g, b;
	for (int i = 0; i < image.rows; i++){
		for (int j = 0; j < image.cols; j++){
			b = input[image.step * i + j * 3];
			g = input[image.step * i + j * 3 + 1];
			r = input[image.step * i + j * 3 + 2];

			std::cout << r << ", " << g << ", " << b << std::endl;
			
		}
		system("PAUSE");
	}*/



	namedWindow("Display window", WINDOW_AUTOSIZE); // Create a window for display.
	imshow("Display window", image2); // Show our image inside it.

	waitKey(0); // Wait for a keystroke in the window

	return 0;
}