#pragma once

#include <GL/gl3w.h>
#include <stdio.h>

namespace ShaderNS
{

	class Shader
	{
	public:
		Shader();
		~Shader();

		GLuint Load(const char * filename,
			GLenum shader_type = GL_FRAGMENT_SHADER,
#ifdef _DEBUG
			bool check_errors = true);
#else
			bool check_errors = false);
#endif

		GLuint link_from_shaders(const GLuint * shaders,
			int shader_count,
			bool delete_shaders,
			bool check_errors = false);
		

	};

	

}

