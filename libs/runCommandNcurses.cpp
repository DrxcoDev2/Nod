#include <ncurses.h>
#include <string>
#include <cstdio>
#include <cstdlib>

void runCommandNcurses(const std::string& cmd) {
    FILE* pipe = popen(cmd.c_str(), "r");
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