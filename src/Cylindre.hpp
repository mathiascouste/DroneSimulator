#ifndef _CYLINDRE_HPP_
#define _CYLINDRE_HPP_

#include <Newton.h> // Header pour utiliser le Newton Engine
#include "CVector.hpp"
#include "Objet.hpp"

class Cylindre : public Objet
{
   public:
      Cylindre (); // Constructeur
      virtual ~Cylindre (); // Destructeur

	  // Fonction se chargeant de l'initialisation de la boîte
      virtual void Initialiser (NewtonWorld * nWorld, double ray, double height, const CVector &, bool mobile = false, float masse = 1.0f);
	  virtual void Dessiner (); // Dessine l'objet
	  virtual void SetColor (const CVector &); // Règle les couleurs

   private:
	  double height;
	  double ray;
};

#endif
