#include "Motor.hpp"

#include <GL/glut.h>
#include <iostream>
using namespace std;

Motor::Motor(double size)
:size(size)
{
}

void Motor::initialize(double xPosition, double yPosition, double zPosition) {
	CVector position, couleur, taille;
    couleur.ReglerCoordonnees(255,0,0);
        
    position.ReglerCoordonnees (xPosition, yPosition, zPosition);
    taille.ReglerCoordonnees (size, size, size);
    
    this->body = new Boite ();
    this->body->Initialiser (Scene::nWorld, taille, position, true, 0.2f);
    this->body->SetColor (couleur);
}

void Motor::setMaxPower(double maxPower) {
	this->maxPower = maxPower;
}

void Motor::setPowerOrder(float order) {
	this->powerOrder = order;
}

void Motor::calculateForceAndTorque() {
		
	dMatrix matrix;
	NewtonBodyGetMatrix (this->body->m_pBody, &matrix[0][0]);
	
	Objet::forces[this->body->m_pBody].push(matrix.m_up*this->maxPower*this->powerOrder);
}

void Motor::Render() {
	double ray = size*0.5f;
	double height = size;
	// Toutes les modifications effectuées dans le Callback modifient la matrice de
   // l'objet, ce qui permet à l'objet de "bouger"
   matrix matrice;
   NewtonBodyGetMatrix (body->m_pBody, &matrice.matrice [0][0]);

   glPushMatrix (); // On sauvegarde la matrice actuelle
   {
      glMultMatrixf (&matrice.matrice [0][0]); // On multiplie la matrice actuelle
                                               // par la matrice du corps, ainsi
                                               // le corps sera dessiné au bon endroit
      //glTranslatef(x,y+height/2,z);
      glRotatef(90,1,0,0);
      
      glColor3f (0,255,0);
      
		GLUquadric* params = gluNewQuadric();
		gluQuadricDrawStyle(params,GLU_FILL);
		gluCylinder(params,ray,ray,height,20,1);

		gluDeleteQuadric(params);
   }
   glPopMatrix (); // On rétablit la matrice
}
