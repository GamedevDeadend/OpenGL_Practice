#pragma once
#include "Renderer.h"

class Texture
{
private:
	unsigned int m_RendererID;
	std::string m_Filepath;
	unsigned char* m_LocalBuffer;
	int m_Width; 
	int m_Height;
	/// <summary>
	/// Bytes Per Pixel RGBA
	/// </summary>
	int m_BPP;

public:

	Texture(const std::string& filePath);
	~Texture();

	void Bind(unsigned int slot = 0)const;
	void UnBind()const;

	inline unsigned int GetWidth()const { return m_Width; }
	inline unsigned int GetHeight()const { return m_Height; }

};
