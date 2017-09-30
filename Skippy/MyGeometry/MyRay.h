#ifndef MY_RAY
#define MY_RAY

#include <glm/glm.hpp>

#include "MyBox.h"

class MyRay
{
public:
	bool m_valid;

	glm::vec3 m_original;
	glm::vec3 m_direction;
	int m_sign[3];

public:
	MyRay(): m_valid(false){}

	MyRay(glm::vec3 original, glm::vec3 direction):
		m_valid(true),
		m_original(original),
		m_direction(glm::normalize(direction))
	{
		m_sign[0] = (m_direction.x < 0);
		m_sign[1] = (m_direction.y < 0);
		m_sign[2] = (m_direction.z < 0);
	}
	~MyRay() {}


	bool isRayIntersectWithBox(const AABBox& aabbox)
	{
		if (!m_valid)
			return false;

		m_direction = glm::normalize(m_direction);

		m_direction = 1.0f / m_direction;

		float t1 = (aabbox.m_min.x - m_original.x) * m_direction.x;
		float t2 = (aabbox.m_max.x - m_original.x) * m_direction.x;
		float t3 = (aabbox.m_min.y - m_original.y) * m_direction.y;
		float t4 = (aabbox.m_max.y - m_original.y) * m_direction.y;
		float t5 = (aabbox.m_min.z - m_original.z) * m_direction.z;
		float t6 = (aabbox.m_max.z - m_original.z) * m_direction.z;

		float tmin = qMax(qMax(qMin(t1, t2), qMin(t3, t4)), qMin(t5, t6));
		float tmax = qMin(qMin(qMax(t1, t2), qMax(t3, t4)), qMax(t5, t6));

		if (tmax < 0)
		{
			//t = tmax;
			return false;
		}

		if (tmin > tmax)
		{
			//t = tmax;
			return false;
		}

		//t = 0.0f;
		return true;
	}

	// 没有和box相交的射线
	float distanceFromBoxToRay(const AABBox& aabbox)
	{
		  
	}

};

#endif // !MY_RAY

