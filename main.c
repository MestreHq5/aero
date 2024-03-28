#include <stdio.h>
#include "functions.h"

int main(int argc, char *argv[]) {

    printf("\n"); // This line separates the execution to the outuput of the program

    if (argc < 2) {
        printf("Too few arguments: Execution failed...\n");
        arguments_error();
        return 1;
    }
    
    return 0;
}
