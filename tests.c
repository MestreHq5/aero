#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "functions.h"


#define DIM_COORD 15
#define EARTH_RADIUS 6371
#define REAL_RADIUS 6381
#define PI 3.1415926535


//gcc tests.c -o tests
//./tests

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
    real_coordinates_source[0] = REAL_RADIUS * cos(source_rad_lat) * cos(source_rad_long);
    real_coordinates_source[1] = REAL_RADIUS * cos(source_rad_lat) * sin(source_rad_long);
    real_coordinates_source[2] = REAL_RADIUS * sin(source_rad_lat);

    //Real coordinates of airport B
    real_coordinates_destiny[0] = REAL_RADIUS * cos(destiny_rad_lat) * cos(destiny_rad_long);
    real_coordinates_destiny[1] = REAL_RADIUS * cos(destiny_rad_lat) * sin(destiny_rad_long);
    real_coordinates_destiny[2] = REAL_RADIUS * sin(destiny_rad_lat);

    //Calculate the angle in radians between the airports
    angle_airports = acos((real_coordinates_source[0] * real_coordinates_destiny[0] + real_coordinates_source[1] * real_coordinates_destiny[1] + real_coordinates_source[2] * real_coordinates_destiny[2]) / (sqrt(real_coordinates_source[0] * real_coordinates_source[0] + real_coordinates_source[1] * real_coordinates_source[1] + real_coordinates_source[2] * real_coordinates_source[2]) * sqrt(real_coordinates_destiny[0] * real_coordinates_destiny[0] + real_coordinates_destiny[1] * real_coordinates_destiny[1] + real_coordinates_destiny[2] * real_coordinates_destiny[2])));

    return angle_airports * EARTH_RADIUS;
}   

int main (void){
    distance_airports(airport,"FCO", "LIS");
}