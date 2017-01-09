/*
 * Autores:
 * Nip:
 * Fecha de entrega: 10/01/2017
 */
#include "../headers/Busqueda.h"
#include <math.h>

using namespace std;

int posDeReemplazo(int* c, int n) {
	int pos = 0;
	int min = c[pos];
	int i = 1;
	while (min != 0 && i < n) {
		if (min > c[i]) {
			min = c[i];
			pos = i;
		}
		i++;
	}
	return pos;
}
//TODO: cambiar algoritmo de ordenacion por std::sort y arreglar parametros de entrada
int busquedaMonumento(const vector<Monumento>& m, vector<Monumento> resultado, Monumento& a, int n,
		int n2) {
	int c[n2];
	for (int i = 0; i < n2; i++) {
		c[i] = 0;
	}
	int numElementos = 0;
	int minCoincidencias = 0;
	int reemplazo = 0;
	int coincidencias;
	int i = 0;
	while (i < n && minCoincidencias != 5) {
		coincidencias = m[i].compare(a);
		if (coincidencias > minCoincidencias) {
			resultado[reemplazo] = m[i];
			reemplazo = posDeReemplazo(c, n2);
			minCoincidencias = c[reemplazo];
			if (numElementos < 5) {
				numElementos++;
			}
		}
		i++;
	}
	//Ordenar de mayor a menor
	Monumento aux1;
	int aux2;
	i = 0;
	while (i < numElementos - 1) {
		if (c[i] < c[i + 1]) {
			aux1 = resultado[i];
			aux2 = c[i];
			resultado[i] = resultado[i + 1];
			c[i] = c[i + 1];
			resultado[i + 1] = aux1;
			c[i + 1] = aux2;
			if (i != 0) {
				i--;
			}
		} else {
			i++;
		}
	}
        return numElementos;
}

//Función que calcula la distancia entre dos coordenadas del plano cartesiano
double Calculo(double North1, double East1, double North2, double East2) {
	double result = pow((North1 - North2), 2) + pow((East1 - East2), 2);
	return sqrt(result);
}

//fórmula de búsqueda de restaurantes en función de la cercanía a un punto.
Restaurante BusquedaRestauranteCerc(const Monumento& m,
		const vector<Restaurante>& r) {

	//Sacar coordenadas del Monumento
	double latRest = 90;
	double lonRest = 180;
	double lat = m.getLat();	// Extraer de tabla de datos
	double lon = m.getLon();	// Extraer de tabal de datos
	Restaurante res;
	//Sacar coordenadas de los restaurantes
	for (auto a : r) {
		double auxLat = a.getLat();	// Extraer de tabla de datos
		double auxLon = a.getLon();	// Extraer de tabla de datos
		//Si es menor distancia: actualizar
		if (Calculo(lat, lon, latRest, lonRest)
				> Calculo(lat, lon, auxLat, auxLon)) {
			latRest = auxLat;
			lonRest = auxLon;
			res = a;
		}
	}
	return res;
	/*Parte de otra funci�n
	 string cmd("firefox https://www.google.com/maps/place/"+to_string(latRest)+","+to_string(lonRest));
	 //"system" requiere un "char *", que es lo que nos da el operador "c_str()" de la clase string de C++
	 int resCall = system(cmd.c_str());
	 if(resCall != 0){
	 cerr << "Ha habido algún problema al abrir el navegador" << endl;
	 return 1;
	 }else{
	 return 0;
	 }*/
}

