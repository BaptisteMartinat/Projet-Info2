#include <allegro.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "BadLand.h"

int main() {
    Joueur joueur;
    Acteur obstacles[MAX_OBSTACLES];
    int nb_obstacles = 0;
    int decor_scroll = 0;

    allegro_init();
    install_keyboard();
    install_mouse();
    set_color_depth(32);
    set_gfx_mode(GFX_AUTODETECT_WINDOWED, SCREEN_W, SCREEN_H, 0, 0);

    BITMAP *page = create_bitmap(SCREEN_W, SCREEN_H);
    BITMAP *sprite1 = load_bitmap("personnage1.bmp", NULL);
    BITMAP *sprite2 = load_bitmap("personnage2.bmp", NULL);

    if (!sprite1 || !sprite2) {
        allegro_message("Erreur de chargement des sprites !");
        return 1;
    }

    int frame_counter = 0;

    generer_obstacles(obstacles, &nb_obstacles);

    if (!menu(page)) {
        destroy_bitmap(page);
        return 0;
    }

    clear_keybuf(); // pour éviter un ENTER résiduel
    init_jeu(&joueur);
    charger_joueur(&joueur);

    while (!key[KEY_ESC]) {
        if (key[KEY_SPACE]) {
            joueur.dy = JUMP_STRENGTH;
        }

        decor_scroll += DECOR_SCROLL_SPEED;

        update_jeu(&joueur, obstacles, nb_obstacles, decor_scroll);

        if (joueur.x < -joueur.width) {
            allegro_message("Game Over : vous avez quitté l'écran !");
            break;
        }

        clear_bitmap(page);
        BITMAP *sprite_actuel = (frame_counter / 10 % 2 == 0) ? sprite1 : sprite2;
        draw_jeu(&joueur, page, decor_scroll, obstacles, nb_obstacles, sprite_actuel);
        blit(page, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
        rest(20);
        frame_counter++;
    }

    sauvegarder_joueur(&joueur);
    destroy_bitmap(sprite1);
    destroy_bitmap(sprite2);
    destroy_bitmap(page);
    allegro_exit();
    return 0;
}
END_OF_MAIN();
