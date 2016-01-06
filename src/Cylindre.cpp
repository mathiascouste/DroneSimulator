#include "Cylindre.hpp"

#include <GL/glut.h>

using namespace std;

Cylindre::Cylindre ()
: Objet ()
{
   // Ne fais rien
}

Cylindre::~Cylindre()
{
   // Ne fait rien, l'objet est détruit par la classe de base
}

void Cylindre::Initialiser (NewtonWorld * nWorld, double ray, double height, const CVector & position, 
	                 bool mobile, float masse)
{
	this->ray = ray;
	this->height = height;
	
	// si nWorld est NULL, le cylindre est juste affiche
   if(nWorld != NULL) {
	   // On définit la masse de l'objet
	   m_masse = masse;

	   matrix matrice; // On créé une matrice

	   // On initialise la matrice, qu'on définit comme matrice identité
	   for (int x = 0 ; x < 4 ; ++x)
		  for (int y = 0 ; y < 4 ; y++)
		  {
			 if (x == y)
				matrice.matrice [x][y] = 1.0f;
			 else
				matrice.matrice [x][y] = 0.0f;
		   }

	   // On définit la matrice de manière à ce que l'objet soit placé aux positions
	   // spécifiées en utilisant la dernière colonne de la matrice
	   matrice.matrice [3][0] = position.x;
	   matrice.matrice [3][1] = position.y;
	   matrice.matrice [3][2] = position.z;

		
	   // On initialise la boîte de collision
	   NewtonCollision * collision = NULL;

	   // On créé la boite de collision aux dimensions de l'objet
	   collision = NewtonCreateBox (nWorld, 1,1,1, 0, NULL);
	   //collision = NewtonCreateCylinder(nWorld, ray, height,0, NULL);

	   // On initialise le corps avec la boite de collision
	   m_pBody = NewtonCreateDynamicBody (nWorld, collision, &matrice.matrice[0][0]);

	   if (m_pBody == NULL)
		  std::cerr << "Impossible d'initialiser le corps.";

	   // On détruit la boite de collision, on n'en a plus besoin
	   //NewtonReleaseCollision (nWorld, collision);

	   // Enfin, on affecte notre matrice (qui représente donc sa position dans l'espace)
	   // à notre corps grâce à la fonction NewtonBodySetMatrix
	   NewtonBodySetMatrix (m_pBody, &matrice.matrice [0][0]);
	   

	   // On initialise à présent les propriétés physiques de l'objet. Toutefois, donner
	   // à un objet qui ne bougera pas une masse, lui associer un callback,... n'a aucun
	   // intêret, on vérifie donc si l'objet sera mobile ou immobile
	   if (mobile == true)
	   {
		  // On calcul l'inertie du corps, en passant par une petite formule
		  CVector inertie;

		  inertie.x = 0.25f * m_masse * (ray * ray + height * height / 3);
		  inertie.y = 0.25f * m_masse * (ray * ray + height * height / 3);
		  inertie.z = 0.5f * m_masse * ray * ray;

		  // On définit ensuite la masse et l'inertie pour ce corps
		  NewtonBodySetMassMatrix (m_pBody, m_masse, inertie.x, inertie.y, inertie.z);

		  // On règle enfin le Callback, qui sera nécessaire pour que le corps bouge
		  NewtonBodySetForceAndTorqueCallback(m_pBody, ApplyForceAndTorqueCallback);
	   }
	}
}

void Cylindre::SetColor (const CVector & couleur)
{
   m_couleur.x = couleur.x / 255;
   m_couleur.y = couleur.y / 255;
   m_couleur.z = couleur.z / 255;
}

void Cylindre::Dessiner ()
{
   // Toutes les modifications effectuées dans le Callback modifient la matrice de
   // l'objet, ce qui permet à l'objet de "bouger"
   matrix matrice;
   NewtonBodyGetMatrix (m_pBody, &matrice.matrice [0][0]);

   glPushMatrix (); // On sauvegarde la matrice actuelle
   {
      glMultMatrixf (&matrice.matrice [0][0]); // On multiplie la matrice actuelle
                                               // par la matrice du corps, ainsi
                                               // le corps sera dessiné au bon endroit
      
      
      glColorMaterial (GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
      glColor3f (m_couleur.x, m_couleur.y, m_couleur.z);
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
