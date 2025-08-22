/**
 * Permite ejecutar comandos en ncurses
 * y asegura que se muestren en la ventana
 * el resultado del comando
 */


#include <ncurses.h>
#include <iostream>
#include <cstdio>
#include <memory>
#include <string>

void runCommandNcurses(const std::string& cmd);

int main() {
    initscr();
    cbreak();
    noecho();
    scrollok(stdscr, TRUE);

    printw("Ejecutando comando dentro de ncurses...\n");
    refresh();

    runCommandNcurses("ls -l");

    printw("\n\nPulsa cualquier tecla para salir.");
    refresh();
    getch();

    endwin();
    return 0;
}
