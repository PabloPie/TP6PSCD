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
#include "../headers/DataInitializer.h"
#include "../headers/Restaurante.h"
#include "../headers/Monumento.h"

using namespace std;

//constantes para la conexión al servidor
const int MESSAGE_SIZE = 4001; //mensajes de no más 4000 caracteres
const string MENS_FIN("END OF SERVICE");

//Constantes del sistema
const int precio = 100;
const string f_restaurante("datos/restaurantes.json");
const string f_monumentos("datos/arte.json");

//Vectores con la información de los JSON
vector<Restaurante> restaurantes;
vector<Monumento> monumentos;

//Declaración funciones privadas
void atenderCliente(int cliente, Socket &sck);
void inicializarDatos();
void pruebas();

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
	while (true) {
		int client_fd = socket.Accept();
		if (client_fd > 0) {
			cout << "Cliente " << con++ << endl;
			//thread t = thread(&atenderCliente, client_fd, ref(socket));
			//t.detach();

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
		} else {
			cout << "Mensaje recibido: '" << buffer << "'" << endl;

			//Parseamos el mensaje
			string info[9]; // 8 terminos y id cliente
			string s = buffer;
			string delimiter = "*";
			int i = 0;
			size_t pos = 0;
			string token;
			while ((pos = s.find(delimiter)) != string::npos) {
				if (i > 9) {
					cout << "Error, mensaje recibido no respeta formato."
							<< endl;
					sck.Send(cliente, "-1");
					sck.Close(cliente);
					return;
				}
				token = s.substr(0, pos);
				info[i] = token;
				i++;
				s.erase(0, pos + 1);
			}
			info[8] = s;
			string msg;
			//TODO: Buscamos con los datos que nos da el cliente

			// Enviamos la respuesta
			int send_bytes = sck.Send(cliente, msg);
			if (send_bytes == -1) {
				cerr << "Error al enviar datos: " << strerror(errno) << endl;
				// Cerramos el socket
				sck.Close(cliente);
				exit(1);
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

void inicializarDatos() {
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
