#include "Boite.hpp"

#include <GL/glut.h>

using namespace std;

Boite::Boite ()
: Objet ()
{
   // Ne fais rien
}

Boite::~Boite()
{
   // Ne fait rien, l'objet est détruit par la classe de base
}

void Boite::Initialiser (NewtonWorld * nWorld, const CVector & taille, const CVector & position, 
	                 bool mobile, float masse)
{
   // On initialise le vecteur de dimensions
   m_taille.x = taille.x;
   m_taille.y = taille.y;
   m_taille.z = taille.z;

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
   collision = NewtonCreateBox (nWorld, m_taille.x, m_taille.y, m_taille.z, 0, NULL);

   // On initialise le corps avec la boite de collision
   m_pBody = NewtonCreateDynamicBody (nWorld, collision, &matrice.matrice[0][0]);

   if (m_pBody == NULL)
      std::cerr << "Impossible d'initialiser le corps.";
   
   // On ajoute l'objet à la liste des forces
   Objet::forces[m_pBody] = stack<dVector>();
   Objet::torques[m_pBody] = stack<dVector>();

   // On détruit la boite de collision, on n'en a plus besoin
   //NewtonReleaseCollision (nWorld, collision);

   // Enfin, on affecte notre matrice (qui représente donc sa position dans l'espace)
   // à notre corps grâce à la fonction NewtonBodySetMatrix
   NewtonBodySetMatrix (m_pBody, &matrice.matrice [0][0]);

   // Rotation :
   /*dFloat omega[3] = {1.0f, 1.0f, 1.0f};
   NewtonBodySetOmega (m_pBody, &omega[0]);*/

   // On initialise à présent les propriétés physiques de l'objet. Toutefois, donner
   // à un objet qui ne bougera pas une masse, lui associer un callback,... n'a aucun
   // intêret, on vérifie donc si l'objet sera mobile ou immobile
   if (mobile == true)
   {
      // On calcul l'inertie du corps, en passant par une petite formule
      CVector inertie;

      inertie.x = 0.7f * m_masse * (m_taille.y * m_taille.y + m_taille.z * m_taille.z) / 12;
      inertie.y = 0.7f * m_masse * (m_taille.x * m_taille.x + m_taille.z * m_taille.z) / 12;
      inertie.z = 0.7f * m_masse * (m_taille.x * m_taille.x + m_taille.y * m_taille.y) / 12;

      // On définit ensuite la masse et l'inertie pour ce corps
      NewtonBodySetMassMatrix (m_pBody, m_masse, inertie.x, inertie.y, inertie.z);

      // On règle enfin le Callback, qui sera nécessaire pour que le corps bouge
      NewtonBodySetForceAndTorqueCallback(m_pBody, ApplyForceAndTorqueCallback);
   }

   m_taille.x *= 0.5f;
   m_taille.y *= 0.5f;
   m_taille.z *= 0.5f;
}

void Boite::SetColor (const CVector & couleur)
{
   m_couleur.x = couleur.x / 255;
   m_couleur.y = couleur.y / 255;
   m_couleur.z = couleur.z / 255;
}

void Boite::Dessiner ()
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
      //glColorMaterial (GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
      glColor3f (m_couleur.x, m_couleur.y, m_couleur.z);
      //glColorMaterial (GL_FRONT_AND_BACK, GL_SPECULAR);
      //glColor3f (1.0f, 1.0f, 1.0f);
      //glColorMaterial (GL_FRONT_AND_BACK, GL_EMISSION);
      //glColor3f (0.0f, 0.0f, 0.0f);
      //glMaterialf (GL_FRONT_AND_BACK, GL_SHININESS, 128);

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
