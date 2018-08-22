/* An abstraction for simple menus. */

/* Gives a menu to the user and returns the choice. Returns -1 in the case of
 * error. */
int menu_choice(char* title, int choiceCount, char** choices);

/* Opens a pause window that remains until the pause key is pressed again. */
void menu_pauseMenu(void);

