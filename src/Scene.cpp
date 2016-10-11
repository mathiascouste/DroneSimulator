#include "Scene.hpp"

#include <GL/glut.h>

NewtonWorld * Scene::nWorld;

Scene::~Scene() {
	if(this->sol != NULL) {
		delete this->sol;
		this->sol = NULL;
	}
}

void Scene::initialize() {
	nWorld = NewtonCreate (/*NULL, NULL*/);
	srand (time (NULL));
	
	this->initGround();
}

void Scene::initGround() {
	
   // On définit le sol
   CVector taille (80.0f, 0.1f, 80.0f);
   CVector position (0.0f, -5.0f, 0.0f);
   CVector couleur (100, 100, 100);

   // On alloue la mémoire. Le troisième argument est false pour signifier qu'il sera
   // immobile et donc ne bougera pas
   sol = new Boite ();
   sol->SetColor (couleur);
   sol->Initialiser (nWorld, taille, position, false, 0.0f);
}

void Scene::render() {
   glClearColor(0.0f, 0.0, 0.3, 0.0);
   glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glLoadIdentity ();

   gluLookAt (0.0f, 0.0f, 40.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

   // Puis quelques rotations afin de bien voir le sol
   glRotatef(15, 1, 0, 0);
   glRotatef(45, 0, 1, 0);

   // On va d'abord dessiner le sol
   sol->Dessiner ();
}
