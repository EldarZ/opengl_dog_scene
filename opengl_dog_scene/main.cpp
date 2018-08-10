#include <windows.h>
#include <iostream>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_freeglut.h"
#include "imgui/imgui_impl_opengl2.h"
#include <GL\freeglut.h>
#include "Context.h"

using namespace std;

//single point of access to all rendered objects
Context gContext;

//gui interaction handling via imgui
void guiInteraction()
{
	ImGuiWindowFlags window_flags = 0;
	if (ImGui::Begin("Opengl Dog Scene", false, window_flags))
	{
		ImGui::RadioButton("external view", &gContext.isDogView, 0); ImGui::SameLine();
		ImGui::RadioButton("doggy view", &gContext.isDogView, 1);

		if (ImGui::CollapsingHeader("Dog"))
		{
			ImGui::SliderFloat("head horizontal", &gContext.dog.headHorizontalAngle, -30.0f, 30.0f);
			ImGui::SliderFloat("head vertical", &gContext.dog.headVerticalAngle, -5.0f, 50.0f);
			ImGui::SliderFloat("tail horizontal", &gContext.dog.tailHorizontalAngle, -25.0f, 25.0f);
			ImGui::SliderFloat("tail vertical", &gContext.dog.tailVerticalAngle, -14.0f, 50.0f);
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
		static bool pointlight = true;
		static bool spotlight = true;
		if (ImGui::CollapsingHeader("Lights"))
		{
			ImGui::SliderFloat("ambient light adjust", &gContext.globalAmbient, 0.0f, 1.0f);
			ImGui::Checkbox("enable pointlight", &pointlight);
			
			ImGui::ColorEdit3("point light color", (float*)&gContext.pointlight.color);
			ImGui::SliderFloat("pointlight source x", &gContext.pointlight.position[0], -10.0f, 10.0f);
			ImGui::SliderFloat("pointlight source y", &gContext.pointlight.position[1], -10.0f, 10.0f);
			ImGui::SliderFloat("pointlight source z", &gContext.pointlight.position[2], -10.0f, 10.0f);

			ImGui::Checkbox("enable spotlight", &spotlight);
			ImGui::ColorEdit3("spotlight color", (float*)&gContext.spotlight.color);
			ImGui::SliderFloat("spotlight source x", &gContext.spotlight.position[0], -10.0f, 10.0f);
			ImGui::SliderFloat("spotlight source y", &gContext.spotlight.position[1], -10.0f, 10.0f);
			ImGui::SliderFloat("spotlight source z", &gContext.spotlight.position[2], -10.0f, 10.0f);
			ImGui::SliderFloat("spotlight target x", &gContext.spotlight.target[0], -10.0f, 10.0f);
			ImGui::SliderFloat("spotlight target y", &gContext.spotlight.target[1], -10.0f, 10.0f);
			ImGui::SliderFloat("spotlight target z", &gContext.spotlight.target[2], -10.0f, 10.0f);
			ImGui::SliderFloat("spotlight cutoff", &gContext.spotlight.cutoff, 0.0f, 90.0f);
			ImGui::SliderFloat("spotlight exponent", &gContext.spotlight.exponent, 0.0f, 90.0f);

			pointlight ? gContext.pointlight.enable() : gContext.pointlight.disable();
			spotlight ? gContext.spotlight.enable() : gContext.spotlight.disable();
		}
		if (ImGui::CollapsingHeader("Walls"))
		{
			ImGui::SliderFloat("alpha channel", &gContext.walls.alpha, 0.0f, 1.0f);
		}
		if (ImGui::CollapsingHeader("Help"))
		{
			ImGui::Text("Viewing modes:");
			ImGui::TextWrapped((string("There are 2 viewing modes, 'external view' and doggy view', external view is controlled by the 'Camera'")+ 
				string(" section, the 'doggy view' is controlled explicitly by the doggy head position and rotation. ")).c_str());
			ImGui::Text("Keyboard control:");
			ImGui::TextWrapped((string("The keyboard arrows control the doggy position on the XZ plane - the floor plane.").c_str()));
			ImGui::Text("Dog section:");
			ImGui::TextWrapped((string("The controls in the Dog section are controlling the head vertical and horizontal orientaion and the")+
				string(" tail vertical and horizontal orientation")).c_str());
			ImGui::Text("Camera section:");
			ImGui::TextWrapped((string("The controls in the Camera section are controling the camera position in space and the camera target")+
				string(" point in space.")).c_str());
			ImGui::Text("Lights section:");
			ImGui::TextWrapped((string("The controls in the Light section are controling the Light in the scene, 'pointlight' and 'spotlight' are 2") +
				string(" light sources that can be turned on a off by the checkboxes. 'ambient light adjust' controls the global ambient light value, ") +
				string(" The spotlight controls the spotlight position in space and the spotlight target in space. the pointlight controls the pointlight position in space")).c_str());
		}
		if (ImGui::Button("Quit"))
		{
			exit(0);
		}
	}
	ImGui::End();
}

//keyboard events handling
void keyboard(int key, int, int) {
	switch (key) {
	case GLUT_KEY_LEFT:
		gContext.dog.nextMove = []() { glRotatef(10, 0, 1, 0); };
		break;
	case GLUT_KEY_RIGHT:
		gContext.dog.nextMove = []() { glRotatef(-10, 0, 1, 0); };
		break;
	case GLUT_KEY_UP:
		gContext.dog.nextMove = []() { glTranslated(0, 0, 0.3); };
		break;
	case GLUT_KEY_DOWN:
		gContext.dog.nextMove = []() { glTranslated(0, 0, -0.3); };
		break;
	}
	glutPostRedisplay();
}

void drawScene() {
	glPushMatrix();
	glTranslatef(gContext.pointlight.position[0], gContext.pointlight.position[1], gContext.pointlight.position[2]);
	gContext.pointlight.addLight();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(gContext.spotlight.position[0], gContext.spotlight.position[1], gContext.spotlight.position[2]);
	gContext.spotlight.addlight();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(gContext.spotlight.position[0], gContext.spotlight.position[1], gContext.spotlight.position[2]);
	gContext.spotlight.draw();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(gContext.pointlight.position[0], gContext.pointlight.position[1], gContext.pointlight.position[2]);
	gContext.pointlight.draw();
	glPopMatrix();

	gContext.floor.draw();

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
	glTranslatef(-3.0f, 0, 3);
	glRotatef(90, 0, 1, 0);
	gContext.snowman.draw();
	glPopMatrix();

	glPushMatrix();
	glTranslated(1.0f, 1.5f, -4.99f);
	gContext.art.draw();
	glPopMatrix();

	gContext.walls.draw();
}

//display handling, rendering all objects
void display() {
	//imgui new frame
	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplFreeGLUT_NewFrame();
	
	//update interaction
	guiInteraction();

	ImGui::Render();	
	ImGuiIO& io = ImGui::GetIO();

	glViewport(0, 0, (GLsizei)io.DisplaySize.x, (GLsizei)io.DisplaySize.y);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(40.0, io.DisplaySize.x / io.DisplaySize.y, 1.0, 150.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	//update the dog's transformation matrix
	if (gContext.dog.nextMove) {
		gContext.dog.isMoving = true;
		GLfloat viewModelMatrix[16];
		glGetFloatv(GL_MODELVIEW_MATRIX, viewModelMatrix);
		glLoadMatrixf(gContext.dog.local);
		gContext.dog.nextMove();
		gContext.dog.nextMove = nullptr;
		glGetFloatv(GL_MODELVIEW_MATRIX, gContext.dog.local);
		glLoadMatrixf(viewModelMatrix);
	}
	
	//change viewing mode if in Doggy view setup
	if (gContext.isDogView) {
		GLfloat viewModelMatrix[16];
		glGetFloatv(GL_MODELVIEW_MATRIX, viewModelMatrix);
		glLoadMatrixf(gContext.dog.local);

		glRotatef(gContext.dog.headVerticalAngle, 1, 0, 0);
		glRotatef(gContext.dog.headHorizontalAngle, 0, 1, 0);
		glTranslated(0, 0.75, 0.9);

		GLfloat cameraPoseInDogView[16];
		glGetFloatv(GL_MODELVIEW_MATRIX, cameraPoseInDogView);
		glLoadMatrixf(viewModelMatrix);

		GLfloat zAngle = atan2(-cameraPoseInDogView[2], cameraPoseInDogView[0]);
		GLfloat yAngle = atan2(-cameraPoseInDogView[9], cameraPoseInDogView[5]);;
		
		gluLookAt(cameraPoseInDogView[12], cameraPoseInDogView[13], cameraPoseInDogView[14],
			sin(zAngle) + cameraPoseInDogView[12],
			-yAngle + cameraPoseInDogView[13],
			cos(zAngle) + cameraPoseInDogView[14],
			0, 1, 0);
	}
	else
	{
		//view mode of camera view setup
		gluLookAt(gContext.camera.position[0], gContext.camera.position[1], gContext.camera.position[2],
			gContext.camera.target[0], gContext.camera.target[1], gContext.camera.target[2], 0, 1, 0);
	}

	GLfloat globalAmbientVec[4] = { gContext.globalAmbient, gContext.globalAmbient, gContext.globalAmbient, 1.0 };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbientVec);

	drawScene();	
	
	//create mirror effect
	glEnable(GL_STENCIL_TEST); 
	glColorMask(0, 0, 0, 0); //Disable drawing colors to the screen
	glDisable(GL_DEPTH_TEST); //Disable depth testing
	glStencilFunc(GL_ALWAYS, 1, 1); //Make the stencil test always pass
									//Make pixels in the stencil buffer be set to 1 when the stencil test passes
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	//Set all of the pixels covered by the mirror to be 1 in the stencil buffer
	glPushMatrix();
	glRotatef(90, 0, 1, 0);
	glTranslatef(-1, 0.2, 4.99f);
	gContext.mirror.draw();
	glPopMatrix();

	glColorMask(1, 1, 1, 1); //Enable drawing colors to the screen
	glEnable(GL_DEPTH_TEST); //Enable depth testing
							 //Make the stencil test pass only when the pixel is 1 in the stencil buffer
	glStencilFunc(GL_EQUAL, 1, 1);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP); //Make the stencil buffer not change

	//Draw the scene, reflected in the mirror
	glPushMatrix();
	glTranslatef(10.001f, 0, 0);
	glScalef(-1, 1, 1);
	drawScene();
	glPopMatrix();

	glDisable(GL_STENCIL_TEST); //Disable using the stencil buffer

	//Blend the mirror onto the screen
	glEnable(GL_BLEND);
	glPushMatrix();
	glRotatef(90, 0, 1, 0);
	glTranslatef(-1, 0.2, 4.99f);
	gContext.mirror.draw();
	glPopMatrix();
	glDisable(GL_BLEND);

	//add the wall next to the mirror
	gContext.walls.draw({ 2 });

	//imgui does not handle light well
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
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_MULTISAMPLE | GLUT_STENCIL);
	glutInitWindowPosition(80, 80);
	glutInitWindowSize(1200, 600);
	glutCreateWindow("Computer Graphics Project");
	glutDisplayFunc(display);

	// Setup ImGui binding
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	ImGui_ImplFreeGLUT_Init();
	ImGui_ImplFreeGLUT_InstallFuncs();
	ImGui_ImplOpenGL2_Init();

	glutSpecialFunc(keyboard);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	gContext.pointlight.enable();
	gContext.spotlight.enable();
	gContext.dog.init();
	gContext.art.init();

	// Setup style
	ImGui::StyleColorsClassic();

	glutMainLoop();

	// Cleanup
	ImGui_ImplOpenGL2_Shutdown();
	ImGui_ImplFreeGLUT_Shutdown();
	ImGui::DestroyContext();

	return 0;
}