#include "Test_Glass.h"

Test_Glass::Test_Glass()
{
	texture = new TextureNS::Texture();
	shader = new ShaderNS::Shader();
	userController = new UserController();
}


Test_Glass::~Test_Glass()
{
}

void Test_Glass::Start()
{
	static const GLfloat grass_blade[] =
	{
		-0.3f, 0.0f,
		0.3f, 0.0f,
		-0.20f, 4.0f,
		0.1f, 8.3f,
		-0.05f, 10.3f,
		0.0f, 3.3f
	};

	glGenBuffers(1, &grass_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, grass_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(grass_blade), grass_blade, GL_STATIC_DRAW);

	glGenVertexArrays(1, &grass_vao);
	glBindVertexArray(grass_vao);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	grass_program = glCreateProgram();
	GLuint grass_vs = shader->Load("C:/Media/Shaders/Test_Glass/Test_Glass.vs.glsl", GL_VERTEX_SHADER);
	GLuint grass_fs = shader->Load("C:/Media/Shaders/Test_Glass/Test_Glass.fs.glsl", GL_FRAGMENT_SHADER);

	glAttachShader(grass_program, grass_vs);
	glAttachShader(grass_program, grass_fs);

	glLinkProgram(grass_program);
	glDeleteShader(grass_fs);
	glDeleteShader(grass_vs);

	uniforms.mvpMatrix = glGetUniformLocation(grass_program, "mvpMatrix");

	glActiveTexture(GL_TEXTURE1);
	tex_grass_length = texture->LoadKTX("C:/Media/Textures/grass_length.ktx");
	glActiveTexture(GL_TEXTURE2);
	tex_grass_orientation = texture->LoadKTX("C:/Media/Textures/grass_orientation.ktx");
	glActiveTexture(GL_TEXTURE3);
	tex_grass_color = texture->LoadKTX("C:/Media/Textures/grass_color.ktx");
	glActiveTexture(GL_TEXTURE4);
	tex_grass_bend = texture->LoadKTX("C:/Media/Textures/grass_bend.ktx");
}

void Test_Glass::Render(double currentTime)
{
	static const GLfloat black[] = { 0.85f, 0.95f, 1.0f, 1.0f };
	static const GLfloat one = 1.0f;
	glClearBufferfv(GL_COLOR, 0, black);
	glClearBufferfv(GL_DEPTH, 0, &one);

	userController->computeMatricesFromInputs(window);

	glm::mat4 View = userController->getViewMatrix();

	glm::mat4 Model = glm::mat4(1.0f);

	glm::mat4 proj_matrix = userController->getProjectionMatrix();

	glm::mat4 mvp_matrix = proj_matrix * View * Model ;

	glUseProgram(grass_program);
	glUniformMatrix4fv(uniforms.mvpMatrix, 1, GL_FALSE, &mvp_matrix[0][0]);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glViewport(0, 0, info.windowWidth, info.windowHeight);

	glBindVertexArray(grass_vao);
	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 6, 1024 * 1024);
}

void Test_Glass::OnClear()
{
	glDeleteProgram(grass_program);
}


//DECLARE_MAIN(Test_Glass);
