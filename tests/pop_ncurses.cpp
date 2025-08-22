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

void runCommandNcurses(const char* cmd) {
    FILE* pipe = popen(cmd, "r");
    if (!pipe) {
        printw("Error al ejecutar el comando.\n");
        return;
    }

    char buffer[256];
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        printw("%s", buffer);
        refresh();
    }
    pclose(pipe);
}

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
