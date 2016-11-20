#include "Test.h"

#include <iostream>
using namespace std;

//#define MANY_CUBES

Test::Test()
{
	shader = new ShaderNS::Shader();
}

Test::~Test()
{

}

void Test::Start()
{

	program = glCreateProgram();
	
	GLuint vs = shader->Load("C:/Media/Shaders/Test/Test.vs.glsl", GL_VERTEX_SHADER);
	GLuint fs = shader->Load("C:/Media/Shaders/Test/Test.fs.glsl", GL_FRAGMENT_SHADER);

	glAttachShader(program, vs);
	glAttachShader(program, fs);

	glLinkProgram(program);

	mv_location = glGetUniformLocation(program, "mv_matrix");
	proj_location = glGetUniformLocation(program, "proj_matrix");

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	static const GLfloat vertex_positions[] =
	{
		-0.25f,  0.25f, -0.25f,
		-0.25f, -0.25f, -0.25f,
		0.25f, -0.25f, -0.25f,

		0.25f, -0.25f, -0.25f,
		0.25f,  0.25f, -0.25f,
		-0.25f,  0.25f, -0.25f,

		0.25f, -0.25f, -0.25f,
		0.25f, -0.25f,  0.25f,
		0.25f,  0.25f, -0.25f,

		0.25f, -0.25f,  0.25f,
		0.25f,  0.25f,  0.25f,
		0.25f,  0.25f, -0.25f,

		0.25f, -0.25f,  0.25f,
		-0.25f, -0.25f,  0.25f,
		0.25f,  0.25f,  0.25f,

		-0.25f, -0.25f,  0.25f,
		-0.25f,  0.25f,  0.25f,
		0.25f,  0.25f,  0.25f,

		-0.25f, -0.25f,  0.25f,
		-0.25f, -0.25f, -0.25f,
		-0.25f,  0.25f,  0.25f,

		-0.25f, -0.25f, -0.25f,
		-0.25f,  0.25f, -0.25f,
		-0.25f,  0.25f,  0.25f,

		-0.25f, -0.25f,  0.25f,
		0.25f, -0.25f,  0.25f,
		0.25f, -0.25f, -0.25f,

		0.25f, -0.25f, -0.25f,
		-0.25f, -0.25f, -0.25f,
		-0.25f, -0.25f,  0.25f,

		-0.25f,  0.25f, -0.25f,
		0.25f,  0.25f, -0.25f,
		0.25f,  0.25f,  0.25f,

		0.25f,  0.25f,  0.25f,
		-0.25f,  0.25f,  0.25f,
		-0.25f,  0.25f, -0.25f
	};

	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER,
		sizeof(vertex_positions),
		vertex_positions,
		GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	//fixed bug of "proj_matrix" matrix value error in the program first run
	onResize(info.windowWidth, info.windowHeight);

}

void Test::Render(double currentTime)
{

	static const GLfloat green[] = { 0.0f, 0.25f, 0.0f, 1.0f };
	static const GLfloat one = 1.0f;

	glViewport(0, 0, info.windowWidth, info.windowHeight);
	glClearBufferfv(GL_COLOR, 0, green);
	glClearBufferfv(GL_DEPTH, 0, &one);

	glUseProgram(program);

	glUniformMatrix4fv(proj_location, 1, GL_FALSE, &proj_matrix[0][0]);


#ifdef MANY_CUBES
	int i;
	for (i = 0; i < 24; i++)
	{
		float f = (float)i + (float)currentTime * 0.3f;
		glm::mat4 View = glm::lookAt(
			glm::vec3(sinf(2.1f * f) * 0.5f,
				cosf(1.7f * f) * 0.5f,
				sinf(1.3f * f) * cosf(1.5f * f) * 80.0f), // Camera is at (4,3,-3), in World Space
			glm::vec3(0, 0, 0), // and looks at the origin
			glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
		);

		glm::mat4 Model = glm::translate(glm::mat4(1.0f), glm::vec3(sinf(2.1f * f) * 4.0f, cosf(1.7f * f) * 8.0f, -2.0f));

		glm::mat4 mv_matrix = View * Model;

		glUniformMatrix4fv(mv_location, 1, GL_FALSE, &mv_matrix[0][0]);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

#else

	float f = (float)currentTime * 0.3f;
	glm::mat4 View = glm::lookAt(
		glm::vec3(sinf(2.1f * f) * 0.5f, 
			cosf(1.7f * f) * 0.5f, 
			sinf(1.3f * f) * cosf(1.5f * f) * 8.0f), 
		glm::vec3(0, 0, 0), 
		glm::vec3(0, 1, 0)  
	);

	glm::mat4 Model = glm::mat4(1.0f);

	glm::mat4 mv_matrix = View * Model;

	glUniformMatrix4fv(mv_location, 1, GL_FALSE, &mv_matrix[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, 36);

#endif


}

void Test::onResize(int w, int h)
{
	AncestorNS::Ancestor::onResize(w, h);

	aspect = (float)w / (float)h;

	proj_matrix = glm::perspective(45.0f, aspect, 0.1f, 1000.0f);

}

void Test::OnClear()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteProgram(program);
	glDeleteBuffers(1, &buffer);
}


//DECLARE_MAIN(Test);
