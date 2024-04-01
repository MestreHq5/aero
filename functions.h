#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>

//Auxialiary definitions
#define DIM_NAME 40
#define DIM_COORD 20

//Structures ***************

//Airports Related

/*Stack to store the information of each airport*/
typedef struct airport {
    char ICAO[5];
    char IATA[4];
    char latitude[DIM_COORD];
    char longitude[DIM_COORD];
    char city[DIM_NAME];
    int timezone;

} Airport;

typedef struct stack_airport {
    Airport airport;
    struct stack_airport *next_airport;
} StackAirport;

//Routes Related

/*Stack to store the information of each route*/
typedef struct route{
    char airline[DIM_NAME];
    char tripcode[10];
    char IATA_source[4], IATA_destiny[4];
    char departure_time[6], arrival_time[6];
    float distance;
} Route;

typedef struct stack_routes {
    Route route;
    struct stack_routes *next_route;
} StackRoute;


typedef struct keep_route{
    Route *route, *route_two, *route_three;
    struct keep_route *next_route;
}KeepRoute;




// Function prototypes

//Main function *************

/*This function checks the arguments inputed and detects several errors of data placement as well as detect if the user is asking for the routes or for the airports. 
If it inputs a route, it will pass it to the function manage routes*/
void handle_arguments(int argc, char *argv[], StackAirport *airports, StackRoute *routes);

/*This function detects the number of layovers that the user is asking for and provides the list of routes possible, accordingly sorted*/
void manage_routes(int argc, char *argv[], StackAirport *airports, StackRoute *routes);


//General Functions **********

/*Function that is called to help the user defining what he wants from the program.
It list the order in which the user should input the data*/
void arguments_error();

/*Function to determine if the number of layovers is valid (0<L<2)
If it is, it outputs the number of layovers,
If it's not, it will print an error message and stop the program*/
int layover_number(char *option);

/*It will open the file and read, write or append on it*/
FILE *open_file(char *filename, char *mode);

/*Function to analyse the sorting according to time
It returns 0 to sort from the earliest flight to the latest flight
It returns 1 to sort from the latest flight to the earliest flight
It stops the program if neither -TC nor -TD is inputed*/
int time_sort_option(char *option);


//Aiports Functions **********

/*Function that feeds the airports in the aeroportos.txt file into the airport stack*/
StackAirport *init_airports(FILE *fp);

/**/
int handle_airport_line(char *line, Airport *airport);

/*Function that prints all of the airports in the airport stack*/
void show_airports(StackAirport *top_airport);

/**/
void free_airports(StackAirport *top_airport);

/*Function that receives the airport stack and the IATA code for an airport and returns the pointer to that airport*/
Airport *find_airport_by_IATA(StackAirport *airport, const char *code_IATA);


//Routes Functions ***********

/*Function that feeds the airports in the rotas.txt file into the routes stack*/
StackRoute *init_routes(FILE *fp, StackAirport *airports);

/**/
void find_airline(char *line, char *key_airline, char **airline);

/*Function to determine if a line is empty*/
int line_is_empty(char *line);

/**/
void free_routes(StackRoute *top_route);

/*Receives the stack of airports and the route and it calculates the route's distance*/
float distance_airports(StackAirport *airports, Route* route);


//Distance Functions **********

/**/
void coordinates_parser(Airport *airport, double coord_vector[2]);

/*Converts the coordinates from latitude and longitude to X,Y and Z*/
void real_coordinates(double source_coord[2], double real_source_coord[3], float radius);


//List Flights Functions**********

//Initializes the top of the stack fields with NULL
void null_init_top(KeepRoute *top_stack);

/*Function to list the routes with 0 layovers*/
void list_direct_flights(StackAirport *airports, StackRoute *routes, Airport *airport_source, Airport *airport_destiny, int time_sort_option);

/*Function to list the routes with 1 layover*/
void list_one_layover(StackAirport *airports, StackRoute *routes, Airport *airport_source, Airport *airport_destiny, int time_sort_option); 

/*Function to list the routes with 2 layovers*/
void list_two_layovers(StackAirport *airports, StackRoute *routes, Airport *airport_source, Airport *airport_destiny, int time_sort_option);


//Free Functions****************

/**/
void free_keep_route(KeepRoute *top_route);

/**/
void free_routes(StackRoute *top_route);


//Algorithms Functions**********

/**/
void insertion_sort_keep_route(KeepRoute **top_route, int time_sort_option);

float numeric_time(const char *time);



//Functions to find routes with 0-2 layovers**********

/*Find routes with 0 layovers*/
void find_routes_no_layover(StackRoute *routes, const char *airport_source, const char *airport_destiny, KeepRoute **top_stack);

/*Find routes with 1 layover*/
void find_routes_one_layover(StackRoute *routes, const char *departure, const char *destination, KeepRoute **top_stack);

/*Find routes with 2 layovers*/
void find_routes_two_layover(StackRoute *routes, const char *departure, const char *destination, KeepRoute **top_stack);


//Show KeepStack Functions****************
/**/
void show_keep_route(KeepRoute *top_route);

/**/
void show_keep_route_one_layover(KeepRoute *top_route);

/**/
void show_keep_route_two_layover(KeepRoute *top_route);

/**/
void show_individual_route(Route *route);

#endif
