/*
 * Autores: Beatriz Martínez, Santiago Morón, Pablo Piedrafita
 * Nip: 687943,681417,691812
 * Fecha de entrega: 10/01/2017
 */
#ifndef _RESTAURANTE_H
#define _RESTAURANTE_H

#include <string>

using namespace std;

class Restaurante{
private:

	string title;
	string link;
	string descripcion;
	string categoria;
	string fecha;
	string icono;
	double lat;
	double lon;

public:
	Restaurante();//Constructor
	//Getters & setters
	const string& getCategoria() const;
	void setCategoria(const string& categoria);
	const string& getDescripcion() const;
	void setDescripcion(const string& descripcion);
	const string& getDate() const;
	void setDate(const string& fecha);
	const string& getIcon() const;
	void setIcon(const string& icono);
	double getLat() const;
	void setLat(double lat);
	const string& getURL() const;
	void setURL(const string& link);
	double getLon() const;
	void setLon(double lon);
	const string& getTitle() const;
	void setTitle(const string& title);

	//Se compara por posición, si la latitud y longitud coinciden, son el mismo
	bool operator==(Restaurante m);
};

#endif
