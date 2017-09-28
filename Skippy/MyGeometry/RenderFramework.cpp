#include "RenderFramework.h"

RenderFramework::RenderFramework(QObject *parent)
	: QObject(parent)
{
	m_cameraPos = glm::vec3(2.0f, 2.0f, 2.0f);

	m_modelMat = glm::mat4(1.0f);
	m_viewMat = glm::lookAt(m_cameraPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	m_projMat = glm::perspective(45.0f, 1.0f / 1.0f, 0.1f, 1000.0f);
}

RenderFramework::~RenderFramework()
{
}


void RenderFramework::draw()
{
	m_model.draw();
	m_stroke.draw();
}


void RenderFramework::addModel(QString filename)
{
	m_model.setupMeshFromFile(filename);
	m_model.setupShaderProgram("./Shader/modelShader.vert", "./Shader/modelShader.frag");
	m_model.setupBufferData();
	m_model.m_valid = true;
	m_model.m_modelMat = m_modelMat;
	m_model.m_viewMat = m_viewMat;
	m_model.m_projMat = m_projMat;
}


void RenderFramework::addStroke()
{
	m_stroke.setupShaderProgram("./shader/strokeShader.vert", "./shader/strokeShader.frag");
	m_stroke.setupBufferData();
	m_stroke.m_valid = true;
}


void RenderFramework::mousePressEvent(QMouseEvent *mouseEvent)
{
	
}


void RenderFramework::mouseReleaseEvent(QMouseEvent *mouseEvent)
{

}


void RenderFramework::mouseMoveEvent(QMouseEvent *mouseEvent)
{
	m_stroke.addPoint(glm::vec2(mouseEvent->x(), mouseEvent->y()));
	m_stroke.resamplePoint();
	m_stroke.m_cameraPos = m_cameraPos;
	m_stroke.setupRay(m_viewMat, m_projMat);
	m_stroke.classfyPoints(m_model.m_mesh->m_box);
	m_stroke.updateBufferData();

	//qInfo() << QTime::currentTime().toString() << ", x = " << mouseEvent->x() << ", y = " << mouseEvent->y();
}
