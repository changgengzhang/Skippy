#include "MyMesh.h"

MyMesh::MyMesh()
{
	m_vertexCount = 0;
	m_faceCount = 0;
	m_boundaryVertexCount = 0;

	m_adjacentVV = nullptr;
	m_adjacentVF = nullptr;
	m_adjacentFF = nullptr;
	
}


MyMesh::~MyMesh()
{
}

bool MyMesh::setupMeshFromFile(QString filePath)
{
	// only support .obj
	QString type = filePath.split(".").last();
	if (type == "obj")
	{
		bool ret = this->parseFile(filePath);
		if (ret)
		{
			m_vertexCount = m_vertices.count();
			m_faceCount = m_faces.count();
			return true;
		}
		else
		{
			qErrnoWarning("Can not load mesh from file");
			return false;
		}

	}
	else if (type == "ply")
	{
		return false;
	}
	return false;
}


bool MyMesh::parseFile(QString filePath)
{
	QFile file(filePath);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		qErrnoWarning("Can not open model file");
		return false;
	}
	QTextStream stream(&file);

	QString line, dataType;
	QStringList tokens, tokens2;
	float x, y, z;
	while (stream.readLineInto(&line))
	{
		line = line.trimmed();		// remove the leading and trailing space
		tokens = line.split(QRegExp("\\s+"));
		dataType = tokens.first().toLower();

		if (dataType == "v")
		{
			x = tokens.at(1).toFloat();
			y = tokens.at(2).toFloat();
			z = tokens.at(3).toFloat();
			m_vertices.push_back(glm::vec3(x, y, z));
			m_originalVertices.push_back(glm::vec3(x, y, z));
		}
		else if (dataType == "f")
		{
			int fi1 = tokens.at(1).split("/").at(0).toUInt() - 1;
			int fi2 = tokens.at(2).split("/").at(0).toUInt() - 1;
			int fi3 = tokens.at(3).split("/").at(0).toUInt() - 1;

			m_faces.push_back(Face(fi1, fi2, fi3));
			m_indices.push_back(fi1);
			m_indices.push_back(fi2);
			m_indices.push_back(fi3);
		}
	}

	return true;
}


// 将model缩放至unitBox中
bool MyMesh::needScaleToUintBox()
{
	glm::vec3 dist = this->computeMaxCoord() - this->computeMinCoord();

	float scale = (dist.x > dist.y) ? (dist.x > dist.z ? dist.x : dist.z) : (dist.y > dist.z ? dist.y : dist.z);

	if (scale <= 0)
	{
		qErrnoWarning("Scale to unit box failed");
		return false;
	}

	for (uint i = 0; i < m_vertices.count(); i++)
	{
		m_vertices[i] /= scale;
	}
	return true;
}


bool MyMesh::needMoveToCenter()
{
	glm::vec3 center = (this->computeMaxCoord() + this->computeMinCoord()) / 2.0f;

	QString out;
	for (uint i = 0; i < m_vertexCount; i++)
	{
		m_vertices[i] -= center;
	}
	return true;
}


/*
同时计算了面的法向量和点的法向量
*/
bool MyMesh::needNormal()
{
	glm::vec3 *vnormal;
	glm::vec3 faceNormal;
	glm::vec3 p0, p1, p2;
	glm::vec3 v01, v02, v12;
	uint v0, v1, v2;
	float lenV01, lenV02, lenV12;

	vnormal = new glm::vec3[m_vertexCount];

	for (uint i = 0; i < m_faceCount; i++)
	{
		// index of vertex in one face
		v0 = m_faces[i][0];
		v1 = m_faces[i][1];
		v2 = m_faces[i][2];
		// position of vertex in one face
		p0 = m_vertices.at(v0);
		p1 = m_vertices.at(v1);
		p2 = m_vertices.at(v2);
		/*
		in trimesh2 compute normal in this way
		QVector3D a = p0-p1, b = p1-p2, c = p2-p0;
		QVector3D facenormal = flip ? (b CROSS a) : (a CROSS b);
		*/
		v01 = p1 - p0;
		v02 = p2 - p0;
		v12 = p2 - p1;

		lenV01 = glm::length(v01);
		lenV02 = glm::length(v02);
		lenV12 = glm::length(v12);

		faceNormal = glm::cross(v01, v02);

		// vertex normal is average of face normal which belongs to
		vnormal[v0] += faceNormal * (1.0f / (lenV01 + lenV02));
		vnormal[v1] += faceNormal * (1.0f / (lenV01 + lenV12));
		vnormal[v2] += faceNormal * (1.0f / (lenV02 + lenV12));

		faceNormal = glm::normalize(faceNormal);
		m_faceNormals.push_back(faceNormal);
	}

	for (uint i = 0; i < m_vertexCount; i++)
	{
		vnormal[i] = glm::normalize(vnormal[i]);
		m_normals.push_back(vnormal[i]);
	}

	delete[] vnormal;

	return true;
}


/*
在计算边界时，一定需要邻接的vertex-vertex, vrtex-face关系
*/
bool MyMesh::needBoundary()
{
	int numAdjV, numAdjF;

	m_boundaryVertexCount = 0;
	for (uint i = 0; i < m_vertexCount; i++)
	{
		numAdjV = m_adjacentVV->getOneRowValueCount(i);
		numAdjF = m_adjacentVF->getOneRowValueCount(i);
		m_isBoundary.append(numAdjV != numAdjF);
		m_boundaryVertexCount += numAdjV != numAdjF ? 1 : 0;
	}
	return true;
}


bool MyMesh::needAdjacentVertexToVertex()
{
	m_adjacentVV = new SparseMatrix<int>(m_vertexCount);

	int p0, p1, p2;
	for (uint i = 0; i < m_faceCount; i++)
	{
		p0 = m_faces[i][0];
		p1 = m_faces[i][1];
		p2 = m_faces[i][2];

		// (i, j) = 1 means that there is edge between i and j, the direction is i to j
		// (i, j) = -1 means that there is edge between i and j, the direction is j to i
		m_adjacentVV->set(1, p0, p1);
		m_adjacentVV->set(1, p1, p2);
		m_adjacentVV->set(1, p2, p0);
		m_adjacentVV->setIfNotExist(-1, p0, p2);
		m_adjacentVV->setIfNotExist(-1, p2, p1);
		m_adjacentVV->setIfNotExist(-1, p1, p0);
	}
	return true;
}


bool MyMesh::needAdjacentVertexToFace()
{
	m_adjacentVF = new SparseMatrix<int>(m_vertexCount, m_faceCount);

	for (uint i = 0; i < m_faceCount; i++)
	{
		// the face index start form zero
		m_adjacentVF->set(1, m_faces[i][0], i);
		m_adjacentVF->set(1, m_faces[i][1], i);
		m_adjacentVF->set(1, m_faces[i][2], i);
	}
	return true;
}


/*
在计算face-face关系时，需要先计算 vertex-face关系
*/
bool MyMesh::needAdjacentFaceToFace()
{
	m_adjacentFF = new SparseMatrix<int>(m_faceCount);

	int v0, v1, v2;
	uint faceIndex;
	QVector<int> oneRow;

	for (uint i = 0; i < m_faceCount; i++)
	{
		v0 = m_faces[i][0];
		v1 = m_faces[i][1];
		v2 = m_faces[i][2];

		oneRow = m_adjacentVF->getOneRowColIndex(v0);
		for (uint j = 0; j < oneRow.count(); j++)
		{
			faceIndex = oneRow.at(j);
			// must share one edge if two face is adjacent.
			if (faceIndex != i &&  m_faces[faceIndex].indexof(v1)!= -1)
			{
				// assume face index is start from zero
				m_adjacentFF->set(1, i, faceIndex);
			}
		}

		oneRow = m_adjacentVF->getOneRowColIndex(v1);
		for (uint j = 0; j < oneRow.count(); j++)
		{
			faceIndex = oneRow.at(j);
			if (faceIndex != i &&  m_faces[faceIndex].indexof(v2) != -1)
			{
				m_adjacentFF->set(1, i, faceIndex);
			}
		}

		oneRow = m_adjacentVF->getOneRowColIndex(v2);
		for (uint j = 0; j < oneRow.count(); j++)
		{
			faceIndex = oneRow.at(j);
			if (faceIndex != i &&  m_faces[faceIndex].indexof(v0) != -1)
			{
				m_adjacentFF->set(1, i, faceIndex);
			}
		}
	}
	return true;
}

bool MyMesh::needAABBOX()
{
	//计算 
	m_box.setBound(this->computeMinCoord());
	m_box.setBound(this->computeMaxCoord());
	return true;
}

// 计算每个分量的最大值
const glm::vec3  MyMesh::computeMaxCoord() const
{
	glm::vec3 maxCoord = glm::vec3(std::numeric_limits<float>::min());
	glm::vec3 vertex;

	for (uint i = 0; i < m_vertexCount; i++)
	{
		vertex = m_vertices[i];
		maxCoord = glm::vec3(
			vertex.x > maxCoord.x ? vertex.x : maxCoord.x,
			vertex.y > maxCoord.y ? vertex.y : maxCoord.y,
			vertex.z > maxCoord.z ? vertex.z : maxCoord.z
		);
	}

	return maxCoord;
}


//计算每个分量最小值
const glm::vec3 MyMesh::computeMinCoord() const
{
	glm::vec3 minCoord = glm::vec3(std::numeric_limits<float>::max());
	glm::vec3 vertex;

	for (uint i = 0; i < m_vertexCount; i++)
	{
		vertex = m_vertices[i];
		minCoord = glm::vec3(
			vertex.x < minCoord.x ? vertex.x : minCoord.x,
			vertex.y < minCoord.y ? vertex.y : minCoord.y,
			vertex.z < minCoord.z ? vertex.z : minCoord.z
		);
	}

	return minCoord;
}

