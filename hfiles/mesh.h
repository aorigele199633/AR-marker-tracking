#pragma once
#pragma once

#include "vector"
#include "iostream"
#include "string"
#include "fstream"
#include "sstream"
#include "algorithm"
#include "assert.h"
#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>


#pragma comment(lib, "glut.lib")

using namespace std;

struct vec3
{
	double x, y, z;
};

struct vec2
{
	double x, y;
};

class Vertex
{
public:
	int vertIndex;     // �˶����ڶ����б��е�����
	int normIndex;     // ����ķ�������
	int textIndex;     // �������������
};

class Face
{
public:
	vector<Vertex> vertex; // ����ͷ���������ɵ��б�

	Face() {}
	~Face() {}
};

class Mesh
{
private:
	vector<vec3> vVertex;  // ��������
	vector<vec2> vText;    // ��������
	vector<vec3> vNorm;    // ��������
	vector<Face> vFace;    // ��Ƭ����

public:
	Mesh() {};
	~Mesh() {};

	bool readFile(char* path);
	void drawMesh();
};

bool Mesh::readFile(char* path)
{
	ifstream file(path);
	if (!file)
	{
		cerr << "Error::ObjLoader, could not open obj file:"
			<< path << " for reading." << std::endl;
		return false;
	}
	string line;
	while (getline(file, line))
	{
		if (line.substr(0, 2) == "vt")     // ����������������
		{
			istringstream s(line.substr(2));
			vec2 v;
			s >> v.x; s >> v.y;
			//cout << "vt " << v.x << " " << v.y << endl;
			v.y = -v.y;                     // ע��������ص�dds���� Ҫ��y���з�ת
			vText.push_back(v);
		}
		else if (line.substr(0, 2) == "vn") // ���㷨��������
		{
			istringstream s(line.substr(2));
			vec3 v;
			s >> v.x; s >> v.y; s >> v.z;
			//cout << "vn " << v.x << " " << v.y << " " << v.z << endl;
			vNorm.push_back(v);
		}
		else if (line.substr(0, 1) == "v")  // ����λ������
		{
			istringstream s(line.substr(1));
			vec3 v;
			s >> v.x; s >> v.y; s >> v.z;
			//cout << "v " << v.x << " " << v.y << " " << v.z << endl;
			vVertex.push_back(v);
		}
		else if (line.substr(0, 1) == "f")  // ������
		{
			Face face;
			//cout << "f ";
			istringstream vtns(line.substr(1));
			string vtn;
			while (vtns >> vtn)             // ����һ���ж����������
			{
				Vertex vertex;
				replace(vtn.begin(), vtn.end(), '/', ' ');
				istringstream ivtn(vtn);
				if (vtn.find("  ") != string::npos) // û���������ݣ�ע��������2���ո�
				{
					ivtn >> vertex.vertIndex
						>> vertex.normIndex;

					vertex.vertIndex--;     //ʹ���±��0��ʼ
					vertex.normIndex--;
				}
				else
				{
					ivtn >> vertex.vertIndex
						>> vertex.textIndex
						>> vertex.normIndex;

					//cout <<  vertex.vertIndex << "/" << vertex.textIndex << "/" << vertex.normIndex << " ";
					vertex.vertIndex--;
					vertex.textIndex--;
					vertex.normIndex--;
				}
				face.vertex.push_back(vertex);
			}
			vFace.push_back(face);
			//cout << endl;
		}
		else if (line[0] == '#')            // ע�ͺ���
		{
		}
		else
		{
			// �������� ��ʱ������
		}
	}

	return true;
}

void Mesh::drawMesh()
{
	if (vFace.empty())
		return;

	// ������
	if (vText.size() > 0)
	{
		for (int f = 0; f < vFace.size(); f++)  // ����ÿ����Ƭ
		{
			int n = vFace[f].vertex.size();    // ��Ķ�����
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glBegin(GL_TRIANGLES);
			glColor3f(1, 1, 1);
			for (int v = 0; v < n; v++)
			{
				int it = vFace[f].vertex[v].textIndex;
				glTexCoord2f(vText[it].x, vText[it].y);

				int in = vFace[f].vertex[v].normIndex;
				glNormal3f(vNorm[in].x, vNorm[in].y, vNorm[in].z);

				int iv = vFace[f].vertex[v].vertIndex;
				glVertex3f(vVertex[iv].x, vVertex[iv].y, vVertex[iv].z);
			}
			glEnd();

			glBegin(GL_LINE_LOOP);
			glColor3f(0, 0, 1);
			for (int v = 0; v < n; v++)
			{
				int it = vFace[f].vertex[v].textIndex;
				glTexCoord2f(vText[it].x, vText[it].y);

				int in = vFace[f].vertex[v].normIndex;
				glNormal3f(vNorm[in].x, vNorm[in].y, vNorm[in].z);

				int iv = vFace[f].vertex[v].vertIndex;
				glVertex3f(vVertex[iv].x, vVertex[iv].y, vVertex[iv].z);
			}
			glEnd();
		}
	}
	// û������
	else
	{
		for (int f = 0; f < vFace.size(); f++)  // ����ÿ����Ƭ
		{
			int n = vFace[f].vertex.size();    // ��Ķ�����
			glBegin(GL_TRIANGLES);
			for (int v = 0; v < n; v++)
			{
				int in = vFace[f].vertex[v].normIndex;
				glNormal3f(vNorm[in].x, vNorm[in].y, vNorm[in].z);

				int iv = vFace[f].vertex[v].vertIndex;
				glVertex3f(vVertex[iv].x, vVertex[iv].y, vVertex[iv].z);
			}
			glEnd();
		}
	}
}

	//glFlush();
