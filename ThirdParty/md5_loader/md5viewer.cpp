#include "stdafx.h"
#include "defs.h"
#include "md5mesh.h"
#include "timer.h"

const SDL_VideoInfo* vidInfo;
int iWidth = 800;
int iHeight = 600;
int videoFlags;
SDL_Surface* screen;
SDL_Event event;


float fRotation = 0.0f;
float xRotation = 0.0f;
float yRotation = 0.0f;

bool bMouseActive;
bool bAnimate;

MD5Mesh* pModel;
Timer* pTimer;


void RenderMesh(MD5Mesh*);
void InitSDL ();
void InitGL ();
void Render ();
void MainLoop ();

void InitSDL ()
{
	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cerr << "Erreur lors de l'initialisation de la video" << std::endl;
		exit (-1);
	}
	
	vidInfo = SDL_GetVideoInfo();
	
	if(vidInfo == NULL) 
	{
		std::cerr << "Impossible d'obtenir les informations sur la carte graphique" << std::endl;
		exit (-1);
	}
	
	videoFlags = SDL_OPENGL | SDL_HWPALETTE | SDL_RESIZABLE | SDL_HWSURFACE;
	
	screen = SDL_SetVideoMode(iWidth, iHeight, vidInfo->vfmt->BitsPerPixel, videoFlags);

	//"reelle" initialisation de l'affichage
	{
		GLfloat ratio;
		
		if(iHeight == 0) 
		{
			iHeight = 1;
		}
		
		ratio = (GLfloat)iWidth / (GLfloat)iHeight;
	
		glViewport(0, 0, (GLsizei)iWidth, (GLsizei)iHeight);
		
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		#define FOV 90
		gluPerspective(FOV/2, ratio, 0.1f, 10000.0f);
		
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}
	
	
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	
	SDL_ShowCursor(SDL_ENABLE);
}

void InitGL ()
{
	glClearColor(0.3f, 0.3f, 0.3f, 0.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDisable (GL_LIGHTING);
}


void Render ()
{
	pTimer->CalculateFrameRate ();
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );
	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity ();
	
	glMatrixMode (GL_MODELVIEW);
	glTranslatef(-100.0f, -100.0f, -400.0f);
	glRotatef(-yRotation, 1.0f, 0.0f, 0.0f);
	glRotatef(-xRotation, 0.0f, 1.0f, 0.0f);
	
	if (bAnimate)
		pModel->PreRender(pTimer->GetFrameInterval());
	RenderMesh(pModel);
	
	
	glFlush();
	SDL_GL_SwapBuffers();
	
}

void RenderMesh (MD5Mesh* mesh)
{
	Vertice* pVerts;
	unsigned int* pIndexes;
	unsigned int iNumMeshes = mesh->GetMeshesCount();
	
	glPushMatrix ();
	glRotatef(-90,1,0,0);
	glColor3f(1,1,1);
	
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnable (GL_TEXTURE_2D);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);
	for (unsigned int j=0; j<iNumMeshes; j++)
	{
		pVerts = mesh->GetVertexList(j);
		pIndexes = mesh->GetIndexes(j);
		
		if (mesh->GetMaterial(j) && mesh->GetMaterial(j)->HasPrimary())
			glBindTexture (GL_TEXTURE_2D, mesh->GetMaterial(j)->GetFirstTexture()->iId);
			
		glVertexPointer(3, GL_FLOAT, sizeof(Vertice) , &pVerts[0].vPosition);
		glTexCoordPointer(2, GL_FLOAT, sizeof(Vertice), &pVerts[0].s);
		
		glDrawElements(GL_TRIANGLES, mesh->GetTrianglesCount(j)*3, GL_UNSIGNED_INT, pIndexes);
	}
	glFrontFace(GL_CCW);
	//desactivation de la texture de couleur 
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisable (GL_TEXTURE_2D);
	
	glPopMatrix ();
}

void MainLoop ()
{
	bool done = false;
	
	float pOldMousePos[] = {0.0f,0.0f};
	float pCurrentMousePos[] = {0.0f,0.0f};
	int mX, mY;
	
	while (!done)
	{
		while(SDL_PollEvent(&event)) 
		{
			switch(event.type) 
			{
				case SDL_MOUSEBUTTONDOWN:
					SDL_GetMouseState(&mX,&mY);
					pOldMousePos[0] = pCurrentMousePos[0] = mX;
					pOldMousePos[1] = pCurrentMousePos[1] = mY;
            				bMouseActive = true;
					break;
				case SDL_MOUSEBUTTONUP:
					bMouseActive = false;
				case SDL_MOUSEMOTION:
					SDL_GetMouseState(&mX,&mY);
					pCurrentMousePos[0] = mX;
					pCurrentMousePos[1] = mY;

					if(bMouseActive)
					{
						xRotation -= (pCurrentMousePos[0] - pOldMousePos[0]);
						yRotation -= (pCurrentMousePos[1] - pOldMousePos[1]);
					}

					pOldMousePos[0] = pCurrentMousePos[0];
					pOldMousePos[1] = pCurrentMousePos[1];
					break;
				
				case SDL_KEYDOWN:
					switch (event.key.keysym.sym)
					{
						case 27: //Echape
							done = true;
							break;
						case SDLK_SPACE:
							bAnimate = bAnimate ? false : true;
							break;
						default:
							break;
					}
				default:
					break;
			}
		
			
		}
		Render ();
	}
}

#ifdef __cplusplus
extern "C"
#endif
int main(int argc, char *argv[])
{
  if (argc < 3) {
    printf("Usage :<md5mesh> <md5anim>");
    return -1;
  }
	InitSDL ();
	InitGL ();
	
	pTimer = new Timer();
	
	pModel = new MD5Mesh ();
	pModel->LoadMesh(argv[1]);
	pModel->LoadAnim(argv[2]);
	pModel->SetAnimation("boblampclean");
	
	bAnimate = true;
	
	MainLoop ();
	SDL_Quit ();
	return 0;
}

