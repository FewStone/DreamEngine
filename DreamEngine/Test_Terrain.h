#pragma once
#include "Ancestor.h"
#include "Texture.h"
#include "Shader.h"
#include "UserController.h"

class Test_Terrain :
	public AncestorNS::Ancestor
{
public:
	Test_Terrain();
	~Test_Terrain();

	void Start();
	void Render(double currentTime);
	void OnClear();
	void load_shaders();
	void onKey(int key, int action);

private:
	TextureNS::Texture *texture;
	ShaderNS::Shader *shader;
	UserController *userController;

	GLuint          program;
	GLuint          vao;
	GLuint          tex_displacement;
	GLuint          tex_color;
	float           dmap_depth;
	bool            enable_displacement;
	bool            wireframe;
	bool            enable_fog;
	bool            paused;

	struct
	{
		GLint       mvp_matrix;
		GLint       mv_matrix;
		GLint       proj_matrix;
		GLint       dmap_depth;
		GLint       enable_fog;
	} uniforms;

};

