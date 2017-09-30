#ifndef MY_STROKE_H
#define MY_STROKE_H

#include <qobject.h>
#include <qopenglfunctions.h>
#include <qopenglvertexarrayobject.h>
#include <qopenglshaderprogram.h>
#include <qopenglbuffer.h>
#include <qdebug.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>



#include "Utils.h"
#include "MyBox.h"
#include "MyRay.h"
#include "MyTools/MySplineCommon.h"

class MyStroke : public QObject, protected QOpenGLFunctions
{
	Q_OBJECT
public:
	bool m_valid;
	// ���λ��
	glm::vec3 m_cameraPos;

public:
	MyStroke(QObject *parent = nullptr);
	~MyStroke();

	void draw();
	// ��Ⱦ׼��
	void setupShaderProgram(QString vertexFile, QString fragmentFile);
	void setupBufferData();
	void updateBufferData();

	// ����׼��
	void addPoint(glm::vec2 point);
	void resamplePoint();
	void setupRay(glm::mat4 viewMat, glm::mat4 projMat);
	void classfyPoints(const AABBox& aabbox);

	// ���
	void reset();
	
	// ����߶ȳ�
	void computeHeightField();
private:
	void drawLine();
	void drawPoints();

	void setUniformValues();

private:
	// render buffer
	QOpenGLShaderProgram *m_shaderProgram;
	QOpenGLVertexArrayObject *m_vao;
	QOpenGLBuffer *m_vbo;

	// use input
	uint m_pointCount;
	QVector<glm::vec2> m_inputPoints;
	QVector<glm::vec2> m_resamplePoints;
	//����on off segment ����ɫ
	QVector<glm::vec3> m_colors;

	// �����λ�õ�ȡ���ĵ������
	QVector<MyRay> m_rays;

	// on off segment ���
	QVector<bool> m_pointFlags;

	// ���ߵ���Χ�еľ���
	QVector<float> m_distant;

	// ========== uniform value ==========
	glm::mat4 m_orthoProjMat;

	

};


#endif // !MY_STROKE_H

