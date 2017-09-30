#ifndef MY_SPLINE_COMMON_h
#define MY_SPLINE_COMMON_h

typedef unsigned int uint;

#include <vector>
#include <algorithm>

#include <glm/glm.hpp>

// =========== Eigen ===========
#include <Eigen\Dense>

#include "MyMathUtils.h"

using namespace std;

/* Bezier Spline Implementation
* Based on this article:
* http://www.particleincell.com/blog/2012/bezier-splines/
*/

class BezierSpine
{
private:
	vector<glm::vec2>  m_points;
	size_t m_pointsCount;
	Eigen::MatrixXf *m_a;
	Eigen::MatrixXf *m_b;

public:
	BezierSpine()
	{}

	//////////////////////////////////////////////////////////////////////////
	void addPoint(const glm::vec2 p)
	{
		m_points.push_back(p);
		m_pointsCount = m_points.size();
		m_a = new Eigen::MatrixXf();
	}

	void computeCoefficient()
	{
		/*
		hi*mi + 2*(hi + hi+1)*mi+1 + hi+1*mi+2
		=  6{ (yi+2 - yi+1)/hi+1 - (yi+1 - yi)/hi }

		so, ignore the both ends:
		| -     -     -        0           ...             0     |  |m0 |
		| h0 2(h0+h1) h1       0           ...             0     |  |m1 |
		| 0     h1    2(h1+h2) h2 0        ...                   |  |m2 |
		|         ...                      ...             0     |  |...|
		| 0       ...           0 h(n-2) 2(h(n-2)+h(n-1)) h(n-1) |  |   |
		| 0       ...                      ...             -     |  |mn |

		*/

		const int count = m_pointsCount;
		const int count1 = m_pointsCount - 1;
		const int count2 = m_pointsCount - 2;
		const int count3 = m_pointsCount - 3;

	}

	void eval(float t)
	{

	}
	
};


#endif