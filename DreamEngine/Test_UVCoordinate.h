#pragma once
#include "Ancestor.h"
#include "Shader.h"

class Test_UVCoordinate :
	public AncestorNS::Ancestor
{
public:
	Test_UVCoordinate();
	~Test_UVCoordinate();

	void Init();
	void Start();
	void Render(double currentTime);
	void OnClear();
	void generate_texture(float * data, int width, int height);

private:
	GLuint      texture;
	GLuint      program;
	GLuint      vao;
	ShaderNS::Shader		*shader;

};

