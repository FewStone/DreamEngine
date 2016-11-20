#pragma once
#include "Ancestor.h"
#include "Shader.h"

class Test_MultiViewport :
	public AncestorNS::Ancestor
{
public:
	Test_MultiViewport();
	~Test_MultiViewport();

	void Start();
	void Render(double currentTime);
	void OnClear();

private:
	ShaderNS::Shader *shader;

	GLuint          program;
	GLuint          vao;
	GLuint          position_buffer;
	GLuint          index_buffer;
	GLuint          uniform_buffer;
	GLint           mv_location;
	GLint           proj_location;

};

