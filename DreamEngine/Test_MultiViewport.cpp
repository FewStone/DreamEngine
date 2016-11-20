#include "Test_MultiViewport.h"

#include <glm/glm.hpp>

Test_MultiViewport::Test_MultiViewport()
{
	shader = new ShaderNS::Shader();
}


Test_MultiViewport::~Test_MultiViewport()
{
}

void Test_MultiViewport::Start()
{
	program = glCreateProgram();

	GLuint vs = shader->Load("C:/Media/Shaders/Test_MultiViewport/Test_MultiViewport.vs.glsl",GL_VERTEX_SHADER);
	GLuint	gs = shader->Load("C:/Media/Shaders/Test_MultiViewport/Test_MultiViewport.gs.glsl", GL_GEOMETRY_SHADER);
	GLuint	fs = shader->Load("C:/Media/Shaders/Test_MultiViewport/Test_MultiViewport.fs.glsl", GL_FRAGMENT_SHADER);

	glAttachShader(program, vs);
	glAttachShader(program, gs);
	glAttachShader(program, fs);

	glLinkProgram(program);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	static const GLushort vertex_indices[] =
	{
		0, 1, 2,
		2, 1, 3,
		2, 3, 4,
		4, 3, 5,
		4, 5, 6,
		6, 5, 7,
		6, 7, 0,
		0, 7, 1,
		6, 0, 2,
		2, 4, 6,
		7, 5, 3,
		7, 3, 1
	};

	static const GLfloat vertex_positions[] =
	{
		-0.25f, -0.25f, -0.25f,
		-0.25f,  0.25f, -0.25f,
		0.25f, -0.25f, -0.25f,
		0.25f,  0.25f, -0.25f,
		0.25f, -0.25f,  0.25f,
		0.25f,  0.25f,  0.25f,
		-0.25f, -0.25f,  0.25f,
		-0.25f,  0.25f,  0.25f,
	};

	glGenBuffers(1, &position_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, position_buffer);
	glBufferData(GL_ARRAY_BUFFER,
		sizeof(vertex_positions),
		vertex_positions,
		GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &index_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		sizeof(vertex_indices),
		vertex_indices,
		GL_STATIC_DRAW);

	glGenBuffers(1, &uniform_buffer);
	glBindBuffer(GL_UNIFORM_BUFFER, uniform_buffer);
	glBufferData(GL_UNIFORM_BUFFER, 4 * sizeof(glm::mat4), NULL, GL_DYNAMIC_DRAW);

	glEnable(GL_CULL_FACE);
	 //glFrontFace(GL_CW);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
}

void Test_MultiViewport::Render(double currentTime)
{
	int i;
	static const GLfloat black[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	static const GLfloat one = 1.0f;

	glViewport(0, 0, info.windowWidth, info.windowHeight);
	glClearBufferfv(GL_COLOR, 0, black);
	glClearBufferfv(GL_DEPTH, 0, &one);

	// Each rectangle will be 7/16 of the screen
	float viewport_width = (float)(7 * info.windowWidth) / 16.0f;
	float viewport_height = (float)(7 * info.windowHeight) / 16.0f;

	// Four rectangles - lower left first...
	glViewportIndexedf(0, 0, 0, viewport_width, viewport_height);

	// Lower right...
	glViewportIndexedf(1,
		info.windowWidth - viewport_width, 0,
		viewport_width, viewport_height);

	// Upper left...
	glViewportIndexedf(2,
		0, info.windowHeight - viewport_height,
		viewport_width, viewport_height);

	// Upper right...
	glViewportIndexedf(3,
		info.windowWidth - viewport_width,
		info.windowHeight - viewport_height,
		viewport_width, viewport_height);

	glm::mat4 proj_matrix = glm::perspective(50.0f,
		(float)info.windowWidth / (float)info.windowHeight,
		0.1f,
		1000.0f);

	float f = (float)currentTime * 3;

	glBindBufferBase(GL_UNIFORM_BUFFER, 0, uniform_buffer);
	glm::mat4 * mv_matrix_array = (glm::mat4 *)glMapBufferRange(GL_UNIFORM_BUFFER,
		0,
		4 * sizeof(glm::mat4),
		GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);

	for (i = 0; i < 4; i++)
	{
		glm::mat4 View = glm::lookAt(
			glm::vec3(cosf(f) * tanf((i + 2) * 2), sinf(f) * i, 4 - cosf(i)),
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(0, 1, 0)
		);

		glm::mat4 Model = glm::mat4(1.0f);

		mv_matrix_array[i] = proj_matrix * View * Model;
	}

	glUnmapBuffer(GL_UNIFORM_BUFFER);

	glUseProgram(program);

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);
}

void Test_MultiViewport::OnClear()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteProgram(program);
	glDeleteBuffers(1, &position_buffer);
}


//DECLARE_MAIN(Test_MultiViewport);