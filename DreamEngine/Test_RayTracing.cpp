#include "Test_RayTracing.h"


Test_RayTracing::Test_RayTracing()
:prepare_program(0),
trace_program(0),
blit_program(0),
max_depth(1),
debug_depth(0),
debug_mode(DEBUG_NONE),
paused(false)
{
	shader = new ShaderNS::Shader();

	AncestorNS::Ancestor::Init();

}


Test_RayTracing::~Test_RayTracing()
{
}

void Test_RayTracing::Start()
{
	int i;

	load_shaders();

	glGenBuffers(1, &uniforms_buffer);
	glBindBuffer(GL_UNIFORM_BUFFER, uniforms_buffer);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(uniforms_block), NULL, GL_DYNAMIC_DRAW);

	glGenBuffers(1, &sphere_buffer);
	glBindBuffer(GL_UNIFORM_BUFFER, sphere_buffer);
	glBufferData(GL_UNIFORM_BUFFER, 128 * sizeof(sphere), NULL, GL_DYNAMIC_DRAW);

	glGenBuffers(1, &plane_buffer);
	glBindBuffer(GL_UNIFORM_BUFFER, plane_buffer);
	glBufferData(GL_UNIFORM_BUFFER, 128 * sizeof(plane), NULL, GL_DYNAMIC_DRAW);

	glGenBuffers(1, &light_buffer);
	glBindBuffer(GL_UNIFORM_BUFFER, light_buffer);
	glBufferData(GL_UNIFORM_BUFFER, 128 * sizeof(sphere), NULL, GL_DYNAMIC_DRAW);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenFramebuffers(MAX_RECURSION_DEPTH, ray_fbo);
	glGenTextures(1, &tex_composite);
	glGenTextures(MAX_RECURSION_DEPTH, tex_position);
	glGenTextures(MAX_RECURSION_DEPTH, tex_reflected);
	glGenTextures(MAX_RECURSION_DEPTH, tex_refracted);
	glGenTextures(MAX_RECURSION_DEPTH, tex_reflection_intensity);
	glGenTextures(MAX_RECURSION_DEPTH, tex_refraction_intensity);

	glBindTexture(GL_TEXTURE_2D, tex_composite);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB16F, MAX_FB_WIDTH, MAX_FB_HEIGHT);

	for (i = 0; i < MAX_RECURSION_DEPTH; i++)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, ray_fbo[i]);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, tex_composite, 0);

		glBindTexture(GL_TEXTURE_2D, tex_position[i]);
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB32F, MAX_FB_WIDTH, MAX_FB_HEIGHT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, tex_position[i], 0);

		glBindTexture(GL_TEXTURE_2D, tex_reflected[i]);
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB16F, MAX_FB_WIDTH, MAX_FB_HEIGHT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, tex_reflected[i], 0);

		glBindTexture(GL_TEXTURE_2D, tex_refracted[i]);
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB16F, MAX_FB_WIDTH, MAX_FB_HEIGHT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, tex_refracted[i], 0);

		glBindTexture(GL_TEXTURE_2D, tex_reflection_intensity[i]);
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB16F, MAX_FB_WIDTH, MAX_FB_HEIGHT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, tex_reflection_intensity[i], 0);

		glBindTexture(GL_TEXTURE_2D, tex_refraction_intensity[i]);
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB16F, MAX_FB_WIDTH, MAX_FB_HEIGHT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT5, tex_refraction_intensity[i], 0);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Test_RayTracing::Render(double currentTime)
{
	static const GLfloat zeros[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	static const GLfloat gray[] = { 0.1f, 0.1f, 0.1f, 0.0f };
	static const GLfloat ones[] = { 1.0f };
	static double last_time = 0.0;
	static double total_time = 0.0;

	if (!paused)
		total_time += (currentTime - last_time);
	last_time = currentTime;

	float f = (float)total_time;

	vmath::vec3 view_position = vmath::vec3(sinf(f * 0.3234f) * 28.0f, cosf(f * 0.4234f) * 28.0f, cosf(f * 0.1234f) * 28.0f); // sinf(f * 0.2341f) * 20.0f - 8.0f);
	vmath::vec3 lookat_point = vmath::vec3(sinf(f * 0.214f) * 8.0f, cosf(f * 0.153f) * 8.0f, sinf(f * 0.734f) * 8.0f);
	vmath::mat4 view_matrix = vmath::lookat(view_position,
		lookat_point,
		vmath::vec3(0.0f, 1.0f, 0.0f));

	glBindBufferBase(GL_UNIFORM_BUFFER, 0, uniforms_buffer);
	uniforms_block * block = (uniforms_block *)glMapBufferRange(GL_UNIFORM_BUFFER,
		0,
		sizeof(uniforms_block),
		GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);

	vmath::mat4 model_matrix = vmath::scale(7.0f);

	// f = 0.0f;

	block->mv_matrix = view_matrix * model_matrix;
	block->view_matrix = view_matrix;
	block->proj_matrix = vmath::perspective(50.0f,
		(float)info.windowWidth / (float)info.windowHeight,
		0.1f,
		1000.0f);

	glUnmapBuffer(GL_UNIFORM_BUFFER);

	glBindBufferBase(GL_UNIFORM_BUFFER, 1, sphere_buffer);
	sphere * s = (sphere *)glMapBufferRange(GL_UNIFORM_BUFFER, 0, 128 * sizeof(sphere), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);

	int i;

	for (i = 0; i < 128; i++)
	{
		// float f = 0.0f;
		float fi = (float)i / 128.0f;
		s[i].center = vmath::vec3(sinf(fi * 123.0f + f) * 15.75f, cosf(fi * 456.0f + f) * 15.75f, (sinf(fi * 300.0f + f) * cosf(fi * 200.0f + f)) * 20.0f);
		s[i].radius = fi * 2.3f + 3.5f;
		float r = fi * 61.0f;
		float g = r + 0.25f;
		float b = g + 0.25f;
		r = (r - floorf(r)) * 0.8f + 0.2f;
		g = (g - floorf(g)) * 0.8f + 0.2f;
		b = (b - floorf(b)) * 0.8f + 0.2f;
		s[i].color = vmath::vec4(r, g, b, 1.0f);
	}

	glUnmapBuffer(GL_UNIFORM_BUFFER);

	glBindBufferBase(GL_UNIFORM_BUFFER, 2, plane_buffer);
	plane * p = (plane *)glMapBufferRange(GL_UNIFORM_BUFFER, 0, 128 * sizeof(plane), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);

	//for (i = 0; i < 1; i++)
	{
		p[0].normal = vmath::vec3(0.0f, 0.0f, -1.0f);
		p[0].d = 30.0f;

		p[1].normal = vmath::vec3(0.0f, 0.0f, 1.0f);
		p[1].d = 30.0f;

		p[2].normal = vmath::vec3(-1.0f, 0.0f, 0.0f);
		p[2].d = 30.0f;

		p[3].normal = vmath::vec3(1.0f, 0.0f, 0.0f);
		p[3].d = 30.0f;

		p[4].normal = vmath::vec3(0.0f, -1.0f, 0.0f);
		p[4].d = 30.0f;

		p[5].normal = vmath::vec3(0.0f, 1.0f, 0.0f);
		p[5].d = 30.0f;
	}

	glUnmapBuffer(GL_UNIFORM_BUFFER);

	glBindBufferBase(GL_UNIFORM_BUFFER, 3, light_buffer);
	light * l = (light *)glMapBufferRange(GL_UNIFORM_BUFFER, 0, 128 * sizeof(light), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);

	f *= 1.0f;

	for (i = 0; i < 128; i++)
	{
		float fi = 3.33f - (float)i; //  / 35.0f;
		l[i].position = vmath::vec3(sinf(fi * 2.0f - f) * 15.75f,
			cosf(fi * 5.0f - f) * 5.75f,
			(sinf(fi * 3.0f - f) * cosf(fi * 2.5f - f)) * 19.4f);
	}

	glUnmapBuffer(GL_UNIFORM_BUFFER);

	glBindVertexArray(vao);
	glViewport(0, 0, info.windowWidth, info.windowHeight);

	glUseProgram(prepare_program);
	glUniformMatrix4fv(uniforms.ray_lookat, 1, GL_FALSE, view_matrix);
	glUniform3fv(uniforms.ray_origin, 1, view_position);
	glUniform1f(uniforms.aspect, (float)info.windowHeight / (float)info.windowWidth);
	glBindFramebuffer(GL_FRAMEBUFFER, ray_fbo[0]);
	static const GLenum draw_buffers[] =
	{
		GL_COLOR_ATTACHMENT0,
		GL_COLOR_ATTACHMENT1,
		GL_COLOR_ATTACHMENT2,
		GL_COLOR_ATTACHMENT3,
		GL_COLOR_ATTACHMENT4,
		GL_COLOR_ATTACHMENT5
	};
	glDrawBuffers(6, draw_buffers);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glUseProgram(trace_program);
	recurse(0);

	glPolygonOffset(4.0f, 4.0f);

	glUseProgram(blit_program);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDrawBuffer(GL_BACK);

	glEnable(GL_MULTISAMPLE);

	glActiveTexture(GL_TEXTURE0);
	switch (debug_mode)
	{
	case DEBUG_NONE:
		glBindTexture(GL_TEXTURE_2D, tex_composite);
		break;
	case DEBUG_REFLECTED:
		glBindTexture(GL_TEXTURE_2D, tex_reflected[debug_depth]);
		break;
	case DEBUG_REFRACTED:
		glBindTexture(GL_TEXTURE_2D, tex_refracted[debug_depth]);
		break;
	case DEBUG_REFLECTED_COLOR:
		glBindTexture(GL_TEXTURE_2D, tex_reflection_intensity[debug_depth]);
		break;
	case DEBUG_REFRACTED_COLOR:
		glBindTexture(GL_TEXTURE_2D, tex_refraction_intensity[debug_depth]);
		break;
	}
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void Test_RayTracing::load_shaders()
{
	GLuint      shaders[2];

	shaders[0] = shader->Load("C:/Media/Shaders/Test_Raytracing/trace-prepare.vs.glsl", GL_VERTEX_SHADER);
	shaders[1] = shader->Load("C:/Media/Shaders/Test_Raytracing/trace-prepare.fs.glsl", GL_FRAGMENT_SHADER);

	if (prepare_program != 0)
		glDeleteProgram(prepare_program);

	prepare_program = shader->link_from_shaders(shaders, 2, true);

	uniforms.ray_origin = glGetUniformLocation(prepare_program, "ray_origin");
	uniforms.ray_lookat = glGetUniformLocation(prepare_program, "ray_lookat");
	uniforms.aspect = glGetUniformLocation(prepare_program, "aspect");

	shaders[0] = shader->Load("C:/Media/Shaders/Test_Raytracing/raytracer.vs.glsl", GL_VERTEX_SHADER);
	shaders[1] = shader->Load("C:/Media/Shaders/Test_Raytracing/raytracer.fs.glsl", GL_FRAGMENT_SHADER);

	if (trace_program)
		glDeleteProgram(trace_program);

	trace_program = shader->link_from_shaders(shaders, 2, true);

	shaders[0] = shader->Load("C:/Media/Shaders/Test_Raytracing/blit.vs.glsl", GL_VERTEX_SHADER);
	shaders[1] = shader->Load("C:/Media/Shaders/Test_Raytracing/blit.fs.glsl", GL_FRAGMENT_SHADER);

	if (blit_program)
		glDeleteProgram(blit_program);

	blit_program = shader->link_from_shaders(shaders, 2, true);
}

void Test_RayTracing::recurse(int depth)
{
	glBindFramebuffer(GL_FRAMEBUFFER, ray_fbo[depth + 1]);

	static const GLenum draw_buffers[] =
	{
		GL_COLOR_ATTACHMENT0,
		GL_COLOR_ATTACHMENT1,
		GL_COLOR_ATTACHMENT2,
		GL_COLOR_ATTACHMENT3,
		GL_COLOR_ATTACHMENT4,
		GL_COLOR_ATTACHMENT5
	};
	glDrawBuffers(6, draw_buffers);

	glEnablei(GL_BLEND, 0);
	glBlendFunci(0, GL_ONE, GL_ONE);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex_position[depth]);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, tex_reflected[depth]);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, tex_reflection_intensity[depth]);

	// Render
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	if (depth != (max_depth - 1))
	{
		recurse(depth + 1);
	}

	glDisablei(GL_BLEND, 0);
}

void Test_RayTracing::onKey(int key, int action)
{
	if (action)
	{
		switch (key)
		{
		case GLFW_KEY_KP_ADD:
			max_depth++;
			if (max_depth > MAX_RECURSION_DEPTH)
				max_depth = MAX_RECURSION_DEPTH;
			break;
		case GLFW_KEY_KP_SUBTRACT:
			max_depth--;
			if (max_depth < 1)
				max_depth = 1;
			break;
		case 'P':
			paused = !paused;
			break;
		case 'R':
			load_shaders();
			break;
		case 'Q':
			debug_mode = DEBUG_NONE;
			break;
		case 'W':
			debug_mode = DEBUG_REFLECTED;
			break;
		case 'E':
			debug_mode = DEBUG_REFRACTED;
			break;
		case 'S':
			debug_mode = DEBUG_REFLECTED_COLOR;
			break;
		case 'D':
			debug_mode = DEBUG_REFRACTED_COLOR;
			break;
		case 'A':
			debug_depth++;
			if (debug_depth > MAX_RECURSION_DEPTH)
				debug_depth = MAX_RECURSION_DEPTH;
			break;
		case 'Z':
			debug_depth--;
			if (debug_depth < 0)
				debug_depth = 0;
			break;
		}
	}
}


