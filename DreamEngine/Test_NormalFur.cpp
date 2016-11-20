#include "Test_NormalFur.h"

Test_NormalFur::Test_NormalFur()
{
	AncestorNS::Ancestor::Init();

	model = new ModelNS::Model();
	shader = new ShaderNS::Shader();
	userController = new UserController();
}


Test_NormalFur::~Test_NormalFur()
{
}

void Test_NormalFur::Start()
{
	program = glCreateProgram();

	GLuint vs = shader->Load("C:/Media/Shaders/Test_NormalFur/Test_NormalFur.vs.glsl", GL_VERTEX_SHADER);
	GLuint gs = shader->Load("C:/Media/Shaders/Test_NormalFur/Test_NormalFur.gs.glsl",GL_GEOMETRY_SHADER);
	GLuint fs = shader->Load("C:/Media/Shaders/Test_NormalFur/Test_NormalFur.fs.glsl", GL_FRAGMENT_SHADER);

	glAttachShader(program, vs);
	glAttachShader(program, gs);
	glAttachShader(program, fs);

	glLinkProgram(program);

	mv_location = glGetUniformLocation(program, "mv_matrix");
	proj_location = glGetUniformLocation(program, "proj_matrix");
	normal_length_location = glGetUniformLocation(program, "normal_length");

	model->LoadSBM("C:/Media/Models/dragon.sbm");

	 glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
}

void Test_NormalFur::Render(double currentTime)
{
	static const GLfloat black[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	static const GLfloat one = 1.0f;

	glViewport(0, 0, info.windowWidth, info.windowHeight);
	glClearBufferfv(GL_COLOR, 0, black);
	glClearBufferfv(GL_DEPTH, 0, &one);

	glUseProgram(program);

	userController->computeMatricesFromInputs(window);

	glm::mat4 View = userController->getViewMatrix();

	glm::mat4 Model = glm::mat4(1.0f);

	glm::mat4 mv_matrix = View * Model;

	glm::mat4 proj_matrix = userController->getProjectionMatrix();

	glm::mat4 mvp_matrix = proj_matrix * mv_matrix;

	glUniformMatrix4fv(proj_location, 1, GL_FALSE, &proj_matrix[0][0]);

	glUniformMatrix4fv(mv_location, 1, GL_FALSE, &mv_matrix[0][0]);

	glUniform1f(normal_length_location, sinf((float)currentTime * 8.0f) * cosf((float)currentTime * 6.0f) * 0.03f + 0.05f);

	model->render();
}

void Test_NormalFur::OnClear()
{
	model->free();
	glDeleteProgram(program);
}

//DECLARE_MAIN(Test_NormalFur);
