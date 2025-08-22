g++ -S ./libs/runCommandNcurses.cpp -o ./libs/runCommandNcurses.s
g++ -c ./libs/runCommandNcurses.s -o ./libs/runCommandNcurses.o
g++ ./tests/pop_ncurses.cpp ./libs/runCommandNcurses.o -o ./tests/pop_ncurses -lncurses