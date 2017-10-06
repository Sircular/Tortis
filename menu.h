/* An abstraction for simple menus. */

/* Gives a menu to the user and returns the choice. Returns -1 in the case of
 * error. */
int menu_choice(char* title, int choiceCount, char** choices);

/* Displays information to the user. */
void menu_show(char* title, char* text);
