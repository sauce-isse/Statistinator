#include <ncurses.h>
#include <menu.h>
#include <string.h>

int main() {
    // Menu qui permet de choisir le mode entre: 
    // Entrer des valeurs
    // Afficher différents types de graphique,
    // Afficher des valeurs comme la variance, la moyenne, l'écart-type, la médiane, les quartiles...
    ITEM *items[4];
    int n_items = 4;
    MENU *menu;
    WINDOW *menu_win;
    int i;

    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    box(stdscr, 0, 0);
    items[0] = new_item("Entrer des valeurs dans le tableau", "");
    items[1] = new_item("Graphiques", "");
    items[2] = new_item("Afficher les variables","");
    items[3] = new_item("Quitter","");
    items[4] = NULL;

    menu = new_menu(items);
    // Centrer le menu

    int menu_width = 0;
    int menu_height = n_items + 2;
    for (i = 0; i<n_items; i++){
        int len = strlen(item_name(items[i]));
        if (len > menu_width){
            menu_width = len;
        }
    }
    menu_width += 4;
    int startx = (COLS - menu_width) / 2;
    int starty = (LINES - menu_height) / 2;
    menu_win = newwin(menu_height, menu_width, starty, startx);

    keypad(menu_win, TRUE);
    box(menu_win, 0, 0);

    set_menu_win(menu, menu_win);
    set_menu_sub(menu, derwin(menu_win, n_items, menu_width - 2, 1, 1));
    set_menu_mark(menu, " > ");

    post_menu(menu);
    wrefresh(menu_win);

    int ch;
    while((ch = wgetch(menu_win)) != 'q') {
        switch(ch) {
            case KEY_DOWN:
                menu_driver(menu, REQ_DOWN_ITEM);
                break;
            case KEY_UP:
                menu_driver(menu, REQ_UP_ITEM);
                break;
            case 10: // Entrée
                if(item_index(current_item(menu)) == n_items - 1) { // Quitter
                    goto exit_loop;
                }
                break;
        }
        wrefresh(menu_win);
    }

exit_loop:
    // Nettoyage
    unpost_menu(menu);
    free_menu(menu);
    for(i = 0; i < n_items; ++i) free_item(items[i]);
    delwin(menu_win);
    endwin();

    return 0;
}