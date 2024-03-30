#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "functions.h"

int main(int argc, char *argv[]) {

    printf("\n"); // This line separates the terminal commands to the output of the program

    //Opening files (If an error occurs, the program will stop running and exit with code 1)
    FILE *fproutes = open_file("rotas.txt", "r");
    FILE *fpairports = open_file("aeroportos.txt", "r");

    //Reading the files and initializing the structures
    StackAirport *airports = init_airports(fpairports);
    StackRoute *routes = init_routes(fproutes, airports);
    
    //Handling the arguments: This function will control all the execution of the program
    handle_arguments(argc, argv, airports, routes);   
    
    //Closing files
    fclose(fproutes);
    fclose(fpairports);

    //Freeing the memory
    printf("\nFreeing memory...\n");
    free_airports(airports);
    free_routes(routes);

    return 0;
}