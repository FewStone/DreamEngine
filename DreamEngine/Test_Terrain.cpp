#include "Test_Terrain.h"


Test_Terrain::Test_Terrain() : program(0),
enable_displacement(true),
wireframe(false),
enable_fog(true),
paused(false)
{
	texture = new TextureNS::Texture();
	shader = new ShaderNS::Shader();
	userController = new UserController();

	AncestorNS::Ancestor::Init();
}

Test_Terrain::~Test_Terrain()
{
}

void Test_Terrain::Start()
{
	load_shaders();

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glPatchParameteri(GL_PATCH_VERTICES, 4);

	glEnable(GL_CULL_FACE);

	tex_displacement = texture->LoadKTX("C:/Media/Textures/terrain_highmap.ktx");
	glActiveTexture(GL_TEXTURE1);
	tex_color = texture->LoadKTX("C:/Media/Textures/terrain_color.ktx");
}
void Test_Terrain::Render(double currentTime)
{
	static const GLfloat black[] = { 0.85f, 0.95f, 1.0f, 1.0f };
	static const GLfloat one = 1.0f;
	static double last_time = 0.0;
	static double total_time = 0.0;

	if (!paused)
		total_time += (currentTime - last_time);
	last_time = currentTime;

	glViewport(0, 0, info.windowWidth, info.windowHeight);
	glClearBufferfv(GL_COLOR, 0, black);
	glClearBufferfv(GL_DEPTH, 0, &one);

	userController->computeMatricesFromInputs(window);

	glm::mat4 View = userController->getViewMatrix();

	glm::mat4 Model = glm::mat4(1.0f);

	glm::mat4 mv_matrix = View * Model;

	glm::mat4 proj_matrix = userController->getProjectionMatrix();

	glm::mat4 mvp_matrix = proj_matrix * mv_matrix;

	glUseProgram(program);

	glUniformMatrix4fv(uniforms.mv_matrix, 1, GL_FALSE, &mv_matrix[0][0]);
	glUniformMatrix4fv(uniforms.proj_matrix, 1, GL_FALSE, &proj_matrix[0][0]);
	glUniformMatrix4fv(uniforms.mvp_matrix, 1, GL_FALSE, &mvp_matrix[0][0]);
	glUniform1f(uniforms.dmap_depth, enable_displacement ? dmap_depth : 0.0f);
	glUniform1i(uniforms.enable_fog, enable_fog ? 1 : 0);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	if (wireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArraysInstanced(GL_PATCHES, 0, 4, 64 * 64);
}

void Test_Terrain::OnClear()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteProgram(program);
}

void Test_Terrain::onKey(int key, int action)
{
	if (action == 1)
	{
		switch (key)
		{
		case GLFW_KEY_KP_ADD: dmap_depth += 0.1f;
			break;
		case GLFW_KEY_KP_SUBTRACT: dmap_depth -= 0.1f;
			break;
		case 'F': enable_fog = !enable_fog;
			break;
		case 'D': enable_displacement = !enable_displacement;
			break;
		case 'W': wireframe = !wireframe;
			break;
		case 'P': paused = !paused;
			break;
		case 'R':
			load_shaders();
			break;
		default:
			break;
		};
	}
}

void Test_Terrain::load_shaders()
{
	if (program)
		glDeleteProgram(program);

	GLuint vs = shader->Load("C:/Media/Shaders/Test_Terrain/Test_Terrain.vs.glsl", GL_VERTEX_SHADER);
	GLuint tcs = shader->Load("C:/Media/Shaders/Test_Terrain/Test_Terrain.tc.glsl", GL_TESS_CONTROL_SHADER);
	GLuint tes = shader->Load("C:/Media/Shaders/Test_Terrain/Test_Terrain.te.glsl", GL_TESS_EVALUATION_SHADER);
	GLuint fs = shader->Load("C:/Media/Shaders/Test_Terrain/Test_Terrain.fs.glsl", GL_FRAGMENT_SHADER);

	program = glCreateProgram();

	glAttachShader(program, vs);
	glAttachShader(program, tcs);
	glAttachShader(program, tes);
	glAttachShader(program, fs);

	glLinkProgram(program);

	uniforms.mv_matrix = glGetUniformLocation(program, "mv_matrix");
	uniforms.mvp_matrix = glGetUniformLocation(program, "mvp_matrix");
	uniforms.proj_matrix = glGetUniformLocation(program, "proj_matrix");
	uniforms.dmap_depth = glGetUniformLocation(program, "dmap_depth");
	uniforms.enable_fog = glGetUniformLocation(program, "enable_fog");
	dmap_depth = 6.0f;
}


//DECLARE_MAIN(Test_Terrain);

