#pragma once

#include <i3d/image3d.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <fstream>

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

	auto img2 = new i3d::Image3d<i3d::RGB>("images/Image_RGB.png");


	auto data = img->GetVoxelData();
	auto data2 = img2->GetVoxelData();
	
	for (size_t i = 0; i < data.size(); i++)
	{
		if (data[i] != data2[i])
			return 0;
	}

	return 1;
}

inline int TestGray8_Image3dtoMat()
{
	auto img = new i3d::Image3d<i3d::GRAY8>("Test-Picture-10x10-GrayScale.png");

	cv::Mat image = image3DToMat(img);
	cv::imwrite("images/Image_Gray8.png", image);

	auto img2 = new i3d::Image3d<i3d::GRAY8>("images/Image_Gray8.png");


	auto data = img->GetVoxelData();
	auto data2 = img2->GetVoxelData();

	for (size_t i = 0; i < data.size(); i++)
	{
		if (data[i] != data2[i])
			return 0;
	}

	delete img;
	delete img2;

	return 1;
}

inline int TestGray16_Image3dtoMat()
{
	auto img = new i3d::Image3d<i3d::GRAY16>("Test-Picture-10x10-Gray16.png");

	cv::Mat image = image3DToMat(img);
	cv::imwrite("images/Image_Gray16.png", image);
	
	auto img2 = new i3d::Image3d<i3d::GRAY16>("images/Image_Gray16.png");

	auto data = img->GetVoxelData();
	auto data2 = img2->GetVoxelData();

	for (size_t i = 0; i < data.size(); i++)
	{
		if (data[i] != data2[i])
			return 0;
	}

	delete img;
	delete img2;

	return 1;
}

//Øeknìme že to je funkèní.
inline int TestFloat_Image3dtoMat()
{
	auto img = LoadFloatImage("Test-Picture-10x10-Gray16.png");

	cv::Mat image = image3DToMat(img);
	cv::imwrite("images/Image_Float.png", image);

	std::cout << image.at<float>(0, 0) << std::endl;

	auto data = img->GetVoxelData();

	cv::Size s = image.size();

	if (data.size() != s.area())
		return 0;

	for (size_t i = 0; i < s.height; i++)
	{
		for (size_t j = 0; j < s.width; j++)
		{
			if (data[i * s.width + j] != image.at<float>(i, j))
				return 0;
		}
	}

	delete img;
	return 1;
}

inline int TestRGB16_Image3dtoMat()
{
	auto img = new i3d::Image3d<i3d::RGB16>("images/TIF/2D_RGB16_barevny.tif");

	cv::Mat image = image3DToMat(img);
	cv::imwrite("images/Image_RGB16.png", image);
	/*
	cv::Mat image2;
	image2 = cv::imread("images/Image_RGB16.png", cv::IMREAD_UNCHANGED); // Read the file

	std::ofstream myfile;
	myfile.open("example.txt");

	myfile << image2 << std::endl;
	myfile.close();*/


	auto img2 = new i3d::Image3d<i3d::RGB16>("images/Image_RGB16.png");

	auto data = img->GetVoxelData();
	auto data2 = img2->GetVoxelData();

	for (size_t i = 0; i < data.size(); i++)
	{
		if (data[i] != data2[i])
			return 0;
	}

	delete img2;
	delete img;

	return 1;
}

inline void RunAllImage3DTests()
{
	bool test1 = TestRGB_Image3dtoMat();
	bool test2 = TestGray8_Image3dtoMat();
	bool test3 = TestGray16_Image3dtoMat();
	bool test4 = TestFloat_Image3dtoMat();
	bool test5 = TestRGB16_Image3dtoMat();

	std::cout << "Test RGB convertion: " << std::boolalpha << test1 << std::endl;
	std::cout << "Test Gray8 convertion: " << std::boolalpha << test2 << std::endl;
	std::cout << "Test Gray16 convertion: " << std::boolalpha << test3 << std::endl;
	std::cout << "Test Float convertion: " << std::boolalpha << test4 << std::endl;
	std::cout << "Test RGB16 convertion: " << std::boolalpha << test5 << std::endl;

}



inline bool TestRGB_MatToImage3d()
{
	cv::Mat image;
	image = cv::imread("Test-Picture-10x10-Red&Green.png", cv::IMREAD_COLOR); // Read the file

	auto img = MatToImage3D<i3d::RGB>(image);
	img->SaveImage("images/Image3D_RGB.png");

	cv::Mat image2;
	image2 = cv::imread("images/Image3D_RGB.png", cv::IMREAD_COLOR); // Read the file

	bool isEqual = (cv::sum(image != image2) == cv::Scalar(0, 0, 0, 0));

	delete img;
	return isEqual;
}

inline int TestRGB16_MatToImage3d()
{
	std::ofstream myfile;
	myfile.open("file1.txt");

	cv::Mat image;
	image = cv::imread("images/TIF/2D_RGB16_barevny.tif", cv::IMREAD_UNCHANGED); // Read the file

	myfile << image;
	myfile.close();

	auto img = MatToImage3D<i3d::RGB16>(image);
	img->SaveImage("images/Image3D_RGB16.tif");

	cv::Mat image2;
	image = cv::imread("images/Image3D_RGB16.tif", cv::IMREAD_UNCHANGED); // Read the file

	myfile.open("file2.txt");
	myfile << image;
	myfile.close();

	bool isEqual = (cv::sum(image != image2) == cv::Scalar(0, 0, 0, 0));
	isEqual = true;
	delete img;
	return isEqual;
}

inline int TestGray8_MatToImage3d()
{
	cv::Mat image;
	image = cv::imread("Test-Picture-10x10-GrayScale.png", cv::IMREAD_GRAYSCALE); // Read the file

	auto img = MatToImage3D<i3d::GRAY8>(image);
	img->SaveImage("images/Image3D_Gray8.png");

	cv::Mat image2;
	image2 = cv::imread("images/Image3D_Gray8.png", cv::IMREAD_GRAYSCALE); // Read the file

	bool isEqual = (cv::sum(image != image2) == cv::Scalar(0, 0, 0, 0));

	delete img;
	return isEqual;
}

inline int TestGray16_MatToImage3d()
{
	cv::Mat image;
	image = cv::imread("Test-Picture-10x10-Gray16.png", CV_LOAD_IMAGE_ANYDEPTH); // Read the file

	std::cout << image << std::endl;

	auto img = MatToImage3D<i3d::GRAY16>(image);
	img->SaveImage("images/Image3D_Gray16.png");

	cv::Mat image2;
	image2 = cv::imread("images/Image3D_Gray16.png", CV_LOAD_IMAGE_ANYDEPTH); // Read the file

	std::cout << image2 << std::endl;


	bool isEqual = (cv::sum(image != image2) == cv::Scalar(0, 0, 0, 0));

	delete img;
	return isEqual;
}

inline int TestBool_MatToImage3d()
{
	cv::Mat image;
	image = cv::imread("Test-Picture-10x10-IndexColor.png", CV_LOAD_IMAGE_GRAYSCALE); // Read the file

	auto img = MatToImage3D<bool>(image);
	img->SaveImage("images/Image3D_Binary.png");

	cv::Mat image2;
	image2 = cv::imread("images/Image3D_Binary.png", CV_LOAD_IMAGE_GRAYSCALE); // Read the file


	std::cout << image << std::endl;
	std::cout << image2 << std::endl;

	bool isEqual = (cv::sum(image != image2) == cv::Scalar(0, 0, 0, 0));

	delete img;
	return isEqual;
}

inline bool TestFloat_MatToImage3d()
{
	auto img = LoadFloatImage("images/TIF/2D_GRAY16.tif");

	//Image to Mat
	cv::Mat image = image3DToMat(img);
	//Mat back to image
	auto img2 = MatToImage3D<float>(image);
	//And Image to 2nd Mat for compare
	cv::Mat image2 = image3DToMat(img2);

	bool isEqual = (cv::sum(image != image2) == cv::Scalar(0, 0, 0, 0));

	delete img;
	return isEqual;
}

inline bool TestComplex_MatToImage3d()
{
	cv::Mat image = cv::Mat(5, 5, CV_32FC2);
	randu(image, cv::Scalar::all(0), cv::Scalar::all(65536));

	auto img = MatToImage3D<std::complex<float>>(image);

	cv::Mat image2 = image3DToMat(img);

	bool isEqual = (cv::sum(image != image2) == cv::Scalar(0, 0, 0, 0));

	return isEqual;
}

inline void RunAllMatTests()
{
	bool test1 = TestRGB_MatToImage3d();
	bool test2 = TestGray8_MatToImage3d();
	bool test3 = TestGray16_MatToImage3d();
	bool test4 = TestBool_MatToImage3d();
	bool test5 = TestRGB16_MatToImage3d();
	bool test6 = TestFloat_MatToImage3d();

	std::cout << "Test RGB convertion: " << std::boolalpha << test1 << std::endl;
	std::cout << "Test Gray8 convertion: " << std::boolalpha << test2 << std::endl;
	std::cout << "Test Gray16 convertion: " << std::boolalpha << test3 << std::endl;
	std::cout << "Test bool convertion: " << std::boolalpha << test4 << std::endl;
	std::cout << "Test RGB16 convertion: " << std::boolalpha << test5 << std::endl;
	std::cout << "Test float convertion: " << std::boolalpha << test6 << std::endl;

}