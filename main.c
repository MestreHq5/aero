#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "functions.h"

int main(int argc, char *argv[]) {

    FILE* fprotas, *fpaeroportos;

    fprotas = fopen("rotas.txt", "r");
    fpaeroportos = fopen("aeroportos.txt", "r");

    printf("\n"); // This line separates the execution to the outuput of the program

    if (argc < 2) {
        printf("Too few arguments: Execution failed...\n");
        arguments_error();
        
        fclose(fprotas);
        fclose(fpaeroportos);
        return 1;
    }

    if(argc > 2){
       
    }


    
    fclose(fprotas);
    fclose(fpaeroportos);
    return 0;
}

//Calculador da distância entre o aeroporto A e B dadas as suas latitudes e longitudes respetivamentes
float distancecalculator (int geocoordinatesA[2], int geocoordinatesB[2]){
int Earthradius =6371// Depois meter isto como #define
int Realradius=Earthradius+10;
int realcoordinatesA[3]; // x y z
int realcoordinatesB[3]; // x y z
int angleinbetweenairports;

//Coordenadas reais do aeroporto A
realcoordinatesA[0]=Realradius*cos(geocoordinatesA[0])*cos(geocoordinatesA[1]);
realcoordinatesA[1]=Realradius*cos(geocoordinatesA[0])*sin(geocoordinatesA[1]);
realcoordinatesA[2]=Realradius*sin(geocoordinatesA[0]);

//Coordenadas reais do aeroporto B
realcoordinatesB[0]=Realradius*cos(geocoordinatesB[0])*cos(geocoordinatesB[1]);
realcoordinatesB[1]=Realradius*cos(geocoordinatesB[0])*sin(geocoordinatesB[1]);
realcoordinatesB[2]=Realradius*sin(geocoordinatesB[0]);

//Calcular o ângulo em radianos entre os aeroportos
angleinbetweenairports = acos((realcoordinatesA[0]*realcoordinatesB[0] + realcoordinatesA[1]*realcoordinatesB[1] + realcoordinatesA[2]*realcoordinatesB[2])/(sqrt(realcoordinatesA[0]*realcoordinatesA[0] + realcoordinatesA[1]*realcoordinatesA[1] + realcoordinatesA[2]*realcoordinatesA[2])*sqrt(realcoordinatesB[0]*realcoordinatesB[0] + realcoordinatesB[1]*re    alcoordinatesB[1] + realcoordinatesB[2]*realcoordinatesB[2])));
}
