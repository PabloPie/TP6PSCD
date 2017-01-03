//*****************************************************************************
// File:   main.cpp
// Author: 
// Date:   September 2016
// Coms:   Versión ligeramente adaptada de "http://www.gpsy.com/gpsinfo/geotoutm/gantz/"
//*****************************************************************************


//UTM Conversion.cpp- test program for lat/long to UTM and UTM to lat/long conversions
#include <iostream>
#include <iomanip>
#include <string>
#include "LatLong-UTMconversion.h"

using namespace std;

// Ejemplo: EINA: 41.6834N, 0.8874W

int main(){
	double Lat = 41.6834;
	double Long = -0.8874;
	double UTMNorthing;
	double UTMEasting;
	char UTMZone[4];
	int RefEllipsoid = 23;//WGS-84. See list with file "LatLong- UTM conversion.cpp" for id numbers

	cout << "EINA: starting position(Lat, Long):  (" << Lat << "," << Long << ")" << endl;

	LLtoUTM(RefEllipsoid, Lat, Long, UTMNorthing, UTMEasting, UTMZone);
	cout << setiosflags(ios::showpoint | ios::fixed) << setprecision(5)
	     << "EINA: calculated UTM position(Northing, Easting, Zone):  ("
	     << UTMNorthing << "," << UTMEasting << "," << UTMZone << ")" << endl;
	//----------------------------------------------------------------
	
	UTMtoLL(RefEllipsoid, UTMNorthing, UTMEasting, UTMZone, Lat, Long);
	cout << "EINA: calculated Lat, Long position(Lat, Long):  (" << Lat << "," << Long << ")" << endl;
	//----------------------------------------------------------------
	// http://www.zaragoza.es/ciudad/artepublico/detalle_ArtePublico?id=97
	//  676105.25, 4610179.46

	char UTMZoneZgza[4] = "30T";
	UTMNorthing = 4610179.46;
	UTMEasting = 676105.25;

	UTMtoLL(RefEllipsoid, UTMNorthing, UTMEasting, UTMZoneZgza, Lat, Long);
	cout << "detalle_ArtePublico_97 en Torrero (Lat, Long):  (" << Lat << "," << Long << ")" << endl;
	string cmd("firefox https://www.google.com/maps/place/"+to_string(Lat)+","+to_string(Long));
	//"system" requiere un "char *", que es lo que nos da el operador "c_str()" de la clase string de C++
	int resCall = system(cmd.c_str());
	if(resCall != 0){
		cerr << "Ha habido algún problema al abrir el navegador" << endl;
		return 1;
	}else{
		return 0;
	}
}
