

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
int charger_ressources(BITMAP **fond, BITMAP **collision_map, BITMAP **sprite1,
                       BITMAP **sprite2, BITMAP **img_gros, BITMAP **img_petit, BITMAP *roues[]);
void gerer_roues(int *etape_roue, int *dernier_changement_roue, int temps_actuel);
void gerer_saut(Joueur *joueur, int *sprite_state, int *timerinterne);
void appliquer_physique(Joueur *joueur);
int verifier_collision_roue(Joueur *joueur, BITMAP *roue, int roue_x, int roue_y, int decor_scroll);
void gerer_collisions(Joueur *joueur, BITMAP *collision_map, int decor_scroll);
int collision(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2);
void afficher_scene(BITMAP *page, BITMAP *fond, BITMAP *sprite1, BITMAP *sprite2,
                    BITMAP *roues[], int etape_roue, Joueur *joueur, int sprite_state,
                    float facteur_perso, int decor_scroll, int temps_actuel,
                    int afficher_gros, int gros_collision, BITMAP *img_gros,
                    int gros_x, int gros_y, int afficher_petit, int petit_collision, BITMAP *img_petit,
                    int petit_x, int petit_y);
void nettoyer_ressources(BITMAP *fond, BITMAP *collision_map, BITMAP *sprite1,
                         BITMAP *sprite2, BITMAP *img_gros, BITMAP *img_petit, BITMAP *roues[], BITMAP *page);


#endif //BADLAND_H
