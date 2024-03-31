#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "functions.h"


//Auxiliary Definitions
#define EARTH_RADIUS 6371.0
#define REAL_RADIUS 6381.0 // Earth radius + 10 km altitude
#define DIM_COORD 15
#define PI 3.1415926535
#define LATITUDE 0 
#define LONGITUDE 1
#define X 0 
#define Y 1 
#define Z 2


//General Functions

int layover_number(char *option){

    //Variables
    int layovers = atoi(option);

    //Check if the number of layovers is in the range 0-2
    if (layovers < 0 || layovers > 2){
        printf("Invalid number of layovers: Execution failed...\n");
        exit(1);
    }

    return layovers;

}

int time_sort_option(char *option){

    if (strcmp(option, "-TC") == 0){
        return 1;

    } else if (strcmp(option, "-TD") == 0){
        return 0;

    } else {
        
        printf("Invalid option to sort by time: Execution failed...\n");
        exit(1);
    }

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

    //Necessary to avoid memory issues in the sprintf
    if(n_conv != 12){
        return n_conv;
    }

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
StackRoute *init_routes(FILE *fproutes, StackAirport *airports){

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

        //Calculating the distance between the airports
        route->distance = distance_airports(airports, route);
        
        //Check if the line is unformatted
        if(n_conv != 5 || route->distance < 0){
            free(route);
            free(new_route_container);
            continue; //Skip the current iteration
        }

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
    for (index = 0; (index < DIM_NAME) && (*beginning_pointer != '\r') && (*beginning_pointer != '\n'); index++){

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
        
        printf("Route: [%s] %s %s ---> %s %s [%.2f]km %s", current_route->route.tripcode, current_route->route.departure_time, current_route->route.IATA_source, current_route->route.IATA_destiny, current_route->route.arrival_time, current_route->route.distance, current_route->route.airline);

        current_route = current_route->next_route; //Move to the next route
    }
}

float distance_airports(StackAirport *airports, Route* route){
        
    //Variables
    Airport *airport_source = find_airport_by_IATA(airports, route->IATA_source);
    Airport *airport_destiny = find_airport_by_IATA(airports, route->IATA_destiny);
    
    double source_coord[2], destiny_coord[2];
    double real_source_coord[3], real_destiny_coord[3];
    float angle_airports;

    //Check if the IATA codes are valid
    if(airport_source == NULL || airport_destiny == NULL){
        return -1.0; //Negative distance implies the airports were not found 
    }

    //Parse the strings to extract the coordinates in degrees, minutes and seconds and convert to degree
    coordinates_parser(airport_source, source_coord);
    coordinates_parser(airport_destiny, destiny_coord);

    //Convert the angle to radians
    source_coord[LATITUDE] = source_coord[LATITUDE] * PI / 180;
    source_coord[LONGITUDE] = source_coord[LONGITUDE] * PI / 180;
    destiny_coord[LATITUDE] = destiny_coord[LATITUDE] * PI / 180;
    destiny_coord[LONGITUDE] = destiny_coord[LONGITUDE] * PI / 180;

    //Real Coordinates
    real_coordinates(source_coord, real_source_coord, REAL_RADIUS);
    real_coordinates(destiny_coord, real_destiny_coord, REAL_RADIUS);

    //Calculate the angle between the real coordinates
    angle_airports = acos((real_source_coord[X] * real_destiny_coord[X] + real_source_coord[Y] * real_destiny_coord[Y] + real_source_coord[Z] * real_destiny_coord[Z]) / (sqrt(real_source_coord[X] * real_source_coord[X] + real_source_coord[Y] * real_source_coord[Y] + real_source_coord[Z] * real_source_coord[Z]) * sqrt(real_destiny_coord[X] * real_destiny_coord[X] + real_destiny_coord[Y] * real_destiny_coord[Y] + real_destiny_coord[Z] * real_destiny_coord[Z])));

    return angle_airports * EARTH_RADIUS;
}

void coordinates_parser(Airport *airport, double coord_vector[2]){

    //Variables
    int lat_deg, lat_min, lat_sec, lon_deg, lon_min, lon_sec;
    char lat_dir, lon_dir;

    //Extract latitude and longitude information from the airport
    sscanf(airport->latitude, "%d %d %d %c", &lat_deg, &lat_min, &lat_sec, &lat_dir);
    sscanf(airport->longitude, "%d %d %d %c", &lon_deg, &lon_min, &lon_sec, &lon_dir);

    //Calculate total degrees
    coord_vector[LATITUDE] = lat_deg + (lat_min / 60.0) + (lat_sec / 3600.0);
    coord_vector[LONGITUDE] = lon_deg + (lon_min / 60.0) + (lon_sec / 3600.0);

    //Check if Directions: -1 -1 -1 N == 1 1 1 S (example)
    if(lat_dir == 'S'){
        coord_vector[0] = -coord_vector[0];
    }

    if(lon_dir == 'W'){
        coord_vector[1] = -coord_vector[1];
    }

}

void real_coordinates(double coord[3], double real_coord[3], float radius){

    //Calculate Coordinates (Origin --> Center of Earth)
    real_coord[X] = radius * cos(coord[LATITUDE]) * cos(coord[LONGITUDE]);
    real_coord[Y] = radius * cos(coord[LATITUDE]) * sin(coord[LONGITUDE]);
    real_coord[Z] = radius * sin(coord[LATITUDE]);
    
}

void free_routes(StackRoute *top_route) {
    StackRoute *current_route = top_route;

    while (current_route != NULL) {
        StackRoute *next_route = current_route->next_route;
        free(current_route);
        current_route = next_route;
    }
}


//Algorithms Functions
void list_direct_flights(StackAirport *airports, StackRoute *routes, Airport *airport_source, Airport *airport_destiny, int time_sort_option) {
    
    //Create a stack of routes to keep (may be sorted or not ---> depends on time_sort_option)
    KeepRoute *top = NULL;
    top = (KeepRoute *)malloc(sizeof(KeepRoute));

    if (top == NULL) {
        printf("Memory allocation failed while finding routes...\n");
        exit(1);
    }

    //Initialize the stack of keep routes
    find_routes_no_layover(routes, airport_source->IATA, airport_destiny->IATA, &top);

    //Check if sorted is needed
    if (time_sort_option == -1) {
        show_keep_route(top);
    } else if( time_sort_option == 1){
        insertion_sort_keep_route(&top);
        show_keep_route(top);
    }

    free_keep_route(top);
}

void show_keep_route(KeepRoute *top_route) {
    
    //Creating a pointer that moves through the stack of routes
    KeepRoute *current_route = top_route;

    while(current_route != NULL){
        
        int count = 0;

        if(current_route->route != NULL){

        printf("Route: [%s] %s %s ---> %s %s [%.2f]km %s\n", current_route->route->tripcode, current_route->route->departure_time, current_route->route->IATA_source, current_route->route->IATA_destiny, current_route->route->arrival_time, current_route->route->distance, current_route->route->airline);

        } 


        

        current_route = current_route->next_route; //Move to the next route
    }
}

void find_routes_no_layover(StackRoute *routes, const char *airport_source, const char *airport_destiny, KeepRoute **top_stack) {
    StackRoute *current = routes;

    while (current != NULL) {
        if (strcmp(current->route.IATA_source, airport_source) == 0 && strcmp(current->route.IATA_destiny, airport_destiny) == 0) {

            // Allocate memory for a new KeepRoute node
            KeepRoute *new_node = (KeepRoute *)malloc(sizeof(KeepRoute));
            if (new_node == NULL) {
                fprintf(stderr, "Memory allocation failed while finding routes...\n");
                exit(1);
            }

            // Copy the contents of the current route into the newly allocated Route structure
            new_node->route = &(current->route);


            // Add the route to the top of the existing stack
            new_node->next_route = *top_stack;
            *top_stack = new_node;

            //As layover is not needed, the other routes are NULL
            new_node->route_two = NULL;
            new_node->route_three = NULL;
        }

        // Move to the next route in the Global Stack of Routes
        current = current->next_route;
    }
}

//Free the memory allocated for the stack of routes
void free_keep_route(KeepRoute *top_route){

    while (top_route != NULL){
        KeepRoute *temp = top_route;
        top_route = top_route->next_route;
        free(temp);
    }
}

//Order the stack of routes by distance using Insertion Sort
void insertion_sort_keep_route(KeepRoute **top){
    
    //Checks if the stack is empty or has only one element (already sorted)
    if (*top == NULL || (*top)->next_route == NULL) {
        return;
    }

    KeepRoute *sortedStack = NULL;
    KeepRoute *current = *top;

    while (current != NULL) {
        KeepRoute *next = current->next_route;
        if (sortedStack == NULL || sortedStack->route->distance >= current->route->distance) {
            current->next_route = sortedStack;
            sortedStack = current;
        } else {
            KeepRoute *ptr = sortedStack;
            while (ptr->next_route != NULL && ptr->next_route->route->distance < current->route->distance) {
                ptr = ptr->next_route;
            }
            current->next_route = ptr->next_route;
            ptr->next_route = current;
        }
        current = next;
    }

    *top = sortedStack; // Update the top pointer to point to the sorted stack
}

// Function to find routes with one layover
void find_routes_one_layover(StackRoute *routes, const char *departure, const char *destination) {

    StackRoute *current_route = routes;

    //Loop to find flights from the departure airport 
    while (current_route != NULL) {
        //Find a the first route that departures from the departure airport
        if (strcmp(current_route->route.IATA_source, departure) == 0) {

            StackRoute *layover_route = routes;

            //Loop to find flights that departure from where the first flight lands and that land at the destination airport
            while (layover_route != NULL) {
                //Find the second route that departures from the airport that the first route lands on and that arrives on the destination airport
                if (strcmp(layover_route->route.IATA_source, current_route->route.IATA_destiny) == 0 && strcmp(layover_route->route.IATA_destiny, destination) == 0) {
                 
                 //Save the layover_route(Second flight) and the current_route(First flight)
            
                }
                layover_route = layover_route->next_route;
            }
        }
        current_route = current_route->next_route;
    }
}


// Function to find routes with two layover
void find_routes_two_layover(StackRoute *routes, const char *departure, const char *destination) {

    StackRoute *current_route = routes;

    //Loop to find flights from the departure airport 
    while (current_route != NULL) {
        //Find a the first route that departures from the departure airport
        if (strcmp(current_route->route.IATA_source, departure) == 0) {

            StackRoute *layover_route_one = routes;
            
            //Loop to find flights that departure from where the first flight lands
            while (layover_route_one != NULL) {
                //Find the second route that departures from the airport that the first route lands  
                if (strcmp(layover_route_one->route.IATA_source, current_route->route.IATA_destiny) == 0) {
                 
                    StackRoute *layover_route_two = routes;
                    
                    //Loop to find the flights that departure from where the second one lands and that arrive at the final destination
                    while(layover_route_two != NULL){
                        //Find the thrird flight that departure from where the second one lands and that arrive at the final destination
                        if(strcmp(layover_route_two->route.IATA_source, layover_route_one->route.IATA_destiny) && strcmp(layover_route_two->route.IATA_destiny, destination)){
                        
                        //Save the layover_route_two(Third flight), the layover_roure_one(Second flight) and the current_route(First flight)

                        }
                      layover_route_two = layover_route_two->next_route;  
                    }
            
                }
                layover_route_one = layover_route_one->next_route;
            }
        }
        current_route = current_route->next_route;
    }
}









