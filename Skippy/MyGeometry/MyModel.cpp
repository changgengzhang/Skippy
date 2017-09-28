#include "MyModel.h"

MyModel::MyModel(QObject *parent)
	: QObject(parent)
{
	m_valid = false;

	m_mesh = nullptr;

	m_shaderProgram = new QOpenGLShaderProgram();
	m_vao = new QOpenGLVertexArrayObject();
	m_vbo = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
	m_ebo = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);

}

MyModel::~MyModel()
{
	if (m_mesh != nullptr) { delete m_mesh; m_mesh = nullptr; }
	if (m_shaderProgram != nullptr) { delete m_shaderProgram; m_shaderProgram = nullptr; }
	if (m_vao != nullptr) { delete m_vao; m_vao = nullptr; }
	if (m_vbo != nullptr) { delete m_vbo; m_vbo = nullptr; }
	if (m_ebo != nullptr) { delete m_ebo; m_ebo = nullptr; }
}


void MyModel::draw()
{
	if (m_valid)
	{
		//glCullFace(GL_BACK);

		glEnable(GL_DEPTH_TEST);
		//glEnable(GL_CULL_FACE);
		glEnable(GL_DEBUG_OUTPUT);

		this->setUniformValues();

		m_shaderProgram->bind();
		m_vao->bind();
		m_ebo->bind();
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glDrawElements(GL_TRIANGLES, m_mesh->m_faceCount * 3, GL_UNSIGNED_INT, 0);
		m_ebo->release();
		m_vao->release();
		m_shaderProgram->release();


		glDisable(GL_DEPTH_TEST);
		//glDisable(GL_CULL_FACE);
		glDisable(GL_DEBUG_OUTPUT);
	}
}

void MyModel::setupMeshFromFile(QString filename)
{
	if (!m_mesh) delete m_mesh;

	m_mesh = new MyMesh();
	
	m_mesh->setupMeshFromFile(filename);
	m_mesh->needScaleToUintBox();
	m_mesh->needMoveToCenter();
	m_mesh->needNormal();
	m_mesh->needAABBOX();
}


void MyModel::setupShaderProgram(QString vertexFile, QString fragmentFile)
{
	initializeOpenGLFunctions();
	// 重新使用新的Shader
	if (m_shaderProgram->isLinked()) m_shaderProgram->removeAllShaders();
	
	m_shaderProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, vertexFile);
	m_shaderProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, fragmentFile);

	// link for compile shader
	m_shaderProgram->link();
}


void MyModel::setupBufferData(AttrType position, AttrType color, AttrType normal, AttrType textureCoord)
{
	// 数据更新后重新绑定，清除原先的绑定
	if (m_vao->isCreated()) m_vao->destroy();
	if (m_vbo->isCreated()) m_vbo->destroy();
	if (m_ebo->isCreated()) m_vbo->destroy();
	

	// 计算分配空间的大小
	uint positionSize = 0, colorSize = 0, normalSize = 0, textureCoordSize = 0;
	if (position == AttrType::POSITION)
		positionSize = m_mesh->m_vertices.count() * sizeof(glm::vec3);

	if (color == AttrType::COLOR)
		colorSize = m_mesh->m_colors.count() * sizeof(glm::vec3);

	if (normal == AttrType::NORMAL)
		normalSize = m_mesh->m_normals.count() * sizeof(glm::vec3);


	m_shaderProgram->bind();
	// create vao
	m_vao->create();
	m_vao->bind();

	m_vbo->create();
	m_vbo->bind();
	m_vbo->setUsagePattern(QOpenGLBuffer::StaticDraw);
	m_vbo->allocate(positionSize + colorSize + normalSize);

	if (position == AttrType::POSITION)
	{
		m_vbo->write(0, m_mesh->m_vertices.constData(), positionSize);
		//m_vbo->allocate(m_mesh->m_vertices.constData(), m_mesh->m_vertexCount * sizeof(glm::vec3));
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);
		//m_shaderProgram->enableAttributeArray(0);
		//m_shaderProgram->setAttributeBuffer(0, GL_FLOAT, offset, 3, sizeof(vec3));
	}

	if (color == AttrType::COLOR)
	{
		m_vbo->write(positionSize, m_mesh->m_colors.constData(), colorSize);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void *)positionSize);
	}

	if (normal == AttrType::NORMAL)
	{
		m_vbo->write(positionSize + colorSize, m_mesh->m_normals.constData(), normalSize);
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void *)(positionSize + colorSize));
	}
	m_vbo->release();

	m_ebo->create();
	m_ebo->bind();
	m_ebo->setUsagePattern(QOpenGLBuffer::StaticDraw);
	//m_ebo->allocate(m_mesh->m_indices.constData(), m_mesh->m_faceCount * 3 * sizeof(int));
	m_ebo->allocate(m_mesh->m_faces.constData(), m_mesh->m_faces.count() * sizeof(Face));
	// 注意ebo不能release, 如果release了，在glDrawElements 时要bind
	m_ebo->release();

	m_vao->release();
	m_shaderProgram->release();
}


// 鼠标操作
void MyModel::mousePressEvent(QMouseEvent *mouseEvent)
{

}


void MyModel::mouseReleaseEvent(QMouseEvent *mouseEvent)
{

}


void MyModel::mouseMoveEvent(QMouseEvent *mouseEvent)
{

}


void MyModel::setUniformValues()
{
	m_shaderProgram->bind();

	glUniformMatrix4fv(m_shaderProgram->uniformLocation("modelMat"), 1, GL_FALSE, glm::value_ptr(m_modelMat));
	glUniformMatrix4fv(m_shaderProgram->uniformLocation("viewMat"), 1, GL_FALSE, glm::value_ptr(m_viewMat));
	glUniformMatrix4fv(m_shaderProgram->uniformLocation("projMat"), 1, GL_FALSE, glm::value_ptr(m_projMat));

	m_shaderProgram->release();
}