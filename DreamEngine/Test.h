#pragma once
#include "Ancestor.h"
#include "Shader.h"

class Test :
	public AncestorNS::Ancestor
{
public:
	Test();
	~Test();

	void Start();
	void Render(double currentTime);
	void OnClear();
	void onResize(int w, int h);

private:
	GLuint          program;
	GLuint          vao;
	GLuint          buffer;
	GLint           mv_location;
	GLint           proj_location;

	float           aspect;
	glm::mat4     proj_matrix;

	ShaderNS::Shader *shader;

};

