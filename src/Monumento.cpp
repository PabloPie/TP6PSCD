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
	this->icono = icono,
	this->lat = lat;
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

bool Monumento::operator==(Monumento m) {
	return this->getLat() == m.getLat() && this->getLon() == m.getLon();
}



int Monumento::compare(Monumento m) {
	int i = 0;
	if (!this->title.compare(m.title) && !this->title.empty()) {
		i++;
	}
	if (!this->link.compare(m.link) && !this->link.empty()) {
		i++;
	}
	if (!this->descripcion.compare(m.descripcion)
			&& !this->descripcion.empty()) {
		i++;
	}
	if (!this->categoria.compare(m.categoria) && !this->categoria.empty()) {
		i++;
	}
	if (!this->fecha.compare(m.fecha) && !this->fecha.empty()) {
		i++;
	}
	if (!this->icono.compare(m.icono) && !this->icono.empty()) {
		i++;
	}
        /*
	if (this->lat == m.lat && this->lat != -200) {
		i++;
	}
	if (this->lon == m.lon && this->lat != -200) {
		i++;
	}
        */
	return i;
}
