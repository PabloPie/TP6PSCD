#include "../headers/Restaurante.h"




double Restaurante::getLat() const {
	return lat;
};

void Restaurante::setLat(double lat) {
	this->lat = lat;
};

double Restaurante::getLon() const {
	return lon;
};

void Restaurante::setLon(double lon) {
	this->lon = lon;
};

const string& Restaurante::getNombre() const {
	return nombre;
};

void Restaurante::setNombre(const string& nombre) {
	this->nombre = nombre;
};

const string& Restaurante::getUrl() const {
	return URL;
};

void Restaurante::setUrl(const string& url) {
	URL = url;
};
