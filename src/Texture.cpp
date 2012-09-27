#include "Texture.h"


Texture::Texture(string FilePath)
{
	if (!m_Texture.load(FilePath))
	{
		std::cerr << "Could not load the grass texture" << std::endl;

	}
}