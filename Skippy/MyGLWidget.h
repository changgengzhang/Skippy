#ifndef MY_GLWIDGET_H
#define MY_GLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>

#include "MyGeometry/RenderFramework.h"

class MyGLWidget : public QOpenGLWidget,protected QOpenGLFunctions
{
	Q_OBJECT

public:
	MyGLWidget(QWidget *parent = Q_NULLPTR);
	~MyGLWidget();

	void setRenderFramework(RenderFramework *rf);


protected:
	// ============== virtual function inherited form QOpenGLWidget ================
	void initializeGL() override;
	void paintGL() override;
	void resizeGL(int width, int height) override;

	void mousePressEvent(QMouseEvent *mouseEvent) override;
	void mouseReleaseEvent(QMouseEvent *mouseEvent) override;
	void mouseMoveEvent(QMouseEvent *mouseEvent) override;

private:

	RenderFramework *m_rf;
};


#endif