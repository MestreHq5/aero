#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "functions.h"
#include "algorithms.h"



//Functions **********

//Functions Related to the list of direct flights  
void list_direct_flights(StackAirport *airports, StackRoute *routes, Airport *airport_source, Airport *airport_destiny, int time_sort_option) {
    
    //Create a stack of routes to keep (may be sorted or not ---> depends on time_sort_option)
    KeepRoute *top = (KeepRoute *)malloc(sizeof(KeepRoute));
    if (top == NULL) {
        printf("Memory allocation failed while finding routes...\n");
        exit(1);
    }

    //Initialize the stack of keep routes
    find_routes_no_layover(routes, airport_source->IATA, airport_destiny->IATA, &top);

    //Check if sorted is needed
    if (time_sort_option == -1) {
        show_keep_route(top);
    } else if( time_sort_option == 1){
        insertion_sort_keep_route(&top);
        show_keep_route(top);
    }
}


void show_keep_route(KeepRoute *top_route) {
    //Creating a pointer that moves through the stack of routes
    KeepRoute *current = top_route;
   
    while (current != NULL) {
        printf("Route: %s -> %s\n", current->route->IATA_source, current->route->IATA_destiny);
        
        //Move pointer to the next route
        current = current->next_route;
    }
}

void find_routes_no_layover(StackRoute *routes, const char *airport_source, const char *airport_destiny, KeepRoute **top_stack) {
    StackRoute *current = routes;

    while (current != NULL) {
        if (strcmp(current->route.IATA_source, airport_source) == 0 && strcmp(current->route.IATA_destiny, airport_destiny) == 0) {
            // A flight was found -> will be added to the existing stack 

            // Allocate memory for a new KeepRoute node
            KeepRoute *new_node = (KeepRoute *)malloc(sizeof(KeepRoute));
            if (new_node == NULL) {
                fprintf(stderr, "Memory allocation failed while finding routes...\n");
                exit(1);
            }

            // Allocate memory for a new Route structure and copy the current route's data into it
            new_node->route = (Route *)malloc(sizeof(Route));
            if (new_node->route == NULL) {
                fprintf(stderr, "Memory allocation failed while finding routes...\n");
                exit(1);
            }
            // Copy the contents of the current route into the newly allocated Route structure
            *(new_node->route) = current->route;

            // Add the route to the top of the existing stack
            new_node->next_route = *top_stack;
            *top_stack = new_node;
        }

        // Move to the next route in the Global Stack of Routes
        current = current->next_route;
    }
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


// Function to find routes with one layover
void find_routes_one_layover(StackRoute *routes, const char *departure, const char *destination) {
    StackRoute *current_route = routes;
    while (current_route != NULL) {
        if (strcmp(current_route->route.IATA_source, departure) == 0) {
            StackRoute *layover_route = routes;
            while (layover_route != NULL) {
                if (strcmp(layover_route->route.IATA_source, current_route->route.IATA_destiny) == 0 && strcmp(layover_route->route.IATA_destiny, destination) == 0) {
                 
            
                }
                layover_route = layover_route->next_route;
            }
        }
        current_route = current_route->next_route;
    }
}


