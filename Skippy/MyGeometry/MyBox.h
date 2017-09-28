/*
	参照trimesh2的数据结构，包括box和sphere两种包围盒
*/
#ifndef MY_BOX_H
#define MY_BOX_H

#include <glm/glm.hpp>

// 包围球
class BSphere {
public:
	glm::vec3 center;
	float r;
	bool valid;
	BSphere() : valid(false)
	{}
};

// 包围盒
class AABBox {

public:
	glm::vec3 m_min, m_max;
	bool m_valid;

	AABBox() : m_valid(false)
	{}

	// Construct from a single point
	AABBox(const glm::vec3 &p) : m_min(p), m_max(p), m_valid(true)
	{}

	// Mark invalid
	void clear()
	{
		m_valid = false;
	}

	// Return center point, (vector) diagonal, and (scalar) radius
	const glm::vec3 center() const
	{
		if (m_valid)
			return 0.5f * (m_min + m_max);
		else
			return glm::vec3(0.0f);
	}

	const glm::vec3 size() const
	{
		if (m_valid)
			return m_max - m_min;
		else
			return glm::vec3(0.0f);
	}

	const float radius() const
	{
		if (m_valid)
			return 0.5f * glm::length(m_max - m_min);
		else
			return 0.0f;
	}


	// 比较set的点的各个分量和当前min的各个分量
	void setBound(const glm::vec3 &p)
	{
		if (!m_valid)
		{
			m_min = p;
			m_max = p;
			m_valid = true;
		}
		else
		{
			m_min = glm::vec3(
				m_min.x < p.x ? m_min.x : p.x,
				m_min.y < p.y ? m_min.y : p.y,
				m_min.z < p.z ? m_min.z : p.z);
			m_max = glm::vec3(
				m_max.x > p.x ? m_max.x : p.x,
				m_max.y > p.y ? m_max.y : p.y,
				m_max.z > p.z ? m_max.z : p.z);
			
		}
	}

};

#endif
