#include <iostream>
#include "BorderIndices.h"

BorderIndices::BorderIndices(struct BorderIndices const &f, int mirror)

{

	// safeback init...

	xDataPre = yDataPre = zDataPre = NULL;

	xDataPost = yDataPost = zDataPost = NULL;

	xLength = yLength = zLength = 0;



	long **tmp = new long *[6];

	tmp[0] = f.xDataPre;

	tmp[1] = f.xDataPost;

	tmp[2] = f.yDataPre;

	tmp[3] = f.yDataPost;

	tmp[4] = f.zDataPre;

	tmp[5] = f.zDataPost;

	int i, j;



	// "reduce" the number of pointers

	for (i = 0; i < 5; ++i)

		for (j = i + 1; j < 6; ++j)

			if (tmp[i] == tmp[j]) tmp[j] = NULL;



	// (mirror-)copy relevant data

	if (tmp[0] != NULL)

	{

		xDataPre = new long[f.xLength];

		if (f.xDataPre != f.xDataPost) xDataPost = new long[f.xLength]; else xDataPost = xDataPre;

		xLength = f.xLength;



		if (mirror) {

			for (i = 0; i < xLength; ++i) xDataPre[i] = f.xDataPre[xLength - 1 - i];

			if (f.xDataPre != f.xDataPost) for (i = 0; i < xLength; ++i) xDataPost[i] = f.xDataPost[xLength - 1 - i];

		}
		else {

			for (i = 0; i < xLength; ++i) xDataPre[i] = f.xDataPre[i];

			if (f.xDataPre != f.xDataPost) for (i = 0; i < xLength; ++i) xDataPost[i] = f.xDataPost[i];

		}

	}

	if (tmp[2] != NULL)

	{

		yDataPre = new long[f.yLength];

		if (f.yDataPre != f.yDataPost) yDataPost = new long[f.yLength]; else yDataPost = yDataPre;

		yLength = f.yLength;



		if (mirror) {

			for (i = 0; i < yLength; ++i) yDataPre[i] = f.yDataPre[yLength - 1 - i];

			if (f.yDataPre != f.yDataPost) for (i = 0; i < xLength; ++i) yDataPost[i] = f.yDataPost[xLength - 1 - i];

		}
		else {

			for (i = 0; i < yLength; ++i) yDataPre[i] = f.yDataPre[i];

			if (f.yDataPre != f.yDataPost) for (i = 0; i < xLength; ++i) yDataPost[i] = f.yDataPost[i];

		}

	}

	if (tmp[4] != NULL)

	{

		zDataPre = new long[f.zLength];

		if (f.zDataPre != f.zDataPost) zDataPost = new long[f.zLength]; else zDataPost = zDataPre;

		zLength = f.zLength;



		if (mirror) {

			for (i = 0; i < zLength; ++i) zDataPre[i] = f.zDataPre[zLength - 1 - i];

			if (f.zDataPre != f.zDataPost) for (i = 0; i < xLength; ++i) zDataPost[i] = f.zDataPost[xLength - 1 - i];

		}
		else {

			for (i = 0; i < zLength; ++i) zDataPre[i] = f.zDataPre[i];

			if (f.zDataPre != f.zDataPost) for (i = 0; i < xLength; ++i) zDataPost[i] = f.zDataPost[i];

		}

	}



	delete[]tmp;



	// "bump-up back" the number of pointers

	if (f.yDataPre == f.xDataPre) { yDataPre = xDataPre; yDataPost = xDataPost; yLength = xLength; }

	if (f.zDataPre == f.yDataPre) { zDataPre = yDataPre; zDataPost = yDataPost; zLength = yLength; }

	else if (f.zDataPre == f.xDataPre) { zDataPre = xDataPre; zDataPost = xDataPost; zLength = xLength; }

}



void BorderIndices::PrintIndices(void) {

	std::cout << "Border indices paddings with the following content:\nX: ";

	if (xLength > 0) {

		std::cout << xLength << " values: ";

		for (int i = 0; i < (xLength - 1); ++i) std::cout << xDataPre[i] << ",";

		std::cout << xDataPre[xLength - 1] << " @" << reinterpret_cast<long>(xDataPre) << " <-> ";

		for (int i = 0; i < (xLength - 1); ++i) std::cout << xDataPost[i] << ",";

		std::cout << xDataPost[xLength - 1] << " @" << reinterpret_cast<long>(xDataPost) << "\nY: ";



	}
	else std::cout << "empty\nY: ";

	if (yLength > 0) {

		std::cout << yLength << " values: ";

		for (int i = 0; i < (yLength - 1); ++i) std::cout << yDataPre[i] << ",";

		std::cout << yDataPre[yLength - 1] << " @" << reinterpret_cast<long>(yDataPre) << " <-> ";

		for (int i = 0; i < (yLength - 1); ++i) std::cout << yDataPost[i] << ",";

		std::cout << yDataPost[yLength - 1] << " @" << reinterpret_cast<long>(yDataPost) << "\nZ: ";



	}
	else std::cout << "empty\nZ: ";

	if (zLength > 0) {

		std::cout << zLength << " values: ";

		for (int i = 0; i < (zLength - 1); ++i) std::cout << zDataPre[i] << ",";

		std::cout << zDataPre[zLength - 1] << " @" << reinterpret_cast<long>(zDataPre) << " <-> ";

		for (int i = 0; i < (zLength - 1); ++i) std::cout << zDataPost[i] << ",";

		std::cout << zDataPost[zLength - 1] << " @" << reinterpret_cast<long>(zDataPost) << "\n\n";



	}
	else std::cout << "empty\n\n";

}



void BorderIndices::DisposeData(void)

{

	long **tmp = new long *[6];

	tmp[0] = xDataPre;

	tmp[1] = xDataPost;

	tmp[2] = yDataPre;

	tmp[3] = yDataPost;

	tmp[4] = zDataPre;

	tmp[5] = zDataPost;



	int i, j, imax = 6;



	for (i = 0; i < imax - 1; ++i)

		for (j = i + 1; j < imax; ++j)

		{

			if (tmp[i] == tmp[j])

				tmp[j] = NULL;

		}



	for (i = 0; i < imax; ++i)

	{

		if (tmp[i] != NULL)

			delete[]tmp[i];

		tmp[i] = NULL;

	}

	delete[]tmp;

}



BorderIndices::~BorderIndices()

{

	this->DisposeData();

}