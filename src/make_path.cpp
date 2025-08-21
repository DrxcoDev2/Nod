#include <iostream>
#include <filesystem>
#include <fstream>

using namespace std::filesystem;
using namespace std;

int main(int argc, char *argv[]) {
    path directory = argv[1];

    if (!exists(directory)) {
        cout << "Directory does not exist" << endl;
        return 1;
    }

    path filepath = directory / argv[2];

    if (exists(filepath)) {
        cout << "File already exists \"" << filepath << "\"" << endl;
        return 1;
    }

    //Create a use file
    ofstream file(filepath);
    if (file.is_open()) {
        file << argv[3];
        file.close();
        cout << "File created \"" << filepath << "\"" << endl;
    } else {
        cout << "Unable to open file \"" << filepath << "\"" << endl;
        return 1;
    }
    
    return 0;
}
