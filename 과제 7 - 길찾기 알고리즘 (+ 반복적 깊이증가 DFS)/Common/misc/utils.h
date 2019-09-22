#ifndef UTILS_H
#define UTILS_H

#include <math.h>
#include <sstream>
#include <string>
#include <vector>
#include <limits>
#include <cassert>
#include <iomanip>

const int     MaxInt    = (std::numeric_limits<int>::max)();
const double  MaxDouble = (std::numeric_limits<double>::max)();
const double  MinDouble = (std::numeric_limits<double>::min)();
const float   MaxFloat  = (std::numeric_limits<float>::max)();
const float   MinFloat  = (std::numeric_limits<float>::min)();

const double   Pi        = 3.14159;
const double   TwoPi     = Pi * 2;
const double   HalfPi    = Pi / 2;
const double   QuarterPi = Pi / 4;

template <typename T>
inline bool isNaN(T val)
{
	return val != val;
}

inline double DegsToRads(const double& degs)
{
	return TwoPi * (degs / 360.0);
}

inline bool IsZero(const double& val)
{
	return ((-MinDouble < val) && (val < MinDouble));
}

inline bool InRange(const double& start, const double& end, const double& val)
{
	if (start < end)
	{
		if ((val > start) && (val < end)) return true;
		else return false;
	}
	else
	{
		if ((val < start) && (val > end)) return true;
		else return false;
	}
}

template <class T>
T Maximum(const T& v1, const T& v2)
{
	return v1 > v2 ? v1 : v2;
}

inline int RandInt(const int& x, const int& y)
{
	assert(y >= x && "<RandInt>: y is less than x");
	return rand() % (y - x + 1) + x;
}

inline double RandFloat() { return ((rand()) / (RAND_MAX + 1.0)); }

inline double RandInRange(const double& x, const double& y) { return x + RandFloat()*(y - x); }

inline bool RandBool()
{
	if (RandFloat() > 0.5) return true;
	else return false;
}

inline double RandomClamped() { return RandFloat() - RandFloat(); }

inline double RandGaussian(double mean = 0.0, double standard_deviation = 1.0)
{
	double x1, x2, w, y1;
	static double y2;
	static int use_last = 0;

	if (use_last)
	{
		y1 = y2;
		use_last = 0;
	}
	else
	{
		do
		{
			x1 = 2.0 * RandFloat() - 1.0;
			x2 = 2.0 * RandFloat() - 1.0;
			w = x1 * x1 + x2 * x2;
		} while (w >= 1.0);

		w = sqrt((-2.0 * log(w)) / w);
		y1 = x1 * w;
		y2 = x2 * w;
		use_last = 1;
	}

	return(mean + y1 * standard_deviation);
}

inline double Sigmoid(double input, double response = 1.0)
{
	return (1.0 / (1.0 + exp(-input / response)));
}

template <class T>
inline T MaxOf(const T& a, const T& b)
{
	if (a > b) return a; return b;
}

template <class T>
inline T MinOf(const T& a, const T& b)
{
	if (a < b) return a; return b;
}

template <class T, class U, class V>
inline void Clamp(T& arg, const U& minVal, const V& maxVal)
{
	assert(((double)minVal < (double)maxVal) && "<Clamp>MaxVal < MinVal!");
	if (arg < (T)minVal) arg = (T)minVal;
	if (arg > (T)maxVal) arg = (T)maxVal;
}

inline int Rounded(const double& val)
{
	int integral = (int)val;
	double mantissa = val - integral;

	if (mantissa < 0.5) return integral;
	else return integral + 1;
}

inline int RoundUnderOffset(const double& val, const double& offset)
{
	int integral = (int)val;
	double mantissa = val - integral;

	if (mantissa < offset) return integral;
	else return integral + 1;
}

inline bool isEqual(const float& a, const float& b)
{
	if (fabs(a - b) < 1E-12) return true;
	return false;
}

inline bool isEqual(const double& a, const double& b)
{
	if (fabs(a - b) < 1E-12) return true;
	return false;
}

template <class T>
inline double Average(const std::vector<T>& v)
{
	double average = 0.0;
	for (unsigned int i = 0; i < v.size(); ++i) average += (double)v[i];
	return average / (double)v.size();
}

inline double StandardDeviation(const std::vector<double>& v)
{
	double sd = 0.0;
	double average = Average(v);

	for (unsigned int i = 0; i < v.size(); ++i) sd += (v[i] - average) * (v[i] - average);
	sd = sd / v.size();
	return sqrt(sd);
}

template <class container>
inline void DeleteSTLContainer(container& c)
{
	for (container::iterator it = c.begin(); it != c.end(); ++it)
	{
		delete *it;
		*it = NULL;
	}
}

template <class map>
inline void DeleteSTLMap(map& m)
{
	for (map::iterator it = m.begin(); it != m.end(); ++it)
	{
		delete it->second;
		it->second = NULL;
	}
}

#endif