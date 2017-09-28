#ifndef MY_MODEL_H
#define MY_MODEL_H

#include <qopenglfunctions.h>
#include <QOpenGLWidget>
#include <qopenglvertexarrayobject.h>
#include <qopenglshaderprogram.h>
#include <qopenglbuffer.h>
#include <qstring.h>
#include <qevent.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>


#include "MyMesh.h"
#include "MyBox.h"

class MyModel : public QObject, protected QOpenGLFunctions
{
	Q_OBJECT
public:
	// 坐标，颜色，法线，纹理坐标
	enum AttrType{NONE, POSITION, COLOR, NORMAL, TEXTURECOORD};

	bool m_valid;

	MyMesh *m_mesh;
	// ========== uniform value ==========
	glm::mat4 m_modelMat;
	glm::mat4 m_viewMat;
	glm::mat4 m_projMat;

public:
	MyModel(QObject *parent = Q_NULLPTR);
	~MyModel();

	void draw();
	// 渲染数据处理
	void setupMeshFromFile(QString filename);
	void setupShaderProgram(QString vertexFile, QString fragmentFile);
	void setupBufferData(
		AttrType position = AttrType::POSITION,
		AttrType color = AttrType::NONE,
		AttrType normal = AttrType::NONE,
		AttrType textureCoord = AttrType::TEXTURECOORD);

	// 交互
	void mousePressEvent(QMouseEvent *mouseEvent);
	void mouseReleaseEvent(QMouseEvent *mouseEvent);
	void mouseMoveEvent(QMouseEvent *mouseEvent);


private:
	void setUniformValues();

private:

	QOpenGLShaderProgram *m_shaderProgram;
	QOpenGLVertexArrayObject *m_vao;
	QOpenGLBuffer *m_vbo;
	QOpenGLBuffer *m_ebo;

	

};


#endif