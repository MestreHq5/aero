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
void arguments_error();
FILE *open_file(char *filename, char *mode);
float distance_airports(int *geocoordinatesA, int *geocoordinatesB);


//Aiports Functions **********
StackAirport *init_airports(FILE *fp);
int handle_airport_line(char *line, Airport *airport); //Returns the number of conversions made
void show_airports(StackAirport *top_airport);


//Routes Functions ***********
StackRoute *init_routes(FILE *fp);
char *find_airline(char *line, char *key_airline);
int line_is_empty(char *line);




#endif