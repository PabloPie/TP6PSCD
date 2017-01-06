#include "../headers/Restaurante.h"

const string& Restaurante::getCategoria() const {
	return categoria;
}

void Restaurante::setCategoria(const string& categoria) {
	this->categoria = categoria;
}

const string& Restaurante::getDescripcion() const {
	return descripcion;
}

void Restaurante::setDescripcion(const string& descripcion) {
	this->descripcion = descripcion;
}

const string& Restaurante::getDate() const {
	return fecha;
}

void Restaurante::setDate(const string& fecha) {
	this->fecha = fecha;
}

const string& Restaurante::getIcon() const {
	return icono;
}

void Restaurante::setIcon(const string& icono) {
	this->icono = icono;
}

double Restaurante::getLat() const {
	return lat;
}

void Restaurante::setLat(double lat) {
	this->lat = lat;
}

const string& Restaurante::getURL() const {
	return link;
}

void Restaurante::setURL(const string& link) {
	this->link = link;
}

double Restaurante::getLon() const {
	return lon;
}

void Restaurante::setLon(double lon) {
	this->lon = lon;
}

const string& Restaurante::getTitle() const {
	return title;
}

void Restaurante::setTitle(const string& title) {
	this->title = title;
}

bool Restaurante::operator==(Restaurante m){
	return this->getLat()==m.getLat() && this->getLon()==m.getLon();
}
