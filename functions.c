#include "functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

//Auxiliary Definitions
#define EARTH_RADIUS 6371
#define REAL_RADIUS 6381 // Earth radius + 10 km altitude
#define DIM_COORD 15
#define PI 3.1415926535



//Functions
void handle_arguments(int argc, char *argv[], StackAirport *airports, StackRoute *routes){

    //Variables
    Airport *airport_source, *airport_destiny;

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

    //Check if the second argument and divide the cases
    if (strcmp(argv[1], "-rotas") == 0){
        show_routes(routes);
        return;

    } else if (strcmp(argv[1], "-aeroportos") == 0){
        show_airports(airports);
        return;
    }

    //Check if the second argument is a valid IATA code
    airport_source = find_airport_by_IATA(airports, argv[1]);

    if (airport_source == NULL){
        printf("Invalid IATA code for the source airport: Execution failed...\n");
        return;
    }

    //Check if the third argument is a valid IATA code
    airport_destiny = find_airport(airports, argv[2]);

    if (airport_destiny == NULL){
        printf("Invalid IATA code for the destiny airport: Execution failed...\n");
        return;
    }

    //Check if the fourth argument is a valid option
    return;
}

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

/*
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
*/

//Airports Functions
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

Airport *find_airport_by_IATA(StackAirport *airport, const char *code_IATA) {
    
    StackAirport *current = airport;

    while (current != NULL) {
        if (strcmp(current->airport.IATA, code_IATA) == 0) {
            return &(current->airport);
        }

        current = current->next_airport;// Move to the next airport in the stack
    }

    return NULL;

}

void free_airports(StackAirport *top_airport){

    //Variables
    StackAirport *current_airport = top_airport;

    while(current_airport != NULL){
        StackAirport *next_airport = current_airport->next_airport;
        free(current_airport);
        current_airport = next_airport;
    }
}


//Routes Functions
StackRoute *init_routes(FILE *fproutes){

    //Variables
    StackRoute *top_route = NULL;
    char line[100], current_airline[DIM_NAME] = "", *key_airline = "AIRLINE: ";

    while (fgets(line, 100, fproutes) != NULL){

        //Check if the line is empty
        if (line_is_empty(line)){
            continue; //Skip the current iteration
        } 
        
        //Check if the line contains the airline name
        char *airline = find_airline(line, key_airline);

        if(airline != NULL){
            strcpy(current_airline, airline);
            continue; //Proceed to the next file line
        } 

        //Variables
        StackRoute *new_route_container = (StackRoute *)malloc(sizeof(StackRoute));
        Route *route = (Route *)malloc(sizeof(Route));
        int n_conv;

        //Reading the line and storing the data in the route structure
        n_conv = sscanf(line, "%s %s %s %s %s", route->tripcode, route->IATA_source, route->departure_time, route->IATA_destiny, route->arrival_time);

        //Check if the line is unformatted
        if(n_conv != 5){
            printf("\nError [Unformatted Line]: %s", line);
            free(route);
            free(new_route_container);
            continue; //Skip the current iteration
        }

        //Calculating the distance between the airports

         
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

    int index;
    char *airline = (char *)malloc(DIM_NAME * sizeof(char));
    char *beginning_pointer = strstr(line, key_airline);
    
    // Check if the keyword is in the line or the allocation failed
    if ((beginning_pointer == NULL) || airline == NULL){
        return NULL;  
    }

    //Advance to the final of the Keyword "AIRLINE:"
    beginning_pointer += strlen(key_airline);

    //Copy the Ailine Name to a string
    for (index = 0; (index < DIM_NAME) && (*beginning_pointer != ('\r' || '\n')); index++){

        airline[index] = *beginning_pointer;
        beginning_pointer++;
    
    }

    //Teminate the string an return
    airline[index] = '\0';

    return airline;
}

void show_routes(StackRoute *top_route){

    //Variables
    StackRoute *current_route = top_route;

    while(current_route != NULL){
        
        printf("Route: [%s] %s %s ---> %s %s [%f] %s", current_route->route.tripcode, current_route->route.departure_time, current_route->route.IATA_source, current_route->route.IATA_destiny, current_route->route.arrival_time, current_route->route.distance, current_route->route.airline);

        current_route = current_route->next_route; //Move to the next route
    }
}

char *find_airport(StackAirport *airport, const char *targetIATA) {
    StackAirport *current = airport;

    while (current != NULL) {
        // Check if the current airport's IATA code matches the target
        if (strcmp(current->airport.IATA, targetIATA) == 0) {
            // If found, create a string with the information about the airport
            char *info = (char*)malloc(256 * sizeof(char));
            if (info == NULL) {
                fprintf(stderr, "Memory allocation failed\n");
                return NULL; // or handle error appropriately
            }

            sprintf(info, "ICAO: %s\nIATA: %s\nLatitude: %s\nLongitude: %s\nCity: %s\nTimezone: %d\n",
                    current->airport.ICAO, current->airport.IATA, current->airport.latitude,
                    current->airport.longitude, current->airport.city, current->airport.timezone);
            return info;
        }
        // Move to the next airport in the stack
        current = current->next_airport;
    }

    // If the target airport is not found, return NULL
    return NULL;
}

void free_routes(StackRoute *top_route) {
    StackRoute *current_route = top_route;

    while (current_route != NULL) {
        StackRoute *next_route = current_route->next_route;
        free(current_route);
        current_route = next_route;
    }
}

/*

char* find_lat_long(char *info) {

    float lat_deg, lat_min, lat_sec;
    float long_deg, long_min, long_sec;
    char lat_dir, long_dir;

    char *latitude = (char*)malloc(DIM_COORD * sizeof(char));
    if (latitude == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }

    // Parse the input string to extract degrees, minutes, and seconds
    sscanf(latitude, "%lf %lf %lf %c ", &lat_deg, &lat_min, &lat_sec, &lat_dir);


    char *longitude = (char*)malloc(DIM_COORD * sizeof(char));
    if (longitude == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }

    // Parse the input string to extract degrees, minutes, and seconds
    sscanf(longitude, "%lf %lf %lf ", &long_deg, &long_min, &long_sec, &long_dir);

    // Calculate total degrees
    double totalDegrees_lat = lat_deg + (lat_min / 60.0) + (lat_min / 3600.0);
    double rad_lat=totalDegrees_lat* PI/180; //PI definido em cima

    if(lat_dir=='S')
    rad_lat=-rad_lat;


    double totalDegrees_long = long_deg + (long_min / 60.0) + (long_sec / 3600.0);
    double rad_long=totalDegrees_long * PI/180;//PI definido em cima

    if(long_dir=='W')
    rad_long=-rad_long;
    

    // Allocate memory for the result string
    char* result = (char*)malloc(100 * sizeof(char));
    if (result == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }

    // Format rad_lat and rad_long into the result string
    sprintf(result, "Rad_lat: %f\nRad_long: %f\n", rad_lat, rad_long);

    return result;
}

double* calculate_real_coordinates(double rad_lat, double rad_long) {
    // Allocate memory for the array
    double* coordinates = (double*)malloc(3 * sizeof(double));
    if (coordinates == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }

    // Calculate the Cartesian coordinates
    coordinates[0] = REAL_RADIUS * cos(rad_lat) * cos(rad_long);
    coordinates[1] = REAL_RADIUS * cos(rad_lat) * sin(rad_long);
    coordinates[2] = REAL_RADIUS * sin(rad_lat);

    return coordinates;
}

//Calculating the distance between the airports
float distance_airports(StackAirport *airport, char *IATA_source, char *IATA_destiny){
        

        //Variables
        char* Airport_source = find_airport(airport, IATA_source);
        char* Lat_long_source = find_lat_long(Airport_source);
            
        char* Airport_destiny= find_airport(StackAirport *airport, IATA_destiny);
        char* Lat_long_destiny = find_lat_long(Airport_destiny);

    double source_rad_lat, source_rad_long, destiny_rad_lat, destiny_rad_long;

        // Parse the input string to extract rad_lat and rad_long
    sscanf(Lat_long_source, "Rad_lat: %lf\nRad_long: %lf", &source_rad_lat, &source_rad_long);
    sscanf(Lat_long_destiny, "Rad_lat: %lf\nRad_long: %lf", &destiny_rad_lat, &destiny_rad_long);


    int real_coordinates_source[3]; // xa ya za
    int real_coordinates_destiny[3]; // xb yb zb
    int angle_airports;

    //Real coordinates of airport A
    double* calculate_real_coordinates(double *source_rad_lat, double *source_rad_long)
    
    //Real coordinates of airport B
    double* calculate_real_coordinates(double *destiny_rad_long, double *destiny_rad_long)

    //Calculate the angle in radians between the airports
    angle_airports = acos((real_coordinates_source[0] * real_coordinates_destiny[0] + real_coordinates_source[1] * real_coordinates_destiny[1] + real_coordinates_source[2] * real_coordinates_destiny[2]) / (sqrt(real_coordinates_source[0] * real_coordinates_source[0] + real_coordinates_source[1] * real_coordinates_source[1] + real_coordinates_source[2] * real_coordinates_source[2]) * sqrt(real_coordinates_destiny[0] * real_coordinates_destiny[0] + real_coordinates_destiny[1] * real_coordinates_destiny[1] + real_coordinates_destiny[2] * real_coordinates_destiny[2])));

    return angle_airports * EARTH_RADIUS;
}   

*/
