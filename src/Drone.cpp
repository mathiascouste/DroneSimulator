#include "Drone.hpp"

#include <GL/glut.h>
#include <iostream>
#include <math.h>

using namespace std;

Drone* Drone::instance = new Drone();

Drone::Drone()
:size(INITIAL_SIZE)
{
	this->frontMotor = Motor(size/16);
	this->rightMotor = Motor(size/16);
	this->backMotor = Motor(size/16);
	this->leftMotor = Motor(size/16);
}

Drone::~Drone() {
	delete this->body;
}

void Drone::genMaxPower(double maxPower, double moreOrLess) {
	this->frontMotor.setMaxPower(maxPower + ((double)rand())*2*moreOrLess/((double)RAND_MAX) - moreOrLess);
	this->rightMotor.setMaxPower(maxPower + ((double)rand())*2*moreOrLess/((double)RAND_MAX) - moreOrLess);
	this->backMotor.setMaxPower(maxPower + ((double)rand())*2*moreOrLess/((double)RAND_MAX) - moreOrLess);
	this->leftMotor.setMaxPower(maxPower + ((double)rand())*2*moreOrLess/((double)RAND_MAX) - moreOrLess);
}

void Drone::initialize() {
	CVector position, couleur, taille;
    couleur.ReglerCoordonnees(255,0,0);
        
    position.ReglerCoordonnees (0.0f, (double)INITIAL_HEIGHT, 0.0f);
    taille.ReglerCoordonnees (size, 0.25f*size, size);
    this->body = new Boite ();
    this->body->Initialiser (Scene::nWorld, taille, position, true, 0.2f);
    this->body->SetColor (couleur);
    
    double armLength = sqrt(98)*size/8;
    
    this->frontMotor.initialize(0, (double)INITIAL_HEIGHT, armLength);
    this->rightMotor.initialize(-armLength, (double)INITIAL_HEIGHT, 0);
    this->backMotor.initialize(0, (double)INITIAL_HEIGHT, -armLength);
    this->leftMotor.initialize(armLength, (double)INITIAL_HEIGHT, 0);
}

void Drone::render() {
	renderBodyAtPos();
	this->frontMotor.Render();
	this->rightMotor.Render();
	this->backMotor.Render();
	this->leftMotor.Render();
}

void Drone::renderBodyAtPos() {
	CVector m_taille;
	m_taille.x = 0.125f*size;
	m_taille.y = 0.125f*size;
	m_taille.z = 0.125f*size;
	
	matrix matrice;
   NewtonBodyGetMatrix (body->m_pBody, &matrice.matrice [0][0]);

   glPushMatrix (); // On sauvegarde la matrice actuelle
   {
      glMultMatrixf (&matrice.matrice [0][0]); // On multiplie la matrice actuelle
                                               // par la matrice du corps, ainsi
                                               // le corps sera dessiné au bon endroit
      glColor3f (1,0,0); // ROUGE

      glBegin(GL_QUADS);
         // Devant
         glNormal3f (0.0f, 0.0f, 1.0f);
         glVertex3f (-m_taille.x, -m_taille.y, m_taille.z);
         glVertex3f (m_taille.x, -m_taille.y, m_taille.z);
         glVertex3f (m_taille.x, m_taille.y, m_taille.z);
         glVertex3f (-m_taille.x, m_taille.y, m_taille.z);

         // Derrière
         glNormal3f (0.0f, 0.0f, -1.0f);
         glVertex3f (-m_taille.x, -m_taille.y, -m_taille.z);
         glVertex3f (-m_taille.x, m_taille.y, -m_taille.z);
         glVertex3f (m_taille.x, m_taille.y, -m_taille.z);
         glVertex3f (m_taille.x, -m_taille.y, -m_taille.z);

         // Haut
         glNormal3f (0.0f, 1.0f, 0.0f);
         glVertex3f (-m_taille.x, m_taille.y, -m_taille.z);
         glVertex3f (-m_taille.x, m_taille.y, m_taille.z);
         glVertex3f (m_taille.x, m_taille.y, m_taille.z);
         glVertex3f (m_taille.x, m_taille.y, -m_taille.z);

         // Bas
         glNormal3f (0.0f, -1.0f, 0.0f);
         glVertex3f (-m_taille.x, -m_taille.y, -m_taille.z);
         glVertex3f (m_taille.x, -m_taille.y, -m_taille.z);
         glVertex3f (m_taille.x, -m_taille.y, m_taille.z);
         glVertex3f (-m_taille.x, -m_taille.y, m_taille.z);

         // Droite
         glNormal3f (1.0f, 0.0f, 0.0f);
         glVertex3f (m_taille.x, -m_taille.y, -m_taille.z);
         glVertex3f (m_taille.x, m_taille.y, -m_taille.z);
         glVertex3f (m_taille.x, m_taille.y, m_taille.z);
         glVertex3f (m_taille.x, -m_taille.y, m_taille.z);

         // Gauche
         glNormal3f (-1.0f, 0.0f, 0.0f);
         glVertex3f (-m_taille.x, -m_taille.y, -m_taille.z);
         glVertex3f (-m_taille.x, -m_taille.y, m_taille.z);
         glVertex3f (-m_taille.x, m_taille.y, m_taille.z);
         glVertex3f (-m_taille.x, m_taille.y, -m_taille.z);
      glEnd ();
   }
   glPopMatrix (); // On rétablit la matrice
}

void Drone::setMotorPower(float front, float right, float back, float left) {
	this->frontMotor.setPowerOrder(front);
	this->rightMotor.setPowerOrder(right);
	this->backMotor.setPowerOrder(back);
	this->backMotor.setPowerOrder(left);
}

void Drone::calculateForceAndTorque() {
	this->frontMotor.calculateForceAndTorque();
	this->rightMotor.calculateForceAndTorque();
	this->backMotor.calculateForceAndTorque();
	this->leftMotor.calculateForceAndTorque();
}

dVector CalculateToqueAtPoint (const NewtonBody* body, const dVector& point, const dVector& force)
{
	dVector com;
	dMatrix matrix;
 
	NewtonBodyGetMatrix (body, &matrix[0][0]);
	NewtonBodyGetCentreOfMass (body, &matrix[0][0]);
	com = matrix.TransformVector (com);
	return (point - com) *  force;
}
