#pragma once

#include <iostream>
#include <GL/gl3w.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace AncestorNS
{

	class Ancestor
	{
	public:
		GLFWwindow* window;

		struct APPINFO
		{
			char title[128];
			int windowWidth;
			int windowHeight;
			int majorVersion;
			int minorVersion;
			int samples;
			union
			{
				struct
				{
					unsigned int    fullscreen : 1;
					unsigned int    vsync : 1;
					unsigned int    cursor : 1;
					unsigned int    stereo : 1;
					unsigned int    debug : 1;
					unsigned int    robust : 1;
				};
				unsigned int        all;
			} flags;
		};

		Ancestor();
		~Ancestor();

		virtual void Init();
		virtual void Start();
		virtual void Run(Ancestor *the_app);
		virtual void Render(double currentTime);
		virtual void OnClear();

		void setWindowTitle(const char * title)
		{
			glfwSetWindowTitle(window, title);
		}

		virtual void onResize(int w, int h)
		{
			info.windowWidth = w;
			info.windowHeight = h;
		}

		virtual void onKey(int key, int action)
		{

		}

		virtual void onMouseButton(int button, int action)
		{

		}

		virtual void onMouseMove(int x, int y)
		{

		}

		virtual void onMouseWheel(int pos)
		{

		}

		void getMousePosition(int& x, int& y)
		{
			double dx, dy;
			glfwGetCursorPos(window, &dx, &dy);

			x = static_cast<int>(floor(dx));
			y = static_cast<int>(floor(dy));
		}

	protected:
		APPINFO     info;
		static AncestorNS::Ancestor *app;

		static void glfw_onResize(GLFWwindow* window, int w, int h)
		{
			app->onResize(w, h);
		}

		static void glfw_onKey(GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			app->onKey(key, action);
		}

		static void glfw_onMouseButton(GLFWwindow* window, int button, int action, int mods)
		{
			app->onMouseButton(button, action);
		}

		static void glfw_onMouseMove(GLFWwindow* window, double x, double y)
		{
			app->onMouseMove(static_cast<int>(x), static_cast<int>(y));
		}

		static void glfw_onMouseWheel(GLFWwindow* window, double xoffset, double yoffset)
		{
			app->onMouseWheel(static_cast<int>(yoffset));
		}

	};
}


#define DECLARE_MAIN(a)                             \
int main(int argc, const char ** argv)              \
{                                                   \
    a *app = new a;                                 \
    app->Run(app);                                  \
    delete app;                                     \
    return 0;                                       \
}
