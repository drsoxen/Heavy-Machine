
#include <fstream>
#include <iostream>

#include"Model.h"
#include "example.h"

Model::Model()
{
  m_vertexBuffer = m_indexBuffer = m_colorBuffer = 0;
  m_offsetX = 0;
  m_offsetY = 0;
  m_offsetZ = 0;
  m_RotateX = 0;
  m_RotateY = 0;
  m_RotateZ = 0;
  m_ShowNormals = false;
  m_Lights = true;
  m_Shininess = 25;
}

void Model::Init()
{
	generateVertices();
	generateTexCoords();
	generateNormals();
}

void Model::generateVertices()
{
  glGenBuffers(1, &m_vertexBuffer); //Generate a buffer for the vertices
  glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer); //Bind the vertex buffer
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * Verticies.size() * 3, &Verticies[0], GL_STATIC_DRAW); //Send the data to OpenGL
}

void Model::generateTexCoords()
{
  glGenBuffers(1, &m_texCoordBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, m_texCoordBuffer); //Bind the vertex buffer
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * TextureCoords.size() * 2, &TextureCoords[0], GL_STATIC_DRAW); //Send the data to OpenGL
}

void Model::generateNormals()
{
  glGenBuffers(1, &m_normalCoordBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, m_normalCoordBuffer); //Bind the vertex buffer
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * NormalCoords.size() * 3, &NormalCoords[0], GL_STATIC_DRAW); //Send the data to OpenGL
}

void Model::Draw(GLSLProgram * shader)
{
	glPushMatrix();

	glBindTexture(GL_TEXTURE_2D, m_ID);
	shader->sendUniform("texture0", 0); 
	shader->sendUniform("material_shininess", m_Shininess);
	shader->sendUniform("Lights", m_Lights); 

	glTranslatef(m_offsetX,m_offsetY,m_offsetZ);

	glRotatef(m_RotateX,1,0,0);
	glRotatef(m_RotateY,0,1,0);
	glRotatef(m_RotateZ,0,0,1);
		
	float modelviewMatrix[16];

	glGetFloatv(GL_MODELVIEW_MATRIX, modelviewMatrix);
	shader->sendUniform4x4("modelview_matrix", modelviewMatrix);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
	glVertexAttribPointer((GLint)0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_texCoordBuffer);
	glVertexAttribPointer((GLint)1, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_normalCoordBuffer);
	glVertexAttribPointer((GLint)2, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glDrawArrays(GL_TRIANGLES,0, Verticies.size());

	if(m_ShowNormals)
	{			
		for (int j = 0;j<Verticies.size();j+=3)
		{
			glBegin(GL_LINES);
			
			float VTempX = ((Verticies[j].x + Verticies[j+1].x + Verticies[j+2].x) /3);
			float VTempY = ((Verticies[j].y + Verticies[j+1].y + Verticies[j+2].y) /3);
			float VTempZ = ((Verticies[j].z + Verticies[j+1].z + Verticies[j+2].z) /3);

			float NTempX = ((NormalCoords[j].x + NormalCoords[j+1].x + NormalCoords[j+2].x) /3);
			float NTempY = ((NormalCoords[j].y + NormalCoords[j+1].y + NormalCoords[j+2].y) /3);
			float NTempZ = ((NormalCoords[j].z + NormalCoords[j+1].z + NormalCoords[j+2].z) /3);

			glVertex3f(VTempX,VTempY,VTempZ);

			glVertex3f(VTempX + NTempX, VTempY + NTempY, VTempZ + NTempZ);

			glEnd();
		}
	}

	
	for(int i = 0; i<m_ChildList.size();i++)
	{
		m_ChildList[i]->Draw(shader);
	}

	glPopMatrix();

}