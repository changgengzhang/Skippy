#include "MyStroke.h"

MyStroke::MyStroke(QObject *parent /* = nullptr */)
	: QObject(parent)
{
	m_valid = false;

	m_shaderProgram = new QOpenGLShaderProgram();
	m_vao = new QOpenGLVertexArrayObject();
	m_vbo = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);

	//m_projMat = glm::ortho(-RenderViewWidth/2.0f, RenderViewWidth/2.0f, -RenderViewHeight/2.0f, RenderViewHeight/2.0f, -1.0f, 1.0f);
	//m_orthoProjMat = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
	m_orthoProjMat = glm::ortho(0.0f, RenderViewWidth, 0.0f, RenderViewHeight, -1.0f, 1.0f);
}


MyStroke::~MyStroke()
{

}


void MyStroke::draw()
{
	if (m_valid)
		this->drawPoints();
}

void MyStroke::drawLine()
{
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_LINE_STIPPLE);
	glLineWidth(3);

	this->setUniformValues();

	m_shaderProgram->bind();
	m_vao->bind();

	glDrawArrays(GL_LINE_STRIP, 0, m_inputPoints.count());

	m_vao->release();
	m_shaderProgram->release();


	glDisable(GL_LINE_STIPPLE);
	glDisable(GL_LINE_SMOOTH);
}


void MyStroke::drawPoints()
{
	glEnable(GL_POINT_SMOOTH);

	glEnable(GL_PROGRAM_POINT_SIZE);
	glPointSize(3);
	this->setUniformValues();

	m_shaderProgram->bind();
	m_vao->bind();

	glDrawArrays(GL_POINTS, 0, m_inputPoints.count());

	m_vao->release();
	m_shaderProgram->release();

	glDisable(GL_PROGRAM_POINT_SIZE);
	glDisable(GL_POINT_SMOOTH);
}



void MyStroke::setupShaderProgram(const QString vertexShaderFilePath, const QString fragmentShaderFilePath)
{
	initializeOpenGLFunctions();

	if (m_shaderProgram->isLinked())
	{
		m_shaderProgram->removeAllShaders();
	}

	m_shaderProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, vertexShaderFilePath);
	m_shaderProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, fragmentShaderFilePath);

	// link for compile shader
	m_shaderProgram->link();
}


void MyStroke::setupBufferData()
{

	if (m_vao->isCreated()) m_vao->destroy();
	if (m_vbo->isCreated()) m_vbo->destroy();
	
	m_shaderProgram->bind();
	// create vao
	m_vao->create();
	m_vao->bind();

	// vbo set
	m_vbo->create();
	m_vbo->bind();
	m_vbo->setUsagePattern(QOpenGLBuffer::DynamicDraw);

	m_vbo->allocate(m_resamplePoints.count() * sizeof(glm::vec2) + m_colors.count() * sizeof(glm::vec3));

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), 0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void *)(m_resamplePoints.count() * sizeof(glm::vec2)));

	m_vbo->release();

	m_vao->release();
	m_shaderProgram->release();

}


void MyStroke::updateBufferData()
{
	m_shaderProgram->bind();
	m_vao->bind();

	m_vbo->bind();

	m_vbo->allocate(m_resamplePoints.count() * sizeof(glm::vec2) + m_colors.count() * sizeof(glm::vec3));
	//m_vbo->allocate(m_originalPoint.count() * sizeof(glm::vec2));

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), 0);
	m_vbo->write(0, m_resamplePoints.constData(), m_resamplePoints.count() * sizeof(glm::vec2));

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void *)(m_resamplePoints.count() * sizeof(glm::vec2)));
	m_vbo->write(m_resamplePoints.count() * sizeof(glm::vec2), m_colors.constData(), m_colors.count() * sizeof(glm::vec3));

	m_vbo->release();

	m_vao->release();
	m_shaderProgram->release();

}


void MyStroke::addPoint(glm::vec2 point)
{
	// 转换到坐标域为 -1, 1之间
	glm::vec2 reversPoint = glm::vec2(point.x, RenderViewHeight - point.y);

	if (m_inputPoints.count() == 0)
	{
		m_inputPoints.push_back(reversPoint);
		m_pointCount = m_inputPoints.count();
	}
	// 检查距离，如果太小，就不放进去了
	float dist = glm::length(m_inputPoints.back() - reversPoint);
	if (dist > 10.0f)
	{
		m_inputPoints.push_back(reversPoint);
		m_pointCount = m_inputPoints.count();
	}
}


void MyStroke::resamplePoint()
{
	m_resamplePoints.clear();
	m_resamplePoints.resize(m_pointCount);

	if (m_pointCount < 3)
	{
		for (uint i = 0; i < m_pointCount; i++)
		{
			m_resamplePoints[i] = m_inputPoints[i];
		}
		return;
	}


	CubicSpine spline;

	for (uint i = 0; i < m_inputPoints.count(); i++)
	{
		spline.addPoint(m_inputPoints[i]);
	}

	spline.compute();
	
	float step = 1.0f / m_pointCount;
	for (uint i = 0; i < m_inputPoints.count(); i++)
	{
		m_resamplePoints[i] = spline.eval(step * i);
	}

}


void MyStroke::setupRay(glm::mat4 viewMat, glm::mat4 projMat)
{
	
	m_rays.clear();
	m_rays.resize(m_pointCount);

	float nearClippingPlane = 0.1;

	for (uint i = 0; i < m_pointCount; i++)
	{ 
		// 注意这里【 z 值为1.0 】，我还没想明白为什么。

		glm::vec4 normPos;
		normPos.x = 2 * m_resamplePoints.at(i).x / RenderViewWidth - 1;
		normPos.y = 2 * m_resamplePoints.at(i).y / RenderViewHeight - 1;
		normPos.z = 0.1f;
		normPos.w = 1.0f;

		glm::vec4 worldPos = glm::inverse(projMat * viewMat) * glm::vec4(normPos);



		if (worldPos.w != 0.0)
		{
			worldPos.x /= worldPos.w;
			worldPos.y /= worldPos.w;
			worldPos.z /= worldPos.w;
		}

		m_rays[i] = MyRay(m_cameraPos, glm::normalize(glm::vec3(worldPos) - m_cameraPos));
		
	}
}


void MyStroke::classfyPoints(const AABBox& aabbox)
{
	m_pointFlags.clear();
	m_pointFlags.resize(m_pointCount);

	m_colors.clear();
	m_colors.resize(m_pointCount);

	m_distant.clear();
	m_distant.resize(m_pointCount);

	for (uint i = 0; i < m_rays.count(); i++)
	{
		bool ret = m_rays[i].isRayIntersectWithBox(aabbox);
		
		m_pointFlags[i] = ret;

		if (ret == true)
		{
			m_colors[i] = glm::vec3(1.0f, 0.0f, 0.0f);
		}
		else
		{
			m_colors[i] = glm::vec3(0.0f, 1.0f, 0.0f);
		}

	}

}


void MyStroke::computeHeightField()
{
	for (uint i = 0; i < m_pointCount; i++)
	{
		// on model point
		if (m_pointFlags.at(i))
		{
			
		}
		else
		{

		}
	}
}


void MyStroke::reset()
{
	m_inputPoints.clear();
	m_resamplePoints.clear();
}


void MyStroke::setUniformValues()
{
	m_shaderProgram->bind();

	glUniformMatrix4fv(m_shaderProgram->uniformLocation("projMat"), 1, GL_FALSE, glm::value_ptr(m_orthoProjMat));

	m_shaderProgram->release();
}