#ifndef _MONUMENTO_H
#define _MONUMENTO_H

#include <string>

using namespace std;

class Monumento{
public:
	const string& getAutor() const;
	void setAutor(const string& autor);

	const string& getBarrio() const;
	void setBarrio(const string& barrio);

	const string& getCategoria() const;
	void setCategoria(const string& categoria);

	const string& getEpoca() const;
	void setEpoca(const string& epoca);

	const string& getIconografia() const;
	void setIconografia(const string& iconografia);

	double getLat() const;
	void setLat(double lat);

	double getLon() const;
	void setLon(double lon);

	const string& getMaterial() const;
	void setMaterial(const string& material);

	const string& getNombre() const;
	void setNombre(const string& nombre);

	const string& getUrl() const;
	void setUrl(const string& url);

private:
	string nombre;
	string autor;
	string categoria;
	string epoca;
	string barrio;
	string iconografia;
	string material;
	string URL;
	double lat;
	double lon;
};



#endif
