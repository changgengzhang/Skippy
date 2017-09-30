#ifndef MY_MATH_UTILS_H
#define MY_MATH_UTILS_H

#include <math.h>
using namespace std;

class MyMathUtils
{
public:
	template <typename Number>
	static inline bool IsNearZero(Number value)
	{
		if (value < 0)
			value = -value;
		if (value >= 0 && value <= numeric_limits<Number>::epsilon())
			return true;
		return false;
	}
};

#endif
