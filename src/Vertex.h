#ifndef __VERTEX_H_
#define __VERTEX_H_

class Vertex 
{
public:

	float X,Y,Z,R,G,B,A;

	Vertex()
	{
		X = Y = Z = R = G = B = A = 0.0f;
	}

	Vertex(float x,float y,float z,float r,float g,float b,float a)
	{
		X = x;
		Y = y;
		Z = z;
		R = r;
		G = g;
		B = b;
		A = a;
	}
};

#endif