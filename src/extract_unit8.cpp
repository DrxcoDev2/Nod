#include <iostream>
#include <cstdint>
#include <cstdio>
#include <cstring>

using namespace std;

void Write(uint8_t* data, uint8_t size) {
    double d;
    int i;
    for (i=0; i<size/sizeof(double); ++i) {
        d = ((double*)data)[i];
        printf("d = %g\n", d);
    }
}

void Write_int(uint8_t* data, uint8_t size) {
    int i;
    for (i=0; i<size/sizeof(int); ++i) {
        printf("i = %d\n", ((int*)data)[i]);
    }
}

/*Crud unit8_t array*/
void Write_uint8(uint8_t* data, size_t size) {
    for (size_t i=0; i<size; ++i) {
        printf("i = %d\n", data[i]);
    }
}

int main(int argc, char* argv[]) {

    if (argc < 2) {
        printf("Usage 1 or 2");
    }
    
    if (strcmp(argv[1], "1") == 0) {
        double arr[] = {1.1, 2.2, 3.3, 4.4, 5.5};
        int arr2[] = {1, 2, 3, 4, 5};
        Write((uint8_t*)arr, sizeof(arr));
        Write_int((uint8_t*)arr2, sizeof(arr2));
    }

    else if (strcmp(argv[1], "2") == 0) {
        uint8_t arr[] = {1, 2, 3, 4, 5};
        Write_uint8((uint8_t*)arr, sizeof(arr));
    }


    
    return 0;
}