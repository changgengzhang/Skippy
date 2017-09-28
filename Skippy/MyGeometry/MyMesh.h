#ifndef MY_MESH_H
#define MY_MESH_H

#include <qvector.h>
#include <qstring.h>
#include <qdebug.h>
#include <qfile.h>

#include <glm/glm.hpp>

#include "MyTools/SparseMatrix.h"
#include "MyDataStruct.h"
#include "MyBox.h"

class MyMesh 
{

public:

	// ============ values for mesh ==============
	int m_vertexCount;
	int m_faceCount;
	int m_boundaryVertexCount;
	// basic vertex and face
	QVector<glm::vec3> m_vertices;
	QVector<glm::vec3> m_originalVertices;
	QVector<Face> m_faces;
	QVector<uint> m_indices;

	// Other per-vertex properties
	QVector<glm::vec3> m_colors;

	// normal
	QVector<glm::vec3> m_normals;
	QVector<glm::vec3> m_faceNormals;

	QVector<bool> m_isBoundary;
	//QVector<glm::vec2> m_textureCoord;

	// ============ sparse matrix for vertex-vertex vertex-face face-face =============
	SparseMatrix<int>* m_adjacentVV;
	SparseMatrix<int>* m_adjacentVF;
	SparseMatrix<int>* m_adjacentFF;

	AABBox m_box;

public:
	MyMesh();
	~MyMesh();

	bool setupMeshFromFile(QString filePath);
	bool needScaleToUintBox();
	bool needMoveToCenter();
	bool needNormal();
	bool needBoundary();
	bool needAdjacentVertexToVertex();
	bool needAdjacentVertexToFace();
	bool needAdjacentFaceToFace();
	bool needAABBOX();
	// bool needBSphere();

private:
	bool parseFile(QString filePath);
	// AABBB
	const glm::vec3 computeMaxCoord() const;
	const glm::vec3 computeMinCoord() const;
	
};

#endif
