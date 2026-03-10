#include <ncurses.h>
#include <menu.h>
#include <string.h>

ITEM *items[5];
int n_items = 4;
MENU *menu;
WINDOW *menu_window;
void cursor(int startx, int starty, int cell_width, int cell_height, int rows, int cols);
void mainmenu(MENU *menu, WINDOW *menu_window, ITEM *items[], int n_items);

void get_values(int v1, int v2){
}

void tableau() {
    int rows = 2;
    int cols = 1;
    int cell_width = 7;
    int cell_height = 2;
    int totalheight = rows * cell_height;
    int totalwidth = cols * cell_width;
    const char *title = "Tableau de valeurs";
    int cury, curx;

    clear();

    int table_width  = cols * cell_width + (cols + 1);
    int table_height = rows * cell_height + (rows + 1);

    int startx = (COLS - table_width) / 2;
    int starty = (LINES - table_height) / 2;
    int h, i, j;
    int y = starty, x = startx;

    box(stdscr,0,0);
    mvprintw(0, ((COLS - strlen(title)) / 2), "%s", title);
    // Lignes horizontales
    for (j = 0; j<=rows; j++){
        for (i = 0; i<totalwidth; i++){
            mvaddch(y, x+i, ACS_HLINE);
        }
        y += cell_height;
    }
    // Lignes verticales
    y = starty;
    x = startx;
    for (j = 0; j<=cols; j++){
        for (i=0; i<totalheight; i++){
            mvaddch(y+i, x, ACS_VLINE);
        }
        x += cell_width;
    }

    // T et croix
    x = startx;
    for (j = 0; j <= cols; j++){
        for (i = 0; i <= rows; i++){
            y = starty + cell_height * i;
            if (x == startx){
                mvaddch(y,x,ACS_LTEE);
            }else if (y == starty){
                mvaddch(y,x,ACS_TTEE);
            }else if (x == startx + totalwidth){
                mvaddch(y,x,ACS_RTEE);
            }else if (y == starty + totalheight){
                mvaddch(y,x,ACS_BTEE);
            }
            else{
                mvaddch(y,x,ACS_PLUS);
            }
        }
        x += cell_width;
    }
        // Bordures
    x = startx;
    y = starty;

    mvaddch(y, x, ACS_ULCORNER);
    x = startx + totalwidth;
    mvaddch(y, x, ACS_URCORNER);
    
    y = starty + totalheight;
    x = startx;
    mvaddch(y, x, ACS_LLCORNER);

    x = startx + totalwidth;
    mvaddch(y, x, ACS_LRCORNER);
    cursor(startx,starty,cell_width,cell_height,rows,cols);
    return;
}
void cursor(int startx, int starty, int cell_width, int cell_height, int rows, int cols){
    int cur_row = 0;
    int cur_col = 0;
    int curx, cury;
    move(starty+1, startx+1);
    echo();
    refresh();

    int ch;
    while ((ch=getch()) != 'q'){
        switch(ch){
            case KEY_DOWN:
                cur_row++;
                break;
            case KEY_UP:
                cur_row--;
                break;
            case KEY_RIGHT:
                cur_col++;
                break;
            case KEY_LEFT:
                cur_col--;
                break;
        }
        if (cur_row < 0){
            cur_row++;
        } if (cur_row >= rows){
            cur_row--;
        } if (cur_col < 0){
            cur_col++;
        } if (cur_col > cols-1){
            cur_col--;
        }
        curx = startx+cell_width*cur_col+1;
        cury = starty+cell_height*cur_row+1;
        move(cury,curx);
    }

    return;
}



int main() {
    // Menu qui permet de choisir le mode entre: 
    // Entrer des valeurs
    // Afficher différents types de graphique,
    // Afficher des valeurs comme la variance, la moyenne, l'écart-type, la médiane, les quartiles...
    int i;

    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    box(stdscr, 0, 0);
    
    const char *title = "Statistinator";
    mvprintw(0, ((COLS - strlen(title)) / 2), "%s", title);
    refresh();

    items[0] = new_item("Entrer des valeurs dans le tableau", "");
    items[1] = new_item("Graphiques", "");
    items[2] = new_item("Afficher les variables","");
    items[3] = new_item("Quitter","");
    items[4] = NULL;

    menu = new_menu(items);

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
    menu_window = newwin(menu_height, menu_width, starty, startx);

    keypad(menu_window, TRUE);
    box(menu_window, 0, 0);

    set_menu_win(menu, menu_window);
    set_menu_sub(menu,  derwin(menu_window, n_items, menu_width - 2, 1, 1));
    set_menu_mark(menu, " > ");

    // Invoquer le menu
    mainmenu(menu, menu_window, items, n_items);

    // Libérer le menu et quitter
    unpost_menu(menu);

    free_menu(menu);

    for(i = 0; i < n_items; ++i) free_item(items[i]);
    delwin(menu_window);
    endwin();
    return 0;
}


void mainmenu(MENU *menu, WINDOW *menu_window, ITEM *items[], int n_items){
    int i;
    clear();
    post_menu(menu);
    wrefresh(menu_window);
    const char *title = "Statistinator";

    int ch;
    while((ch = wgetch(menu_window)) != 'q') {
        switch(ch) {
            case KEY_DOWN:
                menu_driver(menu, REQ_DOWN_ITEM);
                break;
            case KEY_UP:
                menu_driver(menu, REQ_UP_ITEM);
                break;
            case 10: // Entrée
                ITEM *cur = current_item(menu);
                int idx = item_index(cur);

                if(idx == 3){ // Quitter
                    return;
                }
                if(idx == 0){
                    unpost_menu(menu);
                    wclear(menu_window);
                    wrefresh(menu_window);
                    clear();
                    refresh();
                    tableau();

                    clear();
                    box(stdscr, 0,0);
                    mvprintw(0, ((COLS - strlen(title)) / 2), "%s", title);
                    refresh();
                    int menu_width = 0;
                    int menu_height = n_items + 2;
                    for (i = 0; i<n_items; i++){
                        int len = strlen(item_name(items[i]));
                        if (len > menu_width){
                            menu_width = len;
                        }
                    }
                    menu_width += 4;
                    box(menu_window, 0, 0);
                    set_menu_win(menu, menu_window);
                    set_menu_sub(menu, derwin(menu_window, n_items, menu_width - 2, 1, 1));
                    post_menu(menu);
                    wrefresh(menu_window);
                }

                break;
        }
        wrefresh(menu_window);
    }
    return;
}
