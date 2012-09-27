#ifndef TEXTURE_H
#define TEXTURE_H

#ifdef _WIN32
#include <windows.h>
#endif
#include <iostream>

#include <vector>
#include <string>
#include "vertex.h"
#include "Model.h"
#include "targa.h"
#include "ModelLoader.h"

using namespace std;

class Texture
{
public:
	Texture(string FilePath);
	TargaImage m_Texture;


};

#endif