#pragma once
#include "Ancestor.h"
#include "Shader.h"
#include "Math.h"

class Test_Flocking :
	public AncestorNS::Ancestor
{
public:
	Test_Flocking();
	~Test_Flocking();

	void Start();
	void Render(double currentTime);
	void load_shaders();

private:
	ShaderNS::Shader *shader;

	GLuint      flock_update_program;
	GLuint      flock_render_program;

	GLuint      flock_buffer[2];

	GLuint      flock_render_vao[2];
	GLuint      geometry_buffer;

	struct flock_member
	{
		vmath::vec3 position;
		unsigned int : 32;
		vmath::vec3 velocity;
		unsigned int : 32;
	};

	struct
	{
		struct
		{
			GLint       goal;
		} update;
		struct
		{
			GLuint      mvp;
		} render;
	} uniforms;

	GLuint      frame_index;

	enum
	{
		WORKGROUP_SIZE = 256,
		NUM_WORKGROUPS = 64,
		FLOCK_SIZE = (NUM_WORKGROUPS * WORKGROUP_SIZE)
	};

};

