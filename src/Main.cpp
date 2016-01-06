#include <iostream>
#include <ctime>
#include <SDL/SDL.h>
#include <GL/glew.h>
#include "Scene.hpp"
#include "Drone.hpp"
#include "FlyComputerInterface.hpp"

void Prepare ();
void InitSDL ();
void InitOGL ();
void Shutdown ();

int main (int argc, char *argv [])
{
	Scene scene;
	//Drone drone;
	Drone::instance->genMaxPower(5,1);
	InitSDL();
	InitOGL();
	if(fci_init() < 0) {
		
	}

	// On initialise Newton
	scene.initialize();
	Drone::instance->initialize();

   SDL_Event event;
   bool bQuit = false;
   int i=0;

   while (!bQuit)
   {
      Prepare ();
      if(i > 100) {
			if(fci_read() >= 0) {
				Drone::instance->setMotorPower(fci_getValue("NE"),
									fci_getValue("SE"),
									fci_getValue("SO"),
									fci_getValue("NO"));
			}
	  }
	  i++;
      scene.render();
	  Drone::instance->render();
	  SDL_GL_SwapBuffers();

      if (SDL_PollEvent (&event))
      {
         switch (event.type)
         {
            case SDL_QUIT:
               Shutdown ();
               bQuit = true;
               break;
            default:
               break;
         }
      }
    }

   return EXIT_SUCCESS;
}

void InitSDL ()
{
   // Initialisation de SDL
   SDL_Init (SDL_INIT_VIDEO);

   SDL_GL_SetAttribute (SDL_GL_DOUBLEBUFFER, 1); // On active le double buffering

   SDL_Surface * pEcran = SDL_SetVideoMode (640, 480, 32, SDL_OPENGL);

   if (pEcran == NULL)
   {
      std::cerr << "Erreur au paramètrage vidéo : " << SDL_GetError ();
   }

   atexit (SDL_Quit);
}

void InitOGL ()
{
   glClearColor(0.0, 0.0, 0.3, 0.0);
   glClearDepth(1.0);

   GLenum err = glewInit();

   if (GLEW_OK != err)
      std::cerr << "Erreur dans GLEW.";

   glViewport (0, 0, 640, 480); // On règle le Viewport

   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   gluPerspective (60.0, 640.0 / 480.0, 0.1, 1000.0);

   glMatrixMode (GL_MODELVIEW);
   glLoadIdentity (); // On rétablit la matrice identité

   glEnable (GL_DEPTH_TEST);
   glEnable (GL_COLOR_MATERIAL);
   glEnable (GL_LIGHTING);

   glEnable (GL_LIGHT0);
   GLfloat LampeDiffuse [] = {1.0f, 1.0f, 1.0f};
   GLfloat LampeAmbient [] = {0.75f, 0.75f, 0.75f};
   GLfloat LampePosition [] = {6.0f, 10.0f, 5.0f, 1.0f};

   glLightfv (GL_LIGHT0, GL_DIFFUSE, LampeDiffuse);
   glLightfv (GL_LIGHT0, GL_SPECULAR, LampeDiffuse);
   glLightfv (GL_LIGHT0, GL_AMBIENT, LampeAmbient);
   glLightfv (GL_LIGHT0, GL_POSITION, LampePosition);
}

void Prepare ()
{
   NewtonUpdate (Scene::nWorld, 1.0f/80.0f);
}

void Shutdown ()
{
   NewtonDestroy (Scene::nWorld);
}
