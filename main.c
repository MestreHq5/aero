#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "functions.h"

//Function Prototypes
void handle_arguments(int argc, char *argv[], StackAirport *airports, StackRoute *routes);
void manage_routes(int argc, char *argv[], StackAirport *airports, StackRoute *routes);



int main(int argc, char *argv[]) {

    printf("\n"); // This line separates the terminal commands to the output of the program

    //Opening files (If an error occurs, the program will stop running and exit with code 1)
    FILE *fproutes = open_file("rotas.txt", "r");
    FILE *fpairports = open_file("aeroportos.txt", "r");

    //Reading the files and initializing the structures
    StackAirport *airports = init_airports(fpairports);
    StackRoute *routes = init_routes(fproutes, airports);
    
    //Closing files
    fclose(fproutes);
    fclose(fpairports);


    //Handling the arguments: This function will control all the execution of the program
    handle_arguments(argc, argv, airports, routes);   
    

    //Freeing the memory
    printf("\nFreeing memory...\n");
    
    free_airports(airports);
    free_routes(routes);

    return 0;
}


void handle_arguments(int argc, char *argv[], StackAirport *airports, StackRoute *routes){

    //Check if the number of arguments is correct
    if (argc < 2 || argc == 3 || argc == 4) {
        printf("Too few arguments: Execution failed...\n");
        arguments_error();
        return;
    } else if (argc > 7){
        printf("Too many arguments: Execution failed...\n");
        arguments_error();
        return;
    }

    //Check the second argument and divide the cases
    if (strcmp(argv[1], "-voos") == 0){
        show_routes(routes);
        return;

    } else if (strcmp(argv[1], "-aeroportos") == 0){
        show_airports(airports);
        return;
    }

    //At this point, the program will certainly ask for routes
    manage_routes(argc, argv, airports, routes);
    return;

}

void manage_routes(int argc, char *argv[], StackAirport *airports, StackRoute *routes){
    
    //Variables
    Airport *airport_source, *airport_destiny;
    int layovers, time_option;

    //Check if the IATA codes are valid and avaiable
    airport_source = find_airport_by_IATA(airports, argv[1]);
    airport_destiny = find_airport_by_IATA(airports, argv[2]);

    if (airport_source == NULL || airport_destiny == NULL){
        printf("Invalid IATA code: Execution failed...\n");
        return;
    }

    //Check if the layover option is valid
    if (strcmp(argv[3], "-L") != 0){
        printf("Invalid Layover specifier: Execution failed...\n");
        arguments_error();
        return;
    }

    layovers = layover_number(argv[4]); //will validate the layover number

    if (argc == 5){
        if (layovers == 0){
            list_direct_flights(airports, routes, airport_source, airport_destiny, 0); //will list all the direct flights (no sort)

        } else if(layovers == 1){
            list_one_layover(airports, routes, airport_source, airport_destiny, 0); //One layover (no sort)

        }else{
            list_two_layovers(airports, routes, airport_source, airport_destiny, 0); //Two layovers (no sort)

        }
    }

    if (argc == 6){
        
        time_option = time_sort_option(argv[5]);
        if (layovers == 0){
            list_direct_flights(airports, routes, airport_source, airport_destiny, time_option); //List direct flights (sorted)

        } else if(layovers == 1){
            list_one_layover(airports, routes, airport_source, airport_destiny, time_option); //One layover (sorted)

        }else{
            list_two_layovers(airports, routes, airport_source, airport_destiny, time_option); //Two layovers (sorted)

        }
    }

    if (argc == 7 && layovers > 0){
        time_option = time_sort_option(argv[5]);
        if (layovers == 0){
            list_direct_flights(airports, routes, airport_source, airport_destiny, time_option); //List direct flights (sorted)

        } else if(layovers == 1){
            list_one_layover(airports, routes, airport_source, airport_destiny, time_option); //One layover (sorted)

        }else{
            list_two_layovers(airports, routes, airport_source, airport_destiny, time_option); //Two layovers (sorted)

        }
    }
}
