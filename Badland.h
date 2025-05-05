

#ifndef BADLAND_H
#define BADLAND_H

#include <allegro.h>

#define SCREEN_W 800
#define SCREEN_H 600
#define SCENE_WIDTH 13500
#define DECOR_SCROLL_SPEED 3
#define GRAVITY 1
#define JUMP_STRENGTH -11
#define MAX_NAME_LENGTH 30
#define SAVE_FILE "save.txt"

typedef struct {
    int x, y;
    int dx, dy;
    int largeur, hauteur;
    char nom[MAX_NAME_LENGTH];
    int niveau;
} Joueur;

void initialisation_allegro();
void init_jeu(Joueur *joueur);
void jeu_scrolling(const char *pseudo);
void draw_jeu(Joueur *joueur, BITMAP *page, int decor_scroll, BITMAP *sprite);
void menu_principal();
void menu_selection_map(const char *pseudo);


#endif //BADLAND_H
