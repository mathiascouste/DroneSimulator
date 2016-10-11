#include "Objet.hpp"

#include "Drone.hpp"
#include <iostream>
#include <ctime>

using namespace std;

map<const NewtonBody *, stack<dVector> > Objet::forces;
map<const NewtonBody *, stack<dVector> > Objet::torques;

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

   NewtonBodyGetMass(nBody, &masse, &inertie.x, &inertie.y, &inertie.z);

   force.x = 0.0f;
   force.y = masse * -9.81;
   force.z = 0.0f;

   forceRot.x = 0.0f;
   forceRot.y = 0.0f;
   forceRot.z = 0.0f;

   NewtonBodyAddForce (nBody, &force.x); // On ajoute la force au corps
   NewtonBodyAddTorque(nBody, &forceRot.x);
   
   while(!Objet::forces[nBody].empty()) {
	   dVector dForce = Objet::forces[nBody].top();
	   Objet::forces[nBody].pop();
	   float dF[3] = {dForce.m_x, dForce.m_y, dForce.m_z};
	   NewtonBodyAddForce (nBody, dF);
   }
   
   while(!Objet::torques[nBody].empty()) {
	   dVector dTorque = Objet::torques[nBody].top();
	   Objet::torques[nBody].pop();
	   float dT[3] = {dTorque.m_x, dTorque.m_y, dTorque.m_z};
	   NewtonBodyAddTorque (nBody, dT);
   }
}


CVector Objet::addTorque(CVector torque) {
	this->torque = this->torque + torque;
	return this->torque;
}
