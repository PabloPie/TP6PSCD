#ifndef _BUSQUEDA_H
#define _BUSQUEDA_H

#include "Restaurante.h"
#include "Monumento.h"
#include <array>

/*
 * Pre: c != null y n<= que la longitud de c.
 * Post: Devuelve la posicion en la que se encuentre el valor minimo de c en el 
 * rango [0,n], en caso de haber minimos identicos devuelve la posicion del 
 * primero. 
 */
int posDeReemplazo (int* c, int n);

/*
 * Pre: m!= null y n<= que la longitud de m, resultado!= null y n2 = la longitud
 * de resultado, a es un monumento con no más de 5 terminos definidos.
 * Post: Busca en la tabla m monumetos que tengan al menos 1 coincidencia y 
 * rellena la tabla resultado de forma que nos devuelva los Monumentos con mayor
 * indice de proximidad al Monumento a, ordenados de mayor a menor.
 * Devuelve el número de elementos encontrados. 
 */
int busquedaMonumento(const vector<Monumento>& m,array<Monumento,5>& resultado, Monumento& a);

Restaurante BusquedaRestauranteCerc(const Monumento& m,const vector<Restaurante>& r);

#endif
