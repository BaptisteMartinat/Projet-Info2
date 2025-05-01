#include "BadLand.h"
#include <stdio.h>
#include <string.h>

void init_jeu(Joueur *joueur) {
    joueur->x = 100;
    joueur->y = SCREEN_H / 2;
    joueur->width = 40;
    joueur->height = 40;
    joueur->dy = 0;
    joueur->dx = 0;
}

void charger_joueur(Joueur *joueur) {
    FILE *f = fopen(SAVE_FILE, "r");
    if (f) {
        fscanf(f, "%s %d", joueur->name, &joueur->niveau);
        fclose(f);
    } else {
        BITMAP *page = create_bitmap(SCREEN_W, SCREEN_H);
        int index = 0;
        char c;
        joueur->name[0] = '\0';

        clear_keybuf();

        while (!key[KEY_ENTER]) {
            if (keypressed()) {
                c = readkey() & 0xFF;
                if (c >= 32 && c <= 126 && index < MAX_NAME_LENGTH - 1) {
                    joueur->name[index++] = c;
                    joueur->name[index] = '\0';
                } else if (c == 8 && index > 0) {
                    joueur->name[--index] = '\0';
                }
            }

            clear_to_color(page, makecol(30, 30, 30));
            textout_centre_ex(page, font, "Entrez votre pseudo :", SCREEN_W / 2, 120, makecol(255, 255, 255), -1);
            rectfill(page, SCREEN_W / 2 - 150, 170, SCREEN_W / 2 + 150, 200, makecol(255, 255, 255));
            rect(page, SCREEN_W / 2 - 150, 170, SCREEN_W / 2 + 150, 200, makecol(0, 0, 0));
            textout_centre_ex(page, font, joueur->name, SCREEN_W / 2, 178, makecol(0, 0, 0), -1);
            blit(page, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
            rest(20);
        }

        destroy_bitmap(page);
        joueur->niveau = 1;
    }
}

void sauvegarder_joueur(Joueur *joueur) {
    FILE *f = fopen(SAVE_FILE, "w");
    if (f) {
        fprintf(f, "%s %d", joueur->name, joueur->niveau);
        fclose(f);
    }
}

void generer_obstacles(Acteur *obstacles, int *nb) {
    *nb = 0;
    for (int i = 800; i < SCENE_WIDTH; i += 500) {
        obstacles[*nb].x = i;
        obstacles[*nb].y = SCREEN_H - 150;
        obstacles[*nb].tx = 40;
        obstacles[*nb].ty = 150;
        obstacles[*nb].couleur = makecol(0, 0, 0);
        (*nb)++;

        obstacles[*nb].x = i + 200;
        obstacles[*nb].y = 0;
        obstacles[*nb].tx = 40;
        obstacles[*nb].ty = 200;
        obstacles[*nb].couleur = makecol(0, 0, 0);
        (*nb)++;
    }
}

int collisionActeurs(Acteur *a, Acteur *b) {
    return (a->x < b->x + b->tx && a->x + a->tx > b->x &&
            a->y < b->y + b->ty && a->y + a->ty > b->y);
}

void update_jeu(Joueur *joueur, Acteur *obstacles, int nb, int decor_scroll) {
    joueur->dy += GRAVITY;

    int new_y = joueur->y + joueur->dy;
    int new_x = joueur->x + joueur->dx;

    joueur->y = new_y;

    Acteur temp = {
        joueur->x + decor_scroll, joueur->y, 0, 0,
        joueur->width, joueur->height, 0
    };

    for (int i = 0; i < nb; i++) {
        if (collisionActeurs(&temp, &obstacles[i])) {
            joueur->dx = -2;
            break;
        }
    }

    if (joueur->y < 10) {
        joueur->y = 10;
        joueur->dy = 0;
    }
    if (joueur->y + joueur->height > SCREEN_H - 10) {
        joueur->y = SCREEN_H - joueur->height - 10;
        joueur->dy = 0;
    }

    temp.y = joueur->y;
    temp.x = joueur->x + decor_scroll + 1;
    for (int i = 0; i < nb; i++) {
        if (collisionActeurs(&temp, &obstacles[i])) {
            joueur->x -= 1;
            joueur->dx = -2;
        }
    }

    temp.x = joueur->x + decor_scroll - 1;
    for (int i = 0; i < nb; i++) {
        if (collisionActeurs(&temp, &obstacles[i])) {
            joueur->x += 1;
            joueur->dx = -2;
        }
    }

    joueur->x += joueur->dx;
    if (joueur->x < SCREEN_W / 3) {
        joueur->dx = 1;
    }
}

void draw_jeu(Joueur *joueur, BITMAP *page, int decor_scroll, Acteur *obstacles, int nb, BITMAP *sprite) {
    clear_to_color(page, makecol(100, 100, 100));
    rectfill(page, 0, 0, SCREEN_W, 10, makecol(0, 0, 0)); // plafond
    rectfill(page, 0, SCREEN_H - 10, SCREEN_W, SCREEN_H, makecol(0, 0, 0)); // sol

    for (int i = 0; i < nb; i++) {
        int x = obstacles[i].x - decor_scroll;
        if (x + obstacles[i].tx > 0 && x < SCREEN_W) {
            rectfill(page, x, obstacles[i].y,
                     x + obstacles[i].tx, obstacles[i].y + obstacles[i].ty,
                     obstacles[i].couleur);
        }
    }

    draw_sprite(page, sprite, joueur->x, joueur->y);
}

int menu(BITMAP *page) {
    while (1) {
        clear_to_color(page, makecol(50, 50, 50));
        textout_centre_ex(page, font, "BADLAND - MENU", SCREEN_W / 2, 150, makecol(255, 255, 255), -1);
        textout_centre_ex(page, font, "Appuyez sur ENTREE pour jouer", SCREEN_W / 2, 200, makecol(200, 200, 200), -1);
        textout_centre_ex(page, font, "ECHAP pour quitter", SCREEN_W / 2, 230, makecol(200, 200, 200), -1);
        blit(page, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);

        if (key[KEY_ENTER]) return 1;
        if (key[KEY_ESC]) return 0;

        rest(20);
    }
}
