#include <windows.h>
#include <iostream>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_freeglut.h"
#include "imgui/imgui_impl_opengl2.h"
#include <GL\freeglut.h>
#include "Context.h"
#include <functional>

using namespace std;

Context gContext;

std::function<void()> dogMove;

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
		dogMove = []() { glRotatef(10, 0, 1, 0); };
		break;
	case GLUT_KEY_RIGHT: 
		dogMove = []() { glRotatef(-10, 0, 1, 0); };
		break;
	case GLUT_KEY_UP: 
		dogMove = []() { glTranslated(0, 0, 0.3); };
		break;
	case GLUT_KEY_DOWN: 
		dogMove = []() { glTranslated(0, 0, -0.3); };
		break;
	}
	glutPostRedisplay();
}

void interaction()
{
	ImGuiWindowFlags window_flags = 0;
	if (ImGui::Begin("Scene control", false, window_flags))
	{
		static int e = 0;
		ImGui::RadioButton("external view", &e, 0); ImGui::SameLine();
		ImGui::RadioButton("doggy view", &e, 1);

		if (ImGui::CollapsingHeader("Dog"))
		{
			ImGui::SliderFloat("head horizontal", &gContext.dog.headSideRotation, -30.0f, 30.0f);
			ImGui::SliderFloat("head vertical", &gContext.dog.headVerticalRotation, -5.0f, 50.0f);
			ImGui::SliderFloat("tail horizontal", &gContext.dog.tailSideRotation, -25.0f, 25.0f);
			ImGui::SliderFloat("tail vertical", &gContext.dog.tailVerticalRotation, -14.0f, 50.0f);
		}
		if (ImGui::CollapsingHeader("Camera"))
		{
			ImGui::SliderFloat("camera source x", &gContext.camera.position[0], -10.0f, 10.0f);
			ImGui::SliderFloat("camera source y", &gContext.camera.position[1], -10.0f, 10.0f);
			ImGui::SliderFloat("camera source z", &gContext.camera.position[2], -10.0f, 10.0f);
			ImGui::SliderFloat("camera target x", &gContext.camera.target[0], -10.0f, 10.0f);
			ImGui::SliderFloat("camera target y", &gContext.camera.target[1], -10.0f, 10.0f);
			ImGui::SliderFloat("camera target z", &gContext.camera.target[2], -10.0f, 10.0f);
		}
		static bool globalLight = true;
		static bool spotlight = true;
		if (ImGui::CollapsingHeader("Lights"))
		{
			ImGui::Checkbox("global", &globalLight); ImGui::SameLine();
			ImGui::Checkbox("spotlight", &spotlight);
			globalLight ? gContext.light.enable() : gContext.light.disable();
			spotlight ? gContext.spotlight.enable() : gContext.spotlight.disable();
			

			ImGui::SliderFloat("ambient light adjust", &gContext.light.intensity, 0.0f, 1.0f);
			ImGui::ColorEdit3("spotlight color", (float*)&gContext.spotlight.spotlightColor);
			ImGui::SliderFloat("spotlight source x", &gContext.spotlight.position[0], -10.0f, 10.0f);
			ImGui::SliderFloat("spotlight source y", &gContext.spotlight.position[1], -10.0f, 10.0f);
			ImGui::SliderFloat("spotlight source z", &gContext.spotlight.position[2], -10.0f, 10.0f);
			ImGui::SliderFloat("spotlight target x", &gContext.spotlight.direction[0], -10.0f, 10.0f);
			ImGui::SliderFloat("spotlight target y", &gContext.spotlight.direction[1], -10.0f, 10.0f);
			ImGui::SliderFloat("spotlight target z", &gContext.spotlight.direction[2], -10.0f, 10.0f);
			ImGui::SliderFloat("lights cutoff", &gContext.spotlight.cutoff, 0.0f, 90.0f);
			ImGui::SliderFloat("lights exponent", &gContext.spotlight.exponent, 0.0f, 90.0f);
		}
		if (ImGui::CollapsingHeader("Help"))
		{
			ImGui::TextWrapped("This window is being created by the ShowDemoWindow() function. Please refer to the code in imgui_demo.cpp for reference.\n\n");
			ImGui::Text("USER GUIDE:");
		}
	}
	ImGui::End();

}


void display() {
	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplFreeGLUT_NewFrame();
	
	interaction();

	ImGui::Render();	
	ImGuiIO& io = ImGui::GetIO();

	glViewport(0, 0, (GLsizei)io.DisplaySize.x, (GLsizei)io.DisplaySize.y);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(40.0, io.DisplaySize.x / io.DisplaySize.y, 1.0, 150.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	//glPushMatrix();
	//glTranslated(0,0, 10);
	gluLookAt(gContext.camera.position[0], gContext.camera.position[1], gContext.camera.position[2], 
			  gContext.camera.target[0], gContext.camera.target[1], gContext.camera.target[2], 0, 1, 0);

	//glPopMatrix();

	gContext.floor.draw();

	glPushMatrix();
	glTranslatef(gContext.light.position[0], gContext.light.position[1], gContext.light.position[2]);
	gContext.light.draw();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(gContext.spotlight.position[0], gContext.spotlight.position[1], gContext.spotlight.position[2]);
	gContext.spotlight.draw();
	glPopMatrix();

	if (dogMove) {
		//load the dog coord system
		GLfloat viewModelMatrix[16];
		glGetFloatv(GL_MODELVIEW_MATRIX, viewModelMatrix);
		glLoadMatrixf(gContext.dog.local);
		dogMove();
		dogMove = nullptr;
		glGetFloatv(GL_MODELVIEW_MATRIX, gContext.dog.local);
		glLoadMatrixf(viewModelMatrix);
	}

	glPushMatrix();
	glMultMatrixf(gContext.dog.local);
	gContext.dog.draw();
	glPopMatrix();

	glPushMatrix();
	glTranslated(-3, 1.05, -3);
	gContext.table.draw();
	glPopMatrix();
	
	glPushMatrix();
	glTranslated(-2.2, 1.35, -3);
	gContext.teapot.draw();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-3.0f, 0, 1);
	glRotatef(90, 0, 1, 0);
	gContext.snowman.draw();
	glPopMatrix();

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
	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);

	gContext.light.enable();
	gContext.spotlight.enable();
	gContext.dog.init();
	
	// Setup style
	ImGui::StyleColorsClassic();

	glutMainLoop();

	// Cleanup
	ImGui_ImplOpenGL2_Shutdown();
	ImGui_ImplFreeGLUT_Shutdown();
	ImGui::DestroyContext();

	return 0;
}