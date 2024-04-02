#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "functions.h"

//Auxialiary definitions
#define LAYOVERS 0
#define TIME_OPTION 1
#define DISTANCE_OPTION 2

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
    int layover_and_sort[3];

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

    
    layover_and_sort[LAYOVERS] = layover_number(argv[4]); 
    layover_and_sort[TIME_OPTION] = 0; //Default value for sort by time
    layover_and_sort[DISTANCE_OPTION] = 0; //Default value for sort by distance

    if (argc >= 6){
        layover_and_sort[TIME_OPTION] = time_sort_option(argv[5]);
    }

    if (argc == 7){
        layover_and_sort[DISTANCE_OPTION] = distance_sort_option(argv[6]);
    }

    separate_cases_by_layovers(airports, routes, airport_source, airport_destiny, layover_and_sort);
    
}

void separate_cases_by_layovers(StackAirport *airports, StackRoute *routes, Airport *airport_source, Airport *airport_destiny, int layover_and_sort[3]){

    //Variables
    int time_and_distance[2];

    //Initialize the vector
    time_and_distance[0] = layover_and_sort[TIME_OPTION];
    time_and_distance[1] = layover_and_sort[DISTANCE_OPTION];

    
    if (layover_and_sort[0] == 0){
            list_direct_flights(airports, routes, airport_source, airport_destiny, time_and_distance); //List direct flights

        } else if(layover_and_sort[0] == 1){
            list_one_layover(airports, routes, airport_source, airport_destiny, time_and_distance); //One layover

        }else if (layover_and_sort[0] == 2){
            list_two_layovers(airports, routes, airport_source, airport_destiny, time_and_distance); //Two layovers

        }
}




