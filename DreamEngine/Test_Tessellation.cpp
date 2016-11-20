#include "Test_Tessellation.h"

Test_Tessellation::Test_Tessellation()
{
	shader = new ShaderNS::Shader();
}

Test_Tessellation::~Test_Tessellation()
{
}

void Test_Tessellation::Start()
{
	GLuint vs = shader->Load("C:/Media/Shaders/Test_Tessellation/Test_Tessellation.vs.glsl",GL_VERTEX_SHADER);
	GLuint tc = shader->Load("C:/Media/Shaders/Test_Tessellation/Test_Tessellation.tc.glsl",GL_TESS_CONTROL_SHADER);
	GLuint te = shader->Load("C:/Media/Shaders/Test_Tessellation/Test_Tessellation.te.glsl",GL_TESS_EVALUATION_SHADER);
	GLuint fs = shader->Load("C:/Media/Shaders/Test_Tessellation/Test_Tessellation.fs.glsl",GL_FRAGMENT_SHADER);

	shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vs);
	glAttachShader(shaderProgram, tc);
	glAttachShader(shaderProgram, te);
	glAttachShader(shaderProgram, fs);

	glLinkProgram(shaderProgram);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	static const GLfloat vertex_positions[] =
	{
		-2.0f,0.0f,0.0f,
		2.0f,0.0f,0.0f
	};

	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_VERTEX_ARRAY, vertexBuffer);
	glBufferData(vertexBuffer, sizeof(vertex_positions), vertex_positions, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

}

void Test_Tessellation::Render(double currentTime)
{
	static const GLfloat green[] = { 0.0f, 0.25f, 0.0f, 1.0f };
	static const GLfloat one = 1.0f;

	glViewport(0, 0, info.windowWidth, info.windowHeight);
	glClearBufferfv(GL_COLOR, 0, green);
	glClearBufferfv(GL_DEPTH, 0, &one);

	glUseProgram(shaderProgram);

	glDrawArrays(GL_PATCHES, 0, 3);


}

void Test_Tessellation::OnClear()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteProgram(shaderProgram);
	glDeleteBuffers(1, &vertexBuffer);
}


//DECLARE_MAIN(Test_Tessellation);
