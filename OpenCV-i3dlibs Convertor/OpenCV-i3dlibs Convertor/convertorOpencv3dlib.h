#pragma once

#include <i3d/image3d.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

inline cv::Mat image3DToMat(const i3d::Image3d<i3d::RGB> img)
{
	//Convertion data to uchar*
	auto data = img.GetVoxelData();
	auto array = new unsigned char(data.size() * 3);


	for (size_t i = 0; i < data.size(); i++)
	{
		array[i * 3] = data[i].red;
		array[i * 3 + 1] = data[i].green;
		array[i * 3 + 2] = data[i].blue;
	}

	//Returning new Mat object with data from img
	return cv::Mat(img.GetSizeX(), img.GetSizeY(), CV_8UC3, array).clone();
}

inline cv::Mat image3DToMat(const i3d::Image3d<i3d::GRAY8> img)
{
	//Convertion data to uchar*
	auto data = img.GetVoxelData();
	auto array = new unsigned char(data.size());

	//Returning new Mat object with data from img
	return cv::Mat(img.GetSizeX(), img.GetSizeY(), CV_8UC1, array).clone();
}

inline cv::Mat image3DToMat(const i3d::Image3d<i3d::GRAY16> img)
{
	//Convertion data to uchar*
	auto data = img.GetVoxelData();
	auto array = new unsigned char(data.size());

	//Returning new Mat object with data from img
	return cv::Mat(img.GetSizeX(), img.GetSizeY(), CV_16UC1, array).clone();
}

inline cv::Mat image3DToMat(const i3d::Image3d<i3d::RGB16> img)
{
	//Convertion data to uchar*
	auto data = img.GetVoxelData();
	auto array = new unsigned char(data.size());

	//Returning new Mat object with data from img
	return cv::Mat(img.GetSizeX(), img.GetSizeY(), CV_16UC3, array).clone();
}

