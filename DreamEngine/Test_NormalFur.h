#pragma once
#include "Ancestor.h"
#include "Shader.h"
#include "Model.h"
#include "UserController.h"

class Test_NormalFur :
	public AncestorNS::Ancestor
{
public:
	Test_NormalFur();
	~Test_NormalFur();

	void Start();
	void Render(double currentTime);
	void OnClear();

private:
	ShaderNS::Shader *shader;
	UserController *userController;
	ModelNS::Model *model;

	GLuint program;
	GLuint mv_location;
	GLuint proj_location;
	GLuint normal_length_location;

};

