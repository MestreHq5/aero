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
    char IATA_source[10], IATA_destiny[10];
    char departure_time[10], arrival_time[10];
    float distance, numeric_departure, numeric_arrival;
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

/*
 * brief: This function checks the arguments inputed and detects several errors of data placement as well as detect if the user is asking for the list of routes or airports.
 * param: int argc: Represents the number of command-line arguments.
 * param: char *argv[]: An array of strings containing the actual command-line arguments.
 * param: StackAirport *airports: A pointer to the stack containing the airports.
 * param: StackRoutes *routes: A pointer to the stack containing the routes.
 * return: void
 */
void handle_arguments(int argc, char *argv[], StackAirport *airports, StackRoute *routes);

/*
 * brief: This function detects the number of layovers that the user is asking for and provides the list of routes possible, accordingly filtered and sorted.
 * param: int argc: Represents the number of command-line arguments.
 * param: char *argv[]: An array of strings containing the actual command-line arguments.
 * param: StackAirport *airports: A pointer to the stack containing the airports.
 * param: StackRoutes *routes: A pointer to the stack containing the routes.
 * return: void
 */
void manage_routes(int argc, char *argv[], StackAirport *airports, StackRoute *routes);






void separate_cases_by_layovers(StackAirport *airports, StackRoute *routes, Airport *airport_source, Airport *airport_destiny, int layover_and_sort[3]);


//General Functions **********

/*
 * brief: Function that is called to help the user by listing the order in which the data should be inputed.
 * param: void
 * return: void
 */
void arguments_error();

/*
 * brief: Function to determine if the number of layovers is valid (0<L<2).
 * param: char *option: A pointer to a string representing the number of layovers.
 * return: If the number of layovers is valid, it will return it, if not, it will end the program and print an error message
 */
int layover_number(char *option);

/*
 * brief: It will open the file and read, write or append on it
 * param: char *filename: A pointer to a string representing the name of the file
 * param: char *mode: A pointer to a string representing the mode in which the file should be opened
 * return: If the number of layovers is valid, it will return it, if not, it will end the program and print an error message
 */
FILE *open_file(char *filename, char *mode); 

/*Function to analyse the sorting according to time
*/

/*
 * brief: Function to analyse the sorting according to time
 * param: char *option: A pointer to a string representing the number of layovers.
 * return: It returns 1 if -TC is inputed
           It returns -1 if -TD is inputed
           It stops the program  and prints an error message if neither -TC nor -TD is inputed
 */
int time_sort_option(char *option);

int distance_sort_option(char *option);



//Aiports Functions **********


/*
 * brief: Function that feeds the airports in the aeroportos.txt file into the airport stack.
 * param: File *fp: A pointer to the airports file.
 * return: It returns a pointer to the top of the stack of airports.
 */
StackAirport *init_airports(FILE *fp);

/*
 * brief: Function that reads an airport line and analyses it to make sure that the line has all of the needed arguments.
 * param: char *line: A pointer to a line.
 * return: It returns the number of sucessful conversions.
 */
int handle_airport_line(char *line, Airport *airport);

/*Function that prints all of the airports in the airport stack*/

/*
 * brief: Function that prints all of the airports in the airport stack.
 * param: StackAirport *top_airport: A pointer to the top of the stack of airports.
 * return: void
 */
void show_airports(StackAirport *top_airport);



/*
 * brief: Function that deallocates the memory allocated for the stack of airports.
 * param: StackAirport *top_airport: A pointer to the top of the stack of airports.
 * return: void
 */
void free_airports(StackAirport *top_airport);

/*
 * brief: Function that receives the airport stack and the IATA code for an airport and returns the pointer to that airport.
 * param: StackAirport *top_airport: A pointer to the top of the stack of airports.
 * param: const char *code_IATA: A pointer to a constant character string containing the IATA code of an airport.
 * return: It returns a pointer to the airport correspondant to the IATA code.
 */
Airport *find_airport_by_IATA(StackAirport *airport, const char *code_IATA);


//Routes Functions ***********

/*
 * brief: Function that feeds the routes in the rotas.txt file as well as their distance into the routes stack.
 * param: File *fp: A pointer to the routes file.
 * param: StackAirport *airport: A pointer to the top of the stack of airports.
 * return: It returns a pointer to the top of the stack of routes.
 */
StackRoute *init_routes(FILE *fp, StackAirport *airports);

/**/
void find_airline(char *line, char *key_airline, char **airline);

/*Function to determine if a line is empty*/
int line_is_empty(char *line);

/**/
void free_routes(StackRoute *top_route);

/*Receives the stack of airports and the route and it calculates the route's distance*/
float distance_airports(StackAirport *airports, Route* route);

void show_routes(StackRoute *top_route);


//Distance Functions **********

/**/
void coordinates_parser(Airport *airport, double coord_vector[2]);

/*Converts the coordinates from latitude and longitude to X,Y and Z*/
void real_coordinates(double source_coord[2], double real_source_coord[3], float radius);


//List Flights Functions**********

//Initializes the top of the stack fields with NULL
void null_init_top(KeepRoute *top_stack);

/*Function to list the routes with 0 layovers*/
void list_direct_flights(StackAirport *airports, StackRoute *routes, Airport *airport_source, Airport *airport_destiny, int *time_and_distance);
/*Function to list the routes with 1 layover*/
void list_one_layover(StackAirport *airports, StackRoute *routes, Airport *airport_source, Airport *airport_destiny, int *time_and_distance); 

/*Function to list the routes with 2 layovers*/
void list_two_layovers(StackAirport *airports, StackRoute *routes, Airport *airport_source, Airport *airport_destiny, int *time_and_distance);


//Free Functions****************

/**/
void free_keep_route(KeepRoute *top_route);

/**/
void free_routes(StackRoute *top_route);


//Algorithms Functions**********

/**/
void bubble_sort_keep_route(KeepRoute **top_route, int tso);

void swap(KeepRoute *a, KeepRoute *b);

float get_departure_time(KeepRoute *node);

float get_arrival_time(KeepRoute *node);

float numeric_time(char *time);

void drop_connecting_concern(KeepRoute **top);

void remove_trip(KeepRoute **top, KeepRoute *trip_to_remove); //remove a trip from the stack that is causing problems in the insert sort

//Distance Sort Functions**********

float get_lesser_distance(KeepRoute *top);

void drop_long_trips(KeepRoute **top);


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
