#ifndef BADLAND_H
#define BADLAND_H

#include <allegro.h>

#define SCREEN_W 800
#define SCREEN_H 600
#define SCENE_WIDTH 13500
#define DECOR_SCROLL_SPEED 3
#define GRAVITY 1
#define JUMP_STRENGTH -10
#define MAX_NAME_LENGTH 30
#define MAX_OBSTACLES 100
#define SAVE_FILE "save.txt"

typedef struct {
    int x, y;
    int dx, dy;
    int tx, ty;
    int couleur;
    int width, height;
} Acteur;

typedef struct {
    int x, y;
    int dx, dy;
    int width, height;
    char name[MAX_NAME_LENGTH];
    int niveau;
} Joueur;

void init_jeu(Joueur *joueur);
void charger_joueur(Joueur *joueur);
void sauvegarder_joueur(Joueur *joueur);
void generer_obstacles(Acteur *obstacles, int *nb);
void update_jeu(Joueur *joueur, Acteur *obstacles, int nb, int decor_scroll);
void draw_jeu(Joueur *joueur, BITMAP *page, int decor_scroll, Acteur *obstacles, int nb, BITMAP *sprite);
int collisionActeurs(Acteur *a, Acteur *b);
int menu(BITMAP *page);

#endif
