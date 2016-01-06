#include "Drone.hpp"

#include <GL/glut.h>
#include <iostream>

Drone* Drone::instance = new Drone();

Drone::Drone()
:maxPowerNE(1),maxPowerSE(1),maxPowerSO(1),maxPowerNO(1),size(INITIAL_SIZE)
{}

Drone::~Drone() {
}

void Drone::genMaxPower(double maxPower, double moreOrLess) {
	maxPowerNE = maxPower + ((double)rand())*2*moreOrLess/((double)RAND_MAX) - moreOrLess;
	maxPowerSE = maxPower + ((double)rand())*2*moreOrLess/((double)RAND_MAX) - moreOrLess;
	maxPowerSO = maxPower + ((double)rand())*2*moreOrLess/((double)RAND_MAX) - moreOrLess;
	maxPowerNO = maxPower + ((double)rand())*2*moreOrLess/((double)RAND_MAX) - moreOrLess;
}

void Drone::initialize() {
	CVector position, couleur, taille;
    couleur.ReglerCoordonnees(255,0,0);
        
    position.ReglerCoordonnees (0.0f, (double)INITIAL_HEIGHT, 0.0f);
    //taille.ReglerCoordonnees (0.25f*size, 0.25f*size, 0.25f*size);
    taille.ReglerCoordonnees (size, 0.25f*size, size);
    this->body = new Boite ();
    this->body->Initialiser (Scene::nWorld, taille, position, true, 1.0f);
    this->body->SetColor (couleur);
}

void Drone::render() {
	renderBodyAtPos();
	renderMotorAtPos(0.375f*size,0,0.375f*size);
	renderMotorAtPos(0.375f*size,0,-0.375f*size);
	renderMotorAtPos(-0.375f*size,0,-0.375f*size);
	renderMotorAtPos(-0.375f*size,0,0.375f*size);
}

void Drone::renderBodyAtPos() {
	CVector m_taille;
	m_taille.x = 0.25f*size;
	m_taille.y = 0.125f*size;
	m_taille.z = 0.25f*size;
	
	matrix matrice;
   NewtonBodyGetMatrix (body->m_pBody, &matrice.matrice [0][0]);

   glPushMatrix (); // On sauvegarde la matrice actuelle
   {
      glMultMatrixf (&matrice.matrice [0][0]); // On multiplie la matrice actuelle
                                               // par la matrice du corps, ainsi
                                               // le corps sera dessiné au bon endroit
      glColorMaterial (GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
      glColor3f (1,0,0);
      glColorMaterial (GL_FRONT_AND_BACK, GL_SPECULAR);
      glColor3f (1.0f, 1.0f, 1.0f);
      glColorMaterial (GL_FRONT_AND_BACK, GL_EMISSION);
      glColor3f (0.0f, 0.0f, 0.0f);
      glMaterialf (GL_FRONT_AND_BACK, GL_SHININESS, 128);

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

void Drone::renderMotorAtPos(float x, float y, float z) {
	double ray = size*0.125f;
	double height = size*0.25f;
	// Toutes les modifications effectuées dans le Callback modifient la matrice de
   // l'objet, ce qui permet à l'objet de "bouger"
   matrix matrice;
   NewtonBodyGetMatrix (body->m_pBody, &matrice.matrice [0][0]);

   glPushMatrix (); // On sauvegarde la matrice actuelle
   {
      glMultMatrixf (&matrice.matrice [0][0]); // On multiplie la matrice actuelle
                                               // par la matrice du corps, ainsi
                                               // le corps sera dessiné au bon endroit
      glTranslatef(x,y+height/2,z);
      glRotatef(90,1,0,0);
      
      glColorMaterial (GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
      glColor3f (0,255,0);
      glColorMaterial (GL_FRONT_AND_BACK, GL_SPECULAR);
      glColor3f (1.0f, 1.0f, 1.0f);
      glColorMaterial (GL_FRONT_AND_BACK, GL_EMISSION);
      glColor3f (0.0f, 0.0f, 0.0f);
      glMaterialf (GL_FRONT_AND_BACK, GL_SHININESS, 128);
      
		GLUquadric* params = gluNewQuadric();
		gluQuadricDrawStyle(params,GLU_FILL);
		gluCylinder(params,ray,ray,height,20,1);

		gluDeleteQuadric(params);
   }
   glPopMatrix (); // On rétablit la matrice
}

void Drone::setMotorPower(float ne, float se, float so, float no) {
	dMatrix matrix;
	NewtonBodyGetMatrix (this->body->m_pBody, &matrix[0][0]);
	
	dVector up = matrix.m_up;
	
	ne *= maxPowerNE;
	se *= maxPowerSE;
	so *= maxPowerSO;
	no *= maxPowerNO;
	
	std::cout << "ne : " << ne << std::endl;
	std::cout << "se : " << se << std::endl;
	std::cout << "so : " << so << std::endl;
	std::cout << "no : " << no << std::endl;
	
	torque = CalculateToqueAtPoint (this->body->m_pBody, dVector(0.375f*size,0,0.375f*size), up*ne);
	force = up*ne;
	torque += CalculateToqueAtPoint (this->body->m_pBody, dVector(0.375f*size,0,-0.375f*size), up*se);
	force += up*se;
	torque += CalculateToqueAtPoint (this->body->m_pBody, dVector(-0.375f*size,0,-0.375f*size), up*so);
	force += up*so;
	torque += CalculateToqueAtPoint (this->body->m_pBody, dVector(-0.375f*size,0,0.375f*size), up*no);
	force += up*no;
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
