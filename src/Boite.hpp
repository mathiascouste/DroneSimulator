#ifndef _BOITE_HPP_
#define _BOITE_HPP_

#include <Newton.h> // Header pour utiliser le Newton Engine
#include "CVector.hpp"
#include "Objet.hpp"

class Boite : public Objet
{
   public:
      Boite (); // Constructeur
      virtual ~Boite (); // Destructeur

	  // Fonction se chargeant de l'initialisation de la boîte
      virtual void Initialiser (NewtonWorld * nWorld, const CVector &, const CVector &, bool mobile = false, float masse = 1.0f);
	  virtual void Dessiner (); // Dessine l'objet
	  virtual void SetColor (const CVector &); // Règle les couleurs

   private:
	  CVector m_taille; // Dimensions de la boîte
};

#endif
