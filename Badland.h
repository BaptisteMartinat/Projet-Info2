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
#define COLLISION_RADIUS 40
#define NB_CHECKPOINTS 5

typedef struct {
    int x, y;
    int dx, dy;
    int largeur, hauteur;
    char nom[MAX_NAME_LENGTH];
    int niveau;
} Joueur;

typedef struct {
    int x, y;
    int scroll;
    int actif;
} Checkpoint;

void initialisation_allegro();
void init_jeu(Joueur *joueur);
void jeu_scrolling(const char *pseudo);
void draw_jeu(Joueur *joueur, BITMAP *page, int decor_scroll, BITMAP *sprite);
void menu_principal();
void menu_selection_map(const char *pseudo);

int detecter_checkpoint(BITMAP *map, int cx, int cy, int rayon, int scroll, const int checkpoint_couleurs[], int nb_checkpoints);
void init_checkpoints(Checkpoint checkpoints[NB_CHECKPOINTS]);

#endif //BADLAND_H
