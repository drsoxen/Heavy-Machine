#include "ModelLoader.h"

void MakeNormals(vector<Verts> & verts, vector<Normals> & norms)
{
	for(int i = 0; i < verts.size(); i += 3)
	{
		Verts vA, vB;
		vA.x = verts[i].x - verts[i + 1].x;
		vA.y = verts[i].y - verts[i + 1].y;
		vA.z = verts[i].z - verts[i + 1].z;

		vB.x = verts[i + 1].x - verts[i + 2].x;
		vB.y = verts[i + 1].y - verts[i + 2].y;
		vB.z = verts[i + 1].z - verts[i + 2].z;

		Normals n;
		
		n.x = (vA.y * vB.z - vA.z * vB.y);
		n.y = (vA.z * vB.x - vA.x * vB.z);
		n.z = (vA.x * vB.y - vA.y * vB.x);

		float Normalize = sqrt((n.x*n.x) + (n.y*n.y) + (n.z*n.z));

		n.x /= Normalize;
		n.y /= Normalize;
		n.z /= Normalize;

		norms.push_back(n);
		norms.push_back(n);
		norms.push_back(n);
	}
}

Model * ModelLoader::Load(const char* FileName)
{
	Model * model = NULL;

	vector<int>Indecies;
	vector<int>texCoordIndices;
	vector<int>normalIndices;

	vector<Verts>UnOrderedVerts;
	vector<TexCoord>UnOrderedTex;
	vector<Normals>UnOrderedNormals;
	vector<Normals>UnOrderedFaceNormals;
	vector<Normals>UnOrderedVertexNormals;

	string FileLine = "";

	fstream stream;

	stream.open(FileName, std::ios::in);

	if(!stream.good())
		return model;

	float x = 0.0f, y = 0.0f, z = 0.0f;

	while(stream>>FileLine)
	{
		if(FileLine == "v")
		{
			stream >>FileLine;
			x = atof(FileLine.c_str());
			stream >>FileLine;
			y = atof(FileLine.c_str());
			stream >>FileLine;
			z = atof(FileLine.c_str());
			UnOrderedVerts.push_back(Verts(x,y,z));
		}

		else if (FileLine == "vt")
		{
			stream >>FileLine;
			x = atof(FileLine.c_str());
			stream >>FileLine;
			y = atof(FileLine.c_str());
			UnOrderedTex.push_back(TexCoord(x,1-y));			
		}

		else if (FileLine == "vn")
		{			
			stream >>FileLine;
			x = atof(FileLine.c_str());
			stream >>FileLine;
			y = atof(FileLine.c_str());
			stream >>FileLine;
			z = atof(FileLine.c_str());
			UnOrderedNormals.push_back(Normals(x,1-y,z));			
		}	

		else if (FileLine == "f")
		{
			vector<string> elementBuffer;
			string temp;

			string line;
			getline(stream, line);
			stringstream lineData(line);
				
			while(getline(lineData, temp, ' '))
			{
				stringstream indices(temp);
				while(getline(indices, temp, '/'))
				{
					if ( ! temp.empty() ) 
					{
						elementBuffer.push_back(temp.c_str());
					}
				}
			}

			int attributes = elementBuffer.size() / 3;

			Indecies.push_back(atoi(elementBuffer[attributes * 0].c_str()) - 1);
            Indecies.push_back(atoi(elementBuffer[attributes * 1].c_str()) - 1);
            Indecies.push_back(atoi(elementBuffer[attributes * 2].c_str()) - 1);

            if(attributes >= 2)
            {
                texCoordIndices.push_back(atoi(elementBuffer[attributes * 0 + 1].c_str()) - 1);
                texCoordIndices.push_back(atoi(elementBuffer[attributes * 1 + 1].c_str()) - 1);
                texCoordIndices.push_back(atoi(elementBuffer[attributes * 2 + 1].c_str()) - 1);
            }

			if(attributes >= 3)
            {
                normalIndices.push_back(atoi(elementBuffer[attributes * 0 + 2].c_str()) - 1);
                normalIndices.push_back(atoi(elementBuffer[attributes * 1 + 2].c_str()) - 1);
                normalIndices.push_back(atoi(elementBuffer[attributes * 2 + 2].c_str()) - 1);
            }
		}
	}

	stream.close();

	model = new Model;
	
	GenerateVector(UnOrderedVerts, Indecies);
	GenerateVector(UnOrderedTex, texCoordIndices);

	UnOrderedFaceNormals = UnOrderedNormals;
	UnOrderedVertexNormals = UnOrderedNormals;

	GenerateVector(UnOrderedVertexNormals, normalIndices);

	UnOrderedFaceNormals.clear();
	MakeNormals(UnOrderedVerts, UnOrderedFaceNormals);

	model->Verticies = UnOrderedVerts;
	model->TextureCoords = UnOrderedTex;
	model->NormalCoords = UnOrderedFaceNormals;	
	model->NormalVertexCoords = UnOrderedVertexNormals;	

	return model;
}

template<typename T>
void ModelLoader::GenerateVector(vector<T> &vec, const vector<int> &indices)
{
    vector<T> newVector;

    for(unsigned int i = 0; i < indices.size(); i++)
        newVector.push_back(vec[indices[i]]);

    vec = newVector;
}