#include "functions.h"
#include <stdio.h>


//Functions
void arguments_error() {
    printf("\n-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");
    printf("\nThe list below explains possible commands for the program:\n");
    printf("\nList All Airports: ./rotas2024 -aeroportos");
    printf("\nList All Flights: ./rotas2024 -voos");
    printf("\nList All Direct Flights LIS --> FCO (example): ./rotas2024 LIS FCO -L 0");
    printf("\nList All Direct Flights LIS --> FCO by departure time + (example): ./rotas2024 LIS FCO -L 0 -TC");
    printf("\nList All Direct Flights LIS --> FCO by departure time - (example): ./rotas2024 LIS FCO -L 0 -TD");
    printf("\nList All Flights LIS --> FCO time- (example): ./rotas2024 LIS FCO -L 2 -TD -L");

    printf("\n");
}

