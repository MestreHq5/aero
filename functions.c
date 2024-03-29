#include "functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

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
        n_conv = handle_airport_line(line, airport);

        if(n_conv != 12){
            printf("Error reading line: %s\n", line);
            free(airport);
            free(new_airport_container);
            continue; //Skip the current iteration
        }

        //Pushing the airport to the stack and updating the top
        new_airport_container->airport = *airport;
        new_airport_container->next_airport = top_airport;
        top_airport = new_airport_container;

    }

    return top_airport;

}

int handle_airport_line(char *line, Airport *airport){

    //Variables
    int n_conv;
    int lat_deg, lat_min, lat_sec, lon_deg, lon_min, lon_sec;
    char lat_dir, lon_dir;

    //Reading the line and storing the data in the airport structure
    n_conv = sscanf(line, "%s %s %d %d %d %c %d %d %d %c %s %d", airport->ICAO, airport->IATA, &lat_deg, &lat_min, &lat_sec, &lat_dir, &lon_deg, &lon_min, &lon_sec, &lon_dir, airport->city, &airport->timezone);

    //Converting the coordinates to a unic string for latitude and a unic string for longitude
    sprintf(airport->latitude, "%d %d %d %c", lat_deg, lat_min, lat_sec, lat_dir);
    sprintf(airport->longitude, "%d %d %d %c", lon_deg, lon_min, lon_sec, lon_dir);

    return n_conv;
}

void show_airports(StackAirport *top_airport ){

    //Variables
    StackAirport *current_airport = top_airport;

    while(current_airport != NULL){
        printf("Airport --> ICAO: [%s]  IATA: [%s]  Latitude: [%s]  Longitude: [%s]  City: [%s]  Time Zone: [GMT %d]\n", current_airport->airport.ICAO, current_airport->airport.IATA, current_airport->airport.latitude, current_airport->airport.longitude, current_airport->airport.city, current_airport->airport.timezone);

        current_airport = current_airport->next_airport;

    }
}



//Routes Functions
StackRoute *init_routes(FILE *fproutes){

    //Variables
    StackRoute *top_route = NULL;
    char line[100], current_airline[DIM_NAME], *key_airline = "AIRLINE: ";

    while (fgets(line, 100, fproutes) != NULL){
        
        //Check if the line contains the airline name
        char *airline = find_airline(line, key_airline);
        printf("Airline: %s ", airline);

        if(airline != NULL){
            strcpy(current_airline, airline);
            continue; //Proceed to the next file line
        } 

        printf("Airline: %s ", current_airline);


        //Variables
        StackRoute *new_route_container = (StackRoute *)malloc(sizeof(StackRoute));
        Route *route = (Route *)malloc(sizeof(Route));
        int n_conv;


        //Reading the line and storing the data in the route structure
        n_conv = sscanf(line, "%s %s %s %s %s", route->tripcode, route->IATA_source, route->departure_time, route->IATA_destiny, route->arrival_time);
    

        if(n_conv != 5){
            if(line_is_empty(line) == 1){
                printf("\nError [Empty Line]\n");

            }else{
                printf("\nError [Unformatted Line]: %s", line);
            }

            free(route);
            free(new_route_container);
            continue; //Skip the current iteration
        }

        //Calculating the distance between the airports
        //route->distance = distance_airports(route->IATA_source, route->IATA_destiny);
         
        //Copy the airline name to the route structure
        strcpy(route->airline, current_airline);




        //Pushing the airport to the stack and updating the top
        new_route_container->route = *route;
        new_route_container->next_route = top_route;
        top_route = new_route_container;

    }

    return top_route;
}

int line_is_empty(char *line){

    //Variables
    int i = 0;

    while(line[i] != '\0'){
        if(line[i] != ' ' && line[i] != '\n' && line[i] != '\t' && line[i] != '\r'){
            return 0; //There is a formatting issue in the line
        }
        i++;
    }

    return 1; //The line is empty
}

char *find_airline(char *line, char *key_airline) {
    
    char *beginning_pointer = strstr(line, key_airline);
    
    // Check if the keyword is in the line
    if (beginning_pointer == NULL) {
        return NULL;  
    }

    beginning_pointer += strlen(key_airline);

    // Find the newline character
    char *endpointer = strchr(beginning_pointer, '\n');


    // Calculate the length of the airline name
    size_t airline_length = endpointer - beginning_pointer;

    // Allocate memory for the airline name
    char *airline_name = (char *)malloc(airline_length + 1);
    if (airline_name == NULL) {
        return NULL;
    }

    // Copy the airline name from the line to the allocated memory
    strncpy(airline_name, beginning_pointer, airline_length);
    airline_name[airline_length] = '\0';

    return airline_name;
}

void show_routes(StackRoute *top_route){

    //Variables
    StackRoute *current_route = top_route;

    while(current_route != NULL){
        
        printf("Route --> Airline: [%s]  Tripcode: [%s]  IATA Source: [%s]  IATA Destiny: [%s]  Departure Time: [%s]  Arrival Time: [%s] Distance: [%f] \n", current_route->route.airline, current_route->route.tripcode, current_route->route.IATA_source, current_route->route.IATA_destiny, current_route->route.departure_time, current_route->route.arrival_time, current_route->route.distance);

        current_route = current_route->next_route; //Move to the next route


    }
}

 



