#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "functions.h"


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

    if(argc == 5){
        separate_according_to_layovers(airports, routes, airport_source, airport_destiny, layovers, 0);
    } else if(argc == 6){
        separate_according_to_layovers(airports, routes, airport_source ,airport_destiny, layovers, time_option);
    } else if(argc == 7){
        time_option = time_sort_option(argv[5]);
        separate_according_to_layovers(airports, routes, airport_source ,airport_destiny, layovers, time_option);
    } 
}

separate_according_to_layovers(StackAirport *airports, StackRoute *routes, Airport *airport_source, Airport *airport_destiny, int layovers, int time_option){
    if (layovers == 0){
            list_direct_flights(airports, routes, airport_source, airport_destiny, time_option); //List direct flights (sorted)

        } else if(layovers == 1){
            list_one_layover(airports, routes, airport_source, airport_destiny, time_option); //One layover (sorted)

        }else if (layovers ==2){
            list_two_layovers(airports, routes, airport_source, airport_destiny, time_option); //Two layovers (sorted)

        }
}