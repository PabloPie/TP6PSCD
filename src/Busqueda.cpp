/*
 * Autores:
 * Nip:
 * Fecha de entrega: 10/01/2017
 */
#include "../headers/Busqueda.h"

using namespace std;
        
int posDeReemplazo (int* c, int n){
    int pos = 0;
    int min = c[pos];
    int i = 1;
    while(min != 0 && i<n){
        if(min > c[i]){
            min = c[i];
            pos = i;
        }
        i++;
    }
    return pos;
}

int busquedaMonumento(Monumento* m,Monumento* resultado, Monumento& a, int n, int n2){
    int c[n2];
    for(int i=0; i<n2; i++){
        c[i] = 0;
    }
    int numElementos = 0;
    int minCoincidencias = 0;
    int reemplazo = 0;
    int coincidencias;
    int i = 0;
    while(i<n && minCoincidencias!=5){
       coincidencias = m[i].compare(a);
       if(coincidencias>minCoincidencias){
           resultado[reemplazo]=m[i];
           reemplazo = posDeReemplazo(c,n2);
           minCoincidencias = c[reemplazo];
           if(numElementos<5){
               numElementos++;
           }
       }
       i++;
    }
    //Ordenar de mayor a menor
    Monumento aux1;
    int aux2;
    i = 0;
    while(i<numElementos-1){
        if(c[i]<c[i+1]){
            aux1 = resultado[i];
            aux2 = c[i];
            resultado[i] = resultado[i+1];
            c[i] = c[i+1];
            resultado[i+1] = aux1;
            c[i+1] = aux2;
            if(i!=0){
                i--;
            }
        }
        else{
            i++;
        }
    }
}

