#include "MyGLWidget.h"
#include "Utils.h"

MyGLWidget::MyGLWidget(QWidget *parent)
	: QOpenGLWidget(parent)
{
	m_rf = nullptr;
}

MyGLWidget::~MyGLWidget()
{
}


void MyGLWidget::initializeGL()
{
	initializeOpenGLFunctions();

	glViewport(0, 0, RenderViewWidth, RenderViewHeight);
	glClearColor(0.1f, 0.2f, 0.3f, 1);

	m_rf->addModel("../Data/Model/cube.obj");
	m_rf->addStroke();
}


void MyGLWidget::paintGL()
{
	makeCurrent();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, RenderViewWidth, RenderViewHeight);

	m_rf->draw();
	
}


/*
Sets up the OpenGL viewport, projection, etc. Gets called whenever the widget has been resized
*/
void MyGLWidget::resizeGL(int width, int height)
{
	//m_projMat = glm::perspective(45.0f, RenderViewWidth / RenderViewHeight, 0.01f, 1000.0f);
}


/* 
	½»»¥²Ù×÷
*/
void MyGLWidget::mousePressEvent(QMouseEvent *mouseEvent)
{
	makeCurrent();
	m_rf->mousePressEvent(mouseEvent);
	update();
}


void MyGLWidget::mouseReleaseEvent(QMouseEvent *mouseEvent)
{
	makeCurrent();
	m_rf->mouseReleaseEvent(mouseEvent);
	update();
}


void MyGLWidget::mouseMoveEvent(QMouseEvent *mouseEvent)
{
	makeCurrent();
	m_rf->mouseMoveEvent(mouseEvent);
	update();
}



void MyGLWidget::setRenderFramework(RenderFramework *rf)
{
	m_rf = rf;
}
