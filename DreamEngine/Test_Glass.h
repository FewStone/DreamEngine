#pragma once

#include "Ancestor.h"
#include "Texture.h"
#include "Shader.h"
#include "UserController.h"

class Test_Glass : public AncestorNS::Ancestor
{
public:
	Test_Glass();
	~Test_Glass();

	void Start();
	void Render(double currentTime);
	void OnClear();

private:
	GLuint      grass_buffer;
	GLuint      grass_vao;

	GLuint      grass_program;

	GLuint      tex_grass_color;
	GLuint      tex_grass_length;
	GLuint      tex_grass_orientation;
	GLuint      tex_grass_bend;

	struct
	{
		GLint   mvpMatrix;
	} uniforms;

	TextureNS::Texture *texture;
	ShaderNS::Shader *shader;
	UserController *userController;

};

