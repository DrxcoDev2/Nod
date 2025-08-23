#pragma once

#include <iostream>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

namespace ConsoleColor {

#ifdef _WIN32
    // Handle de la consola
    inline HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
#endif

    // Enumeraci�n de 100 colores (50 normales + 50 brillantes)
    enum Color {
        // Colores normales
        BLACK = 0, RED, GREEN, YELLOW, BLUE, MAGENTA, CYAN, WHITE,
        DARK_GRAY, LIGHT_RED, LIGHT_GREEN, LIGHT_YELLOW, LIGHT_BLUE,
        LIGHT_MAGENTA, LIGHT_CYAN, LIGHT_WHITE,
        // Colores extendidos normales (hasta 50)
        COLOR_16, COLOR_17, COLOR_18, COLOR_19, COLOR_20,
        COLOR_21, COLOR_22, COLOR_23, COLOR_24, COLOR_25,
        COLOR_26, COLOR_27, COLOR_28, COLOR_29, COLOR_30,
        COLOR_31, COLOR_32, COLOR_33, COLOR_34, COLOR_35,
        COLOR_36, COLOR_37, COLOR_38, COLOR_39, COLOR_40,
        COLOR_41, COLOR_42, COLOR_43, COLOR_44, COLOR_45,
        COLOR_46, COLOR_47, COLOR_48, COLOR_49,

        // Colores brillantes
        BRIGHT_BLACK = 50, BRIGHT_RED, BRIGHT_GREEN, BRIGHT_YELLOW, BRIGHT_BLUE,
        BRIGHT_MAGENTA, BRIGHT_CYAN, BRIGHT_WHITE,
        BRIGHT_COLOR_8, BRIGHT_COLOR_9, BRIGHT_COLOR_10, BRIGHT_COLOR_11,
        BRIGHT_COLOR_12, BRIGHT_COLOR_13, BRIGHT_COLOR_14, BRIGHT_COLOR_15,
        BRIGHT_COLOR_16, BRIGHT_COLOR_17, BRIGHT_COLOR_18, BRIGHT_COLOR_19,
        BRIGHT_COLOR_20, BRIGHT_COLOR_21, BRIGHT_COLOR_22, BRIGHT_COLOR_23,
        BRIGHT_COLOR_24, BRIGHT_COLOR_25, BRIGHT_COLOR_26, BRIGHT_COLOR_27,
        BRIGHT_COLOR_28, BRIGHT_COLOR_29, BRIGHT_COLOR_30, BRIGHT_COLOR_31,
        BRIGHT_COLOR_32, BRIGHT_COLOR_33, BRIGHT_COLOR_34, BRIGHT_COLOR_35,
        BRIGHT_COLOR_36, BRIGHT_COLOR_37, BRIGHT_COLOR_38, BRIGHT_COLOR_39,
        BRIGHT_COLOR_40, BRIGHT_COLOR_41, BRIGHT_COLOR_42, BRIGHT_COLOR_43,
        BRIGHT_COLOR_44, BRIGHT_COLOR_45, BRIGHT_COLOR_46, BRIGHT_COLOR_47
    };

    inline void setColor(Color color) {
#ifdef _WIN32
        WORD wColor = 7; // Default
        switch (color) {
            case BLACK: wColor = 0; break;
            case RED: wColor = FOREGROUND_RED; break;
            case GREEN: wColor = FOREGROUND_GREEN; break;
            case YELLOW: wColor = FOREGROUND_RED | FOREGROUND_GREEN; break;
            case BLUE: wColor = FOREGROUND_BLUE; break;
            case MAGENTA: wColor = FOREGROUND_RED | FOREGROUND_BLUE; break;
            case CYAN: wColor = FOREGROUND_GREEN | FOREGROUND_BLUE; break;
            case WHITE: wColor = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE; break;
            case DARK_GRAY: wColor = FOREGROUND_INTENSITY; break;
            case LIGHT_RED: wColor = FOREGROUND_RED | FOREGROUND_INTENSITY; break;
            case LIGHT_GREEN: wColor = FOREGROUND_GREEN | FOREGROUND_INTENSITY; break;
            case LIGHT_YELLOW: wColor = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY; break;
            case LIGHT_BLUE: wColor = FOREGROUND_BLUE | FOREGROUND_INTENSITY; break;
            case LIGHT_MAGENTA: wColor = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY; break;
            case LIGHT_CYAN: wColor = FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY; break;
            case LIGHT_WHITE: wColor = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY; break;
            default: wColor = 7; break;
        }
        SetConsoleTextAttribute(hConsole, wColor);
#else
        int code = 0;
        if (color <= 49) code = 30 + static_cast<int>(color % 8); // normales
        else code = 90 + static_cast<int>(color % 8); // brillantes
        std::cout << "\033[" << code << "m";
#endif
    }

    inline void resetColor() {
#ifdef _WIN32
        SetConsoleTextAttribute(hConsole, 7);
#else
        std::cout << "\033[0m";
#endif
    }

} // namespace ConsoleColor
