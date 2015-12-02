#pragma once

#include <i3d/image3d.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

/*! \file ObjectConvertor.h
\brief Modul for i3dlib - image manipulation library to provide convertion between both
*	data stuctures.
*
*	<h1>Object Convertor OpenCV -> i3dlibs </h1>
*
*	<B> Licence & Copyright </B>
*
*	Copyright (C) 2015   Masaryk University/Centre for Biomedical Image Analysis (CBIA)
*
*	This program is free software: you can redistribute it and/or modify
*	it under the terms of the GNU General Public License as published by
*	the Free Software Foundation, either version 3 of the License, or
*	(at your option) any later version.
*
*	This program is distributed in the hope that it will be useful,
*	but WITHOUT ANY WARRANTY; without even the implied warranty of
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*	GNU General Public License for more details.
*
*	You should have received a copy of the GNU General Public License
*	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*	 <B> Object Convertor </B>
*
*	I3dlib has object Image3D for representation of 2D/3D picture, OpenCV has object Mat
*	for representation 2D pictures. 
*	Image3D saves data in std::valarray<> like array with diferent structure 
*	for diferent picture type. Example for RGB 16-bit picture has data format 3x unsingned short to one voxel.
*	Mat saves data in array of uchar and channels are 
*
*	To work with this module you must add to yours project i3dlibs library and opencv library
*
*	This modul can support convertion of this type:

*	RGB8 <--> CV_8UC3

*	RGB16 <--> CV_16UC3

*	GRAY8 <--> CV_8UC1

*	GRAY16 <--> CV_16UC1

*	float <--> CV_32UC1

*	bool <--> CV_8UC1

*	std::complex<float> <--> CV_32UC2

*	std::complex<double> <--> CV_64UC2
*
*	<B>Example:</B><BR/>
*	Image3D -> Mat:
*	@code
*		auto img = new i3d::Image3d<i3d::RGB>("Picture.png");			// Load image form file
*
*		cv::Mat image = image3DToMat(img);								// Convert Image<RGB> to Mat
*		cv::imwrite("images/picture.png", image);						// Save image to file
*	@endcode
*
*	Mat -> Image3D
*	@code
*		cv::Mat image;
*		image = cv::imread("Test-Picture-10x10-GrayScale.png", cv::IMREAD_GRAYSCALE);	// Read the file
*
*		auto img = MatToImage3D<i3d::GRAY8>(image);										// Convert Mat to Image3D<GRAY8>
*		img->SaveImage("images/Image3D_Gray8.png");										// Save image to file
*	@endcode
*
*	\author Michal Valeta (xvalent3@fi.muni.cz)
*/



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


//TODO: Pro 3d obrazky vybrat vrstvu. OK
//Komentaøe
//Licence & Copyright
//Zminit resolution

/**
* \brief Method for convertion Image3D structure to Mat struture (i3d -> opencv).
*
* Converts Image3D reprezentations to array of unsigned chars with colors channels in the order BGR.
* Keeps size of images, but description and real world resolution from Image3D structure are lost.
*
* @param img	image saved as Image3d<RGB>
* @param layer	index of layer (first is 0) which you want to choise
* @return	New instance converted image in opencv format Mat CV_8UC3 type.
			If Image has no layer which was given as param, returns Empty Mat.
*/
inline cv::Mat image3DToMat(const i3d::Image3d<i3d::RGB>* img, int layer = 0)
{
	int i = img->GetSizeZ();
	if (img->GetSizeZ() < layer)
	{
		std::cout << "Image has no layer: " << layer << "!" << std::endl;
		return cv::Mat();
	}

	auto slice = new i3d::Image3d<i3d::RGB>();
	slice->MakeRoom(img->GetSizeX(), img->GetSizeY(), 1);

	img->GetSliceZ(*slice, layer);

	//Convertion data from image3d to uchar*
	auto data = slice->GetVoxelData();
	auto array = new unsigned char[data.size() * 3];


	for (size_t i = 0; i < data.size(); i++)
	{
		array[i * 3] = data[i].blue;
		array[i * 3 + 1] = data[i].green;
		array[i * 3 + 2] = data[i].red;
	}

	delete slice;

	//Returning new Mat object with data from img
	return cv::Mat(img->GetSizeY(), img->GetSizeX(), CV_8UC3, array).clone();
}

/**
* @brief Method for convertion Image3D structure to Mat struture (i3d -> opencv).

* Converts Image3D representations of grayscale picture to array of unsigned chars for Mat representation;.
* Keeps size of images, but description and real world resolution from Image3D structure are lost.

* @param img	image saved as Image3d<GRAY8>
* @param layer	index of layer (first is 0) which you want to choise
* @return	New instance converted image in opencv format Mat CV_8UC1 type.
			If Image has no layer which was given as param, returns Empty Mat.
*/
inline cv::Mat image3DToMat(const i3d::Image3d<i3d::GRAY8>* img, int layer = 0)
{
	if (img->GetSizeZ() < layer)
	{
		std::cout << "Image has no layer: " << layer << "!" << std::endl;
		return cv::Mat();
	}

	auto slice = new i3d::Image3d<i3d::GRAY8>();
	slice->MakeRoom(img->GetSizeX(), img->GetSizeY(), 1);

	img->GetSliceZ(*slice, layer);

	//Convertion data to uchar array
	auto data = slice->GetVoxelData();
	auto array = new unsigned char[data.size()];

	for (size_t i = 0; i < data.size(); i++)
	{
		array[i] = data[i];
	}

	//Returning new Mat object with data from img
	return cv::Mat(img->GetSizeY(), img->GetSizeX(), CV_8UC1, array).clone();
}

/**
* @brief Method for convertion Image3D structure to Mat struture (i3d -> opencv).

* Converts Image3D reprezentations of grayscale16 picture to array of unsigned chars for Mat reprezentation;.
* Keeps size of images, but description and real world resolution from Image3D structure are lost.

* @param img	image saved as Image3d<GRAY16>
* @param layer	index of layer (first is 0) which you want to choise
* @return	New instance converted image in opencv format Mat CV_16UC1 type.
			If Image has no layer which was given as param, returns Empty Mat.
*/
inline cv::Mat image3DToMat(const i3d::Image3d<i3d::GRAY16>* img, int layer = 0)
{
	//If image has not enought slices.
	if (img->GetSizeZ() < layer)
	{
		std::cout << "Image has no layer: " << layer << "!" << std::endl;
		return cv::Mat();
	}

	//Get 2DSlice
	auto slice = new i3d::Image3d<i3d::GRAY16>();
	slice->MakeRoom(img->GetSizeX(), img->GetSizeY(), 1);
	img->GetSliceZ(*slice, layer);

	//Convertion data to ushort array
	auto data = slice->GetVoxelData();
	auto array = new unsigned short[data.size()];

	for (size_t i = 0; i < data.size(); i++)
	{
		//Swap Bites because each library works with them diferetly
		unsigned short tmp = data[i];
		unsigned short val = (tmp << 8) | (tmp >> 8);
		array[i] = val;
	}

	//Returning new Mat object with data from img
	return cv::Mat(img->GetSizeY(), img->GetSizeX(), CV_16UC1, array).clone();
}

/**
* @brief Method for convertion Image3D structure to Mat struture (i3d -> opencv).

* Converts Image3D reprezentations of RGB 16-bit image to array of unsigned chars with colors channels in the order BGR.
* Keeps size of images, but description and real world resolution from Image3D structure are lost.

* @param img	image saved as Image3d<RGB16>
* @param layer	index of layer (first is 0) which you want to choise
* @return	New instance converted image in opencv format Mat CV_16UC3 type.
			If Image has no layer which was given as param, returns Empty Mat.
*/
inline cv::Mat image3DToMat(const i3d::Image3d<i3d::RGB16>* img, int layer = 0)
{
	//If image has not enought slices.
	if (img->GetSizeZ() < layer)
	{
		std::cout << "Image has no layer: " << layer << "!" << std::endl;
		return cv::Mat();
	}

	//Get 2DSlice
	auto slice = new i3d::Image3d<i3d::RGB16>();
	slice->MakeRoom(img->GetSizeX(), img->GetSizeY(), 1);
	img->GetSliceZ(*slice, layer);

	auto data = slice->GetVoxelData();
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
	return cv::Mat(img->GetSizeY(), img->GetSizeX(), CV_16UC3, array).clone();
}

/**
* @brief Method for convertion Image3D structure to Mat struture (i3d -> opencv).

* Converts Image3D float reprezentations of grayscale picture to array of unsigned chars for Mat reprezentation;.
* Keeps size of images, but description and real world resolution from Image3D structure are lost.

* @param img	image saved as Image3d<float>
* @param layer	index of layer (first is 0) which you want to choise
* @return	New instance converted image in opencv format Mat CV_32UC1 type.
			If Image has no layer which was given as param, returns Empty Mat.
*/
inline cv::Mat image3DToMat(const i3d::Image3d<float>* img, int layer = 0)
{
	//If image has not enought slices.
	if (img->GetSizeZ() < layer)
	{
		std::cout << "Image has no layer: " << layer << "!" << std::endl;
		return cv::Mat();
	}

	//Get 2DSlice
	auto slice = new i3d::Image3d<float>();
	slice->MakeRoom(img->GetSizeX(), img->GetSizeY(), 1);
	img->GetSliceZ(*slice, layer);

	//Convertion data to ushort array
	auto data = slice->GetVoxelData();
	auto array = new float[data.size()];

	for (size_t i = 0; i < data.size(); i++)
	{
		array[i] = data[i];
	}

	//Returning new Mat object with data from img
	return cv::Mat(img->GetSizeY(), img->GetSizeX(), CV_32FC1, array).clone();
}

/**
* @brief Method for convertion Image3D structure to Mat struture (i3d -> opencv).

* Converts Image3D bool reprezentations of black/white picture to array of unsigned chars for Mat reprezentation;.
* Keeps size of images, but description and real world resolution from Image3D structure are lost.

* @param img	image saved as Image3d<bool>
* @param layer	index of layer (first is 0) which you want to choise
* @return	New instance converted image in opencv format Mat CV_8UC1 type.
			If Image has no layer which was given as param, returns Empty Mat.
*/
inline cv::Mat image3DToMat(const i3d::Image3d<bool>* img, int layer = 0)
{
	//If image has not enought slices.
	if (img->GetSizeZ() < layer)
	{
		std::cout << "Image has no layer: " << layer << "!" << std::endl;
		return cv::Mat();
	}

	//Get 2DSlice
	auto slice = new i3d::Image3d<bool>();
	slice->MakeRoom(img->GetSizeX(), img->GetSizeY(), 1);
	img->GetSliceZ(*slice, layer);

	//Convertion data to ushort array
	auto data = slice->GetVoxelData();
	auto array = new bool[data.size()];

	for (size_t i = 0; i < data.size(); i++)
	{
		array[i] = data[i];
	}

	//Returning new Mat object with data from img
	return cv::Mat(img->GetSizeY(), img->GetSizeX(), CV_8UC1, array).clone();
}

/**
* @brief Method for convertion Image3D structure to Mat struture (i3d -> opencv).

* Converts Image3D complex<float> representations of grayscale picture to array of unsigned chars for Mat representation;.
* Keeps size of images, but description and real world resolution from Image3D structure are lost.

* @param img	image saved as Image3d<complex<float>>
* @param layer	index of layer (first is 0) which you want to choise
* @return	New instance converted image in opencv format Mat CV_32UC1 type.
			If Image has no layer which was given as param, returns Empty Mat.
*/
inline cv::Mat image3DToMat(const i3d::Image3d<std::complex<float>>* img, int layer = 0)
{
	//If image has not enought slices.
	if (img->GetSizeZ() < layer)
	{
		std::cout << "Image has no layer: " << layer << "!" << std::endl;
		return cv::Mat();
	}

	//Get 2DSlice
	auto slice = new i3d::Image3d<std::complex<float>>();
	slice->MakeRoom(img->GetSizeX(), img->GetSizeY(), 1);
	img->GetSliceZ(*slice, layer);

	//Convertion data to ushort array
	auto data = slice->GetVoxelData();
	auto array = new float[data.size() * 2];

	for (size_t i = 0; i < data.size(); i++)
	{
		array[i * 2] = data[i].real();
		array[i * 2 + 1] = data[i].imag();
	}

	//Returning new Mat object with data from img
	return cv::Mat(img->GetSizeY(), img->GetSizeX(), CV_32FC2, array).clone();
}

/**
* @brief Method for convertion Image3D structure to Mat struture (i3d -> opencv).

* Converts Image3D complex<double> reprezentations of grayscale picture to array of unsigned chars for Mat reprezentation;.
* Keeps size of images, but description and real world resolution from Image3D structure are lost.

* @param img	image saved as Image3d<complex<double>>
* @param layer	index of layer (first is 0) which you want to choise
* @return	New instance converted image in opencv format Mat CV_64UC1 type.
If Image has no layer which was given as param, returns Empty Mat.
*/
inline cv::Mat image3DToMat(const i3d::Image3d<std::complex<double>>* img, int layer = 0)
{
	//If image has not enought slices.
	if (img->GetSizeZ() < layer)
	{
		std::cout << "Image has no layer: " << layer << "!" << std::endl;
		return cv::Mat();
	}

	//Get 2DSlice
	auto slice = new i3d::Image3d<std::complex<double>>();
	slice->MakeRoom(img->GetSizeX(), img->GetSizeY(), 1);
	img->GetSliceZ(*slice, layer);

	//Convertion data to ushort array
	auto data = slice->GetVoxelData();
	auto array = new double[data.size() * 2];

	for (size_t i = 0; i < data.size(); i++)
	{
		array[i * 2] = data[i].real();
		array[i * 2 + 1] = data[i].imag();
	}

	//Returning new Mat object with data from img
	return cv::Mat(img->GetSizeY(), img->GetSizeX(), CV_64FC2, array).clone();
}


template <typename VOXEL>
inline i3d::Image3d<VOXEL>* MatToImage3D(const cv::Mat img)
{
	std::cout << "Unknown Type of Convertion. Library only support convertion to:" << std::endl;
	std::cout << "CV_8UC3 -> RBG " << std::endl << " CV_8UC1 -> GRAY8 " << std::endl << " CV_16UC3 -> RGB16 " << std::endl << " CV_16UC1 -> GRAY16 " << std::endl << " CV_32UC1 -> FLOAT " << std::endl << " CV_8UC1 -> BOOL" << std::endl;
	std::cout << "CV_32UC2 -> STD::COMPLEX<FLOAT> " << std::endl << "CV_64UC2 -> STD::COMPLEX<DOUBLE>" << std::endl;
	return nullptr;
}

/**
* @brief Function for convertion Mat structure into equivalent struct v Image3D (opencv -> Mat)

* Function for convertion Mat structure into equivalent struct v Image3D (opencv -> Mat)
* Converts Mat representation to new Image3D object with type of RGB.
* Function only works with CV_8UC3 type of Mat, other types are not supported and function returns nullptr.
* The Image3D structure is created with only basic metadata.
* If you want to set description and resolution, you have to set it manually, using their respective setters.

* @param img image saved as Mat in opencv library
* @return	If type of image is CV_8UC3, returns new image in equivalent Image3D representation.
*			If type of image differs form CV_8UC3, returns nullptr. 
*/
template <>
inline i3d::Image3d<i3d::RGB>* MatToImage3D<i3d::RGB>(const cv::Mat img)
{
	if (img.type() == CV_8UC3)
	{
		cv::Size s = img.size();
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

/**
* @brief Function for convertion Mat structure into equivalent struct v Image3D (opencv -> Mat)

* Function for convertion Mat structure into equivalent struct v Image3D (opencv -> Mat)
* Converts Mat representation to new Image3D object with type of GRAY8.
* Function only works with CV_8UC1 type of Mat, other types are not supported and function returns nullptr.
* The Image3D structure is created with only basic metadata.
* If you want to set description and resolution, you have to set it manually, using their respective setters.

* @param img image saved as Mat in opencv library
* @return	If type of image is CV_8UC1, returns new image in equivalent Image3D representation.
*			If type of image differs form CV_8UC1, returns nullptr.
*/
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

/**
* @brief Function for convertion Mat structure into equivalent struct v Image3D (opencv -> Mat)

* Function for convertion Mat structure into equivalent struct v Image3D (opencv -> Mat)
* Converts Mat representation to new Image3D object with type of GRAY16.
* Function only works with CV_16UC1 type of Mat, other types are not supported and function returns nullptr.
* The Image3D structure is created with only basic metadata.
* If you want to set description and resolution, you have to set it manually, using their respective setters.

* @param img image saved as Mat in opencv library
* @return	If type of image is CV_16UC1, returns new image in equivalent Image3D representation.
*			If type of image differs form CV_16UC1, returns nullptr.
*/
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

/**
* @brief Function for convertion Mat structure into equivalent struct v Image3D (opencv -> Mat)

* Function for convertion Mat structure into equivalent struct v Image3D (opencv -> Mat)
* Converts Mat representation to new Image3D object with type of RGB16.
* Function only works with CV_16UC3 type of Mat, other types are not supported and function returns nullptr.
* The Image3D structure is created with only basic metadata.
* If you want to set description and resolution, you have to set it manually, using their respective setters.

* @param img image saved as Mat in opencv library
* @return	If type of image is CV_16UC3, returns new image in equivalent Image3D representation.
*			If type of image differs form CV_16UC3, returns nullptr.
*/
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

/**
* @brief Function for convertion Mat structure into equivalent struct v Image3D (opencv -> Mat)

* Function for convertion Mat structure into equivalent struct v Image3D (opencv -> Mat)
* Converts Mat representation to new Image3D object with type of float.
* Function only works with CV_32UC1 type of Mat, other types are not supported and function returns nullptr.
* The Image3D structure is created with only basic metadata.
* If you want to set description and resolution, you have to set it manually, using their respective setters.

* @param img image saved as Mat in opencv library
* @return	If type of image is CV_32UC1, returns new image in equivalent Image3D representation.
*			If type of image differs form CV_32UC1, returns nullptr.
*/
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

/**
* @brief Function for convertion Mat structure into equivalent struct v Image3D (opencv -> Mat)

* Function for convertion Mat structure into equivalent struct v Image3D (opencv -> Mat)
* Converts Mat representation to new Image3D object with type of bool.
* Function only works with CV_8UC1 type of Mat, other types are not supported and function returns nullptr.
* The Image3D structure is created with only basic metadata.
* If you want to set description and resolution, you have to set it manually, using their respective setters.

* @param img image saved as Mat in opencv library
* @return	If type of image is CV_8UC1, returns new image in equivalent Image3D representation.
*			If type of image differs form CV_8UC1, returns nullptr.
*/
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

/**
* @brief Function for convertion Mat structure into equivalent struct v Image3D (opencv -> Mat)

* Function for convertion Mat structure into equivalent struct v Image3D (opencv -> Mat)
* Converts Mat representation to new Image3D object with type of std::complex<float>.
* Function only works with CV_32UC2 type of Mat, other types are not supported and function returns nullptr.
* The Image3D structure is created with only basic metadata.
* If you want to set description and resolution, you have to set it manually, using their respective setters.

* @param img image saved as Mat in opencv library
* @return	If type of image is CV_32UC2, returns new image in equivalent Image3D representation.
*			If type of image differs form CV_32UC2, returns nullptr.
*/
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

/**
* @brief Function for convertion Mat structure into equivalent struct v Image3D (opencv -> Mat)

* Function for convertion Mat structure into equivalent struct v Image3D (opencv -> Mat)
* Converts Mat representation to new Image3D object with type of std::complex<double>.
* Function only works with CV_64UC2 type of Mat, other types are not supported and function returns nullptr.
* The Image3D structure is created with only basic metadata.
* If you want to set description and resolution, you have to set it manually, using their respective setters.

* @param img image saved as Mat in opencv library
* @return	If type of image is CV_64UC2, returns new image in equivalent Image3D representation.
*			If type of image differs form CV_64UC2, returns nullptr.
*/
template <>
inline i3d::Image3d<std::complex<double>>* MatToImage3D<std::complex<double>>(const cv::Mat img)
{
	if (img.type() == CV_64FC2)
	{
		cv::Size s = img.size();
		auto image = new i3d::Image3d<std::complex<double>>();

		image->MakeRoom(s.width, s.height, 1);
		auto data = reinterpret_cast<double*>(img.data);

		auto array = new std::vector<std::complex<double>>();

		for (int i = 0; i < img.rows; i++)
		{
			for (int j = 0; j < img.cols; j++)
			{
				float r = img.at<double>(i, j * 2);
				float im = img.at<double>(i, j * 2 + 1);

				array->push_back(std::complex<float>(r, im));
			}
		}

		std::complex<double>* p = image->GetFirstVoxelAddr();
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
		std::cout << "Complex type of convertion only works with CV_64FC2 type of Mat images" << std::endl;
		return nullptr;
	}
}
