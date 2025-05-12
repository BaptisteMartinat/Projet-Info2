

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
int collision_disc(BITMAP *collision_map, int cx, int cy, int radius, int decor_scroll) {
    for (int dx = -radius; dx <= radius; dx++) {
        for (int dy = -radius; dy <= radius; dy++) {
            if (dx*dx + dy*dy <= radius*radius) {
                int x = cx + dx + decor_scroll;
                int y = cy + dy;
                if (x >= 0 && x < collision_map->w &&
                    y >= 0 && y < collision_map->h) {
                    if (getpixel(collision_map, x, y) == makecol(0, 0, 0)) {
                        return 1;  // collision détectée
                    }
                    }
            }
        }
    }
    return 0;
}
int detecter_arrivee(BITMAP *map, int cx, int cy, int rayon, int scroll) {
    for (int dx = -rayon; dx <= rayon; dx++) {
        for (int dy = -rayon; dy <= rayon; dy++) {
            if (dx * dx + dy * dy <= rayon * rayon) {
                int x = cx + dx + scroll;
                int y = cy + dy;
                if (x >= 0 && x < map->w && y >= 0 && y < map->h) {
                    int pixel = getpixel(map, x, y);
                    if (pixel == makecol(0, 255, 0)) {
                        return 1;  // Vert détecté → arrivée
                    }
                }
            }
        }
    }
    return 0;
}
void jeu_scrolling(const char *pseudo) {
    Joueur joueur;
    int decor_scroll = 0;
    BITMAP *page = create_bitmap(SCREEN_W, SCREEN_H);
    BITMAP *fond = load_bitmap("grotte_lave.bmp", NULL);
    BITMAP *collision_map = load_bitmap("grotte_lave_colli.bmp", NULL);
    BITMAP *sprite1 = load_bitmap("personnage1.bmp", NULL);
    BITMAP *sprite2 = load_bitmap("personnage2.bmp", NULL);
    BITMAP *checkpoint_sprite = load_bitmap("checkpoint.bmp", NULL);

    int temps_depart = clock();
    int timerinterne = 0;
    if (!fond || !sprite1 || !sprite2 || !collision_map) {
        allegro_message("Erreur chargement ressources !");
        return;
    }
    int collision_horizontale = 0;
    int collision_verticale = 0;
    init_jeu(&joueur);
    strcpy(joueur.nom, pseudo);
    joueur.niveau = 1;
    int sprite_state = 0;
    Checkpoint checkpoints[NB_CHECKPOINTS] = {0};
    int dernier_checkpoint = -1;
    const int checkpoint_positions[NB_CHECKPOINTS][2] = {
        {1600, 570},
        {3600, 577},
        {5370, 530},
        {7184, 465}
    };
    while (!key[KEY_ESC]) {
        if (keypressed() && (readkey() >> 8) == KEY_SPACE) {
            sprite_state = 1;
            timerinterne = 10;
            joueur.dy = JUMP_STRENGTH;
        }
        if (timerinterne > 0) {
            timerinterne--;
            if (timerinterne == 0) {
                sprite_state = 0;
            }
        }
        joueur.dx = 1;  // avance constante
        joueur.dy += GRAVITY;

        // 1. Déplacement horizontal proposé
        int new_x = joueur.x + joueur.dx;
        int center_x = new_x + joueur.largeur / 2;
        int center_y = joueur.y + joueur.hauteur / 2;

        if (!collision_disc(collision_map, center_x, center_y, COLLISION_RADIUS, decor_scroll)) {
            joueur.x = new_x;
        } else {
            joueur.dx = 0;  // blocage horizontal
            collision_horizontale = 1;
        }

        // 2. Déplacement vertical proposé
        int new_y = joueur.y + joueur.dy;
        center_x = joueur.x + joueur.largeur / 2;

        // Décalage vers le bas si montée pour éviter détection trop tôt

        center_y = new_y + joueur.hauteur / 2;

        if (!collision_disc(collision_map, center_x, center_y, COLLISION_RADIUS, decor_scroll)) {
            joueur.y = new_y;
        } else {
            if (joueur.dy < 0) {
                // Collision plafond
                joueur.dy = 0;
                collision_verticale = 1;
                // Facultatif : ajuster joueur.y légèrement
            } else {
                // Collision sol
                joueur.dy = 0;
                collision_verticale = 1;
                // Facultatif : ajuster joueur.y pour qu’il se pose pile sur le sol
            }
        }

        // Poussée par le décor (si bloqué par l'avant)


        // 3. Scroll du décor (poussée implicite)
        decor_scroll += DECOR_SCROLL_SPEED;

        // 4. Poussée par le décor (s’il est bloqué à droite)

        if (collision_horizontale) {
            center_x = joueur.x + joueur.largeur / 2;
            center_y = joueur.y + joueur.hauteur / 2;

            int recul_max = 3;  // Ne pas pousser plus de 3 pixels par frame
            int recul = 0;

            while (collision_disc(collision_map, center_x, center_y, COLLISION_RADIUS, decor_scroll)
                   && recul < recul_max) {
                joueur.x -= 1;
                center_x = joueur.x + joueur.largeur / 2;
                recul++;

                if (joueur.x + joueur.largeur < 0) break;
                   }

        }
        if (detecter_arrivee(collision_map, center_x, center_y, COLLISION_RADIUS, decor_scroll)) {
            int temps_total = (clock() - temps_depart) / CLOCKS_PER_SEC;
            char msg[100];
            sprintf(msg, "Bravo %s !\nTu as terminé le niveau en %d secondes.", joueur.nom, temps_total);
            allegro_message(msg);
            break;  // quitte la boucle → retour menu
        }
        // Bords écran


        for (int i = 0; i < NB_CHECKPOINTS; i++) {
            if (!checkpoints[i].actif &&
            joueur_sur_checkpoint(&joueur, checkpoint_positions[i][0], checkpoint_positions[i][1],decor_scroll, checkpoint_sprite->w, checkpoint_sprite->h)) {

                checkpoints[i].x = joueur.x;
                checkpoints[i].y = joueur.y;
                checkpoints[i].actif = 1;
                dernier_checkpoint = i;

                char msg[50];
                sprintf(msg, "Checkpoint %d activé !", i + 1);
                allegro_message(msg);
                }
        }


        if (joueur.y + joueur.hauteur > SCREEN_H - 10) joueur.y = SCREEN_H - joueur.hauteur - 10, joueur.dy = 0;

        if (joueur.x + joueur.largeur < 0) {
            if (dernier_checkpoint != -1) {
                joueur.x = checkpoints[dernier_checkpoint].x;
                joueur.y = 320;
                decor_scroll = checkpoint_positions[dernier_checkpoint][0] - joueur.x + 100;
                joueur.dy = 0;
                joueur.dx = 0;
                allegro_message("Reprise depuis le dernier checkpoint !");
            } else {
                allegro_message("GAME OVER - Aucun checkpoint atteint !");
                break;
            }
        }


        clear_bitmap(page);
        blit(fond, page, decor_scroll, 0, 0, 0, SCREEN_W, SCREEN_H);
        BITMAP *sprite = sprite_state ? sprite2 : sprite1;
        for (int i = 0; i < NB_CHECKPOINTS; i++) {
            int draw_x = checkpoint_positions[i][0] - decor_scroll;
            int draw_y = checkpoint_positions[i][1] - checkpoint_sprite->h;
            draw_sprite(page, checkpoint_sprite, draw_x, draw_y);
        }
        draw_jeu(&joueur, page, decor_scroll, sprite);

        char buffer_temps[50];
        int temps_ecoule = (clock() - temps_depart) / CLOCKS_PER_SEC;
        sprintf(buffer_temps, "Temps : %d s", temps_ecoule);
        textout_ex(page, font, buffer_temps, 10, 10, makecol(255, 255, 255), -1);

        blit(page, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
        rest(20);
    }

    destroy_bitmap(fond);
    destroy_bitmap(collision_map);
    destroy_bitmap(sprite1);
    destroy_bitmap(sprite2);
    destroy_bitmap(page);
    destroy_bitmap(checkpoint_sprite);
}

int joueur_sur_checkpoint(Joueur *joueur, int cp_x, int cp_y, int scroll, int sprite_w, int sprite_h) {
    int jx1 = joueur->x;
    int jy1 = joueur->y;
    int jx2 = joueur->x + joueur->largeur;
    int jy2 = joueur->y + joueur->hauteur;

    int cp_draw_x = cp_x - scroll;
    int cp_draw_y = cp_y - sprite_h;

    int cx1 = cp_draw_x;
    int cy1 = cp_draw_y;
    int cx2 = cp_draw_x + sprite_w;
    int cy2 = cp_draw_y + sprite_h;

    // collision rectangle-rectangle
    return !(jx2 < cx1 || jx1 > cx2 || jy2 < cy1 || jy1 > cy2);
}

void menu_principal() {
    BITMAP *buffer = create_bitmap(SCREEN_W, SCREEN_H);
    BITMAP *fondmenu = load_bitmap("fondmenu.bmp", NULL);
    BITMAP *marbre = load_bitmap("marbre.bmp", NULL);
    BITMAP *img_volcan = load_bitmap("volcan.bmp", NULL);
    BITMAP *img_montagne = load_bitmap("montagne.bmp", NULL);
    BITMAP *img_mer = load_bitmap("mer.bmp", NULL);

    if (!fondmenu || !marbre || !img_volcan || !img_montagne || !img_mer) {
        allegro_message("Erreur chargement images menu.");
        return;
    }

    char pseudo[MAX_NAME_LENGTH] = "";
    int etape_menu = 0; // 0 = pseudo, 1 = boutons, 2 = maps
    int pseudo_index = 0;

    while (!key[KEY_ESC]) {
        clear(buffer);
        stretch_blit(fondmenu, buffer, 0, 0, fondmenu->w, fondmenu->h, 0, 0, SCREEN_W, SCREEN_H);
        textout_centre_ex(buffer, font, "==== BADLAND ====", SCREEN_W / 2, 30, makecol(255, 215, 0), -1);

        if (etape_menu == 0) {
    // =========== SAISIE PSEUDO ===========
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
        if (k == KEY_ENTER && pseudo_index > 0) etape_menu = 1;
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
        etape_menu = 1;
        rest(200);
    }

} else if (etape_menu == 1) {
    // ========== BOUTONS NOUVELLE / CHARGER ==========


    textprintf_ex(buffer, font, 10, 40, makecol(255, 255, 255), -1, "Joueur: %s", pseudo);
    textout_centre_ex(buffer, font, "Que veux-tu faire ?", SCREEN_W / 2, 80, makecol(255, 255, 255), -1);

    int btn_w = 250, btn_h = 80;
    int btn_x = (SCREEN_W - btn_w) / 2;
    int btn_y1 = 180;
    int btn_y2 = 300;

    // Fond marbre + texte
    stretch_blit(marbre, buffer, 0, 0, marbre->w, marbre->h, btn_x, btn_y1, btn_w, btn_h);
    stretch_blit(marbre, buffer, 0, 0, marbre->w, marbre->h, btn_x, btn_y2, btn_w, btn_h);
    rect(buffer, btn_x, btn_y1, btn_x + btn_w, btn_y1 + btn_h, makecol(0, 0, 0));
    rect(buffer, btn_x, btn_y2, btn_x + btn_w, btn_y2 + btn_h, makecol(0, 0, 0));
    textout_centre_ex(buffer, font, "Nouvelle Partie", SCREEN_W / 2, btn_y1 + 30, makecol(0, 0, 0), -1);
    textout_centre_ex(buffer, font, "Charger Partie", SCREEN_W / 2, btn_y2 + 30, makecol(0, 0, 0), -1);


    // ==== BOUTONS RETOUR ET QUITTER ====
    int btn_ctrl_w = 100, btn_ctrl_h = 40;
    int retour_x = 20, retour_y = SCREEN_H - 60;
    int quit_x = SCREEN_W - 120, quit_y = SCREEN_H - 60;

    // Bouton Retour
    stretch_blit(marbre, buffer, 0, 0, marbre->w, marbre->h, retour_x, retour_y, btn_ctrl_w, btn_ctrl_h);
    rect(buffer, retour_x, retour_y, retour_x + btn_ctrl_w, retour_y + btn_ctrl_h, makecol(0, 0, 0));
    textout_centre_ex(buffer, font, "< Retour", retour_x + btn_ctrl_w / 2, retour_y + 12, makecol(0, 0, 0), -1);

    // Bouton Quitter
    stretch_blit(marbre, buffer, 0, 0, marbre->w, marbre->h, quit_x, quit_y, btn_ctrl_w, btn_ctrl_h);
    rect(buffer, quit_x, quit_y, quit_x + btn_ctrl_w, quit_y + btn_ctrl_h, makecol(0, 0, 0));
    textout_centre_ex(buffer, font, "Quitter", quit_x + btn_ctrl_w / 2, quit_y + 12, makecol(0, 0, 0), -1);
    // Gestion clic
    if (mouse_b & 1) {
        rest(200);
        // Nouvelle Partie
        if (mouse_x >= btn_x && mouse_x <= btn_x + btn_w &&
            mouse_y >= btn_y1 && mouse_y <= btn_y1 + btn_h) {
            etape_menu = 2;
            }
        // Charger Partie
        else if (mouse_x >= btn_x && mouse_x <= btn_x + btn_w &&
                 mouse_y >= btn_y2 && mouse_y <= btn_y2 + btn_h) {
            allegro_message("Chargement de partie non disponible.");
                 }
        // Retour
        else if (mouse_x >= retour_x && mouse_x <= retour_x + btn_ctrl_w &&
                 mouse_y >= retour_y && mouse_y <= retour_y + btn_ctrl_h) {
            etape_menu = 0;
                 }
        // Quitter
        else if (mouse_x >= quit_x && mouse_x <= quit_x + btn_ctrl_w &&
                 mouse_y >= quit_y && mouse_y <= quit_y + btn_ctrl_h) {
            break;
                 }
    }


} else if (etape_menu == 2) {
    // ========== CHOIX MAPS (inchangé) ==========
    textprintf_ex(buffer, font, 10, 40, makecol(255, 255, 255), -1, "Joueur: %s", pseudo);
    textout_centre_ex(buffer, font, "Choisis un environnement :", SCREEN_W / 2, 80, makecol(255, 255, 255), -1);

    int cadre_w = 220, cadre_h = 300;
    int img_w = 200, img_h = 280;
    int img_y = 130;
    int space = 40;
    int start_x = (SCREEN_W - (3 * cadre_w + 2 * space)) / 2;

    // VOLCAN
    stretch_blit(marbre, buffer, 0, 0, marbre->w, marbre->h, start_x, img_y, cadre_w, cadre_h);
    stretch_blit(img_volcan, buffer, 0, 0, img_volcan->w, img_volcan->h,
                 start_x + (cadre_w - img_w) / 2, img_y + (cadre_h - img_h) / 2, img_w, img_h);
    textout_centre_ex(buffer, font, "Terre de Feu", start_x + cadre_w / 2, img_y + cadre_h + 10, makecol(255, 255, 255), -1);

    // MONTAGNE
    int mx = start_x + cadre_w + space;
    stretch_blit(marbre, buffer, 0, 0, marbre->w, marbre->h, mx, img_y, cadre_w, cadre_h);
    stretch_blit(img_montagne, buffer, 0, 0, img_montagne->w, img_montagne->h,
                 mx + (cadre_w - img_w) / 2, img_y + (cadre_h - img_h) / 2, img_w, img_h);
    textout_centre_ex(buffer, font, "Pics Silencieux", mx + cadre_w / 2, img_y + cadre_h + 10, makecol(255, 255, 255), -1);

    // MER
    int merx = start_x + 2 * (cadre_w + space);
    stretch_blit(marbre, buffer, 0, 0, marbre->w, marbre->h, merx, img_y, cadre_w, cadre_h);
    stretch_blit(img_mer, buffer, 0, 0, img_mer->w, img_mer->h,
                 merx + (cadre_w - img_w) / 2, img_y + (cadre_h - img_h) / 2, img_w, img_h);
    textout_centre_ex(buffer, font, "Rivages Perdus", merx + cadre_w / 2, img_y + cadre_h + 10, makecol(255, 255, 255), -1);


    // ==== BOUTONS RETOUR ET QUITTER ====
    int btn_ctrl_w = 100, btn_ctrl_h = 40;
    int retour_x = 20, retour_y = SCREEN_H - 60;
    int quit_x = SCREEN_W - 120, quit_y = SCREEN_H - 60;

    // Bouton Retour
    stretch_blit(marbre, buffer, 0, 0, marbre->w, marbre->h, retour_x, retour_y, btn_ctrl_w, btn_ctrl_h);
    rect(buffer, retour_x, retour_y, retour_x + btn_ctrl_w, retour_y + btn_ctrl_h, makecol(0, 0, 0));
    textout_centre_ex(buffer, font, "< Retour", retour_x + btn_ctrl_w / 2, retour_y + 12, makecol(0, 0, 0), -1);

    // Bouton Quitter
    stretch_blit(marbre, buffer, 0, 0, marbre->w, marbre->h, quit_x, quit_y, btn_ctrl_w, btn_ctrl_h);
    rect(buffer, quit_x, quit_y, quit_x + btn_ctrl_w, quit_y + btn_ctrl_h, makecol(0, 0, 0));
    textout_centre_ex(buffer, font, "Quitter", quit_x + btn_ctrl_w / 2, quit_y + 12, makecol(0, 0, 0), -1);

    // CLIC MAP
    if (mouse_b & 1) {
        rest(200);
        if (mouse_x >= start_x && mouse_x <= start_x + cadre_w &&
            mouse_y >= img_y && mouse_y <= img_y + cadre_h) {
            jeu_scrolling(pseudo);
            } else if (mouse_x >= mx && mouse_x <= mx + cadre_w &&
                       mouse_y >= img_y && mouse_y <= img_y + cadre_h) {
                allegro_message("Montagne pas encore disponible !");
           } else if (mouse_x >= merx && mouse_x <= merx + cadre_w &&
                      mouse_y >= img_y && mouse_y <= img_y + cadre_h) {
               allegro_message("Mer pas encore disponible !");
                      }
        // Retour
        if (mouse_x >= retour_x && mouse_x <= retour_x + btn_ctrl_w &&
            mouse_y >= retour_y && mouse_y <= retour_y + btn_ctrl_h) {
            etape_menu = 1; // Revenir à l'étape précédente
            }

        // Quitter
        if (mouse_x >= quit_x && mouse_x <= quit_x + btn_ctrl_w &&
            mouse_y >= quit_y && mouse_y <= quit_y + btn_ctrl_h) {
            break; // Sortir du menu principal
            }
    }

}


        blit(buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
        rest(10);
    }

    destroy_bitmap(fondmenu);
    destroy_bitmap(marbre);
    destroy_bitmap(img_volcan);
    destroy_bitmap(img_montagne);
    destroy_bitmap(img_mer);
    destroy_bitmap(buffer);
}
