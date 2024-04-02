#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <float.h>

#include "functions.h"


//Auxiliary Definitions
#define EARTH_RADIUS 6371.0
#define REAL_RADIUS 6381.0 // Earth radius + 10 km altitude
#define DIM_COORD 20
#define PI 3.1415926535
#define LATITUDE 0 
#define LONGITUDE 1
#define X 0 
#define Y 1 
#define Z 2
#define LAYOVERS 0
#define TIME_OPTION 1
#define DISTANCE_OPTION 2


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
        return -1;

    } else {
        
        printf("Invalid option to sort by time: Execution failed...\n");
        exit(1);
    }

}

int distance_sort_option(char *option){

    if (strcmp(option, "-D") == 0){
        return 1;

    } else {
        printf("Invalid option to sort by distance: Execution failed...\n");
        exit(1);
    }

}

void arguments_error() {
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
    printf("\n\n");
}

FILE *open_file(char *filename, char *mode){

    FILE *file_pointer = fopen(filename, mode);
    
    if(filename == NULL){
        printf("Error opening %s\n", filename);
        exit(1);
    }

    return file_pointer;
}

float numeric_time(char *time) {
    
    //Variables
    int hours, minutes;
    float numeric_time;

    //Extract the hours and minutes from the string
    sscanf(time, "%2d:%2d", &hours, &minutes);

    //Convert the time to a numeric value (algrebra in now valid in the if's)
    numeric_time = hours + (minutes / 60.0);

    return numeric_time;
}


//Airports Functions
StackAirport *init_airports(FILE *fpairports){

    //Variables
    StackAirport *top_airport = NULL;
    char line[100];

    while (fgets(line, 100, fpairports) != NULL){
        
        //Variables
        int n_conv;
        StackAirport *new_airport_container = (StackAirport *)malloc(sizeof(StackAirport));
        Airport *airport = (Airport *)malloc(sizeof(Airport));

        if (new_airport_container == NULL || airport == NULL){
            printf("Memory allocation failed while building Airports Stacks...\n");
            exit(1);
        }

        //Reading the line and storing the data in the airport structure
        n_conv = handle_airport_line(line, airport);

        if(n_conv != 12){
            free(airport);
            free(new_airport_container);
            continue; //Skip the current iteration
        }

        //Pushing the airport to the stack and updating the top
        new_airport_container->airport = *airport;
        free(airport);
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
void null_init_route(Route *route){

    //Initialize the fields of the route to NULL (or equivalent 0.0 for floats or '\0' for strings)
    route->tripcode[0] = '\0';
    route->IATA_source[0] = '\0';
    route->departure_time[0] = '\0';
    route->IATA_destiny[0] = '\0';
    route->arrival_time[0] = '\0';
    route->distance = 0.0;
    route->airline[0] = '\0';
    route->numeric_arrival = 0.0;
    route->numeric_departure = 0.0;

}

StackRoute *init_routes(FILE *fproutes, StackAirport *airports){

    //Variables
    StackRoute *top_route = NULL;
    char line[100], *current_airline = NULL, *key_airline = "AIRLINE: ";

    while (fgets(line, 100, fproutes) != NULL){

        //Check if the line is empty
        if (line_is_empty(line)){
            continue; //Skip the current iteration
        } 
        
        //Check if the line contains the airline name
        find_airline(line, key_airline, &current_airline);

        if (current_airline == NULL){
            continue; //Skip the current iteration
        }

        //Variables
        StackRoute *new_route_container = (StackRoute *)malloc(sizeof(StackRoute));
        Route *route = (Route *)malloc(sizeof(Route));
        int n_conv;

        if (new_route_container == NULL || route == NULL){
            printf("Memory allocation failed while building Routes Stacks...\n");
            exit(1);
        }

        //Initialize the fields of the route to NULL (or equivalent 0.0 for floats or '\0' for strings)
        null_init_route(route);

        //Reading the line and storing the data in the route structure
        n_conv = sscanf(line, "%5s %3s %5s %3s %5s", route->tripcode, route->IATA_source, route->departure_time, route->IATA_destiny, route->arrival_time);

        //Calculating the distance between the airports
        route->distance = distance_airports(airports, route);
        
        //Check if the line is unformatted
        if(n_conv != 5 || route->distance < 0){
            free(route);
            free(new_route_container);
            continue; //Skip the current iteration
        }

        //Convert the time to a numeric value to avoid string comparisons
        route->numeric_departure = numeric_time(route->departure_time);
        route->numeric_arrival = numeric_time(route->arrival_time);

        //Copy the airline name to the route structure
        strcpy(route->airline, current_airline);

        //Pushing the airport to the stack and updating the top
        new_route_container->route = *route;
        free(route);
        new_route_container->next_route = top_route;
        top_route = new_route_container;

    }

    // Free the memory for current_airline because it will not be used anymore
    if (current_airline != NULL) {
        free(current_airline);
    }

    return top_route;
}

int line_is_empty(char *line){

    //Variables
    int i = 0;

    while(line[i] != '\0'){
        if(line[i] != ' ' && line[i] != '\n' && line[i] != '\t' && line[i] != '\r'){
            return 0; //There is data in the line
        }
        i++;
    }

    return 1; //The line is empty
}

void find_airline(char *line, char *key_airline, char **airline) {

    int index;
    char *beginning_pointer = strstr(line, key_airline);
    
    // Check if the keyword is in the line
    if (beginning_pointer == NULL){
        return;  // If keyword is not found, do not change airline
    }

    // If keyword is found, free the old airline and allocate memory for the new one
    if (*airline != NULL) {
        free(*airline);
    }
    *airline = (char *)malloc(DIM_NAME * sizeof(char));

    //Advance to the final of the Keyword "AIRLINE:"
    beginning_pointer += strlen(key_airline);

    //Copy the Ailine Name to a string
    for (index = 0; (index < DIM_NAME) && (*beginning_pointer != '\r') && (*beginning_pointer != '\n'); index++){

        (*airline)[index] = *beginning_pointer;
        beginning_pointer++;
    
    }

    //Teminate the string an return
    (*airline)[index] = '\0';

    return;
}

void show_routes(StackRoute *top_route){

    //Variables
    StackRoute *current_route = top_route;

    while(current_route != NULL){
        
        printf("Route: [%s] %s %s ---> %s %s [%.2f]km %s\n", current_route->route.tripcode, current_route->route.departure_time, current_route->route.IATA_source, current_route->route.IATA_destiny, current_route->route.arrival_time, current_route->route.distance, current_route->route.airline);

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


//Distance Functions
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

void real_coordinates(double coord[2], double real_coord[3], float radius){

    //Calculate Coordinates (Origin --> Center of Earth)
    real_coord[X] = radius * cos(coord[LATITUDE]) * cos(coord[LONGITUDE]);
    real_coord[Y] = radius * cos(coord[LATITUDE]) * sin(coord[LONGITUDE]);
    real_coord[Z] = radius * sin(coord[LATITUDE]);
    
}


//List Flights Functions
void null_init_top(KeepRoute *top) {
    
    //Initialize the fields of top
    top->route = NULL;
    top->route_two = NULL;
    top->route_three = NULL;
    top->next_route = NULL;
}

void list_direct_flights(StackAirport *airports, StackRoute *routes, Airport *airport_source, Airport *airport_destiny, int *time_and_distance) {
    
    //Create a stack of routes to keep (may be sorted or not ---> depends on time_and_distance[0])
    KeepRoute *top = (KeepRoute *)malloc(sizeof(KeepRoute));

    if (top == NULL) {
        printf("Memory allocation failed while finding routes...\n");
        exit(1);
    }

    //Initialize the fields of top
    null_init_top(top);

    //Initialize the stack of keep routes
    find_routes_no_layover(routes, airport_source->IATA, airport_destiny->IATA, &top);

    //Check if sorted is needed
    if (time_and_distance[0] == 0) {
        show_keep_route(top);
    } else{
        bubble_sort_keep_route(&top, time_and_distance[0]);
        show_keep_route(top);
    }

    free_keep_route(top);
}

void list_one_layover(StackAirport *airports, StackRoute *routes, Airport *airport_source, Airport *airport_destiny, int *time_and_distance) {
    
    //Create a stack of routes to keep (may be sorted or not ---> depends on time_sort_option)
    KeepRoute *top = (KeepRoute *)malloc(sizeof(KeepRoute));

    if (top == NULL) {
        printf("Memory allocation failed while finding routes...\n");
        exit(1);
    }

    //Initialize the fields of top
    null_init_top(top);

    //Initialize the stack of keep routes
    find_routes_one_layover(routes, airport_source->IATA, airport_destiny->IATA, &top);

    //Check if sorted is needed
    if (time_and_distance[0] == 0) {
        show_keep_route_one_layover(top);

    } else{

        //Given that the routes are sorted by time, the drop connecting concern is needed
        drop_connecting_concern(&top);
        
        if (time_and_distance[1] == 1) {
            drop_long_trips(&top);
        }

        bubble_sort_keep_route(&top, time_and_distance[0]);
        show_keep_route_one_layover(top);
    }

    free_keep_route(top);
} 

void list_two_layovers(StackAirport *airports, StackRoute *routes, Airport *airport_source, Airport *airport_destiny, int *time_and_distance) {
    
    //Create a stack of routes to keep (may be sorted or not ---> depends on time_sort_option)
    KeepRoute *top = (KeepRoute *)malloc(sizeof(KeepRoute));

    if (top == NULL) {
        printf("Memory allocation failed while finding routes...\n");
        exit(1);
    }

    //Initialize the fields of top
    null_init_top(top);

    //Initialize the stack of keep routes
    find_routes_two_layover(routes, airport_source->IATA, airport_destiny->IATA, &top);

    //Check if sorted is needed
    if (time_and_distance[0] == 0) {
        show_keep_route_two_layover(top);

    } else{

        //Given that the routes are sorted by time, the drop connecting concern is needed
        drop_connecting_concern(&top);
        
        if (time_and_distance[1] == 1) {
            drop_long_trips(&top);
        }

        bubble_sort_keep_route(&top, time_and_distance[0]);
        show_keep_route_two_layover(top);
    }

    free_keep_route(top);
}


//Free functions

void free_keep_route(KeepRoute *top_route){

    while (top_route != NULL){
        KeepRoute *temp = top_route;
        top_route = top_route->next_route;
        free(temp);
    }
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
float get_departure_time(KeepRoute *trip) {
    
    // Get departure time (always first flight of the trip)
    if (trip->route != NULL) {
        return trip->route->numeric_departure;

    } else {
        return FLT_MAX;
    }
}

float get_arrival_time(KeepRoute *trip) {
    
    // Get arrival time (first flight if no layovers, second flight if one layover, third flight if two layovers)
    if (trip->route_three != NULL) {
        return trip->route_three->numeric_arrival;

    } else if (trip->route_two != NULL) {
        return trip->route_two->numeric_arrival;

    } else if (trip->route != NULL) {
        return trip->route->numeric_arrival;

    } else {
        return FLT_MAX;
    }
}

void bubble_sort_keep_route(KeepRoute **top, int tso) {
    
    // Check if the stack is empty or has only one element (already sorted)
    if (*top == NULL || (*top)->next_route == NULL) {
        printf("No routes found...\n");
        return;  
    }

    //Variables
    int swapped;
    KeepRoute *trip;
    KeepRoute *last_trip = NULL;
    // Time Sort Option (tso) is a multiplier to sort by time (1 for ascending, -1 for descending)


    //Bubble Sort Ahgorithm
    do {
        swapped = 0;
        trip = *top;

        while (trip->next_route != last_trip) {

            //Initialize the comparing values
            float departure_time_trip = tso * get_departure_time(trip);
            float departure_time_next = tso * get_departure_time(trip->next_route);
            float arrival_time_trip = tso * get_arrival_time(trip);
            float arrival_time_next = tso * get_arrival_time(trip->next_route);

            if (departure_time_trip > departure_time_next || 
                (departure_time_trip == departure_time_next && arrival_time_trip > arrival_time_next)) {
                swap(trip, trip->next_route);
                swapped = 1;
            }
            trip = trip->next_route;
        }
        last_trip = trip;

    } while (swapped);
}

void swap(KeepRoute *route_a, KeepRoute *route_b) {
    Route *temp_route = route_a->route;
    Route *temp_route_two = route_a->route_two;
    Route *temp_route_three = route_a->route_three;

    route_a->route = route_b->route;
    route_a->route_two = route_b->route_two;
    route_a->route_three = route_b->route_three;

    route_b->route = temp_route;
    route_b->route_two = temp_route_two;
    route_b->route_three = temp_route_three;
}


//Sort by Distance Functions
float sum_trip_distances(KeepRoute *trip){

    //Variables
    float sum = 0.0;

    //Sum the distances of the flights
    if (trip->route != NULL) {
        sum += trip->route->distance;
    }

    if (trip->route_two != NULL) {
        sum += trip->route_two->distance;
    }

    if (trip->route_three != NULL) {
        sum += trip->route_three->distance;
    }

    return sum;
}

float get_shortest_distance(KeepRoute *top){
    //Variables
    KeepRoute *current = top;
    float shortest_distance = FLT_MAX; // Initialize shortest_distance to the maximum finite float value
    float current_distance;

    while (current != NULL) {
        
        //Distance of the current trip
        current_distance = sum_trip_distances(current);

        //Avoid Errors in sorting remove the trip with the distance 0
        if (current_distance == 0.0){
            current = current->next_route;
            continue;
        } 

        //Update the shortest distance if the current distance is shortest
        if (current_distance < shortest_distance) {
            shortest_distance = current_distance;
        }

        current = current->next_route;

    }

    return shortest_distance;
}

void drop_long_trips(KeepRoute **top) {
    KeepRoute *current = *top;
    KeepRoute *prev = NULL;
    float shortest_distance = get_shortest_distance(*top);

    while (current != NULL) {
        // Check if current->route is NULL before accessing its distance
        if (current->route == NULL) {
            // Handle the error here, perhaps by skipping this iteration of the loop
            current = current->next_route;
            continue;
        }

        // Get the distance of the current trip
        float current_distance = current->route->distance 
                                 + (current->route_two != NULL ? current->route_two->distance : 0) 
                                 + (current->route_three != NULL ? current->route_three->distance : 0);

        // Drop the trip if the distance is greater than the shortest distance
        if (current_distance > shortest_distance) {
            KeepRoute *temp = current;
            if (prev != NULL) {
                prev->next_route = current->next_route;
            } else {
                *top = current->next_route;
            }
            current = current->next_route;
            free(temp);
        } else {
            prev = current;
            current = current->next_route;
        }
    }
}



// Functions to find routes with 0-2 layovers
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

void find_routes_one_layover(StackRoute *routes, const char *departure, const char *destination, KeepRoute **top_stack) {
    StackRoute *current_route = routes;

    while (current_route != NULL) {
        // Find the first route that departs from the departure airport
        if (strcmp(current_route->route.IATA_source, departure) == 0) {
            StackRoute *layover_route = routes;
            
            while (layover_route != NULL) {
                
                // Find the second route that departs from the destination of the first route
                if (strcmp(layover_route->route.IATA_source, current_route->route.IATA_destiny) == 0 &&
                    strcmp(layover_route->route.IATA_destiny, destination) == 0) {
                    
                    // Allocate memory for a new KeepRoute trip (maximum 3 flights)
                    KeepRoute *trip = (KeepRoute *)malloc(sizeof(KeepRoute));
                    if (trip == NULL) {
                        fprintf(stderr, "Memory allocation failed while finding routes...\n");
                        exit(1);
                    }

                    // Assign routes to the KeepRoute node
                    trip->route = &(current_route->route);
                    trip->route_two = &(layover_route->route);
                    trip->route_three = NULL;

                    // Add the route to the top of the existing stack
                    trip->next_route = *top_stack;
                    *top_stack = trip;
                }
                layover_route = layover_route->next_route;
            }
        }
        current_route = current_route->next_route;
    }
}

void find_routes_two_layover(StackRoute *routes, const char *departure, const char *destination, KeepRoute **top_stack) {

    StackRoute *current_route = routes;
 
    while (current_route != NULL) {
        
        //Find a the first route that departures from the departure airport
        if (strcmp(current_route->route.IATA_source, departure) == 0 && strcmp(current_route->route.IATA_destiny, destination) != 0){

            StackRoute *layover_route_one = routes;
            
            while (layover_route_one != NULL) {
                
                //Find a layover flight that departure from the destination of the first flight and that arrive anywhere (except the departure airport from the first flight)
                if (strcmp(layover_route_one->route.IATA_source, current_route->route.IATA_destiny) == 0 && strcmp(layover_route_one->route.IATA_destiny, current_route->route.IATA_source) != 0){
                 
                    StackRoute *layover_route_two = routes;
                    
                    while(layover_route_two != NULL){
                        //Find the last flight that departure from where the second one lands and arrives at the final destination
                        if(strcmp(layover_route_two->route.IATA_source, layover_route_one->route.IATA_destiny) == 0 && strcmp(layover_route_two->route.IATA_destiny, destination) == 0){
                        
                            KeepRoute *new_node = (KeepRoute *)malloc(sizeof(KeepRoute));
                            if (new_node == NULL) {
                                fprintf(stderr, "Memory allocation failed while finding routes...\n");
                                exit(1);
                            }

                            new_node->route = &(current_route->route);
                            new_node->route_two = &(layover_route_one->route);
                            new_node->route_three = &(layover_route_two->route);

                            // Add the route to the top of the existing stack   
                            new_node->next_route = *top_stack;
                            *top_stack = new_node;
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



//Connecting Flights Concern
void drop_connecting_concern(KeepRoute **top) {
    if (*top == NULL) {
        return;  // If the stack is empty, there's nothing to drop
    }

    KeepRoute *current = *top;
    KeepRoute *prev = NULL;

    while (current != NULL) {
        // Check if the departure time of the layover flight is before the arrival time of the first flight
        if (current->route_two != NULL && current->route_two->numeric_departure < current->route->numeric_arrival) {
            // Drop this element
            KeepRoute *temp = current;
            if (prev != NULL) {
                prev->next_route = current->next_route;
            } else {
                // If prev is NULL, it means we're dropping the first element
                *top = current->next_route;
            }
            current = current->next_route;
            free(temp); // Free the memory of the dropped element
        }
        // Check if the departure time of the third flight is before the arrival time of the second flight
        else if (current->route_three != NULL && current->route_three->numeric_departure < current->route_two->numeric_arrival) {
            // Drop this element
            KeepRoute *temp = current;
            if (prev != NULL) {
                prev->next_route = current->next_route;
            } else {
                // If prev is NULL, it means we're dropping the first element
                *top = current->next_route;
            }
            current = current->next_route;
            free(temp); // Free the memory of the dropped element
        } else {
            // Move to the next element
            prev = current;
            current = current->next_route;
        }
    }
}

void remove_trip(KeepRoute **top, KeepRoute *trip_to_remove) {
    KeepRoute *current = *top;
    KeepRoute *prev = NULL;

    // Iterate over the stack
    while (current != NULL) {
        if (current == trip_to_remove) {
            // Found the node to remove
            if (prev == NULL) {
                // The node to remove is at the top of the stack
                *top = current->next_route;
            } else {
                // The node to remove is somewhere else in the stack
                prev->next_route = current->next_route;
            }
            // Free the memory allocated to the current node
            free(current);
            current = NULL;  // Set current to NULL after freeing it
            return;
        }
        prev = current;
        current = current->next_route;
    }
}



//Show KeepStack Functions

void show_keep_route(KeepRoute *top_route) {
    
    //Variables
    KeepRoute *current_route = top_route; //Creating a pointer that moves through the stack of routes
    int count = 1;

    if (current_route == NULL){
        printf("No routes found...\n");
        return;
    }

    while(current_route != NULL){
        

        if(current_route->route != NULL){
            printf("\nTrip [%d]\n", count);
            count++;
            show_individual_route(current_route->route);
        } 

        current_route = current_route->next_route; //Move to the next route
    }
}

void show_keep_route_one_layover(KeepRoute *top_route) {
    
    //Variables
    KeepRoute *current_route = top_route; //Creating a pointer that moves through the stack of routes
    int count = 1;

    if (current_route == NULL){
        printf("No routes found...\n");
        return;
    }

    while(current_route != NULL){
        
        if(current_route->route != NULL){

            printf("\nTrip [%d]:\n", count);
            count++;
            show_individual_route(current_route->route);

        } 

        if(current_route->route_two != NULL){

            show_individual_route(current_route->route_two);

        } 

        current_route = current_route->next_route; //Move to the next route
    }
}

void show_keep_route_two_layover(KeepRoute *top_route) {
    
    //Variables
    KeepRoute *current_route = top_route; //Creating a pointer that moves through the stack of routes
    int count = 1;

    if (current_route == NULL){
        printf("No routes found...\n");
        return;
    }

    while(current_route != NULL){
        
        if(current_route->route != NULL){

            printf("\nTrip [%d]:\n", count);
            count++;
            show_individual_route(current_route->route);

        } 

        if(current_route->route_two != NULL){

            show_individual_route(current_route->route_two);

        } 

        if(current_route->route_three != NULL){

            show_individual_route(current_route->route_three);

        } 

        current_route = current_route->next_route; //Move to the next route
    }
}

void show_individual_route(Route *route) {
    
    printf("Flight : [%s] %s %s ---> %s %s [%.2f]km %s\n", route->tripcode, route->departure_time, route->IATA_source, route->IATA_destiny, route->arrival_time, route->distance, route->airline);
}