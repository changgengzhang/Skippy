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

public:
	MyRay(): m_valid(false){}

	MyRay(glm::vec3 original, glm::vec3 direction):
		m_valid(true),
		m_original(original),
		m_direction(glm::normalize(direction))
	{}
	~MyRay() {}

	bool isRayIntersectWithBox(const AABBox& aabbox, float &t)
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
			t = tmax;
			return false;
		}

		if (tmin > tmax)
		{
			t = tmax;
			return false;
		}

		t = tmin;
		return true;
	}

};

#endif // !MY_RAY

