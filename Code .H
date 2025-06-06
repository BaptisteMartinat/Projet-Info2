#ifndef BADLAND_H
#define BADLAND_H

#include <allegro.h>
#include <math.h>
// --- Constantes globales ---
#define MY_SCREEN_W 800
#define MY_SCREEN_H 600
#define FIN_MER_X 9450  // Position X de l'image de fin
#define FIN_MER_Y 150    // Position Y de l'image de fin
#define SCENE_WIDTH 13500
#define DECOR_SCROLL_SPEED 3

#define GRAVITY 1
#define JUMP_STRENGTH -11
#define NB_CHAMPI 4
#define NB_LIANE 4
#define MAX_NAME_LENGTH 30
#define SAVE_FILE "save.txt"
#define NB_CHECKPOINTS 4
#define NB_ROUES 5  // Modifier la valeur existante
#define NB_BONUS_TYPES 4  // Gros, Petit, Vite
#define NB_BONUS_PAR_TYPE 3  // Nombre d'instances de chaque type
#define NB_POISSON 4
#define NB_CRABE 7

#define COLLISION_RADIUS 18
SAMPLE* charger_musique(const char* path);

// --- Structures ---
// Types de niveaux
typedef enum {
    NIVEAU_VOLCAN,
    NIVEAU_MONTAGNE,
    NIVEAU_MER,
    NB_NIVEAUX
} TypeNiveau;

// --- Structures ---
typedef struct {
    int x, y;
    int dx, dy;
    int largeur, hauteur;
    char nom[MAX_NAME_LENGTH];
    int niveau; // TypeNiveau
    int bonus_vite_actif;  // Nouveau: état du bonus vitesse
    int sprite_state; // 0=normal, 1=saut
} Joueur;


typedef struct {
    int x, y;
    int frame;
    int sens_vertical; // 1 = descend, -1 = monte
    int limite_haut;
    int limite_bas;
} Roue;

typedef struct {
    int x, y, scroll;
    int actif;
} Checkpoint;



typedef struct {
    int x, y, scroll;

} champi;

typedef struct {
    int x, y, scroll;

} liane;

typedef struct {
    int x, y, scroll;

} poisson;

typedef struct {
    int x, y, scroll;

} crabe;



typedef struct {
    char* fond_path;
    char* collision_path;
    char* sprite1_path;
    char* sprite2_path;
    char* img_vite_path;
    char* img_gros_path;
    char* explosion_path;
    char* champi_path;
    char* liane_path;
    char* poisson_path;
    char* crabe_path;
    char* img_invincible_path;
    int positions_champi[NB_CHAMPI][2];
    int positions_liane[NB_LIANE][2];
    int positions_poisson[NB_POISSON][2];
    int positions_crabe[NB_CRABE][2];
    char* trainee_path;
    char* img_petit_path;
    char* img_fin_path;
    int gros_positions[3][2];  // Jusqu'à 3 bonus "gros"
    int petit_positions[3][2]; // Jusqu'à 3 bonus "petit"
    int vite_positions[3][2];  // Jusqu'à 3 bonus "vite"
    int nb_gros;               // Nombre réel de bonus "gros" dans ce niveau
    int nb_petit;              // Nombre réel de bonus "petit"
    int nb_vite;               // Nombre réel de bonus "vite"
    char* roues_paths[4];
    char* checkpoint_path;
    char* musique_path;
    int positions_cp[NB_CHECKPOINTS][2];
    Roue roues_positions[NB_ROUES];
} NiveauRessources;

typedef struct {
    int x, y;
    int actif;
    int type; // 0=gros, 1=petit, 2=vite
} Bonus;

#define MAX_BONUS 10 // Nombre maximum de bonus par niveau


typedef struct {
    Joueur joueur;
    TypeNiveau niveau_actuel;
    int decor_scroll;
    int temps_jeu;
    int dernier_cp;
    int joueur_est_mort;
    int positions_cp[NB_CHECKPOINTS][2];
    int position_champi[NB_CHAMPI][2];
    int position_liane[NB_LIANE][2];
    int position_poisson[NB_POISSON][2];
    int position_crabe[NB_CRABE][2];
    int timer_saut;
    int temps_trainee;
    int temps_depart;


    // Ressources graphiques
    BITMAP *fond;
    BITMAP *page;
    BITMAP *collision_map;
    BITMAP *sprite_joueur[2]; // 0=normal, 1=saut
    BITMAP *img_bonus[4]; // 0=gros, 1=petit, 2=vite
    BITMAP *img_explosion;
    BITMAP *img_trainee;
    BITMAP *img_checkpoint;
    BITMAP *img_champi;
    BITMAP *img_liane;
    BITMAP *img_poisson;
    BITMAP *img_crabe;
    BITMAP *roues[4];
    BITMAP *img_fin_mer;  // Image de fin du niveau mer


    // Éléments du jeu
    int timer_roues;
    Roue roues_struct[NB_ROUES];

    Checkpoint checkpoints[NB_CHECKPOINTS];
    champi champis[NB_CHAMPI];
    liane lianes[NB_LIANE];
    poisson poissons[NB_POISSON];
    crabe crabes[NB_CRABE];

    // Sons
    SAMPLE *musique_niveau;
    SAMPLE *son_saut;
    SAMPLE *son_explosion;
    SAMPLE *musique_menu;
    SAMPLE *musique_jeu;
    SAMPLE *son_checkpoint;


    // État des bonus
    float facteur_perso; // Facteur de taille (0.8 pour petit, 1.5 pour gros)
    int temps_bonus[3];

    int bonus_pos[NB_BONUS_TYPES][NB_BONUS_PAR_TYPE][2];  // Positions [type][instance][x/y]
    int bonus_actifs[NB_BONUS_TYPES][NB_BONUS_PAR_TYPE];  // Activation
    int bonus_collision[NB_BONUS_TYPES][NB_BONUS_PAR_TYPE]; // Collision
    int temps_bonus_vite;
    int vitesse_scroll;
    Bonus bonus[MAX_BONUS];
    int nb_bonus;
    int invincible_aux_lianes;  // 1 = invincible, 0 = vulnérable
    int temps_invincible_lianes; // Durée restante


} EtatJeu;


// Ajouter le prototype de fonction
void gestion_collision_champis(EtatJeu *etat);
void gestion_collision_lianes(EtatJeu *etat);

// --- Fonctions principales ---
// --- Fonctions principales ---
void initialisation_allegro();
void init_jeu(EtatJeu *etat, TypeNiveau niveau, const char *pseudo);
void jeu_scrolling(const char *pseudo, TypeNiveau niveau, SAMPLE *lave, SAMPLE *menu, SAMPLE *montagne,SAMPLE *mer);
void menu_principal(SAMPLE *lave, SAMPLE *menu, SAMPLE* montagne, SAMPLE* mer);
void charger_ressources_niveau(EtatJeu *etat);

void nettoyer_ressources(EtatJeu *etat);
// --- Affichage ---
void afficher_scene(EtatJeu *etat);

// --- Gestion du joueur / actions ---
void gerer_saut(EtatJeu *etat);
void appliquer_physique(EtatJeu *etat);
void gerer_collisions(EtatJeu *etat);
void gestion_collision_crabe(EtatJeu *etat);
void gestion_collision_poisson(EtatJeu *etat);

// --- Roues (affichage, animation, collisions) ---
void gerer_roues(int *etape_roue, int *dernier_changement_roue, int temps_actuel); // version simple
void maj_roues(EtatJeu *etat);
void gestion_collision_roues(EtatJeu *etat);

int verifier_collision_roue(Joueur *joueur, BITMAP *roue, int roue_x, int roue_y, int decor_scroll);

// --- Checkpoints ---
void maj_checkpoints(EtatJeu *etat);
int joueur_sur_checkpoint(Joueur *joueur, int cp_x, int cp_y, int scroll, int sw, int sh);

// --- Arrivée / victoire ---
int detecter_arrivee(EtatJeu *etat);

// --- Utilitaire ---

int collision(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2);
void initialisation_audio();
SAMPLE* charger_musique(const char* path);
void jouer_musique(SAMPLE* musique, int loop);
void arreter_musique();

#endif // BADLAND_H
