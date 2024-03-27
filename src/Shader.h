#pragma once
#include<string>
#include <unordered_map>

struct parsedShaders
{
	std::string VertexCode;
	std::string FragmentCode;
};

class Shader 
{
private:
	unsigned int m_RendererID;
	std::unordered_map<std::string, int> m_UniformLocationCache;

public:
	Shader(const std::string& filepath);
	~Shader();

	void Bind()const;
	void UnBind()const;

	/// <summary>
	/// Set Uniform
	/// </summary>
	void SetUnifom4f(const std::string& name, float v0, float v1, float v2, float v3);

private:

	int GetUniformLocation(const std::string& name);
	parsedShaders parseShader(const std::string& path);
	unsigned int CreateShader(const std::string& VertexShader, const std::string& FragmentShader);
	unsigned int CompileShader(unsigned int type, const std::string& source);

};

