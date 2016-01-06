#include "MapGenerator.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

unsigned char aleatoire(float a){
	return (float)rand() / RAND_MAX * a;
}

struct calque* init_calque(int t, float p){
    struct calque *s = (struct calque *) malloc(sizeof(struct calque));
    if(!s){
        printf("erreur d'alloc");
        return NULL;
    }

    s->v = (int**)malloc(t*sizeof(int*));
    if(!s->v){
        printf("erreur d'alloc");
        return NULL;
    }
    int i,j;
    for (i=0; i<t ; i++){
        s->v[i]= (int*)malloc(t*sizeof(int));
        if(!s->v[i]) {
            printf("erreur d'alloc");
            return NULL;
        }
        for (j=0; j<t; j++)
            s->v[i][j]=0;
    }
    s->taille = t;
    s->persistance = p;

    return s;
}

void free_calque(struct calque* s){
    int j;
    for (j=0; j<s->taille; j++)
        free(s->v[j]);
    free(s->v);
}

void generer_calque(int frequence,
                    int octaves,
                    float persistance,
                    int liss,
                    struct calque *c){
    // itératif
    int taille = c->taille;
    int i,j,n,f_courante;
    int x,y,k,l;
    int a;
    float pas, sum_persistances;

    pas = (float)(taille)/frequence;
    float persistance_courante = persistance;

    // calque aléatoire
    struct calque *random;
    random = init_calque(taille,1);
    if (!random)
        return;

    for (i=0; i<taille; i++)
        for (j=0; j<taille; j++)
            random->v[i][j]=aleatoire(256);

    // calques de travail
    struct calque **mes_calques = ( struct calque **) malloc(octaves*sizeof(struct calque*));
    for (i=0; i<octaves; i++){
        mes_calques[i] = init_calque(taille,persistance_courante);
        if (!mes_calques[i])
            return;
        persistance_courante*=persistance;
    }

    f_courante = frequence;

    // remplissage de calque
    for (n=0; n<octaves; n++){
        for(i=0; i<taille; i++)
            for(j=0; j<taille; j++) {
                a = valeur_interpolee(i, j, f_courante, random);
                mes_calques[n]->v[i][j]=a;
            }
        f_courante*=frequence;
    }

    sum_persistances = 0;
    for (i=0; i<octaves; i++)
        sum_persistances+=mes_calques[i]->persistance;

    // ajout des calques successifs
    for (i=0; i<taille; i++)
        for (j=0; j<taille; j++){
            for (n=0; n<octaves; n++)
                c->v[i][j]+=mes_calques[n]->v[i][j]*mes_calques[n]->persistance;

            // normalisation
            c->v[i][j] =  c->v[i][j] / sum_persistances;
        }


    // lissage
    struct calque *lissage;
    lissage = init_calque(taille, 0);
    if(!lissage)
        return;

    for (x=0; x<taille; x++)
        for (y=0; y<taille; y++){
            a=0;
            n=0;
            for (k=x-liss; k<=x+liss; k++)
                for (l=y-liss; l<=y+liss; l++)
                    if ((k>=0) && (k<taille) && (l>=0) && (l<taille)) {
                        n++;
                        a+=c->v[k][l];
                    }
            lissage->v[x][y] = (float)a/n;
        }


    // libération mémoire
    free_calque(random);
    free_calque(lissage);
    for (i=0; i<octaves; i++)
        free_calque(mes_calques[i]);
    free(mes_calques);
}

int interpolate(int y1, int y2, int n, int delta){

    // interpolation non linéaire
    if (n==0)
        return y1;
    if (n==1)
        return y2;

    float a = (float)delta/n;

    float fac1 = 3*pow(1-a, 2) - 2*pow(1-a,3);
    float fac2 = 3*pow(a, 2) - 2*pow(a, 3);

    return y1*fac1 + y2*fac2;

	//////////////////////////////////////////////

    // interpolation linéaire
    /*if (n!=0)
        return y1+delta*((float)y2-(float)y1)/(float)n;
    else
        return y1;*/
}


int valeur_interpolee(int i, int j, int frequence, struct calque *r){
    // valeurs des bornes
    int borne1x, borne1y, borne2x, borne2y, q;
    float pas;
    pas = (float)r->taille/frequence;

    q = (float)i/pas;
    borne1x = q*pas;
    borne2x = (q+1)*pas;

    if(borne2x >= r->taille)
        borne2x = r->taille-1;

    q = (float)j/pas;
    borne1y = q*pas;
    borne2y = (q+1)*pas;

    if(borne2y >= r->taille)
        borne2y = r->taille-1;

    int b00,b01,b10,b11;
    b00 = r->v[borne1x][borne1y];
    b01 = r->v[borne1x][borne2y];
    b10 = r->v[borne2x][borne1y];
    b11 = r->v[borne2x][borne2y];

    int v1 = interpolate(b00, b01, borne2y-borne1y, j-borne1y);
    int v2 = interpolate(b10, b11, borne2y-borne1y, j-borne1y);
    int fin = interpolate(v1, v2, borne2x-borne1x , i-borne1x);

    return fin;
}

