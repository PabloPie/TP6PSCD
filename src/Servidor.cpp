//*****************************************************************
// File:   Servidor.cpp
// Author: PSCD-Unizar
// Date:   noviembre 2015
// Coms:   Ejemplo de servidor con comunicación síncrona mediante sockets
//         Compilar el fichero "Makefile" asociado, mediante
//         "make".
//*****************************************************************

#include <iostream>
#include "../headers/Socket.h"
#include <sstream>
#include <thread>
#include <atomic>
#include <stdlib.h>
#include "../headers/DataInitializer.h"
#include "../headers/Restaurante.h"
#include "../headers/Monumento.h"
#include "../headers/ImageDownloader.h"
#include "../headers/Busqueda.h"

using namespace std;

//constantes para la conexión al servidor
const int MESSAGE_SIZE = 4001; //mensajes de no más 4000 caracteres
const string MENS_FIN("END OF SERVICE");

//Constantes del sistema
const int precio = 100;
const string f_restaurante("datos/restaurantes.json");
const string f_monumentos("datos/arte.json");

char url_monumentos[500] =
		"http://www.zaragoza.es/georref/json/hilo/verconsulta_Piezas?georss_tag_1=-&georss_materiales=-&georss_tematica=-&georss_barrio=-&georss_epoca=-";
char url_restaurante[500] =
		"http://www.zaragoza.es/georref/json/hilo/ver_Restaurante";

//Vectores con la información de los JSON
vector<Restaurante> restaurantes;
vector<Monumento> monumentos;

//Declaración funciones privadas
void atenderCliente(int cliente, Socket &sck);
void inicializarDatos();
void pruebas();
bool messageParser(string buffer, array<string, 9> &info, string delimiter);

//-------------------------------------------------------------
int main(int argc, char* argv[]) {
	if (argc < 1) {
		cerr << "Invocación requiere 1 parámetro: [PORT]" << endl;
		return 1;
	}

	// Puerto donde escucha el proceso servidor
	istringstream ss(argv[1]);
	int SERVER_PORT;

	if (!(ss >> SERVER_PORT)) {
		cerr << "Número no válido " << argv[1]
				<< ", asignando puerto por defecto 30001\n";
		SERVER_PORT = 30001;
	}

	// Creación del socket con el que se llevará a cabo
	// la comunicación con el servidor.
	Socket socket(SERVER_PORT);

	// Bind
	int socket_fd = socket.Bind();
	if (socket_fd == -1) {
		cerr << "Error en el bind: " << strerror(errno) << endl;
		exit(1);
	}

	// Listen
	int max_connections = 3;
	int error_code = socket.Listen(max_connections);
	if (error_code == -1) {
		cerr << "Error en el listen: " << strerror(errno) << endl;
		// Cerramos el socket
		socket.Close(socket_fd);
		exit(1);
	}

	int con;	//Cuenta numero de conexiones totales
	inicializarDatos();	//Inicializamos los datos
	bool fin = false;
	while (!fin) {
		int client_fd = socket.Accept();
		if (client_fd > 0) {
			cout << "Cliente " << con++ << endl;
			thread t = thread(&atenderCliente, client_fd, ref(socket));
			t.detach();

		} else {
			cerr << "Error en el accept: " << strerror(errno) << endl;
		}
	}
	/*// Cerramos el socket del servidor
	 error_code = socket.Close(socket_fd);
	 if (error_code == -1) {
	 cerr << "Error cerrando el socket del servidor: " << strerror(errno)
	 << endl;
	 }
	 // Mensaje de despedida
	 cout << "Bye bye" << endl;

	 return error_code;
	 */
}
//-------------------------------------------------------------
void atenderCliente(int cliente, Socket &sck) {
	int length = 100;
	string buffer;
	int consultas = 0;
	bool out = false; // Inicialmente no salir del bucle
	while (!out) {
		// Recibimos el mensaje del cliente
		int rcv_bytes = sck.Recv(cliente, buffer, MESSAGE_SIZE);
		if (rcv_bytes == -1) {
			cerr << "Error al recibir datos: " << strerror(errno) << endl;
			// Cerramos la conexión con el cliente
			sck.Close(cliente);
		}

		if (buffer == MENS_FIN) {
			out = true;
			//Recibir precio
			string price = to_string(consultas * precio);
			// Enviamos el resultado final de sus peticiones
			int send_bytes = sck.Send(cliente, price);
			if (send_bytes == -1) {
				cerr << "Error al enviar datos: " << strerror(errno) << endl;
				// Cerramos el socket
				sck.Close(cliente);
				exit(1);
			}
		} else {
			cout << "Mensaje recibido: '" << buffer << "'" << endl;

			//Parseamos el mensaje:
			//titulo*link*descripcion*categoria*fecha*icono*lat*lon*idusuario
			array<string, 9> info;
			bool parse_ok = messageParser(buffer, info, "*");
			if (!parse_ok) {
				cout << "Error, mensaje recibido no respeta formato." << endl;
				sck.Send(cliente, "-1");
				sck.Close(cliente);
				exit(1);
			}
			//TODO: crear función para procesado de la petición?
			//Creamos un monumento con la info recibida
			double lat = atof(info[6].c_str());
			double lon = atof(info[7].c_str());
			Monumento m(info[0], info[1], info[2], info[3], info[4], info[5],
					atof(info[6].c_str()), atof(info[7].c_str()));

			//Buscamos con los datos que nos da el cliente
			string msg;
			Restaurante r;
			array<Monumento, 5> mon_seleccionados;
			int resultado = busquedaMonumento(monumentos, mon_seleccionados, m);
			//Si no hay resultados
			if (resultado == 0) {
				msg = "Ningun resultado";
				//Enviamos los monumentos
				int send_bytes = sck.Send(cliente, msg);
				if (send_bytes == -1) {
					cerr << "Error al enviar datos: " << strerror(errno)
							<< endl;
					sck.Close(cliente);
					exit(1);
				}
			} else {
				msg = "";
				//Construimos el mensaje para el cliente con los monumentos
				for (Monumento m : mon_seleccionados) {
					msg += m.getURL() + '*' + m.getTitle() + '*';
				}

				//Enviamos los monumentos
				int send_bytes = sck.Send(cliente, msg);
				if (send_bytes == -1) {
					cerr << "Error al enviar datos: " << strerror(errno)
							<< endl;
					sck.Close(cliente);
					exit(1);
				}

				//Recibimos el número de monumento que quiere
				int rcv_bytes = sck.Recv(cliente, buffer, MESSAGE_SIZE);
				if (rcv_bytes == -1) {
					cerr << "Error al recibir datos: " << strerror(errno)
							<< endl;
					// Cerramos la conexión con el cliente
					sck.Close(cliente);
				}
				if (buffer != "0") {
					//Seleccionamos el monumento
					Monumento m1 = mon_seleccionados[stoi(buffer)];
					//Buscamos el restaurante mas cercano
					r = BusquedaRestauranteCerc(m1, restaurantes);
					//lat;lon
					msg = to_string(r.getLat()) + ";" + to_string(r.getLon());
					//Enviamos la posicion del restaurante al cliente
					int send_bytes = sck.Send(cliente, msg);
					if (send_bytes == -1) {
						cerr << "Error al enviar datos: " << strerror(errno)
								<< endl;
						sck.Close(cliente);
						exit(1);
					}
				}
				consultas++;
			}
		}
	}

	// Cerramos el socket del cliente
	int error_code = sck.Close(cliente);
	if (error_code == -1) {
		cerr << "Error cerrando el socket del cliente: " << strerror(errno)
				<< endl;
	}
}

bool messageParser(string buffer, array<string, 9> &info, string delimiter) {
	// 8 terminos y id cliente: titulo*link*...*idcliente
	int i = 0;
	size_t pos = 0;
	string token;
	while ((pos = buffer.find(delimiter)) != string::npos) {
		if (i > info.size()) {
			return false;
		}
		token = buffer.substr(0, pos);
		info[i] = token;
		i++;
		buffer.erase(0, pos + 1);
	}
	info[8] = buffer;
	return true;
}

void inicializarDatos() {
	cout << "Descargando datos..." << endl;
	ImageDownloader i;
	i.downloadImage(url_monumentos, "datos/arte.json");
	i.downloadImage(url_restaurante, "datos/restaurantes.json");
	cout << "Inicializando datos..." << endl;
	int numMon = obtenerDatos(monumentos, f_monumentos);
	int numRes = obtenerDatos(restaurantes, f_restaurante);
	cout << "Número de monumentos: " << numMon << endl;
	cout << "Número de restaurantes: " << numRes << endl;

}

void pruebas() {
	char c = '$';
	int num = 0;
	for (Monumento m : monumentos) {
		if (m.getTitle().find(c) != std::string::npos) {
			cout << m.getTitle() << endl;
			num++;
		}
		if (m.getURL().find(c) != std::string::npos) {
			cout << m.getURL() << endl;
			num++;
		}
		if (m.getDescripcion().find(c) != std::string::npos) {
			cout << m.getDescripcion() << endl;
			num++;
		}
		if (m.getCategoria().find(c) != std::string::npos) {
			cout << m.getCategoria() << endl;
			num++;
		}
		if (m.getDate().find(c) != std::string::npos) {
			cout << m.getDate() << endl;
			num++;
		}
		if (m.getIcon().find(c) != std::string::npos) {
			cout << m.getIcon() << endl;
			num++;
		}
	}
	cout << num << endl;
}
