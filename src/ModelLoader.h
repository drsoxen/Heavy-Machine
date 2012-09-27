#ifndef __MODELLOADER_H_
#define __MODELLOADER_H_

#include <fstream>
#include <string>
#include <sstream>
#include "Model.h"

using namespace std;

class ModelLoader 
{
public:

	static Model * Load(const char * FileName);
	template<typename T> static void GenerateVector(std::vector<T> &vec, const std::vector<int> &indices);
};

#endif