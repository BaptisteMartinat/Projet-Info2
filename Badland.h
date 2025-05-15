#ifndef BADLAND_H
#define BADLAND_H

#include <allegro.h>
#include <math.h>
// --- Constantes globales ---
#define SCREEN_W 800
#define SCREEN_H 600
#define SCENE_WIDTH 13500
#define DECOR_SCROLL_SPEED 3
#define GRAVITY 1
#define JUMP_STRENGTH -11
#define MAX_NAME_LENGTH 30
#define SAVE_FILE "save.txt"
#define NB_CHECKPOINTS 4
#define NB_ROUES 5
#define COLLISION_RADIUS 18

// --- Structures ---
typedef struct {
    int x, y, largeur, hauteur;
    int dx, dy;
    char nom[MAX_NAME_LENGTH];
    int niveau;
} Joueur;

typedef struct {
    int x, y;
    int frame;
} Roue;

typedef struct {
    int x, y, scroll;
    int actif;
} Checkpoint;


// --- Fonctions principales ---
void initialisation_allegro();
void init_jeu(Joueur *joueur);
void jeu_scrolling(const char *pseudo);
void menu_principal();
void menu_selection_map(const char *pseudo);

// --- Chargement / ressources ---
int charger_ressources(BITMAP **fond, BITMAP **collision_map, BITMAP **sprite1,
                       BITMAP **sprite2, BITMAP **img_gros, BITMAP **img_petit, BITMAP *roues[],BITMAP **img_trainee);
void nettoyer_ressources(BITMAP *fond, BITMAP *collision_map, BITMAP *sprite1,
                         BITMAP *sprite2, BITMAP *img_gros, BITMAP *img_petit,
                         BITMAP *roues[], BITMAP *page, BITMAP *img_trainee);

// --- Affichage ---
void afficher_scene(BITMAP *page, BITMAP *fond, BITMAP *sprite1, BITMAP *sprite2,
                    BITMAP *roues[], Roue roues_struct[], int nb_roues, Joueur *joueur, int sprite_state,
                    float facteur_perso, int decor_scroll, int temps_actuel,
                    int afficher_gros, int gros_collision, BITMAP *img_gros,
                    int gros_x, int gros_y, int afficher_petit, int petit_collision, BITMAP *img_petit,
                    int petit_x, int petit_y, BITMAP *img_checkpoint, Checkpoint checkpoints[],
                    const int positions_cp[][2], BITMAP *img_trainee, int temps_trainee);


// --- Gestion du joueur / actions ---
void gerer_saut(Joueur *joueur, int *sprite_state, int *timerinterne, int *temps_trainee);
void appliquer_physique(Joueur *joueur);
void gerer_collisions(Joueur *joueur, BITMAP *collision_map, int decor_scroll);
void afficher_mort(BITMAP *page, BITMAP *explosion, Joueur *joueur);
// --- Roues (affichage, animation, collisions) ---
void gerer_roues(int *etape_roue, int *dernier_changement_roue, int temps_actuel); // version simple
void maj_roues(Roue roues[], int *timer); // version avancée (animation par frames)
int gestion_collision_roues(Joueur *joueur, Roue roues[], int nb, int decor_scroll);
int verifier_collision_roue(Joueur *joueur, BITMAP *roue, int roue_x, int roue_y, int decor_scroll);

// --- Checkpoints ---
void maj_checkpoints(Joueur *joueur, Checkpoint checkpoints[], const int positions[][2], int *dernier_cp, int decor_scroll, BITMAP *sprite);
int joueur_sur_checkpoint(Joueur *joueur, int cp_x, int cp_y, int scroll, int sw, int sh);

// --- Arrivée / victoire ---
int detecter_arrivee(BITMAP *map, int cx, int cy, int rayon, int scroll);

// --- Utilitaire ---
int collision(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2);

#endif // BADLAND_H
