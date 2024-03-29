#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "functions.h"

int main(int argc, char *argv[]) {

    printf("\n"); // This line separates the termunal commands to the output of the program

    //Arguments Test
    if (argc < 2) {
        printf("Too few arguments: Execution failed...\n");
        arguments_error();
        return 1;
    }

    //Opening files (If an error occurs, the program will stop running and exit with code 1)
    FILE *fproutes = open_file("rotas.txt", "r");
    FILE *fpairports = open_file("aeroportos.txt", "r");

    //Reading the files and initializing the structures
    StackAirport *airports = init_airports(fpairports);
    StackRoute *routes = init_routes(fproutes);



    //Closing files
    fclose(fproutes);
    fclose(fpairports);

    return 0;
}