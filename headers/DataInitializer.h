#ifndef DATAINITIALIZER_H
#define DATAINITIALIZER_H

#include "../headers/json.hpp"
#include "../headers/Monumento.h"
#include "../headers/Restaurante.h"
#include <vector>

class DataInitializer{
public:
	static void obtenerMonumentos(vector<Monumento> &monumentos );
	static void obtenerRestaurantes(vector<Restaurante> &restaurantes);
};

#endif
