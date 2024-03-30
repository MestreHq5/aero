#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>

//Auxialiary definitions
#define DIM_NAME 40
#define DIM_COORD 15

//Structures ***************

//Airports Related
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


// Function prototypes

//General Functions **********

/*Function that is called to help the user defining what he wants from the program.
It list the order in which the user should input the data*/
void arguments_error();

/**/
int arguments_number();
FILE *open_file(char *filename, char *mode);
void handle_arguments(int argc, char *argv[], StackAirport *airports, StackRoute *routes);



//Aiports Functions **********
StackAirport *init_airports(FILE *fp);
int handle_airport_line(char *line, Airport *airport);
void show_airports(StackAirport *top_airport);
char *find_airport(StackAirport *airport, const char *targetIATA);
void free_airports(StackAirport *top_airport);
Airport *find_airport_by_IATA(StackAirport *airport, const char *code_IATA);


//Routes Functions ***********
StackRoute *init_routes(FILE *fp, StackAirport *airports);
char *find_airline(char *line, char *key_airline);
int line_is_empty(char *line);
void free_routes(StackRoute *top_route);
float distance_airports(StackAirport *airports, Route* route);


//Distance Functions **********
void coordinates_parser(Airport *airport, double coord_vector[2]);
void real_coordinates(double source_coord[3], double real_source_coord[3], float radius);
char* find_lat_long(char *info);
double* calculate_real_coordinates(double rad_lat, double rad_long);
//float distance_airports(StackAirport *airport, char *IATA_source, char *IATA_destiny);



#endif
