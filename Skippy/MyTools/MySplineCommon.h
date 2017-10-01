#ifndef MY_SPLINE_COMMON_h
#define MY_SPLINE_COMMON_h

typedef unsigned int uint;

#include <vector>
#include <algorithm>

#include <glm/glm.hpp>

// =========== Eigen ===========
#include <Eigen\Dense>


/* Bezier Spline Implementation
* Based on this article:
* http://www.particleincell.com/blog/2012/bezier-splines/
*/

class CubicSplineCoefficient
{
public:
	std::vector<float> a;
	std::vector<float> b;
	std::vector<float> c;
	std::vector<float> d;

	CubicSplineCoefficient(int count)
	{
		a.resize(count);
		b.resize(count);
		c.resize(count);
		d.resize(count);
	}

	~CubicSplineCoefficient()
	{
		std::vector<float>().swap(a);
		std::vector<float>().swap(b);
		std::vector<float>().swap(c);
		std::vector<float>().swap(d);
	}
};


class CubicSpine
{
private:
	std::vector<float>  m_x;
	std::vector<float> m_y;
	size_t m_pointsCount;
	std::vector<float> m_t;
	CubicSplineCoefficient *m_xCoeff;
	CubicSplineCoefficient *m_yCoeff;


public:
	CubicSpine()
	{}


	~CubicSpine()
	{
		delete m_xCoeff;
		delete m_yCoeff;
	}

	//////////////////////////////////////////////////////////////////////////
	void addPoint(const glm::vec2 p)
	{
		m_x.push_back(p.x);
		m_y.push_back(p.y);
		
		m_pointsCount = m_x.size();
		m_xCoeff = new CubicSplineCoefficient(m_pointsCount - 1);
		m_yCoeff = new CubicSplineCoefficient(m_pointsCount - 1);
	}

	void compute()
	{
		this->calParam();
		this->calCoefficient(m_x, m_xCoeff);
		this->calCoefficient(m_y, m_yCoeff);
	}

	glm::vec2 eval(float t)
	{
		int seg = std::lower_bound(m_t.begin(), m_t.end(), t) - m_t.begin();

		if (seg == m_t.size() - 1)
		{
			seg -= 1;
		}

		float x = m_xCoeff->a[seg] 
			+ m_xCoeff->b[seg] * (t - m_t[seg])
			+ m_xCoeff->c[seg] * (t - m_t[seg]) * (t - m_t[seg]) 
			+ m_xCoeff->c[seg] * (t - m_t[seg]) * (t - m_t[seg]) * (t - m_t[seg]);

		float y = m_yCoeff->a[seg]
			+ m_yCoeff->b[seg] * (t - m_t[seg])
			+ m_yCoeff->c[seg] * (t - m_t[seg]) * (t - m_t[seg])
			+ m_yCoeff->c[seg] * (t - m_t[seg]) * (t - m_t[seg]) * (t - m_t[seg]);

		return glm::vec2(x, y);
	}

private:
	// 计算t 值, None-uniform
	void calParam()
	{
		m_t.clear();
		m_t.resize(m_pointsCount);

		float totalLength = 0.0f;
		m_t[0] = 0.0f;
		for (size_t i = 1; i < m_pointsCount; i++)
		{
			float dx = m_x[i] - m_x[i - 1];
			float dy = m_y[i] - m_y[i - 1];
			float len = sqrt(dx * dx + dy * dy);

			totalLength += len;
			m_t[i] = totalLength;
		}

		for (size_t i = 1; i < m_pointsCount; i++)
		{
			m_t[i] /= totalLength;
		}
	}


	void calCoefficient(std::vector<float>& z, CubicSplineCoefficient* coeff)
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

		size_t n = m_t.size();

		Eigen::MatrixXf leftMat = Eigen::MatrixXf::Zero(n, n);
		Eigen::MatrixXf rightMat = Eigen::MatrixXf::Zero(n, 1);

		std::vector<float> h;
		h.resize(n - 1);

		// 计算hi
		for (size_t i = 1; i < n; i++)
		{
			h[i - 1] = m_t[i] - m_t[i - 1];
		}

		// 矩阵的值
		for (size_t i = 0; i < m_pointsCount; i++)
		{
			if (i == 0 || i == m_pointsCount - 1)
			{
				leftMat(i, i) = 1.0f;
				rightMat(i, 0) = 0.0f;

			}
			else
			{
				leftMat(i, i - 1) = h[i - 1];
				leftMat(i, i) = 2 * (h[i - 1] + h[i]);
				leftMat(i, i + 1) = h[i];

				rightMat(i, 0) = 6 * ((z[i + 1] - z[i]) / h[i] - (z[i] - z[i - 1]) / h[i - 1]);
			}
		}

		// 解矩阵
		Eigen::MatrixXf m = leftMat.colPivHouseholderQr().solve(rightMat);
		// 计算参数的值 mi = 2ci
		
		for (size_t i = 0; i < n - 1; i++)
		{
			// ai的值
			coeff->a[i] = z[i];
			// ci的值  m(i, 0) = mi = 2 * ci
			coeff->c[i] = m(i, 0) / 2.0f;
			// di  di = (mi+1 - mi) / 6 * hi
			coeff->d[i] = (m(i + 1, 0) - m(i, 0)) / (6 * h[i]);
			// bi  bi = (zi+1 - zi) / hi - hi * mi / 2 - hi * (mi+1 - mi) * hi / 6
			coeff->b[i] = (z[i + 1] - z[i]) / h[i] - h[i] * m(i, 0) / 2.0f - (m(i + 1) - m(i, 0)) * h[i] / 6.0f;
		}

	}
	
};


#endif