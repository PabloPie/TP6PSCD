#include "../headers/Monumento.h"


const string& Monumento::getAutor() const {
	return autor;
};

void Monumento::setAutor(const string& autor) {
	this->autor = autor;
};

const string& Monumento::getBarrio() const {
	return barrio;
};

void Monumento::setBarrio(const string& barrio) {
	this->barrio = barrio;
};

const string& Monumento::getCategoria() const {
	return categoria;
};

void Monumento::setCategoria(const string& categoria) {
	this->categoria = categoria;
};

const string& Monumento::getEpoca() const {
	return epoca;
};

void Monumento::setEpoca(const string& epoca) {
	this->epoca = epoca;
};

const string& Monumento::getIconografia() const {
	return iconografia;
};

void Monumento::setIconografia(const string& iconografia) {
	this->iconografia = iconografia;
};

double Monumento::getLat() const {
	return lat;
};

void Monumento::setLat(double lat) {
	this->lat = lat;
};

double Monumento::getLon() const {
	return lon;
}

void Monumento::setLon(double lon) {
	this->lon = lon;
};

const string& Monumento::getMaterial() const {
	return material;
};

void Monumento::setMaterial(const string& material) {
	this->material = material;
};

const string& Monumento::getNombre() const {
	return nombre;
};

void Monumento::setNombre(const string& nombre) {
	this->nombre = nombre;
};

const string& Monumento::getUrl() const {
	return URL;
};

void Monumento::setUrl(const string& url) {
	URL = url;
};
