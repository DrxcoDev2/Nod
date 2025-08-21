#include <ncurses.h>
#include <string>
#include <cstdio>
#include <cstdlib>

void run_and_display(const std::string &cmd) {
    FILE* pipe = popen(cmd.c_str(), "r");
    if (!pipe) {
        printw("Error: could not run command\n");
        return;
    }

    char buffer[256];
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        printw("%s", buffer);
    }
    pclose(pipe);
}

int main() {
    initscr();
    cbreak();
    echo();   // <-- Mostrar lo que el usuario escribe

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
            run_and_display("fd");
        }
        else {
            clear();
            printw("[ejecutando %s]\n\n", command.c_str());
            run_and_display(command);
        }

        printw("\n\n(Presiona una tecla para continuar)");
        getch();
        clear();
    }

    endwin();
    return 0;
}
