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

void special(int key, int, int) {
	switch (key) {
	case GLUT_KEY_LEFT: 
		gContext.camera.alpha -= 0.1; 
		gContext.camera.x = gContext.camera.r * sin(gContext.camera.alpha);
		gContext.camera.z = gContext.camera.r * cos(gContext.camera.alpha);
		break;
	case GLUT_KEY_RIGHT: 
		gContext.camera.alpha += 0.1; 
		gContext.camera.x = gContext.camera.r * sin(gContext.camera.alpha);
		gContext.camera.z = gContext.camera.r * cos(gContext.camera.alpha);
		break;
	case GLUT_KEY_UP: gContext.camera.y += 1; break;
	case GLUT_KEY_DOWN: gContext.camera.y -= 1; break;
	case GLUT_KEY_F1:
		gContext.light._temp += 0.3f;
		break;
	case GLUT_KEY_F2:
		gContext.light._temp -= 0.3f;
		break;
	case GLUT_KEY_F3:
		exit(0);
		break;
	}
	glutPostRedisplay();
}

void interaction()
{
	ImGui::Begin("Scene control");
	ImGui::SliderFloat("lights height", &gContext.light._temp, -5.0f, 10.0f);  
	ImGui::SliderFloat("head horizontal", &gContext.dog.headSideRotation, -50.0f, 50.0f);
	ImGui::SliderFloat("head vertical", &gContext.dog.headVerticalRotation, -50.0f, 50.0f);
	ImGui::SliderFloat("tail horizontal", &gContext.dog.tailSideRotation, -50.0f, 50.0f);
	ImGui::SliderFloat("tail vertical", &gContext.dog.tailVerticalRotation, -50.0f, 50.0f);

	ImGui::End();
}

void display() {
	
	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplFreeGLUT_NewFrame();
	
	interaction();

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
	
	gluLookAt(gContext.camera.x, gContext.camera.y, gContext.camera.z, 0, 0, 0, 0, 1, 0);

	gContext.light.draw();
	gContext.dog.draw();
	gContext.floor.draw();
	gContext.table.draw();
	gContext.teapot.draw();

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

	// Setup ImGui binding
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	ImGui_ImplFreeGLUT_Init();
	ImGui_ImplFreeGLUT_InstallFuncs();
	ImGui_ImplOpenGL2_Init();


	glutSpecialFunc(special);
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);

	gContext.light.init();
	
	// Setup style
	ImGui::StyleColorsClassic();

	glutMainLoop();

	// Cleanup
	ImGui_ImplOpenGL2_Shutdown();
	ImGui_ImplFreeGLUT_Shutdown();
	ImGui::DestroyContext();

	return 0;
}