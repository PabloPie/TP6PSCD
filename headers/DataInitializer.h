#ifndef _DATA_INITIALIZER_H
#define _DATA_INITIALIZER_H

#include "../headers/json.hpp"
#include "../headers/LatLong-UTMconversion.h"
#include <string>
#include <fstream>

using namespace std;
using json = nlohmann::json;

//Declaración funciones exclusivas al módulo
json leer_json(string fichero);
template<class T> void json_a_objeto(json &s, T &obj);
template<class T> void obtener_info(T &obj, json &info);
template<class T> void obtener_loc(T &obj, json &loc);

//Añade al vector de T que recibe como parametro los T del
//fichero file_monumentos que debe seguir el formato json del ayuntamiento de Zaragoza
template<class T> int obtenerDatos(vector<T> &v, string fichero) {
	json j = leer_json(fichero);
	json s = j["/features"_json_pointer]; //lista de los monumentos
	//Recorremos lista de monumentos y los añadimos al vector
	for (json::iterator it = s.begin(); it != s.end(); ++it) {
		T obj;
		json_a_objeto(it.value(), obj);
		v.push_back(obj);
	}
	auto last = std::unique(v.begin(), v.end());
	v.erase(last, v.end());
	return v.size();
}

//Asigna al monumento o restaurante latitud y longitud
template<class T> void obtener_loc(T &obj, json &loc) {
	auto coord = loc["/coordinates"_json_pointer];	//Par de coordenadas
	char UTMZoneZgza[4] = "30T";
	double lat;
	double lon;
	UTMtoLL(23, coord[1], coord[0], UTMZoneZgza, lat, lon);
	obj.setLat(lat);
	obj.setLon(lon);
}

//Asigna al monumento o restaurante obj los 6 datos de info
template<class T> void obtener_info(T &obj, json &info) {
	obj.setTitle(info["/title"_json_pointer]);
	obj.setURL(info["/link"_json_pointer]);
	obj.setDescripcion(info["/description"_json_pointer]);
	obj.setCategoria(info["/category"_json_pointer]);
	obj.setDate(info["/date"_json_pointer]);
	obj.setIcon(info["/icon"_json_pointer]);

}

//Recibe el json de un monumento y añade a los campos de obj la info y localizacion
template<class T> void json_a_objeto(json &s, T &obj) {
	json j_localizacion = s["/geometry"_json_pointer]; // {"coordinates":[676105.25,4610179.46],"type":"Point"}
	json j_info = s["/properties"_json_pointer];
	string info[6];
	obtener_info(obj, j_info);
	obtener_loc(obj, j_localizacion);
}
//Lee un fichero json y crea el objeto asociado
json leer_json(string fichero) {
	std::ifstream ifs(fichero);
	json j(ifs);
	return j;
}

#endif
