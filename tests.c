#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "functions.h"




void find_routes_one_layover(StackRoute *routes, const char *departure, const char *destination, KeepRoute **top_stack) {
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
                 
                    KeepRoute *new_node = (KeepRoute *)malloc(sizeof(KeepRoute));
                    if (new_node == NULL) {
                        fprintf(stderr, "Memory allocation failed while finding routes...\n");
                        exit(1);
                    
                    new_node->route = &(current_route->route);
                    new_node->route_two = &(layover_route->route);
                    new_node->route_three = NULL;

                    // Add the route to the top of the existing stack
                    new_node->next_route = *top_stack;
                    *top_stack = new_node;
                    }

                    layover_route = layover_route->next_route;
                }   
            }
        
        current_route = current_route->next_route;
        }
    }
}

void find_routes_one_layover(StackRoute *routes, const char *departure, const char *destination, KeepRoute **top_stack) {
    StackRoute *current_route = routes;

    // Loop to find flights from the departure airport 
    while (current_route != NULL) {
        // Find the first route that departs from the departure airport
        if (strcmp(current_route->route.IATA_source, departure) == 0) {

            StackRoute *layover_route = routes;

            // Loop to find flights that depart from where the first flight lands and that land at the destination airport
            while (layover_route != NULL) {
                // Find the second route that departs from the airport where the first route lands and arrives at the destination airport
                if (strcmp(layover_route->route.IATA_source, current_route->route.IATA_destiny) == 0 && strcmp(layover_route->route.IATA_destiny, destination) == 0) {
                 
                    KeepRoute *new_node = (KeepRoute *)malloc(sizeof(KeepRoute));
                    if (new_node == NULL) {
                        fprintf(stderr, "Memory allocation failed while finding routes...\n");
                        exit(1);
                    }

                    // Assign the first and second routes to the new KeepRoute node
                    new_node->route = &(current_route->route);
                    new_node->route_two = &(layover_route->route);
                    new_node->route_three = NULL;

                    // Add the new node to the top of the existing stack
                    new_node->next_route = *top_stack;
                    *top_stack = new_node;
                }   

                layover_route = layover_route->next_route;
            }
        }
        
        current_route = current_route->next_route;
    }
}

