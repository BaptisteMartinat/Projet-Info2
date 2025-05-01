#include <stdio.h>
#include <allegro.h>
#include <string.h>

void initialisation_allegro();
void jeu_scrolling(const char *pseudo);

#define MAX_PSEUDO_LENGTH 20

int main(int argc, char *argv[]) {
    initialisation_allegro();
    install_mouse();
    show_mouse(screen);

    BITMAP *buffer = create_bitmap(SCREEN_W, SCREEN_H);
    BITMAP *fondmenu = load_bitmap("fondmenu.bmp", NULL);
    BITMAP *marbre = load_bitmap("marbre.bmp", NULL);  // texture marbre

    if (!fondmenu || !marbre) {
        allegro_message("Erreur de chargement des images fondmenu.bmp ou marbre.bmp.");
        return 1;
    }

    char pseudo[MAX_PSEUDO_LENGTH + 1] = "";
    int saisir_pseudo = 1;
    int pseudo_index = 0;

    while (!key[KEY_ESC]) {
        clear(buffer);

        // Fond général
        stretch_blit(fondmenu, buffer, 0, 0, fondmenu->w, fondmenu->h, 0, 0, SCREEN_W, SCREEN_H);

        // Titre "BADLAND"
        textout_centre_ex(buffer, font, "==== BADLAND ====", SCREEN_W / 2, 30, makecol(255, 215, 0), -1);

        // Auteurs
        textout_ex(buffer, font, "FREGE Victor - RUBILLON Aurelien", 10, SCREEN_H - 20, makecol(255, 255, 255), -1);
        textout_ex(buffer, font, "MARTINAT Baptiste - LEPELTIER Corentin", SCREEN_W - 320, SCREEN_H - 20, makecol(255, 255, 255), -1);

        if (saisir_pseudo) {
            // Fond marbre pour la boîte pseudo
            stretch_blit(marbre, buffer, 0, 0, marbre->w, marbre->h, 200, 160, 400, 200);
            rect(buffer, 200, 160, 600, 360, makecol(0, 0, 0));
            textout_centre_ex(buffer, font, "RENTRER PSEUDO", 400, 180, makecol(0, 0, 0), -1);

            // Champ texte
            rectfill(buffer, 250, 220, 550, 260, makecol(255, 230, 220));
            rect(buffer, 250, 220, 550, 260, makecol(0, 0, 0));
            textprintf_ex(buffer, font, 260, 235, makecol(0, 0, 0), -1, "%s", pseudo);

            // Bouton OK avec texture
            int ok_x1 = 350, ok_y1 = 290, ok_x2 = 450, ok_y2 = 320;
            stretch_blit(marbre, buffer, 0, 0, marbre->w, marbre->h, ok_x1, ok_y1, ok_x2 - ok_x1, ok_y2 - ok_y1);
            rect(buffer, ok_x1, ok_y1, ok_x2, ok_y2, makecol(0, 0, 0));
            textout_centre_ex(buffer, font, "OK", 400, 300, makecol(0, 0, 0), -1);

            if (keypressed()) {
                int key_code = readkey();
                int k = key_code >> 8;
                char ch = key_code & 0xff;
                if (k == KEY_ENTER && pseudo_index > 0) {
                    saisir_pseudo = 0;
                } else if (k == KEY_BACKSPACE && pseudo_index > 0) {
                    pseudo_index--;
                    pseudo[pseudo_index] = '\0';
                } else if (pseudo_index < MAX_PSEUDO_LENGTH && ch >= 32 && ch <= 126) {
                    pseudo[pseudo_index++] = ch;
                    pseudo[pseudo_index] = '\0';
                }
            }

            if (mouse_b & 1) {
                if (mouse_x >= ok_x1 && mouse_x <= ok_x2 && mouse_y >= ok_y1 && mouse_y <= ok_y2 && pseudo_index > 0) {
                    saisir_pseudo = 0;
                    rest(200);
                }
            }

        } else {
            textprintf_ex(buffer, font, 10, 40, makecol(255, 255, 255), -1, "Joueur: %s", pseudo);

            const int bouton_largeur = 250;
            const int bouton_hauteur = 50;
            const int centre_x = SCREEN_W / 2 - bouton_largeur / 2;
            const int debut_y = 220;
            const int espacement = 20;

            const char *labels[] = {"NOUVELLE PARTIE", "CHARGER PARTIE", "OPTIONS"};
            int nb_boutons = sizeof(labels) / sizeof(labels[0]);

            for (int i = 0; i < nb_boutons; ++i) {
                int y = debut_y + i * (bouton_hauteur + espacement);
                stretch_blit(marbre, buffer, 0, 0, marbre->w, marbre->h, centre_x, y, bouton_largeur, bouton_hauteur);
                rect(buffer, centre_x, y, centre_x + bouton_largeur, y + bouton_hauteur, makecol(0, 0, 0));
                textout_centre_ex(buffer, font, labels[i], SCREEN_W / 2, y + 15, makecol(0, 0, 0), -1);

                if (mouse_b & 1 && mouse_x >= centre_x && mouse_x <= centre_x + bouton_largeur &&
                    mouse_y >= y && mouse_y <= y + bouton_hauteur) {
                    if (i == 0) { // NOUVELLE PARTIE
                        rest(200);
                        jeu_scrolling(pseudo);
                    }
                }
            }

            // BOUTONS RETOUR & QUITTER
            int bouton_sec_hauteur = bouton_hauteur / 1.5;
            int bouton_sec_largeur = bouton_largeur / 4;
            int espacement_sec = 10;
            int bouton_sec_total_width = bouton_sec_largeur * 2 + espacement_sec;
            int bouton_sec_x = SCREEN_W / 2 - bouton_sec_total_width / 2;
            int bouton_sec_y = debut_y + nb_boutons * (bouton_hauteur + espacement) + 20;

            // RETOUR
            stretch_blit(marbre, buffer, 0, 0, marbre->w, marbre->h, bouton_sec_x, bouton_sec_y, bouton_sec_largeur, bouton_sec_hauteur);
            rect(buffer, bouton_sec_x, bouton_sec_y, bouton_sec_x + bouton_sec_largeur, bouton_sec_y + bouton_sec_hauteur, makecol(0, 0, 0));
            textout_centre_ex(buffer, font, "<-", bouton_sec_x + bouton_sec_largeur / 2, bouton_sec_y + 5, makecol(0, 0, 0), -1);

            // QUITTER
            int bouton_quit_x = bouton_sec_x + bouton_sec_largeur + espacement_sec;
            stretch_blit(marbre, buffer, 0, 0, marbre->w, marbre->h, bouton_quit_x, bouton_sec_y, bouton_sec_largeur, bouton_sec_hauteur);
            rect(buffer, bouton_quit_x, bouton_sec_y, bouton_quit_x + bouton_sec_largeur, bouton_sec_y + bouton_sec_hauteur, makecol(0, 0, 0));
            textout_centre_ex(buffer, font, "X", bouton_quit_x + bouton_sec_largeur / 2, bouton_sec_y + 5, makecol(0, 0, 0), -1);

            if (mouse_b & 1) {
                if (mouse_x >= bouton_sec_x && mouse_x <= bouton_sec_x + bouton_sec_largeur &&
                    mouse_y >= bouton_sec_y && mouse_y <= bouton_sec_y + bouton_sec_hauteur) {
                    pseudo_index = 0;
                    pseudo[0] = '\0';
                    saisir_pseudo = 1;
                    rest(200);
                }
                if (mouse_x >= bouton_quit_x && mouse_x <= bouton_quit_x + bouton_sec_largeur &&
                    mouse_y >= bouton_sec_y && mouse_y <= bouton_sec_y + bouton_sec_hauteur) {
                    break;
                }
            }
        }

        blit(buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
        rest(10);
    }

    destroy_bitmap(fondmenu);
    destroy_bitmap(marbre);
    destroy_bitmap(buffer);
    allegro_exit();
    return 0;
}
END_OF_MAIN();

void initialisation_allegro() {
    allegro_init();
    install_keyboard();
    install_mouse();
    set_color_depth(desktop_color_depth());
    if (set_gfx_mode(GFX_AUTODETECT_WINDOWED, 800, 600, 0, 0) != 0) {
        allegro_message("probleme mode graphique");
        allegro_exit();
        exit(EXIT_FAILURE);
    }
}

// -------- SCROLLING --------
void jeu_scrolling(const char *pseudo) {
    BITMAP *buffer = create_bitmap(SCREEN_W, SCREEN_H);
    BITMAP *fond = load_bitmap("grotte_lave.bmp", NULL);

    if (!fond) {
        allegro_message("Erreur de chargement de grotte_lave.bmp");
        return;
    }

    int scroll_x = 0;
    int jeu_actif = 0;

    while (!key[KEY_ESC]) {
        if (key[KEY_SPACE]) {
            jeu_actif = 1;
        }

        if (jeu_actif) {
            scroll_x += 2;
            if (scroll_x >= fond->w - SCREEN_W)
                scroll_x = fond->w - SCREEN_W;
        }

        blit(fond, buffer, scroll_x, 0, 0, 0, SCREEN_W, SCREEN_H);
        textprintf_ex(buffer, font, 10, 10, makecol(255, 255, 255), -1, "Joueur: %s", pseudo);
        blit(buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
        rest(10);
    }

    destroy_bitmap(fond);
    destroy_bitmap(buffer);
}
