#pragma once

#include <i3d/image3d.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "ObjectConvertor.h"

using namespace i3d;

static Image3d<float>* LoadFloatImage(const char* pth)
{
	ImgVoxelType vt = ReadImageType(pth);
	Image3d <float> *file = NULL;
	switch (vt)
	{
	case Gray8Voxel:
	{
		Image3d <GRAY8> *tmp = new Image3d <GRAY8>(pth);
		file = new Image3d <float>();
		GrayToFloat(*tmp, *file);
		delete tmp;
		break;
	}
	case Gray16Voxel:
	{
		Image3d <GRAY16> *tmp = new Image3d <GRAY16>(pth);
		file = new Image3d <float>();
		GrayToFloat(*tmp, *file);
		delete tmp;
		break;
	}
	case FloatVoxel:
		file = new Image3d <float>(pth);
		break;
	}

	return file;
}

static Image3d<bool>* LoadBoolImage(const char * pth)
{
	ImgVoxelType vt = ReadImageType(pth);
	Image3d <bool> *file = NULL;
	switch (vt)
	{
	case Gray8Voxel:
	{
		Image3d <GRAY8> *tmp = new Image3d <GRAY8>(pth);
		file = new Image3d <bool>();
		GrayToBinary(*tmp, *file);
		delete tmp;
		break;
	}
	case Gray16Voxel:
	{
		Image3d <GRAY16> *tmp = new Image3d <GRAY16>(pth);
		file = new Image3d <bool>();
		GrayToBinary(*tmp, *file);
		delete tmp;
		break;
	}
	case BinaryVoxel:
		file = new Image3d <bool>(pth);
		break;
	}

	return file;
}

inline int TestRGB_Image3dtoMat()
{
	auto img = new i3d::Image3d<i3d::RGB>("Test-Picture-10x10-Red&Green.png");

	cv::Mat image = image3DToMat(img);
	cv::imwrite("images/Image_RGB.png", image);

	delete img;

	return 0;
}

inline int TestGray8_Image3dtoMat()
{
	auto img = new i3d::Image3d<i3d::GRAY8>("Test-Picture-10x10-GrayScale.png");

	cv::Mat image = image3DToMat(img);
	cv::imwrite("images/Image_Gray8.png", image);

	delete img;

	return 0;
}

inline int TestGray16_Image3dtoMat()
{
	auto img = new i3d::Image3d<i3d::GRAY16>("Test-Picture-10x10-Gray16.png");

	cv::Mat image = image3DToMat(img);
	cv::imwrite("images/Image_Gray16.png", image);
	
	delete img;

	return 0;
}
/*
inline int TestGray16_Image3dtoMat()
{
	auto img = new i3d::Image3d<i3d::GRAY16>("images/Image3D_Gray16.png");
	img->SaveImage("images/Image_Gray16_2.png");

	cv::Mat image;
	image = cv::imread("images/Image3D_Gray16.png", CV_LOAD_IMAGE_ANYDEPTH);
	cv::imwrite("images/Image_Gray16_3.png", image);

	std::cout << image << std::endl;

	for (int i = 0; i < 100; i++)
	{
		std::cout << i << ": " << static_cast<int>(image.data[i]) << std::endl;
	}
	system("PAUSE");


	system("PAUSE");
	delete img;

	return 0;
}*/

/*
inline int TestGray16_Image3dtoMat()
{
	cv::Mat O = cv::Mat::eye(10, 10, CV_16UC1);
	randu(O, cv::Scalar::all(0), cv::Scalar::all(65536));
	randu(O, cv::Scalar::all(0), cv::Scalar::all(65536));

	cv::imwrite("Test-Picture-10x10-Gray16.tiff", O);

	auto img = MatToImage3D_GRAY16(O);

	auto P = image3DToMat(img);

	cv::Mat diff;
	cv::compare(O, P, diff, cv::CMP_NE);
	int nz = cv::countNonZero(diff);

	return nz == 0;
}*/


inline int TestFloat_Image3dtoMat()
{
	auto img = LoadFloatImage("Test-Picture-10x10-GrayScale.png");

	cv::Mat image = image3DToMat(img);
	cv::imwrite("images/Image_float.png", image);

	delete img;

	return 0;
}

inline int TestRGB_MatToImage3d()
{
	cv::Mat image;
	image = cv::imread("Test-Picture-10x10-Red&Green.png", cv::IMREAD_COLOR); // Read the file

	auto img = MatToImage3D_RGB(image);
	img->SaveImage("images/Image3D_RGB.png");

	auto image2 = image3DToMat(img);

	cv::Mat diff;
	cv::compare(image2, image, diff, cv::CMP_NE);
	std::cout << diff << std::endl;


	delete img;
	return 0;
}

inline int TestGray8_MatToImage3d()
{
	cv::Mat image;
	image = cv::imread("Test-Picture-10x10-GrayScale.png", cv::IMREAD_GRAYSCALE); // Read the file

	auto img = MatToImage3D_GRAY8(image);
	img->SaveImage("images/Image3D_Gray8.png");

	auto image2 = image3DToMat(img);

	cv::Mat diff;
	cv::compare(image2, image, diff, cv::CMP_NE);
	std::cout << diff << std::endl;

	system("PAUSE");
	delete img;
	return 0;
}

inline int TestGray16_MatToImage3d()
{
	cv::Mat image;
	image = cv::imread("Test-Picture-10x10-Gray16.png", CV_LOAD_IMAGE_ANYDEPTH); // Read the file

	std::cout << image << std::endl;

	auto img = MatToImage3D_GRAY16(image);
	img->SaveImage("images/Image3D_Gray16.png");

	auto image2 = image3DToMat(img);

	std::cout << image << std::endl;

	cv::Mat diff;
	cv::compare(image2, image, diff, cv::CMP_NE);
	std::cout << diff << std::endl;

	system("PAUSE");
	delete img;
	return 0;
}

inline int TestBool_MatToImage3d()
{
	cv::Mat image;
	image = cv::imread("Test-Picture-10x10-GrayScale.png", CV_LOAD_IMAGE_GRAYSCALE); // Read the file

	auto img = MatToImage3D_BOOL(image);
	img->SaveImage("images/Image3D_Binary.png");

	auto image2 = image3DToMat(img);

	std::cout << image << std::endl;

	cv::Mat diff;
	cv::compare(image2, image, diff, cv::CMP_NE);
	std::cout << diff << std::endl;

	system("PAUSE");
	delete img;
	return 0;
}