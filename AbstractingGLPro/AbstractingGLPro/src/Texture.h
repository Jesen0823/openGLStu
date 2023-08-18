#pragma once
#include "Renderer.h"

class Texture
{
private:
	unsigned int m_RendererID;
	std::string m_FilePath;
	unsigned char* m_LocalBuffer;
	int m_Width, m_Height, m_BPP;  // m_BPP ÿ����λ��

public:
	Texture(const std::string& path);
	~Texture();

	/* slot �ۣ�����Ĳ�ۣ�Ĭ�Ͽ�ѡ����0*/
	void Bind(unsigned int slot = 0) const;
	void UnBind() const;

	inline int GetWidth() const { return m_Width; }
	inline int GetHeight() const { return m_Height; }
};