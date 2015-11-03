#pragma once

#include <i3d/image3d.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

std::string getImageType(int number)
{
	// find type
	int imgTypeInt = number % 8;
	std::string imgTypeString;

	switch (imgTypeInt)
	{
	case 0:
		imgTypeString = "8U";
		break;
	case 1:
		imgTypeString = "8S";
		break;
	case 2:
		imgTypeString = "16U";
		break;
	case 3:
		imgTypeString = "16S";
		break;
	case 4:
		imgTypeString = "32S";
		break;
	case 5:
		imgTypeString = "32F";
		break;
	case 6:
		imgTypeString = "64F";
		break;
	default:
		break;
	}

	// find channel
	int channel = (number / 8) + 1;

	std::stringstream type;
	type << "CV_" << imgTypeString << "C" << channel;

	return type.str();
}

//Funkèní
inline cv::Mat image3DToMat(const i3d::Image3d<i3d::RGB>* img)
{
	//Convertion data from image3d to uchar*
	auto data = img->GetVoxelData();
	auto array = new unsigned char[data.size() * 3];


	for (size_t i = 0; i < data.size(); i++)
	{
		array[i * 3] = data[i].blue;
		array[i * 3 + 1] = data[i].green;
		array[i * 3 + 2] = data[i].red;
	}

	//Returning new Mat object with data from img
	return cv::Mat(img->GetSizeX(), img->GetSizeY(), CV_8UC3, array).clone();
}

//Funkèní
inline cv::Mat image3DToMat(const i3d::Image3d<i3d::GRAY8>* img)
{
	//Convertion data to uchar array
	auto data = img->GetVoxelData();
	auto array = new unsigned char[data.size()];

	for (size_t i = 0; i < data.size(); i++)
	{
		array[i] = data[i];
	}

	//Returning new Mat object with data from img
	return cv::Mat(img->GetSizeX(), img->GetSizeY(), CV_8UC1, array).clone();
}

//Funkèní
inline cv::Mat image3DToMat(const i3d::Image3d<i3d::GRAY16>* img)
{
	//Convertion data to ushort array
	auto data = img->GetVoxelData();
	int a = data.size();
	auto array = new unsigned short[a];

	for (size_t i = 0; i < data.size(); i++)
	{
		//Swap Bites because each library works with them diferetly
		unsigned short tmp = data[i];
		unsigned short val = (tmp << 8) | (tmp >> 8);
		array[i] = val;
	}

	//Returning new Mat object with data from img
	return cv::Mat(img->GetSizeX(), img->GetSizeY(), CV_16UC1, array).clone();
}

//Funkèní
inline cv::Mat image3DToMat(const i3d::Image3d<i3d::RGB16>* img)
{
	//Convertion data to uchar*
	auto data = img->GetVoxelData();
	auto array = new unsigned short[data.size() * 3];


	for (size_t i = 0; i < data.size(); i++)
	{
		unsigned short tmp_b = data[i].blue;
		unsigned short tmp_g = data[i].green;
		unsigned short tmp_r = data[i].red;

		unsigned short val_b = (tmp_b << 8) | (tmp_b >> 8);
		unsigned short val_g = (tmp_g << 8) | (tmp_g >> 8);
		unsigned short val_r = (tmp_r << 8) | (tmp_r >> 8);

		array[i * 3] = val_b;
		array[i * 3 + 1] = val_g;
		array[i * 3 + 2] = val_r;
	}

	//Returning new Mat object with data from img
	return cv::Mat(img->GetSizeX(), img->GetSizeY(), CV_16UC3, array).clone();
}

//Funkèní
inline cv::Mat image3DToMat(const i3d::Image3d<float>* img)
{
	//Convertion data to ushort array
	auto data = img->GetVoxelData();
	auto array = new float[data.size()];

	for (size_t i = 0; i < data.size(); i++)
	{
		array[i] = data[i];
	}

	//Returning new Mat object with data from img
	return cv::Mat(img->GetSizeX(), img->GetSizeY(), CV_32FC1, array).clone();
}

//Funkèní
inline cv::Mat image3DToMat(const i3d::Image3d<bool>* img)
{
	//Convertion data to ushort array
	auto data = img->GetVoxelData();
	auto array = new bool[data.size()];

	for (size_t i = 0; i < data.size(); i++)
	{
		array[i] = data[i];
	}

	//Returning new Mat object with data from img
	return cv::Mat(img->GetSizeX(), img->GetSizeY(), CV_8UC1, array).clone();
}


inline cv::Mat image3DToMat(const i3d::Image3d<std::complex<float>>* img)
{
	//Convertion data to ushort array
	auto data = img->GetVoxelData();
	auto array = new float[data.size() * 2];

	for (size_t i = 0; i < data.size(); i++)
	{
		array[i * 2] = data[i].real();
		array[i * 2 + 1] = data[i].imag();
	}

	//Returning new Mat object with data from img
	return cv::Mat(img->GetSizeX(), img->GetSizeY(), CV_32FC2, array).clone();
}


template <typename VOXEL>
inline i3d::Image3d<VOXEL>* MatToImage3D(const cv::Mat img)
{
	std::cout << "Unknown Type of Convertion. Library only support convertion to: RBG, GRAY8, RGB16, GRAY16, FLOAT, BOOL, STD::COMPLEX<FLOAT>";
	return nullptr;
}

//Funkèní
template <>
inline i3d::Image3d<i3d::RGB>* MatToImage3D<i3d::RGB>(const cv::Mat img)
{
	if (img.type() == CV_8UC3)
	{
		cv::Size s = img.size();
		//auto data = static_cast<unsigned char*>(img.data);
		auto array = new std::vector<i3d::RGB>();

		for (int i = 0; i < img.rows; i++)
		{
			for (int j = 0; j < img.cols; j++)
			{
				unsigned char blue = img.at<unsigned char>(i, j * 3);
				unsigned char green = img.at<unsigned char>(i, j * 3 + 1);
				unsigned char red = img.at<unsigned char>(i, j * 3 + 2);
			
				/*int b = data[img.step * i + j * 3];
				int g = data[img.step * i + j * 3 + 1];
				int r = data[img.step * i + j * 3 + 2];*/

				array->push_back(i3d::RGB(red, green, blue));
			}
		}

		i3d::Image3d<i3d::RGB>* image = new i3d::Image3d<i3d::RGB>();
		image->MakeRoom(s.width, s.height, 1);

		i3d::RGB* p = image->GetFirstVoxelAddr();
		for (size_t i = 0; i < s.area(); i++)
		{
			*p = array->at(i);
			p++;
		}
		return image;
	}
	else
	{
		std::cout << "cv::Mat image type " << getImageType(img.type()) << " with this type of convertion" << std::endl;
		std::cout << "RBG type of convertion only works with CV_8UC3 type of images" << std::endl;
		return nullptr;
	}
}

//Funkèní
template <>
inline i3d::Image3d<i3d::GRAY8>* MatToImage3D<i3d::GRAY8>(const cv::Mat img)
{
	if (img.type() == CV_8UC1)
	{
		cv::Size s = img.size();
		auto image = new i3d::Image3d<i3d::GRAY8>();
		image->MakeRoom(s.width, s.height, 1);

		i3d::GRAY8* p = image->GetFirstVoxelAddr();
		for (size_t i = 0; i < s.area(); i++)
		{
			*p = img.data[i];
			p++;
		}
		return image;
	}
	else
	{
		std::cout << "cv::Mat image type " << getImageType(img.type()) << " with this type of convertion" << std::endl;
		std::cout << "Gray8 type of convertion only works with CV_8UC1 type of Mat images" << std::endl;
		return nullptr;
	}
}

//Funkèní
template <>
inline i3d::Image3d<i3d::GRAY16>* MatToImage3D<i3d::GRAY16>(const cv::Mat img)
{
	if (img.type() == CV_16UC1)
	{
		cv::Size s = img.size();
		i3d::Image3d<i3d::GRAY16>* image = new i3d::Image3d<i3d::GRAY16>();
		image->MakeRoom(s.width, s.height, 1);
		auto data = reinterpret_cast<unsigned short*>(img.data);

		i3d::GRAY16* p = image->GetFirstVoxelAddr();
		for (size_t i = 0; i < s.area(); i++)
		{
			unsigned short a = data[i];
			unsigned short val = (a << 8) | (a >> 8);

			*p = val;
			p++;
		}

		return image;
	}
	else
	{
		std::cout << "cv::Mat image type " << getImageType(img.type()) << " with this type of convertion" << std::endl;
		std::cout << "Gray16 type of convertion only works with CV_16UC1 type of Mat images" << std::endl;
		return nullptr;
	}
}

//Zeptat se na RGB16 obrazky
template <>
inline i3d::Image3d<i3d::RGB16>* MatToImage3D<i3d::RGB16>(const cv::Mat img)
{
	if (img.type() == CV_16UC3)
	{
		cv::Size s = img.size();
		i3d::Image3d<i3d::RGB16>* image = new i3d::Image3d<i3d::RGB16>();
		image->MakeRoom(s.width, s.height, 1);
		auto data = reinterpret_cast<unsigned short*>(img.data);

		auto array = new std::vector<i3d::RGB16>();

		for (int i = 0; i < img.rows; i++)
		{
			for (int j = 0; j < img.cols; j++)
			{

				unsigned short b = data[(img.step / 2) * i + j * 3];
				unsigned short g = data[(img.step / 2) * i + j * 3 + 1];
				unsigned short r = data[(img.step / 2) * i + j * 3 + 2];

				unsigned short new_b = (b << 8) | (b >> 8);
				unsigned short new_g = (g << 8) | (g >> 8);
				unsigned short new_r = (r << 8) | (r >> 8);

				array->push_back(i3d::RGB16(r, g, b));
			}
		}


		i3d::RGB16* p = image->GetFirstVoxelAddr();
		for (size_t i = 0; i < s.area(); i++)
		{
			*p = array->at(i);
			p++;
			//image->SetVoxel(i, array->at(i));
		}

		return image;
	}
	else
	{
		std::cout << "cv::Mat image type " << getImageType(img.type()) << " with this type of convertion" << std::endl;
		std::cout << "RBG16 type of convertion only works with CV_16UC3 type of Mat images" << std::endl;
		return nullptr;
	}
}

//Funkèní
template <>
inline i3d::Image3d<float>* MatToImage3D<float>(const cv::Mat img)
{
	if (img.type() == CV_32FC1)
	{
		cv::Size s = img.size();
		auto image = new i3d::Image3d<float>();
		image->MakeRoom(s.width, s.height, 1);
		auto data = reinterpret_cast<float*>(img.data);

		float* p = image->GetFirstVoxelAddr();
		for (size_t i = 0; i < s.area(); i++)
		{
			*p = data[i];
			p++;
		}

		return image;
	}
	else
	{
		std::cout << "cv::Mat image type " << getImageType(img.type()) << " with this type of convertion" << std::endl;
		std::cout << "Float type of convertion only works with CV_32FC1 type of Mat images" << std::endl;
		return nullptr;
	}
}

//Funkèní
template <>
inline i3d::Image3d<bool>* MatToImage3D<bool>(const cv::Mat img)
{
	if (img.type() == CV_8UC1)
	{
		cv::Size s = img.size();
		auto image = new i3d::Image3d<bool>();
		image->MakeRoom(s.width, s.height, 1);
		auto data = reinterpret_cast<bool*>(img.data);

		bool* p = image->GetFirstVoxelAddr();
		for (size_t i = 0; i < s.area(); i++)
		{
			*p = data[i];
			p++;
		}

		return image;
	}
	else
	{
		std::cout << "cv::Mat image type " << getImageType(img.type()) << " with this type of convertion" << std::endl;
		std::cout << "Bool type of convertion only works with CV_8UC1 type of Mat images" << std::endl;
		return nullptr;
	}
}

template <>
inline i3d::Image3d<std::complex<float>>* MatToImage3D<std::complex<float>>(const cv::Mat img)
{
	if (img.type() == CV_32FC2)
	{
		cv::Size s = img.size();
		auto image = new i3d::Image3d<std::complex<float>>();

		image->MakeRoom(s.width, s.height, 1);
		auto data = reinterpret_cast<float*>(img.data);

		auto array = new std::vector<std::complex<float>>();

		for (int i = 0; i < img.rows; i++)
		{
			for (int j = 0; j < img.cols; j++)
			{
				float r = img.at<float>(i, j * 2);
				float im = img.at<float>(i, j * 2 + 1);

				array->push_back(std::complex<float>(r, im));
			}
		}

		std::complex<float>* p = image->GetFirstVoxelAddr();
		for (size_t i = 0; i < s.area(); i++)
		{
			*p = array->at(i);
			p++;
		}

		return image;
	}
	else
	{
		std::cout << "cv::Mat image type " << getImageType(img.type()) << " with this type of convertion" << std::endl;
		std::cout << "Complex type of convertion only works with CV_32FC2 type of Mat images" << std::endl;
		return nullptr;
	}
}
