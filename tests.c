#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "functions.h"



//gcc tests.c -o tests
//./tests

//Finds the airport given and returns the information of that airport
find_airport(char *IATA){
    for (int i = airport->top; i >= 0; i--) {
        if (strcmp(airport->data[i].IATA_source, IATA_source) == 0) {
        airport->data[i]=sscanf(line, "%s %s %d %d %d %c %d %d %d %c %s %d", airport->ICAO, airport->IATA, &source_lat_deg, &source_lat_min, &source_lat_sec, &source_lat_dir, &source_long_deg, &source_long_min, &source_long_sec, &source_long_dir, airport->city, &airport->timezone);
        }
    }
    printf("%d %d %d %c %d %d %d %c %d %d %d %c %d %d %d %c", source_lat_deg, source_lat_min, source_lat_sec, source_lat_dir, source_long_deg, source_long_min, source_long_sec, source_long_dir, destiny_lat_deg, destiny_lat_min, destiny_lat_sec, destiny_lat_dir, destiny_long_deg, destiny_long_min, destiny_long_sec, destiny_long_dir);
}

int main (void){
    find_airport(LIS)
 }