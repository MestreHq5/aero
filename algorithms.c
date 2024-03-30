#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "functions.h"
#include "algorithms.h"



//Functions **********

//Find Possible Flights for a given route (No layover)
KeepRoute *find_routes_no_layover(StackRoute *routes, const char *airport_source, const char *airport_destiny){

    KeepRoute *top = NULL;
    StackRoute *current = routes;

    while (current != NULL){
        if (strcmp(current->route.IATA_source, airport_source) == 0 && strcmp(current->route.IATA_destiny, airport_destiny) == 0){
            
            //A flight was found -> will be stored in a stack 
            KeepRoute *new_node = (KeepRoute *)malloc(sizeof(KeepRoute));
            if (new_node == NULL){
                fprintf(stderr, "Memory allocation failed while finding routes...\n");
                exit(1);
            }

            //Add the route to the stack and update the top pointer
            new_node->route = &current->route;
            new_node->next_route = top;
            top = new_node;
        }

        //Move to the next route in the Global Stack of Routes
        current = current->next_route;
    }

    return top;
}

//Free the memory allocated for the stack of routes
void free_keep_route(KeepRoute *top_route){

    while (top_route != NULL){
        KeepRoute *temp = top_route;
        top_route = top_route->next_route;
        free(temp);
    }
}

//Order the stack of routes by distance using Insertion Sort
void insertion_sort_keep_route(KeepRoute **top){
    
    //Checks if the stack is empty or has only one element (already sorted)
    if (*top == NULL || (*top)->next_route == NULL) {
        return;
    }

    KeepRoute *sortedStack = NULL;
    KeepRoute *current = *top;

    while (current != NULL) {
        KeepRoute *next = current->next_route;
        if (sortedStack == NULL || sortedStack->route->distance >= current->route->distance) {
            current->next_route = sortedStack;
            sortedStack = current;
        } else {
            KeepRoute *ptr = sortedStack;
            while (ptr->next_route != NULL && ptr->next_route->route->distance < current->route->distance) {
                ptr = ptr->next_route;
            }
            current->next_route = ptr->next_route;
            ptr->next_route = current;
        }
        current = next;
    }

    *top = sortedStack; // Update the top pointer to point to the sorted stack
}



/*
StackRoute* select_flights_between_airports(StackRoute* route, const char* airport_A, const char* airport_B) {
    StackRoute* route_filtered = NULL; // Initialize the new stack

    // Iterate through the original stack
    StackRoute* current = route;
    while (current != NULL) {
        // Check if the source and destination airports match A and B
        if (strcmp(current->route.IATA_source, airport_A) == 0 && strcmp(current->route.IATA_destiny, airport_B) == 0) {
            // Create a new node for the matching route
            StackRoute* new_node = (StackRoute*)malloc(sizeof(StackRoute));
            if (new_node == NULL) {
                fprintf(stderr, "Memory allocation failed\n");
                // Handle memory allocation failure
                return 1;
            }
            // Copy the route data
            memcpy(&new_node->route, &current->route, sizeof(Route));
            new_node->next_route = NULL; // Set the next pointer to NULL for now

            // Add the new node to the new stack
            if (route_filtered == NULL) {
                route_filtered = new_node; // If it's the first node, set it as the head of the new stack
            } else {
                // Otherwise, find the last node in the new stack and append the new node
                StackRoute* last_node = route_filtered;
                while (last_node->next_route != NULL) {
                    last_node = last_node->next_route;
                }
                last_node->next_route = new_node;
            }
        }
        // Move to the next route in the original stack
        current = current->next_route;
    }

    return route_filtered;
}

*/

// Function to find routes with one layover
void find_routes_one_layover(StackRoute *routes, const char *departure, const char *destination) {
    StackRoute *current_route = routes;
    while (current_route != NULL) {
        if (strcmp(current_route->route.IATA_source, departure) == 0) {
            StackRoute *layover_route = routes;
            while (layover_route != NULL) {
                if (strcmp(layover_route->route.IATA_source, current_route->route.IATA_destiny) == 0 &&
                    strcmp(layover_route->route.IATA_destiny, destination) == 0) {
                    printf("Found route: %s -> %s (%s) -> %s\n", departure,
                           current_route->route.IATA_destiny, current_route->route.arrival_time, destination);
                }
                layover_route = layover_route->next_route;
            }
        }
        current_route = current_route->next_route;
    }
}


