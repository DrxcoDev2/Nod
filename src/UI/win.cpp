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

int main() {
    initscr();
    cbreak();
    echo();   

    while (true) {
        printw("\n>>> ");
        refresh();

        char input[256];
        getstr(input);
        std::string command = input;

        if (command == "exit" || command == "quit") {
            break;
        }
        else if (command == "fd") {
            clear();
            printw("[file_details output]\n\n");
            printw("Se ejecutara el comando: fd\n");
            runCommandNcurses("fds");
        }
        else {
            clear();
            printw("[ejecutando %s]\n\n", command.c_str());
            runCommandNcurses(command);
        }

        printw("\n\n(Presiona una tecla para continuar)");
        getch();
        clear();
    }

    endwin();
    return 0;
}
