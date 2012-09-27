#ifndef __MODEL_H_
#define __MODEL_H_

#ifdef _WIN32
#include <windows.h>
#endif

#include <string>
#include <vector>

#include "glee/GLee.h"
#include "glslshader.h"


using std::string;
using std::vector;

struct Verts 
{
    float x, y, z;
	Verts(){ x = y = z = 0.0f; }
    Verts(float x, float y, float z) 
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }
};

struct TexCoord
{
    float s, t;
	TexCoord(){ s = t = 0; }
    TexCoord(float s, float t)
    {
        this->s = s;
        this->t = t;
    }
};

struct Normals 
{
    float x, y, z;
	Normals(){ x = y = z = 0.0f; }
    Normals(float x, float y, float z) 
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }
};

struct Color 
{
    float r, g, b;
    Color(float r, float g, float b) 
    {
        this->r = r;
        this->g = g;
        this->b = b;
    }
};

class Model 
{
public:

	vector<Model*> m_ChildList;
	Model* m_Parent;

	Model();
	void Init();
	void Draw(GLSLProgram * shader);	

	vector<Verts>Verticies;
	vector<TexCoord>TextureCoords;
	vector<Normals>NormalCoords;
	vector<Normals>NormalVertexCoords;	
	
	float m_offsetX;
	float m_offsetY;
	float m_offsetZ;

	float m_RotateX;
	float m_RotateY;
	float m_RotateZ;

	bool m_ShowNormals;
	bool m_Lights;

	float m_Shininess;


	GLuint m_ID;

private:

    void generateVertices();
    void generateIndices();
    void generateTexCoords();
	void generateNormals();

    GLuint m_vertexBuffer;
    GLuint m_indexBuffer;
    GLuint m_colorBuffer;
    GLuint m_texCoordBuffer;
    GLuint m_normalCoordBuffer;

    vector<GLuint> m_indices;
};

#endif