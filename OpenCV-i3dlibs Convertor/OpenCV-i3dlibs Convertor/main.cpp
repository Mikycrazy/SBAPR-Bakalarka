#include <iostream>
#include <i3d/image3d.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "ObjectConvertor.h"
#include "Tests.h"

using namespace cv;
using  namespace i3d;

/**
@mainpage

	Convertor Image3D and Mat 

	Library i3dlibs and OpenCV provides a representation of 1D and 2D images. <br/>
	For Eazy work with them at same time sometimes is needed convertion between them. <br/>
	And that is pourpous of this module for i3dlib.

	This module converts images created in OpenCv and converts them into Image3D and vice versa.

	For this is here created one header file \ref ObjectConvertor.h.

*/

#ifndef DOXYGEN_SHOULD_SKIP_THIS
int main()
{
	RunAllImage3DTests();
	//RunAllMatTests();

	//TestComplex_MatToImage3d();

	/*
	auto im4 = LoadBoolImage("Test-Picture-10x10.png");

	Mat image4 = image3DToMat(im4);

	auto img5 = MatToImage3D_BOOL(image4);


	Mat image;
	image = imread("Test-Picture-10x10-Red&Green.png", IMREAD_COLOR); // Read the file

	Mat O = Mat::eye(10, 10, CV_16UC3);
	randu(O, Scalar::all(0), Scalar::all(65536));

	//i3d::Image3d<i3d::RGB16>* img = new i3d::Image3d<i3d::RGB16>("RGB16picture.png");

	//i3d::Image3d<i3d::RGB>* img2(MatToImage3D_RGB(image));
	i3d::Image3d<i3d::RGB16>* img3(MatToImage3D_RGB16(O));
	img3->SaveImage("Test-Picture-10x10_3.png");
	

	auto data = static_cast<unsigned char*>(image.data);*/

	
	/*for (int i = 0; i < 100; i++)
	{
		std::cout << i << ": " << static_cast<int>(data[i]) << std::endl;
	}
	system("PAUSE");*/
	
	/*
	int i, j, r, g, b;
	for (int i = 0; i < image.rows; i++){
		for (int j = 0; j < image.cols; j++){
			b = data[image.step * i + j * 3];
			g = data[image.step * i + j * 3 + 1];
			r = data[image.step * i + j * 3 + 2];

			std::cout << r << ", " << g << ", " << b << std::endl;
			std::cout << (int)img2->GetVoxelData()[i * image.rows + j].red << ", " << (int)img2->GetVoxelData()[i * image.rows + j].green << ", " << (int)img2->GetVoxelData()[i * image.rows + j].blue << std::endl;
			std::cout << (int)img->GetVoxelData()[i * image.rows + j].red << ", " << (int)img->GetVoxelData()[i * image.rows + j].green << ", " << (int)img->GetVoxelData()[i * image.rows + j].blue << std::endl;

		}
		system("PAUSE");
	}*/



	/*namedWindow("Display window", WINDOW_AUTOSIZE); // Create a window for display.
	imshow("Display window", image); // Show our image inside it.

	waitKey(0); // Wait for a keystroke in the window*/

	return 0;
}

#endif
