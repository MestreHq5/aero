#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include <stdio.h>
#include <stdlib.h>

//Auxialiary definitions


//Structures ***************
typedef struct keep_route{
    Route *route;
    struct keep_route *next_route;
}KeepRoute;


// Function Prototypes
void insertion_sort_keep_route(KeepRoute **top_route);

void free_keep_route(KeepRoute *top_route);

void show_keep_route(KeepRoute *top_route);






#endif