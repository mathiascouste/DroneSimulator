#include "Objet.hpp"

#include "Drone.hpp"
#include <iostream>
#include <ctime>

using namespace std;

Objet::Objet ()
: m_pBody (NULL)
{
   // Ne fait rien
}

Objet::~Objet ()
{
   if(m_pBody!=NULL) {
      NewtonDestroyBody (m_pBody);
   }
}

void ApplyForceAndTorqueCallback(const NewtonBody * nBody, dFloat timestep, int threadIndex)
{
   // Cette fonction est une fonction Callback. Elle sera appelée à chaque fois
   // qu'une modification aura lieu sur le corps.

   // On récupère en premier lieu la masse ainsi que l'inertie
   float masse; // Contiendra la masse de l'objet pris en paramètre par la fonction
   CVector inertie; // Contiendra l'inertie du corps
   CVector force; // Spécifiera la force appliquée sur le corps
   CVector forceRot; // Spécifiera la force appliquée sur le corps

   NewtonBodyGetMassMatrix (nBody, &masse, &inertie.x, &inertie.y, &inertie.z);

   force.x = Drone::instance->force.m_x + 0.0f;
   //force.y = Drone::force.m_y - masse * 9.81;
   force.y = Drone::instance->force.m_y;
   force.z = Drone::instance->force.m_z + 0.0f;

   forceRot.x = Drone::instance->torque.m_x + 0.0f;
   forceRot.y = Drone::instance->torque.m_y + 0.0f;
   forceRot.z = Drone::instance->torque.m_z + 0.0f;

   NewtonBodyAddForce (nBody, &force.x); // On ajoute la force au corps
   NewtonBodyAddTorque(nBody, &forceRot.x);
}


CVector Objet::addTorque(CVector torque) {
	this->torque = this->torque + torque;
	return this->torque;
}
