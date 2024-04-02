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
 * brief: This function serves as the entry point for managing routes based on command-line arguments. 
 * param: int argc: Represents the number of command-line arguments.
 * param: char *argv[]: An array of strings containing the actual command-line arguments.
 * param: StackAirport *airports: A pointer to the stack containing the airports.
 * param: StackRoutes *routes: A pointer to the stack containing the routes.
 * return: void
 */
void manage_routes(int argc, char *argv[], StackAirport *airports, StackRoute *routes);

/*
 * brief: This function serves as the entry point for managing routes based on command-line arguments. 
 * param: int argc: Represents the number of command-line arguments.
 * param: char *argv[]: An array of strings containing the actual command-line arguments.
 * param: StackAirport *airports: A pointer to the stack containing the airports.
 * param: StackRoutes *routes: A pointer to the stack containing the routes.
 * return: void
 */
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


/*
 * brief: Function to analyse the sorting according to time
 * param: char *option: A pointer to a string representing the number of layovers.
 * return: It returns 1 if -TC is inputed
           It returns -1 if -TD is inputed
           It stops the program and prints an error message if neither -TC nor -TD is inputed
 */
int time_sort_option(char *option);

/*
 * brief: Function that interprets the user-provided sorting option for distance and validate its correctness.
 * param: char *option: A pointer to a string representing the sorting option for distance.
 * return: It returns 1 if the option is -D.
           It stops the program and prints an error message if the option in not -D.
 */
int distance_sort_option(char *option);

/*
 * brief: Function that converts a time string in the format "HH:MM" to a numeric representation.
 * param: char *time: A pointer to the string representing the time in the format "HH:MM".
 * return: It returns the time as a float number
 */
float numeric_time(char *time);


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

/*
 * brief: Function that extracts the airline name from a line of text based on a specific keyword.
 * param: char *line: A pointer to a string containing the airline name.
 * param: char *key_airline: A pointer to a string containing the keyword that marks the beginning of the airline name.
 * param: char **airline: A pointer to a pointer to a string where the extracted airline name will be stored.
 * return: void.
 */
void find_airline(char *line, char *key_airline, char **airline);

/*
 * brief: Function to determine if a line is empty.
 * param: char *line: A pointer to a string containing a line.
 * return: It returns 1 if the line is empty and returns 0 if it's not.
 */
int line_is_empty(char *line);

/*
 * brief: Function that deallocates the memory allocated for the stack of routes.
 * param: StackRoutes *top_route: A pointer to the top of the stack of routes.
 * return: void
 */
void free_routes(StackRoute *top_route);

/*
 * brief: Receives the stack of airports and the route and it calculates the route's distance.
 * param: StackAirport *airports: A pointer to the top of the stack of airports.
 * param: Route* route: A pointer to a specific route
 * return: It returns the distance in between the two aiports.
 */
float distance_airports(StackAirport *airports, Route* route);

/*
 * brief: Function that prints all of the routes in the routes stack.
 * param: StackRoute *top_route: A pointer to the top of the stack of routes.
 * return: void
 */
void show_routes(StackRoute *top_route);


//Distance Functions **********


/*
 * brief: The purpose of this function is to parse latitude and longitude information stored in the Airport structure and convert it into decimal degree format, adjusted for the appropriate direction.
 * param: Airport *airport: A pointer to the Airport structure containing latitude and longitude information.
 * param: double coord_vector[2]: An array with the latitude and longitude of an airport.
 * return: void
 */
void coordinates_parser(Airport *airport, double coord_vector[2]);

/*
 * brief: Converts the coordinates from latitude and longitude to X,Y and Z.
 * param: double source_coord[2]: An array with the latitude and longitude of an airport.
 * param: double real_source_coord[3]: An array with the x,y and z coordinates of that airport.
 * param: float radius: The radius of the earth plus the altitude that the plane flies at.
 * return: void
 */
void real_coordinates(double source_coord[2], double real_source_coord[3], float radius);


//List Flights Functions**********

/*
 * brief: Initializes the top of the stack fields with NULL
 * param: KeepRoute *top_stack: a pointer to a KeepRoute structure.
 * return: void
 */
void null_init_top(KeepRoute *top_stack);

/*
 * brief: Function to list the routes with 0 layovers.
 * param: StackAirport *airports: A pointer to the top of the stack of airports. 
 * param: StackRoute *routes: A pointer to the top of the stack of routes. 
 * param: Airport *airport_source: A pointer to the source airport from which the flights originate.
 * param: Airport *aiport_destiny: A pointer to the destination airport to which the flights are headed.
 * param: int *time_and_distance: An array containing information about sorting preferences.
 * return: void
 */
void list_direct_flights(StackAirport *airports, StackRoute *routes, Airport *airport_source, Airport *airport_destiny, int *time_and_distance);

/*
 * brief: Function to list the routes with 1 layover.
 * param: StackAirport *airports: A pointer to the top of the stack of airports. 
 * param: StackRoute *routes: A pointer to the top of the stack of routes. 
 * param: Airport *airport_source: A pointer to the source airport from which the flights originate.
 * param: Airport *aiport_destiny: A pointer to the destination airport to which the flights are headed.
 * param: int *time_and_distance: An array containing information about sorting preferences.
 * return: void
 */
void list_one_layover(StackAirport *airports, StackRoute *routes, Airport *airport_source, Airport *airport_destiny, int *time_and_distance); 

/*
 * brief: Function to list the routes with 2 layovers.
 * param: StackAirport *airports: A pointer to the top of the stack of airports. 
 * param: StackRoute *routes: A pointer to the top of the stack of routes. 
 * param: Airport *airport_source: A pointer to the source airport from which the flights originate.
 * param: Airport *aiport_destiny: A pointer to the destination airport to which the flights are headed.
 * param: int *time_and_distance: An array containing information about sorting preferences.
 * return: void
 */
void list_two_layovers(StackAirport *airports, StackRoute *routes, Airport *airport_source, Airport *airport_destiny, int *time_and_distance);


//Free Functions****************

/*
 * brief: Function that releases the memory allocated for a linked list of KeepRoute structures. 
 * param: KeepRoute *top_route: A pointer to the top of the linked list of KeepRoute structures.
 * return: void
 */
void free_keep_route(KeepRoute *top_route);

/*
 * brief: Function that releases the memory allocated for a linked list of StackRoute structures. 
 * param: StackRoute *top_route: A pointer to the top of the linked list of StackRoute structures.
 * return: void
 */
void free_routes(StackRoute *top_route);


//Algorithms Functions**********

/*
 * brief: Function that sorts a stack of KeepRoute structures based on departure and arrival times. 
 * param: KeepRoute **top_route: A pointer to a pointer to the top of the stack of KeepRoute structures.
 * param: int tso: A multiplier to determine the sorting order based on time.
 * return: void.
 */
void bubble_sort_keep_route(KeepRoute **top_route, int tso);

/*
 * brief: Function that swaps the contents of two KeepRoute structures, effectively exchanging the flight routes they represent. 
 * param: KeepRoute *route_a: A pointer to the first KeepRoute structure to be swapped.
 * param: KeepRoute *route_b: A pointer to the second KeepRoute structure to be swapped.
 * return: void.
 */
void swap(KeepRoute *route_a, KeepRoute *route_b);

/*
 * brief: Function that provides a convenient way to retrieve the departure time of the first flight in a trip represented by a KeepRoute structure.
 * param: KeepRoute *node: A pointer to a KeepRoute structure representing a trip consisting of one or more flights.
 * return: Returns the departing time of the first flight.
 */
float get_departure_time(KeepRoute *node);

/*
 * brief: Function that provides a convenient way to retrieve the arrival time of the last flight in a trip represented by a KeepRoute structure.
 * param: KeepRoute *node: A pointer to a KeepRoute structure representing a trip consisting of one or more flights.
 * return: Returns the arrival time of the last flight.
 */
float get_arrival_time(KeepRoute *node);


//Distance Sort Functions**********

/*
 * brief: Function that finds the shortest total distance among all trips stored in a stack of KeepRoute structures. 
 * param: KeepRoute *top: A pointer to the top of the stack of KeepRoute structures.
 * return: It returns the shortest distance
 */
float get_shortest_distance(KeepRoute *top);

/*
 * brief: Function that drops trips from the stack of KeepRoute structures that have a total distance greater than the shortest distance among all trips. 
 * param: KeepRoute **top: A pointer to a pointer to the top of the stack of KeepRoute structures.
 * return: void
 */
void drop_long_trips(KeepRoute **top);

/*
 * brief: Function that calculate the total distance covered in a trip represented by a KeepRoute structure. 
 * param: KeepRoute *trip: A pointer to a KeepRoute structure representing a trip.
 * return: It return the total disatnce traveled.
 */
float sum_trip_distances(KeepRoute *trip);


//Functions to find routes with 0-2 layovers**********

/*
 * brief: Function that finds routes with 0 layovers.
 * param: StackRoute *routes: A pointer to the top of the stack of routes.
 * param: const char *airport_source: A string representing the IATA code of the source airport.
 * param: const char *airport_destiny: A string representing the IATA code of the destination airport.
 * param: KeepRoute **top_stack: A pointer to a pointer to the top of the stack of KeepRoute structures. 
 * return: void
 */
void find_routes_no_layover(StackRoute *routes, const char *airport_source, const char *airport_destiny, KeepRoute **top_stack);

/*
 * brief: Function that finds routes with 1 layover.
 * param: StackRoute *routes: A pointer to the top of the stack of routes.
 * param: const char *airport_source: A string representing the IATA code of the source airport.
 * param: const char *airport_destiny: A string representing the IATA code of the destination airport.
 * param: KeepRoute **top_stack: A pointer to a pointer to the top of the stack of KeepRoute structures. 
 * return: void
 */
void find_routes_one_layover(StackRoute *routes, const char *departure, const char *destination, KeepRoute **top_stack);

/*
 * brief: Function that finds routes with 2 layovers.
 * param: StackRoute *routes: A pointer to the top of the stack of routes.
 * param: const char *airport_source: A string representing the IATA code of the source airport.
 * param: const char *airport_destiny: A string representing the IATA code of the destination airport.
 * param: KeepRoute **top_stack: A pointer to a pointer to the top of the stack of KeepRoute structures. 
 * return: void
 */
void find_routes_two_layover(StackRoute *routes, const char *departure, const char *destination, KeepRoute **top_stack);


//Connecting Flights Concern*************

/*
 * brief: Function that drops trips from the stack of KeepRoute structures if there are concerns with the connecting flights. 
 * param: KeepRoute **top: A pointer to a pointer to the top of the stack of KeepRoute structures.
 * return: void
 */
void drop_connecting_concern(KeepRoute **top);

/*
 * brief: Function that removes a specific trip from the stack of KeepRoute structures.
 * param: KeepRoute **top: A pointer to a pointer to the top of the stack of KeepRoute structures.
 * param: KeepRoute *trip_to_remove: A pointer to the specific trip that needs to be removed from the stack.
 * return: void
 */
void remove_trip(KeepRoute **top, KeepRoute *trip_to_remove);


//Show KeepStack Functions****************

/*
 * brief: Function that visually presents the trips stored in the stack of KeepRoute structures. 
 * param: KeepRoute *top_route: A pointer to the top of the stack of KeepRoute structures.
 * return: void
 */
void show_keep_route(KeepRoute *top_route);

/*
 * brief: Function that visually presents the trips with one layover in a clear and organized manner. 
 * param: KeepRoute *top_route: A pointer to the top of the stack of KeepRoute structures.
 * return: void
 */
void show_keep_route_one_layover(KeepRoute *top_route);

/*
 * brief: Function that visually presents the trips with two layovers in a clear and organized manner. 
 * param: KeepRoute *top_route: A pointer to the top of the stack of KeepRoute structures.
 * return: void
 */
void show_keep_route_two_layover(KeepRoute *top_route);

/*
 * brief: Function that visually presents the details of an individual route. 
 * param: KeepRoute *top_route: A pointer to the top of the stack of KeepRoute structures.
 * return: void
 */
void show_individual_route(Route *route);

#endif
