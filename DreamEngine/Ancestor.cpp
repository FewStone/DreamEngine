#include "Ancestor.h"

namespace AncestorNS
{

	AncestorNS::Ancestor *AncestorNS::Ancestor::app = NULL;


	Ancestor::Ancestor()
	{

	}

	Ancestor::~Ancestor()
	{

	}

	void Ancestor::Init()
	{
		strcpy(info.title, "Dream Engine");
		info.windowWidth = 800;
		info.windowHeight = 600;

#ifdef __APPLE__
		info.majorVersion = 3;
		info.minorVersion = 2;
#else
		info.majorVersion = 4;
		info.minorVersion = 3;
#endif
		info.samples = 0;
		info.flags.all = 0;
		info.flags.cursor = 1;
#ifdef _DEBUG
		info.flags.debug = 1;
#endif

	}

	void Ancestor::Start()
	{

	}

	void Ancestor::Run(Ancestor* the_app)
	{
		bool running = true;
		app = the_app;

		if (!glfwInit())
		{
			fprintf(stderr, "Failed to initialize GLFW\n");
			return;
		}

		Init();

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, info.majorVersion);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, info.minorVersion);

#ifndef _DEBUG
		if (info.flags.debug)
#endif /* _DEBUG */
		{
			glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
		}
		if (info.flags.robust)
		{
			glfwWindowHint(GLFW_CONTEXT_ROBUSTNESS, GLFW_LOSE_CONTEXT_ON_RESET);
		}
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
		glfwWindowHint(GLFW_SAMPLES, info.samples);
		glfwWindowHint(GLFW_STEREO, info.flags.stereo ? GL_TRUE : GL_FALSE);

		window = glfwCreateWindow(info.windowWidth, info.windowHeight, info.title, info.flags.fullscreen ? glfwGetPrimaryMonitor() : NULL, NULL);
		if (!window)
		{
			fprintf(stderr, "Failed to open window\n");
			return;
		}

		glfwMakeContextCurrent(window);

		glfwSetWindowSizeCallback(window, glfw_onResize);
		glfwSetKeyCallback(window, glfw_onKey);
		glfwSetMouseButtonCallback(window, glfw_onMouseButton);
		glfwSetCursorPosCallback(window, glfw_onMouseMove);
		glfwSetScrollCallback(window, glfw_onMouseWheel);

		gl3wInit();

#ifdef _DEBUG
		fprintf(stderr, "VENDOR: %s\n", (char *)glGetString(GL_VENDOR));
		fprintf(stderr, "VERSION: %s\n", (char *)glGetString(GL_VERSION));
		fprintf(stderr, "RENDERER: %s\n", (char *)glGetString(GL_RENDERER));
#endif

		Start();

		do
		{
			Render(glfwGetTime());

			glfwSwapBuffers(window);
			glfwPollEvents();

			running &= (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_RELEASE);
			running &= (glfwWindowShouldClose(window) != GL_TRUE);

		} while (running);

		OnClear();

		glfwDestroyWindow(window);
		glfwTerminate();

	}

	void Ancestor::Render(double currentTime)
	{

	}

	void Ancestor::OnClear()
	{

	}

}


