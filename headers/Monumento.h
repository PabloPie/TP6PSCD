#ifndef _MONUMENTO_H
#define _MONUMENTO_H

#include <string>

using namespace std;

class Monumento{
public:
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
	bool operator==(Monumento m);

private:
	string title;
	string link;
	string descripcion;
	string categoria;
	string fecha;
	string icono;
	double lat;
	double lon;
};



#endif
