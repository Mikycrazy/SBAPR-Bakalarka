#pragma once

struct BorderIndices

{

	long *xDataPre, *xDataPost; ///the padding data: indices to voxels whose intensities will be used as the image extension

	long *yDataPre, *yDataPost; ///the padding data: indices to voxels whose intensities will be used as the image extension

	long *zDataPre, *zDataPost; ///the padding data: indices to voxels whose intensities will be used as the image extension



	long xLength; ///the extension lengths

	long yLength; ///the extension lengths

	long zLength; ///the extension lengths



	/** empty constructor */

	BorderIndices() :xDataPre(0), xDataPost(0), yDataPre(0), yDataPost(0), zDataPre(0), zDataPost(0), xLength(0), yLength(0), zLength(0) { };

	/** copy constructor, if mirror=1 then it makes an identical copy with the only exception

	* that <em>[xyz]Data[Pre/Post]</em> are stored in the reverse order in comparison to the original <em>f,[xyz]Data[Pre/Post]</em>

	*/

	BorderIndices(struct BorderIndices const &f, int mirror = 0);

	/** The desctructor frees allocated memory. */

	~BorderIndices();



	/** Prints the content of the structure to the screen. */

	void PrintIndices(void);



	/** Safely frees all the indices arrays <em>[xyz]Data[Pre/Post]</em> . */

	void DisposeData(void);

};
