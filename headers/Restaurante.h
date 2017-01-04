#ifndef _RESTAURANTE_H
#define _RESTAURANTE_H

#include <string>

using namespace std;

class Restaurante{
private:
	string nombre;
	string URL;
	double lat;
	double lon;

public:

	double getLat() const;
	void setLat(double lat);

	double getLon() const;
	void setLon(double lon);

	const string& getNombre() const;
	void setNombre(const string& nombre);

	const string& getUrl() const;
	void setUrl(const string& url);
};

#endif
