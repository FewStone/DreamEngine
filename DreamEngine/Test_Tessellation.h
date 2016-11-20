#pragma once
#include "Ancestor.h"
#include "Shader.h"

class Test_Tessellation :
	public AncestorNS::Ancestor
{
public:
	Test_Tessellation();
	~Test_Tessellation();

	void Start();
	void Render(double currentTime);
	void OnClear();

private:
	ShaderNS::Shader *shader;

	GLuint shaderProgram;
	GLuint vao;
	GLuint vertexBuffer;



};

