/*
 * Autores: Beatriz Martínez, Santiago Morón, Pablo Piedrafita
 * Nip: 687943,681417,691812
 * Fecha de entrega: 10/01/2017
 */
#include "../headers/Monumento.h"

Monumento::Monumento() {
	this->lat = -200;
	this->lon = -200;
}

Monumento::Monumento(string title, string link, string descripcion,
		string categoria, string fecha, string icono, double lat, double lon) {
	this->title = title;
	this->link = link;
	this->descripcion = descripcion;
	this->categoria = categoria;
	this->fecha = fecha;
	this->icono = icono, this->lat = lat;
	this->lon = lon;
}

const string& Monumento::getCategoria() const {
	return categoria;
}

void Monumento::setCategoria(const string& categoria) {
	this->categoria = categoria;
}

const string& Monumento::getDescripcion() const {
	return descripcion;
}

void Monumento::setDescripcion(const string& descripcion) {
	this->descripcion = descripcion;
}

const string& Monumento::getDate() const {
	return fecha;
}

void Monumento::setDate(const string& fecha) {
	this->fecha = fecha;
}

const string& Monumento::getIcon() const {
	return icono;
}

void Monumento::setIcon(const string& icono) {
	this->icono = icono;
}

double Monumento::getLat() const {
	return lat;
}

void Monumento::setLat(double lat) {
	this->lat = lat;
}

const string& Monumento::getURL() const {
	return link;
}

void Monumento::setURL(const string& link) {
	this->link = link;
}

double Monumento::getLon() const {
	return lon;
}

void Monumento::setLon(double lon) {
	this->lon = lon;
}

const string& Monumento::getTitle() const {
	return title;
}

void Monumento::setTitle(const string& title) {
	this->title = title;
}

bool Monumento::operator==(const Monumento &m) const {
	return this->getLat() == m.getLat() && this->getLon() == m.getLon();
}

int Monumento::compare(const Monumento &m) const {
	int i = 0;

	if (!this->title.empty()
			&& (m.getTitle().find(this->title)) != string::npos) {
		i++;
	}
	if (!this->link.empty() && (m.getURL().find(this->link)) != string::npos) {
		i++;
	}
	if (!this->descripcion.empty()
			&& (m.getDescripcion().find(this->descripcion)) != string::npos) {
		i++;
	}
	if (!this->categoria.empty()
			&& (m.getCategoria().find(this->categoria)) != string::npos) {
		i++;
	}
	if (!this->fecha.empty()
			&& (m.getDate().find(this->fecha)) != string::npos) {
		i++;
	}
	if (!this->icono.empty()
			&& (m.getIcon().find(this->icono)) != string::npos) {
		i++;
	}
	return i;
}
