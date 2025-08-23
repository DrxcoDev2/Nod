#include "../include/color.hpp"

#include <iostream>
#include <filesystem>
#include <fstream>
#include <chrono>
#include <string>


using namespace ConsoleColor;
using namespace std;
using namespace std::filesystem;

int main(int argc, char *argv[]) {
    if (argc < 2) {
        cout << "Usage: " << argv[0] << " <file> [-b] [-kb] [-d] [-lang]" << endl;
        return 1;
    }

    string firstArg = argv[1];

    // Si el primer argumento es -h \u2192 mostrar ayuda y salir
    if (firstArg == "-h") {
        cout << "Usage: " << argv[0] << " <file> [options]\n";
        cout << "Options:\n";
        cout << "  -b      Show file size in bytes\n";
        cout << "  -kb     Show file size in kilobytes\n";
        cout << "  -mb     Show file size in megabytes\n";
        cout << "  -d      Show last modification date\n";
        cout << "  -lang   Detect language based on extension\n";
        return 0;
    }

    // Si no es -h, entonces tratamos como archivo
    path filepath = firstArg;

    if (!exists(filepath)) {
        cout << "File does not exist" << endl;
        return 1;
    }

    cout << filepath << " exists" << endl;

    // Recorremos los flags opcionales
    for (int i = 2; i < argc; i++) {
        string arg = argv[i];

        

        if (arg == "-b") {
            auto size = file_size(filepath);
            cout << "File size: " << size << " bytes" << endl;
        } 
        else if (arg == "-kb") {
            auto size = file_size(filepath) / 1024.0;
            cout << "File size: " << size << " KB" << endl;
        } 
        else if (arg == "-mb") {
            auto size = file_size(filepath) / 1024.0 / 1024.0;
            cout << "File size: " << size << " MB" << endl;
        } 
        else if (arg == "-d") {
            auto ftime = last_write_time(filepath);
            auto sctp = chrono::time_point_cast<chrono::system_clock::duration>(
                ftime - decltype(ftime)::clock::now() + chrono::system_clock::now()
            );
            time_t cftime = chrono::system_clock::to_time_t(sctp);
            cout << "Last write time: " << ctime(&cftime);
        }
        else if (arg == "-lang") {
            std::string ext = filepath.extension().string(); 
        
            std::string language;
            Color color; // ahora usamos la enumeraci�n
        
            if (ext == ".cpp" || ext == ".cc" || ext == ".cxx" || ext == ".h" || ext == ".hpp") {
                language = "C++";
                color = BLUE;
            } else if (ext == ".c") {
                language = "C";
                color = DARK_GRAY;
            } else if (ext == ".py") {
                language = "Python";
                color = CYAN;
            } else if (ext == ".java") {
                language = "Java";
                color = YELLOW;
            } else if (ext == ".js") {
                language = "JavaScript";
                color = LIGHT_YELLOW;
            } else if (ext == ".html" || ext == ".htm") {
                language = "HTML";
                color = LIGHT_RED;
            } else if (ext == ".css") {
                language = "CSS";
                color = LIGHT_BLUE;
            } else if (ext == ".sh") {
                language = "Shell script";
                color = LIGHT_GREEN;
            } else {
                language = "Unknown (" + ext + ")";
                color = LIGHT_RED;
            }
        
            setColor(color);        // cambiar color de consola
            std::cout << language << std::endl;
            resetColor();           // volver al color por defecto
        }
    }

    return 0;
}
