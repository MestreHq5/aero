#include "functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Auxiliary Definitions
#define EARTH_RADIUS 6371
#define REAL_RADIUS 6381 // Earth radius + 10 km altitude



//Functions
void arguments_error() {
    printf("\n-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");
    printf("\nThe list below explain the commands for this aplication:\n");
    printf("\n./rotas2024 LIS FCO -L 2 -TD -D");
    printf("\n      ^      ^   ^   ^ ^  ^   ^");
    printf("\n      |      |   |   | |  |   |");
    printf("\n      1      2   3   4 5  6   7");
    printf("\n\n1. executable file");
    printf("\n2. IATA of Airport of departure (source or A)");
    printf("\n3. IATA of Airport of arrival (destination or B)");
    printf("\n4. -L: Indication of the possibility of Layovers");
    printf("\n5. 0-2: Numbers of layovers");
    printf("\n6. -TD or -TC: Sort by departure time ()");
    printf("\n7. -D: Sort by travelled distance");
    printf("\n\n-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");
    printf("\n");
}

FILE *open_file(char *filename, char *mode){

    FILE *file_pointer = fopen(filename, mode);
    
    if(filename == NULL){
        printf("Error opening %d\n", filename);
        exit(1);
    }

    return file_pointer;
}

float distance_airports(int *geocoordinatesA, int *geocoordinatesB){

    int real_coordinatesA[3]; // xa ya za
    int real_coordinatesB[3]; // xb yb zb
    int angle_airports;

    //Real coordinates of airport A
    real_coordinatesA[0] = REAL_RADIUS * cos(geocoordinatesA[0]) * cos(geocoordinatesA[1]);
    real_coordinatesA[1] = REAL_RADIUS * cos(geocoordinatesA[0]) * sin(geocoordinatesA[1]);
    real_coordinatesA[2] = REAL_RADIUS * sin(geocoordinatesA[0]);

    //Real coordinates of airport B
    real_coordinatesB[0] = REAL_RADIUS * cos(geocoordinatesB[0]) * cos(geocoordinatesB[1]);
    real_coordinatesB[1] = REAL_RADIUS * cos(geocoordinatesB[0]) * sin(geocoordinatesB[1]);
    real_coordinatesB[2] = REAL_RADIUS * sin(geocoordinatesB[0]);

    //Calculate the angle in radians between the airports
    angle_airports = acos((real_coordinatesA[0] * real_coordinatesB[0] + real_coordinatesA[1] * real_coordinatesB[1] + real_coordinatesA[2] * real_coordinatesB[2]) / (sqrt(real_coordinatesA[0] * real_coordinatesA[0] + real_coordinatesA[1] * real_coordinatesA[1] + real_coordinatesA[2] * real_coordinatesA[2]) * sqrt(real_coordinatesB[0] * real_coordinatesB[0] + real_coordinatesB[1] * real_coordinatesB[1] + real_coordinatesB[2] * real_coordinatesB[2])));

    return angle_airports * EARTH_RADIUS;

}


//Aiports Functions
StackAirport *init_airports(FILE *fpairports){

    //Variables
    StackAirport *top_airport = NULL;
    char line[100];

    while (fgets(line, 100, fpairports) != NULL){
        
        //Variables
        StackAirport *new_airport_container = (StackAirport *)malloc(sizeof(StackAirport));
        Airport *airport = (Airport *)malloc(sizeof(Airport));
        int n_conv;

        //Reading the line and storing the data in the airport structure
        n_conv = sscanf(line, "%s %s %s %s %s %d", airport->ICAO, airport->IATA, airport->latitude, airport->longitude, airport->city, airport->timezone);

        if(n_conv != 6){
            printf("Error reading line: %s\n", line);
            continue; //Skip the current iteration
        }

        //Pushing the airport to the stack and updating the top
        new_airport_container->airport = *airport;
        new_airport_container->next_airport = top_airport;
        top_airport = new_airport_container;

    }

    return top_airport;

}

//Routes Functions
StackRoute *init_route(FILE *fproutes){

    //Variables
    StackRoute *top_route = NULL;
    char line[100], current_airline[DIM_NAME], *key_airline = "AIRLINE: ";

    while (fgets(line, 100, fproutes) != NULL){
        
        //Variables
        StackRoute *new_route_container = (StackRoute *)malloc(sizeof(StackRoute));
        Route *route = (Route *)malloc(sizeof(Route));
        int n_conv;

        //Check if the line contains the airline name
        char *airline = find_airline(line, key_airline);
        if(airline != NULL){
            strcpy(current_airline, airline);
            continue; //Proceed to the next file line
        }

        //Reading the line and storing the data in the route structure
        n_conv = sscanf(line, "%s %s %s %s %d", route->tripcode, route->IATA_source, route->IATA_destiny, route->departure_time, route->arrival_time);

        if(n_conv != 6){
            printf("Error reading line: %s\n", line);
            continue; //Skip the current iteration
        }

        //Pushing the airport to the stack and updating the top
        new_route_container->route = *route;
        new_route_container->next_route = top_route;
        top_route = new_route_container;

    }

    return top_route;
}

char *find_airline(char *line, char *key_airline){

    if(strstr(line, key_airline) != NULL){
        return strstr(line, key_airline) + strlen(key_airline); //Return the airline name without the key
    }

    return NULL;
}

void show_airports(Airport* airport)
{
    for (int i = airport->top; i >= 0; i--) {
        printf("%c %c %c %c %c %d\n", airport->ICAO, airport->IATA, airport->latitude, airport->longitude, airport->city, airport->timezone);
         }
}

void show_routes(Routes* routes)
{
    for (int i = routes->top; i >= 0; i--) {
        printf("%c %c %c %c %c %c\n", routes->airline, airport->tripcode, airport->IATA_source, airport->IATA_destiny, airport->departure_time, airport->arrival_time);
         }
}