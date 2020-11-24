#pragma once
#include <string>

class Texture
{
private:
	unsigned int m_RendererID;
	std::string m_Path;
	unsigned char* m_LocalBuffer;
	int m_Width, m_Height, m_BBP;
	std::string m_Type;
public:
	Texture(const std::string& filename, const std::string& type);
	~Texture();

	void Bind(unsigned int slot = 0) const;
	void Unbind() const;

	inline const std::string& GetType() const { return m_Type; }
	inline const std::string& GetPath() const { return m_Path; }
};