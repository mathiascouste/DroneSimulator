#include <iostream>
#include <ctime>
#include <SDL/SDL.h>
#include <GL/glew.h>
#include "Scene.hpp"
#include "Drone.hpp"
#include "FlyComputerInterface.hpp"

#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds

using namespace std;

void Prepare ();
void InitSDL ();
void InitOGL ();
void Shutdown ();

int main (int argc, char *argv [])
{
	Scene scene;
	
	Drone::instance->genMaxPower(1.962, 0);
	
	InitSDL();
	InitOGL();

	// On initialise Newton
	scene.initialize();
	Drone::instance->initialize();

   SDL_Event event;
   bool bQuit = false;
   
   Drone::instance->setMotorPower(1, 0.75, 0.5, 0.25);

   while (!bQuit)
   {
      Prepare ();
	  Drone::instance->calculateForceAndTorque();
	  
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
      
      std::this_thread::sleep_for (std::chrono::milliseconds(1000/80));
      
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
}

void Prepare ()
{
   NewtonUpdate (Scene::nWorld, 1.0f/80.0f);
}

void Shutdown ()
{
   NewtonDestroy (Scene::nWorld);
}
