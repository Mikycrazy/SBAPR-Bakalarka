#pragma once

#include <iostream>
#include <i3d/image3d.h>

#include "BorderIndices.h"

using namespace i3d;

template <class VOXEL>
void PrepareExtendedBoundaryForFilter(Image3d <VOXEL> const &image,
struct Separable3dFilter <VOXEL > const &filter,
	BorderIndices & ind)
{

	int fsize, i, size;

	long *pre;

	long *post;

	ind.DisposeData();

	ind.xLength = 0;

	ind.yLength = 0;

	ind.zLength = 0;



	if (filter.xLength != 0)
		ind.xLength = filter.xLength / 2;



	if (filter.yLength != 0)
		ind.yLength = filter.yLength / 2;



	if (filter.zLength != 0)

		ind.zLength = filter.zLength / 2;



	if (ind.xLength != 0)

	{

		size = image.GetSizeX();

		fsize = ind.xLength;

		pre = new long[fsize];

		post = new long[fsize];

		for (i = 0; i < fsize; ++i)

		{

			pre[i] = 0;

			post[i] = size - 1;

		}

		ind.xDataPre = pre;

		ind.xDataPost = post;

	}



	if (ind.yLength != 0)
	{
		size = image.GetSizeY();
		fsize = ind.yLength;
		pre = new long[fsize];
		post = new long[fsize];
		for (i = 0; i < fsize; ++i)
		{

			pre[i] = 0;

			post[i] = size - 1;

		}

		ind.yDataPre = pre;

		ind.yDataPost = post;

	}

	if (ind.zLength != 0)

	{

		size = image.GetSizeZ();

		fsize = ind.zLength;

		pre = new long[fsize];

		post = new long[fsize];

		for (i = 0; i < fsize; ++i)

		{

			pre[i] = 0;

			post[i] = size - 1;

		}

		ind.zDataPre = pre;

		ind.zDataPost = post;

	}

}





template <class VOXEL>
int SeparableFilterConvolution( i3d::Image3d<VOXEL> const &d,
								i3d::Image3d<VOXEL> &res,
								struct Separable3dFilter<VOXEL> const &f,
								struct BorderIndices const &b)
{

	//first of all, a few integrity tests:

	//filter sizes must be odd or zero

	if (!(f.xLength & 1) && (f.xLength != 0))

	{

		std::cerr << "SeparableFilterConvolution(): x-axis filter length is wrong!\n";

		return (1);

	}

	if (!(f.yLength & 1) && (f.yLength != 0))

	{

		std::cerr << "SeparableFilterConvolution(): y-axis filter length is wrong!\n";

		return (1);

	}

	if (!(f.zLength & 1) && (f.zLength != 0))

	{

		std::cerr << "SeparableFilterConvolution(): z-axis filter length is wrong!\n";

		return (1);

	}

#ifdef DEBUG

	std::cout << "SeparableFilterConvolution(): filter sizes tests passed\n";

#endif



	//padding data length over both sides of given axis must be

	//exactly the filter length minus 1 for the given axis

	if ((2 * b.xLength != f.xLength - 1) && (f.xLength != 0))

	{

		std::cerr << "SeparableFilterConvolution(): Padding in x-axis is insufficient!\n";

		return (2);

	}

	if ((2 * b.yLength != f.yLength - 1) && (f.yLength != 0))

	{

		std::cerr << "SeparableFilterConvolution(): Padding in y-axis is insufficient!\n";

		return (2);

	}

	if ((2 * b.zLength != f.zLength - 1) && (f.zLength != 0))

	{

		std::cerr << "SeparableFilterConvolution(): Padding in z-axis is insufficient!\n";

		return (2);

	}

#ifdef DEBUG

	std::cout << "SeparableFilterConvolution(): border sizes vs filter sizes tests passed\n";

#endif



	//input/output image dimensions

	const long x_size = (long)d.GetWidth();

	const long y_size = (long)d.GetHeight();

	const long z_size = (long)d.GetNumSlices();



	//padding indices must be inside the convolved image

	int ii = 0;

	for (ii = 0; ii < b.xLength; ii++)

	{

		if ((b.xDataPre[ii] <0) || (b.xDataPre[ii] >= x_size))

		{

			std::cerr << "SeparableFilterConvolution(): x-axis: pre-padding index " << b.xDataPre[ii] << " at position " << ii << " is outside the input image!\n";

			return (3);

		}

		if ((b.xDataPost[ii] <0) || (b.xDataPost[ii] >= x_size))

		{

			std::cerr << "SeparableFilterConvolution(): x-axis: post-padding index " << b.xDataPre[ii] << " at position " << ii << " is outside the input image!\n";

			return (3);

		}

	}

	for (ii = 0; ii < b.yLength; ii++)

	{

		if ((b.yDataPre[ii] <0) || (b.yDataPre[ii] >= y_size))

		{

			std::cerr << "SeparableFilterConvolution(): y-axis: pre-padding index " << b.yDataPre[ii] << " at position " << ii << " is outside the input image!\n";

			return (3);

		}

		if ((b.yDataPost[ii] <0) || (b.yDataPost[ii] >= y_size))

		{

			std::cerr << "SeparableFilterConvolution(): y-axis: post-padding index " << b.yDataPre[ii] << " at position " << ii << " is outside the input image!\n";

			return (3);

		}

	}

	for (ii = 0; ii < b.zLength; ii++)

	{

		if ((b.zDataPre[ii] <0) || (b.zDataPre[ii] >= z_size))

		{

			std::cerr << "SeparableFilterConvolution(): z-axis: pre-padding index " << b.zDataPre[ii] << " at position " << ii << " is outside the input image!\n";

			return (3);

		}

		if ((b.zDataPost[ii] <0) || (b.zDataPost[ii] >= z_size))

		{

			std::cerr << "SeparableFilterConvolution(): z-axis: post-padding index " << b.zDataPre[ii] << " at position " << ii << " is outside the input image!\n";

			return (3);

		}

	}



#ifdef DEBUG

	std::cout << "SeparableFilterConvolution(): padding indicies tests passed\n";

#endif



	//determine in which image to store the intermediate results first: res or tmp?

	short output_selector = 0;

	output_selector += (f.xLength > 0) ? 1 : 0;

	output_selector += (f.yLength > 0) ? 1 : 0;

	output_selector += (f.zLength > 0) ? 1 : 0;



	if (output_selector > 0) {

		res.MakeRoom(d.GetSize());

		res.SetOffset(d.GetOffset());

		res.SetResolution(d.GetResolution());

	}
	else return(0);



	//an alternating output image and its metadata

	Image3d <VOXEL> tmp;

	if (output_selector > 1) {

		tmp.MakeRoom(d.GetSize());

		tmp.SetOffset(d.GetOffset());

		tmp.SetResolution(d.GetResolution());

	}



#ifdef DEBUG_INFO

	std::cout << "SeparableFilterConvolution(): input image of size " << x_size << "x" << y_size << "x" << z_size << " voxels\n";

	std::cout << "SeparableFilterConvolution(): borders are " << b.xLength << ", " << b.yLength << ", " << b.zLength << " voxels wide\n";

	std::cout << "SeparableFilterConvolution(): filter sizes are " << f.xLength << ", " << f.yLength << ", " << f.zLength << " voxels\n";

#endif

	//temporary variables

	long i = 0, j = 0, k = 0, c = 0;    //,c2=0;



	//in points to memory from where input for current convolution direction goes in

	//out points to memory where output for current convolution direction should go

	const VOXEL *in = d.GetFirstVoxelAddr();    //the very first input data

	VOXEL *out = res.GetFirstVoxelAddr();

	if (output_selector == 2) {

		out = tmp.GetFirstVoxelAddr();

#ifdef DEBUG

		std::cout << "SeparableFilterConvolution(): input set on the original input image\n";

		std::cout << "SeparableFilterConvolution(): output set to the temporary image\n";

#endif

	}
	else {

#ifdef DEBUG

		std::cout << "SeparableFilterConvolution(): input set on the original input image\n";

		std::cout << "SeparableFilterConvolution(): output set to the output image\n";

#endif

	}



	// ---- X AXIS ----

	if (f.xLength > 0)

	{

#ifdef DEBUG

		std::cout << "SeparableFilterConvolution(): convolving in x-axis...\n";

#endif

		//x-axis filter is present...

		const VOXEL *in3 = in;



		//for each row...

		const signed long min = (b.xLength < x_size) ? b.xLength : x_size;

		const signed long max = ((x_size - b.xLength) > b.xLength) ? (x_size - b.xLength) : b.xLength;



		for (long rows = 0; rows < y_size * z_size; rows++)

		{

			//no need to shift in and out pointers because

			//x-lines are sequentially in memory



			in3 = in;



			//j relative position, k position in filter



			// PREFIX

			for (i = 0; i < min; i++)

			{

				*out = 0;

				k = 0;

#ifdef DEBUG_CORE_X

				std::cout << "SeparableFilterConvolution(): PRE: prefix starts (k=0), ";

#endif

				//prefix

				for (j = i; j < b.xLength; j++, k++)

					*out += *(in3 + b.xDataPre[j]) * f.xData[k];

#ifdef DEBUG_CORE_X

				std::cout << "infix starts (k=" << k << "), ";

#endif

				//infix

				for (j = -b.xLength + k; (i + j < x_size) && (k < f.xLength); j++, k++)

					*out += *(in + j) * f.xData[k];

#ifdef DEBUG_CORE_X

				std::cout << "postfix starts (k=" << k << "), ";

#endif

				//postfix

				for (j = 0; k < f.xLength; j++, k++)

					*out += *(in3 + b.xDataPost[j]) * f.xData[k];



				//this is better to compute afterwards, after all summings

				*out /= f.xDiv;

#ifdef DEBUG_CORE_X

				std::cout << "convolution of " << i << " (row=" << rows << ") is finished\n";

#endif

				in++;

				out++;

			}



			// INFIX

			for (i = b.xLength; i < (x_size - b.xLength); i++)

			{

				*out = 0;

				k = 0;

#ifdef DEBUG_CORE_X

				std::cout << "SeparableFilterConvolution(): IN: infix starts (k=0), ";

#endif

				//infix

				for (j = -b.xLength; k < f.xLength; j++, k++)

					*out += *(in + j) * f.xData[k];



				//this is better to compute afterwards, after all summings

				*out /= f.xDiv;

#ifdef DEBUG_CORE_X

				std::cout << "convolution of " << i << " (row=" << rows << ") is finished\n";

#endif

				in++;

				out++;

			}



			// POSTFIX

			for (i = max; i < x_size; i++)

			{

				*out = 0;

				k = 0;

#ifdef DEBUG_CORE_X

				std::cout << "SeparableFilterConvolution(): POST: infix starts (k=0), ";

#endif

				//infix

				for (j = -b.xLength; i + j < x_size; j++, k++)

					*out += *(in + j) * f.xData[k];

#ifdef DEBUG_CORE_X

				std::cout << "postfix starts (k=" << k << "), ";

#endif

				//postfix

				for (j = 0; k < f.xLength; j++, k++)

					*out += *(in3 + b.xDataPost[j]) * f.xData[k];



				//this is better to compute afterwards, after all summings

				*out /= f.xDiv;

#ifdef DEBUG_CORE_X

				std::cout << "convolution of " << i << " (row=" << rows << ") is finished\n";

#endif

				in++;

				out++;

			}

		}



		if (output_selector == 2) {

			//we were storing into tmp

			in = tmp.GetFirstVoxelAddr();

			out = res.GetFirstVoxelAddr();

#ifdef DEBUG

			std::cout << "SeparableFilterConvolution():   input set on the temporary image\n";

			std::cout << "SeparableFilterConvolution():   output set to the output image\n";

#endif

		}
		else {

			//we were storing into res

			//note: there might be no other convolution taking place (output_selector == 1)

			in = res.GetFirstVoxelAddr();

			out = tmp.GetFirstVoxelAddr();

#ifdef DEBUG

			std::cout << "SeparableFilterConvolution():   input set on the output image\n";

			std::cout << "SeparableFilterConvolution():   output set to the temporary image\n";

#endif

		}

		--output_selector;

	}



	// ---- Y AXIS ----

	if (f.yLength > 0)

	{

		//y-axis filter is present...

#ifdef DEBUG

		std::cout << "SeparableFilterConvolution(): convolving in y-axis...\n";

#endif

		//we don't know the image to which the in pointer belongs, we need to copy it;

		//this is required for setting correct offsets of each column

		const VOXEL *in2 = in, *in3 = in;

		VOXEL *out2 = out;



		//for each column...

		const signed long min = (b.yLength < y_size) ? b.yLength : y_size;

		const signed long max = ((y_size - b.yLength) > b.yLength) ? (y_size - b.yLength) : b.yLength;



		for (long slices = 0; slices < z_size; slices++)

		{

			in = in2 + (slices * y_size * x_size);

			out = out2 + (slices * y_size * x_size);



			in3 = in;



			//j relative position, k position in filter



			// PREFIX

			for (i = 0; i < min; i++)

			{

				for (c = 0; c < x_size; c++)

					*(out + c) = 0;

				k = 0;

#ifdef DEBUG_CORE_Y

				std::cout << "SeparableFilterConvolution(): PRE: prefix starts (k=0), ";

#endif

				//prefix

				for (j = i; j < b.yLength; j++, k++)

					for (c = 0; c < x_size; c++)

						*(out + c) += *(in3 + b.yDataPre[j] * x_size + c) * f.yData[k];

#ifdef DEBUG_CORE_Y

				std::cout << "infix starts (k=" << k << "), ";

#endif

				//infix

				for (j = -b.yLength + k; (i + j < y_size) && (k < f.yLength); j++, k++)

					for (c = 0; c < x_size; c++)

						*(out + c) += *(in + (j * x_size) + c) * f.yData[k];

#ifdef DEBUG_CORE_Y

				std::cout << "postfix starts (k=" << k << "), ";

#endif

				//postfix

				for (j = 0; k < f.yLength; j++, k++)

					for (c = 0; c < x_size; c++)

						*(out + c) += *(in3 + b.yDataPost[j] * x_size + c) * f.yData[k];



				//this is better to compute afterwards, after all summings

				for (c = 0; c < x_size; c++)

					*(out + c) /= f.yDiv;

#ifdef DEBUG_CORE_Y

				std::cout << "convolution of " << i << " (slice=" << slices << ") is finished\n";

#endif

				in += x_size;

				out += x_size;

			}



			// INFIX

			for (i = b.yLength; i < (y_size - b.yLength); i++)

			{

				for (c = 0; c < x_size; c++)

					*(out + c) = 0;

				k = 0;

#ifdef DEBUG_CORE_Y

				std::cout << "SeparableFilterConvolution(): IN: infix starts (k=0), ";

#endif

				//infix

				for (j = -b.yLength; k < f.yLength; j++, k++)

					for (c = 0; c < x_size; c++)

						*(out + c) += *(in + (j * x_size) + c) * f.yData[k];



				//this is better to compute afterwards, after all summings

				for (c = 0; c < x_size; c++)

					*(out + c) /= f.yDiv;

#ifdef DEBUG_CORE_Y

				std::cout << "convolution of " << i << " (slice=" << slices << ") is finished\n";

#endif

				in += x_size;

				out += x_size;

			}



			// POSTFIX

			for (i = max; i < y_size; i++)

			{

				for (c = 0; c < x_size; c++)

					*(out + c) = 0;

				k = 0;

#ifdef DEBUG_CORE_Y

				std::cout << "SeparableFilterConvolution(): POST: infix starts (k=0), ";

#endif

				//infix

				for (j = -b.yLength; i + j < y_size; j++, k++)

					for (c = 0; c < x_size; c++)

						*(out + c) += *(in + (j * x_size) + c) * f.yData[k];

#ifdef DEBUG_CORE_Y

				std::cout << "postfix starts (k=" << k << "), ";

#endif

				//postfix

				for (j = 0; k < f.yLength; j++, k++)

					for (c = 0; c < x_size; c++)

						*(out + c) += *(in3 + b.yDataPost[j] * x_size + c) * f.yData[k];



				//this is better to compute afterwards, after all summings

				for (c = 0; c < x_size; c++)

					*(out + c) /= f.yDiv;

#ifdef DEBUG_CORE_Y

				std::cout << "convolution of " << i << " (slice=" << slices << ") is finished\n";

#endif

				in += x_size;

				out += x_size;

			}

		}



		in = out2; //that's the only sure thing

		if (out2 == tmp.GetFirstVoxelAddr()) {

			out = res.GetFirstVoxelAddr();

#ifdef DEBUG

			std::cout << "SeparableFilterConvolution():   input set to the temporary image\n";

			std::cout << "SeparableFilterConvolution():   output set to the output image\n";

#endif

		}
		else {

			out = tmp.GetFirstVoxelAddr();

#ifdef DEBUG

			std::cout << "SeparableFilterConvolution():   input set to the output image\n";

			std::cout << "SeparableFilterConvolution():   output set to the temporary image\n";

#endif

		}

		--output_selector;

	}



	// ---- Z AXIS ----

	if (f.zLength > 0)

	{

		//z-axis filter is present...

#ifdef DEBUG

		std::cout << "SeparableFilterConvolution(): convolving in z-axis...\n";

#endif

		//we don't know the image to which the in pointer belongs, we need to copy it;

		//this is required for setting correct offsets of each column

		const VOXEL *in2 = in, *in3 = in;

		VOXEL *out2 = out;



		const long s_size = x_size * y_size;    //just helping variable



		//for each slice...

		const signed long min = (b.zLength < z_size) ? b.zLength : z_size;

		const signed long max = ((z_size - b.zLength) > b.zLength) ? (z_size - b.zLength) : b.zLength;



		for (long cols = 0; cols < y_size; cols++)

		{

			in = in2 + (cols * x_size);

			out = out2 + (cols * x_size);



			in3 = in;



			//j relative position, k position in filter



			// PREFIX

			for (i = 0; i < min; i++)

			{

				for (c = 0; c < x_size; c++)

					*(out + c) = 0;

				k = 0;

#ifdef DEBUG_CORE_Z

				std::cout << "SeparableFilterConvolution(): PRE: prefix starts (k=0), ";

#endif

				//prefix

				for (j = i; j < b.zLength; j++, k++)

					for (c = 0; c < x_size; c++)

						*(out + c) += *(in3 + b.zDataPre[j] * s_size + c) * f.zData[k];

#ifdef DEBUG_CORE_Z

				std::cout << "infix starts (k=" << k << "), ";

#endif

				//infix

				for (j = -b.zLength + k; (i + j < z_size) && (k < f.zLength); j++, k++)

					for (c = 0; c < x_size; c++)

						*(out + c) += *(in + (j * s_size) + c) * f.zData[k];

#ifdef DEBUG_CORE_Z

				std::cout << "postfix starts (k=" << k << "), ";

#endif

				//postfix

				for (j = 0; k < f.zLength; j++, k++)

					for (c = 0; c < x_size; c++)

						*(out + c) += *(in3 + b.zDataPost[j] * s_size + c) * f.zData[k];



				//this is better to compute afterwards, after all summings

				for (c = 0; c < x_size; c++)

					*(out + c) /= f.zDiv;

#ifdef DEBUG_CORE_Z

				std::cout << "convolution of " << i << " (col=" << cols << ") is finished\n";

#endif

				in += s_size;

				out += s_size;

			}



			// INFIX

			for (i = b.zLength; i < (z_size - b.zLength); i++)

			{

				for (c = 0; c < x_size; c++)

					*(out + c) = 0;

				k = 0;

#ifdef DEBUG_CORE_Z

				std::cout << "SeparableFilterConvolution(): IN: infix starts (k=0), ";

#endif

				//infix

				for (j = -b.zLength; k < f.zLength; j++, k++)

					for (c = 0; c < x_size; c++)

						*(out + c) += *(in + (j * s_size) + c) * f.zData[k];



				//this is better to compute afterwards, after all summings

				for (c = 0; c < x_size; c++)

					*(out + c) /= f.zDiv;

#ifdef DEBUG_CORE_Z

				std::cout << "convolution of " << i << " (col=" << cols << ") is finished\n";

#endif

				in += s_size;

				out += s_size;

			}



			// POSTFIX

			for (i = max; i < z_size; i++)

			{

				for (c = 0; c < x_size; c++)

					*(out + c) = 0;

				k = 0;

#ifdef DEBUG_CORE_Z

				std::cout << "SeparableFilterConvolution(): POST: infix starts (k=0), ";

#endif

				//infix

				for (j = -b.zLength; i + j < z_size; j++, k++)

					for (c = 0; c < x_size; c++)

						*(out + c) += *(in + (j * s_size) + c) * f.zData[k];

#ifdef DEBUG_CORE_Z

				std::cout << "postfix starts (k=" << k << "), ";

#endif

				//postfix

				for (j = 0; k < f.zLength; j++, k++)

					for (c = 0; c < x_size; c++)

						*(out + c) += *(in3 + b.zDataPost[j] * s_size + c) * f.zData[k];



				//this is better to compute afterwards, after all summings

				for (c = 0; c < x_size; c++)

					*(out + c) /= f.zDiv;

#ifdef DEBUG_CORE_Z

				std::cout << "convolution of " << i << " (col=" << cols << ") is finished\n";

#endif

				in += s_size;

				out += s_size;

			}

		}



		--output_selector;

		in = out = NULL;

	}



	tmp.DisposeData();

	return (0);

}




template <class VOXEL>
inline void GaussFIR_repeatBoundary(i3d::Image3d <VOXEL> const &In, i3d::Image3d <VOXEL> &Out, const VOXEL Sigma, const float Width)
{

	Separable3dFilter<VOXEL> f;
	BorderIndices b;

	const VOXEL SigmaX = (In.GetSizeX() > 1) ? Sigma : 0;
	const VOXEL SigmaY = (In.GetSizeY() > 1) ? Sigma : 0;
	const VOXEL SigmaZ = (In.GetSizeZ() > 1) ? Sigma : 0;

	EstimateGaussKernel<VOXEL>(f, SigmaX, SigmaY, SigmaZ, Width, Width, Width);
	PrepareExtendedBoundaryForFilter<VOXEL>(In, f, b);
	//PrepareZeroBoundaryForFilter<VOXEL>(f,b);

	SeparableFilterConvolution<VOXEL>(In, Out, f, b);

}