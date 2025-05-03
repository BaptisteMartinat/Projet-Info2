

#include "Badland.h"

#include <stdio.h>
#include <string.h>
#include <time.h>

void initialisation_allegro() {
    allegro_init();
    install_keyboard();
    install_mouse();
    set_color_depth(desktop_color_depth());
    if (set_gfx_mode(GFX_AUTODETECT_WINDOWED, SCREEN_W, SCREEN_H, 0, 0) != 0) {
        allegro_message("Problème mode graphique");
        exit(EXIT_FAILURE);
    }
}

void init_jeu(Joueur *joueur) {
    joueur->x = 100;
    joueur->y = SCREEN_H / 2;
    joueur->largeur = 40;
    joueur->hauteur = 40;
    joueur->dy = 0;
    joueur->dx = 0;
}

void draw_jeu(Joueur *joueur, BITMAP *page, int decor_scroll, BITMAP *sprite) {
    draw_sprite(page, sprite, joueur->x, joueur->y);
}

void jeu_scrolling(const char *pseudo) {
    Joueur joueur;
    int decor_scroll = 0;
    BITMAP *page = create_bitmap(SCREEN_W, SCREEN_H);
    BITMAP *fond = load_bitmap("grotte_lave.bmp", NULL);
    BITMAP *sprite1 = load_bitmap("personnage1.bmp", NULL);
    BITMAP *sprite2 = load_bitmap("personnage2.bmp", NULL);
    int temps_depart = clock();

    if (!fond || !sprite1 || !sprite2) {
        allegro_message("Erreur chargement ressources !");
        return;
    }

    init_jeu(&joueur);
    strcpy(joueur.nom, pseudo);
    joueur.niveau = 1;
    int sprite_state = 0;

    while (!key[KEY_ESC]) {
        if (keypressed() && (readkey() >> 8) == KEY_SPACE) {
            sprite_state = !sprite_state;
            joueur.dy = JUMP_STRENGTH;
        }

        decor_scroll += DECOR_SCROLL_SPEED;
        joueur.dy += GRAVITY;
        joueur.y += joueur.dy;

        if (joueur.y < 10) joueur.y = 10, joueur.dy = 0;
        if (joueur.y + joueur.hauteur > SCREEN_H - 10) joueur.y = SCREEN_H - joueur.hauteur - 10, joueur.dy = 0;
        if (joueur.x + joueur.largeur < 0) {
            allegro_message("GAME OVER - Vous êtes sorti de l'écran !");
            break;
        }

        clear_bitmap(page);
        blit(fond, page, decor_scroll, 0, 0, 0, SCREEN_W, SCREEN_H);
        BITMAP *sprite = sprite_state ? sprite2 : sprite1;
        draw_jeu(&joueur, page, decor_scroll, sprite);

        char buffer_temps[50];
        int temps_ecoule = (clock() - temps_depart) / CLOCKS_PER_SEC;
        sprintf(buffer_temps, "Temps : %d s", temps_ecoule);
        textout_ex(page, font, buffer_temps, 10, 10, makecol(255, 255, 255), -1);

        blit(page, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
        rest(20);
    }

    destroy_bitmap(fond);
    destroy_bitmap(sprite1);
    destroy_bitmap(sprite2);
    destroy_bitmap(page);
}

void menu_principal() {
    BITMAP *buffer = create_bitmap(SCREEN_W, SCREEN_H);
    BITMAP *fondmenu = load_bitmap("fondmenu.bmp", NULL);
    BITMAP *marbre = load_bitmap("marbre.bmp", NULL);

    if (!fondmenu || !marbre) {
        allegro_message("Erreur chargement images menu.");
        return;
    }

    char pseudo[MAX_NAME_LENGTH] = "";
    int saisir_pseudo = 1, pseudo_index = 0;

    while (!key[KEY_ESC]) {
        clear(buffer);
        stretch_blit(fondmenu, buffer, 0, 0, fondmenu->w, fondmenu->h, 0, 0, SCREEN_W, SCREEN_H);
        textout_centre_ex(buffer, font, "==== BADLAND ====", SCREEN_W / 2, 30, makecol(255, 215, 0), -1);
        textout_ex(buffer, font, "FREGE Victor - RUBILLON Aurelien", 10, SCREEN_H - 20, makecol(255, 255, 255), -1);
        textout_ex(buffer, font, "MARTINAT Baptiste - LEPELTIER Corentin", SCREEN_W - 320, SCREEN_H - 20, makecol(255, 255, 255), -1);

        if (saisir_pseudo) {
            stretch_blit(marbre, buffer, 0, 0, marbre->w, marbre->h, 200, 160, 400, 200);
            rect(buffer, 200, 160, 600, 360, makecol(0, 0, 0));
            textout_centre_ex(buffer, font, "RENTRER PSEUDO", 400, 180, makecol(0, 0, 0), -1);
            rectfill(buffer, 250, 220, 550, 260, makecol(255, 230, 220));
            rect(buffer, 250, 220, 550, 260, makecol(0, 0, 0));
            textprintf_ex(buffer, font, 260, 235, makecol(0, 0, 0), -1, "%s", pseudo);

            int ok_x1 = 350, ok_y1 = 290, ok_x2 = 450, ok_y2 = 320;
            stretch_blit(marbre, buffer, 0, 0, marbre->w, marbre->h, ok_x1, ok_y1, ok_x2 - ok_x1, ok_y2 - ok_y1);
            rect(buffer, ok_x1, ok_y1, ok_x2, ok_y2, makecol(0, 0, 0));
            textout_centre_ex(buffer, font, "OK", 400, 300, makecol(0, 0, 0), -1);

            if (keypressed()) {
                int key_code = readkey();
                int k = key_code >> 8;
                char ch = key_code & 0xff;
                if (k == KEY_ENTER && pseudo_index > 0) saisir_pseudo = 0;
                else if (k == KEY_BACKSPACE && pseudo_index > 0) pseudo[--pseudo_index] = '\0';
                else if (pseudo_index < MAX_NAME_LENGTH - 1 && ch >= 32 && ch <= 126) {
                    pseudo[pseudo_index++] = ch;
                    pseudo[pseudo_index] = '\0';
                }
            }
            if (mouse_b & 1 &&
                mouse_x >= ok_x1 && mouse_x <= ok_x2 &&
                mouse_y >= ok_y1 && mouse_y <= ok_y2 &&
                pseudo_index > 0) {
                saisir_pseudo = 0;
                rest(200);
            }
        } else {
            textprintf_ex(buffer, font, 10, 40, makecol(255, 255, 255), -1, "Joueur: %s", pseudo);
            int bx = SCREEN_W / 2 - 125, by = 250, bw = 250, bh = 50;
            stretch_blit(marbre, buffer, 0, 0, marbre->w, marbre->h, bx, by, bw, bh);
            rect(buffer, bx, by, bx + bw, by + bh, makecol(0, 0, 0));
            textout_centre_ex(buffer, font, "NOUVELLE PARTIE", SCREEN_W / 2, by + 15, makecol(0, 0, 0), -1);

            if (mouse_b & 1 &&
                mouse_x >= bx && mouse_x <= bx + bw &&
                mouse_y >= by && mouse_y <= by + bh) {
                rest(200);
                jeu_scrolling(pseudo);
            }
        }

        blit(buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
        rest(10);
    }

    destroy_bitmap(fondmenu);
    destroy_bitmap(marbre);
    destroy_bitmap(buffer);
}
