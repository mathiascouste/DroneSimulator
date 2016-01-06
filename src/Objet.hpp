#ifndef _OBJET_HPP_
#define _OBJET_HPP_

#include <iostream>
#include <Newton.h> // Header pour utiliser le Newton Engine
#include "CVector.hpp"


// Structure matrix
struct matrix
{
	float matrice[4][4];
};

class Objet
{
   // Fonction amie qui se chargera d'appliquer les forces à l'objet.
   // Elle sera utilisée en fonction Callback par le Newton Engine
/*   friend void ApplyForceAndTorqueCallback(const NewtonBody * nBody, dFloat timestep, int threadIndex);*/

   public:
      Objet (); // Constructeur
      virtual ~Objet (); // Destructeur

      virtual void Initialiser () {};
      virtual void Dessiner () = 0; // Classe pure
      virtual void SetColor (const CVector &) = 0;
      NewtonBody * m_pBody; // Pointeur vers un NewtonBody

		CVector addTorque(CVector torque);

   protected:
      float m_masse; // Masse de l'objet
      CVector m_couleur; // Couleur de l'objet
      CVector torque;
};

   // Fonction amie qui se chargera d'appliquer les forces à l'objet.
   // Elle sera utilisée en fonction Callback par le Newton Engine
void ApplyForceAndTorqueCallback(const NewtonBody * nBody, dFloat timestep, int threadIndex);

#endif
