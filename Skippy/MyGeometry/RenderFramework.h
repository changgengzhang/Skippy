#ifndef RENDER_FRAMEWORK_H
#define RENDER_FRAMEWORK_H

#include <QObject>

#include "MyModel.h"
#include "MyStroke.h"


class RenderFramework : public QObject
{
	Q_OBJECT

public:
	MyModel m_model;
	MyStroke m_stroke;

	glm::mat4 m_modelMat;
	glm::mat4 m_viewMat;
	glm::mat4 m_projMat;

public:
	RenderFramework(QObject *parent = Q_NULLPTR);
	~RenderFramework();

	void draw();
	void addModel(QString filename);
	void addStroke();

	// ½»»¥
	// interaction
	void mousePressEvent(QMouseEvent *mouseEvent);
	void mouseReleaseEvent(QMouseEvent *mouseEvent);
	void mouseMoveEvent(QMouseEvent *mouseEvent);

private:
	glm::vec3 m_cameraPos;

};

#endif
