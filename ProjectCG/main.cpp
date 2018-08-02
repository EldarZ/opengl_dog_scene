#include <windows.h>
#include <iostream>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_freeglut.h"
#include "imgui/imgui_impl_opengl2.h"
#include <GL\freeglut.h>
#include "Context.h"

using namespace std;

Context gContext;

class Vector
{
public:
	float x, y, z;
	Vector(float X = 0.0, float Y = 0.0, float Z = 0.0)	: x(X), y(Y), z(Z){}
	static Vector Cross(const Vector& vec1, const Vector& vec2)	{
		Vector vecCross;
		vecCross.x = vec1.y * vec2.z - vec1.z * vec2.y;
		vecCross.y = vec1.z * vec2.x - vec1.x * vec2.z;
		vecCross.z = vec1.x * vec2.y - vec1.y * vec2.x;
		vecCross.Normalize();
		return vecCross;
	}

	float Length() const { return sqrt(x * x + y * y + z * z); }
	void Normalize(){ 
		float Len = Length();
		x /= Len;
		y /= Len;
		z /= Len;
	}
};

class Camera
{
public:
	Camera() :x(0), y(5), z(10), r(10), alpha(0) {};
	double x,y,z;
	double r;
	double alpha;
};

Camera gCamera;


void special(int key, int, int) {
	switch (key) {
	case GLUT_KEY_LEFT: 
		gCamera.alpha -= 0.1; 
		gCamera.x = gCamera.r * sin(gCamera.alpha); 
		gCamera.z = gCamera.r * cos(gCamera.alpha);
		break;
	case GLUT_KEY_RIGHT: 
		gCamera.alpha += 0.1; 
		gCamera.x = gCamera.r * sin(gCamera.alpha);
		gCamera.z = gCamera.r * cos(gCamera.alpha);
		break;
	case GLUT_KEY_UP: gCamera.y += 1; break;
	case GLUT_KEY_DOWN: gCamera.y -= 1; break;
	case GLUT_KEY_F1:
		gContext.light._temp += 0.3f;
		break;
	case GLUT_KEY_F2:
		gContext.light._temp -= 0.3f;
		break;
	case GLUT_KEY_F3:
		exit(0);
		break;
	case GLUT_KEY_F4:
		glDisable(GL_LIGHTING);
		break;
	case GLUT_KEY_F5:
		glEnable(GL_LIGHTING);
		break;
	}
	glutPostRedisplay();
}

// On reshape, constructs a camera that perfectly fits the window.
void reshape(GLint w, GLint h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(40.0, GLfloat(w) / GLfloat(h), 1.0, 150.0);
}


static bool show_demo_window = true;
static bool show_another_window = false;
static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

void my_display_code()
{
	// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
	{
		static float f = 0.0f;
		static int counter = 0;

		ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

		ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
		ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
		ImGui::Checkbox("Another Window", &show_another_window);

		ImGui::SliderFloat("float", &gContext.light._temp, -5.0f, 10.0f);            // Edit 1 float using a slider from 0.0f to 1.0f    
		ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

		if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
			counter++;
		ImGui::SameLine();
		ImGui::Text("counter = %d", counter);
		ImGui::End();
	}
}

void display() {
	
	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplFreeGLUT_NewFrame();
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	my_display_code();

	// Rendering
	ImGui::Render();
	
	ImGuiIO& io = ImGui::GetIO();

	glViewport(0, 0, (GLsizei)io.DisplaySize.x, (GLsizei)io.DisplaySize.y);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(40.0, io.DisplaySize.x / io.DisplaySize.y, 1.0, 150.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	
	gluLookAt(gCamera.x, gCamera.y, gCamera.z, 0, 0, 0, 0, 1, 0);
	
	gContext.light.draw();
	gContext.dog.draw();
	gContext.floor.draw();
	
	gContext.table.draw();
	glDisable(GL_LIGHTING);
	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
	glEnable(GL_LIGHTING);

	glFlush();
	glutSwapBuffers();
	glutPostRedisplay();
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_MULTISAMPLE);
	glutInitWindowPosition(80, 80);
	glutInitWindowSize(800, 600);
	glutCreateWindow("CG Project");
	glutDisplayFunc(display);
	//glutReshapeFunc(reshape);

	// Setup ImGui binding
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls

	ImGui_ImplFreeGLUT_Init();
	ImGui_ImplFreeGLUT_InstallFuncs();
	ImGui_ImplOpenGL2_Init();


	glutSpecialFunc(special);
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);

	gContext.light.init();
	
	// Setup style
	ImGui::StyleColorsDark();

	glutMainLoop();

	// Cleanup
	ImGui_ImplOpenGL2_Shutdown();
	ImGui_ImplFreeGLUT_Shutdown();
	ImGui::DestroyContext();

	return 0;
}