#ifndef _MAPGENERATOR_HPP_
#define _MAPGENERATOR_HPP_

#include <SDL/SDL.h>
#define FREQUENCE 5
#define OCTAVE 4
#define PERSISTANCE 0.5
#define LISSAGE 7
#define WORLD_SIZE 100

struct calque{
    int **v;
    int taille;
    float persistance;
};

struct calque* init_calque(int, float);
void free_calque(struct calque*);
void generer_calque(int frequence, int octaves, float persistance, int liss, struct calque *c);
int interpolate(int, int, int, int);
int valeur_interpolee(int, int, int, struct calque*);

#endif
