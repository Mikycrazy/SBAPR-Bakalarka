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
	//Convertion data to uchar array
	auto data = img.GetVoxelData();
	auto array = new unsigned char(data.size());

	for (size_t i = 0; i < data.size(); i++)
	{
		array[i] = data[i];
	}

	//Returning new Mat object with data from img
	return cv::Mat(img.GetSizeX(), img.GetSizeY(), CV_8UC1, array).clone();
}

inline cv::Mat image3DToMat(const i3d::Image3d<i3d::GRAY16> img)
{
	//Convertion data to ushort array
	auto data = img.GetVoxelData();
	auto array = new unsigned short(data.size());

	for (size_t i = 0; i < data.size(); i++)
	{
		array[i] = data[i];
	}

	//Returning new Mat object with data from img
	return cv::Mat(img.GetSizeX(), img.GetSizeY(), CV_16UC1, array).clone();
}
//TO DO
inline cv::Mat image3DToMat(const i3d::Image3d<i3d::RGB16> img)
{
	//Convertion data to uchar*
	auto data = img.GetVoxelData();
	auto array = new unsigned char(data.size());

	//Returning new Mat object with data from img
	return cv::Mat(img.GetSizeX(), img.GetSizeY(), CV_16UC3, array).clone();
}
//TODO
template<typename VOXEL>
inline i3d::Image3d<VOXEL> MatToImage3D(const cv::Mat img)
{

	switch (img.type())
	{
		case CV_8UC3:
		{
			cv::Size s = img.size();
			auto data = static_cast<unsigned char*>(img.data);
			auto array = new std::valarray<i3d::RGB>(s.area() / 3);

			for (size_t i = 0; i < array->size(); i++)
			{
				array[i] = new i3d::RGB(data[i * 3], data[i * 3 + 1], data[i * 3 + 2]);
			}

			i3d::Image3d<i3d::RGB> image;
			image.MakeRoom(s.width, s.height, 1);

			for (size_t i = 0; i < s.area(); i++)
			{
				image.SetVoxel(i, array[i]);
			}

			return image;
		}
		case CV_8UC1:
		{
			cv::Size s = img.size();
			i3d::Image3d<i3d::GRAY8> image;
			image.MakeRoom(s.width, s.height, 1);
			auto data = static_cast<unsigned char*>(img.data);

			for (size_t i = 0; i < s.area(); i++)
			{
				image.SetVoxel(i, data[i]);
			}

			return image;
		}
		default:
		{
			std::cout << "cv::Mat image type not handled in switch:" << img.type() << std::endl;
			return nullptr;
		}

	}
}