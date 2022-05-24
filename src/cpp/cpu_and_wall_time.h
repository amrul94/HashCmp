#ifndef CPU_AND_WALL_TIME_INCLUDED
#  define CPU_AND_WALL_TIME_INCLUDED

#  ifdef _WIN32
#    include <Windows.h> // Windows

// Возвращает фактическое время в Windows
double get_wall_time()
{
	LARGE_INTEGER time, freq;
	if (!QueryPerformanceFrequency(&freq)) return 0; // error
	if (!QueryPerformanceCounter(&time)) return 0; // error
	return (double)time.QuadPart / freq.QuadPart;
}

// Возвращает процессорное время в Windows
double get_cpu_time()
{
	FILETIME a, b, c, d;
	if (GetProcessTimes(GetCurrentProcess(), &a, &b, &c, &d) != 0)
		return (double)(d.dwLowDateTime | ((unsigned long long)d.dwHighDateTime << 32)) * 0.0000001; // total user time without kernel times
	else return 0; // error
}

#  else // Posix/Linux
#    include <time.h>
#    include <sys/time.h>

// Возвращает фактическое время в Unix-системах
double get_wall_time()
{
	struct timeval time;
	if (gettimeofday(&time, NULL)) return 0; // error
	return (double)time.tv_sec + (double)time.tv_usec * 0.000001;
}

// Возвращает процессорное время в Unix-системах
double get_cpu_time()
{
	return (double)clock() / CLOCKS_PER_SEC;
}
#  endif // #ifdef _WIN32
#endif // #ifndef CPU_AND_WALL_TIME_INCLUDED