#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#undef RGB
#include <stdint.h> 


typedef struct timeval {
	long tv_sec;
	long tv_usec;
} timeval;

inline int gettimeofday(struct timeval * tp, struct timezone * tzp)
{
	static const uint64_t EPOCH = static_cast<uint64_t>(116444736000000000ULL);

	SYSTEMTIME  system_time;
	FILETIME    file_time;
	uint64_t    time;

	GetSystemTime(&system_time);
	SystemTimeToFileTime(&system_time, &file_time);
	time = static_cast<uint64_t>(file_time.dwLowDateTime);
	time += static_cast<uint64_t>(file_time.dwHighDateTime) << 32;

	tp->tv_sec = static_cast<long>((time - EPOCH) / 10000000L);
	tp->tv_usec = static_cast<long>(system_time.wMilliseconds * 1000);
	return 0;
}

inline double TimeDiff(struct timeval t1, struct timeval t2)
{
	double t;

	t = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
	t += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms

	return t;
}