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
/*
const string f_restaurante("datos/restaurantes.json");
const string f_monumentos("datos/arte.json");
*/

//Para Hendrix cambiar al directorio propio
const string f_restaurante("/home/a681417/Desktop/tNP_681417_691812_687943/datos/restaurantes.json");
const string f_monumentos("/home/a681417/Desktop/tNP_681417_691812_687943/datos/arte.json");


char url_monumentos[500] =
		"http://www.zaragoza.es/georref/json/hilo/verconsulta_Piezas?georss_tag_1=-&georss_materiales=-&georss_tematica=-&georss_barrio=-&georss_epoca=-";
char url_restaurante[500] =
		"http://www.zaragoza.es/georref/json/hilo/ver_Restaurante";

//Vectores con la información de los JSON
vector<Restaurante> restaurantes;
vector<Monumento> monumentos;

//Declaración funciones privadas
void atenderCliente(int cliente, Socket &sck,bool &threadFinalizado);
void inicializarDatos();
void seHaTerminado (bool &seguir, int SERVER_PORT);
void prueba();
bool messageParser(const string &buffer, array<string, 6> &info,
		string delimiter);

//-------------------------------------------------------------
int main(int argc, char* argv[]) {
	if (argc < 2) {
		cerr << "Invocación requiere 1 parámetro: [PORT]" << endl;
		return 1;
	}

	// Puerto donde escucha el proceso servidor
	istringstream ss(argv[1]);
	int SERVER_PORT;
        bool seguir = true;

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
        const int MAX_CONNECTIONS = 3;
        int error_code = socket.Listen(MAX_CONNECTIONS);
        if(error_code == -1) {
            cerr << "Error en el listen: " << strerror(errno) << endl;
            // Cerramos el socket
            socket.Close(socket_fd);
            exit(1);
        }
        
        int client_fd;
        thread t[MAX_CONNECTIONS];								    //contador para generar distintos nombres para las imagenes
        int i=0;											//contador de conexiones activas
        int p=0;											//posicion del vector de threads donde se creara el nuevo
        bool libres_threads[MAX_CONNECTIONS];			            //array con las posiciones de threads libres
        bool terminado_threads[MAX_CONNECTIONS];			                //array con las posiciones de threads terminados a la espera de join
        for (int k=0; k<MAX_CONNECTIONS; k++) {
                libres_threads[k] = true;
                terminado_threads[k] = true;
        }
	int numCliente = 0;
	inicializarDatos();	//Inicializamos los datos
    thread f (&seHaTerminado,ref(seguir),SERVER_PORT);
	while (seguir) {
        i = 0;
        //antes de aceptar una nueva conexion, comprueba los threads disponibles y elimina hilos acabados
        for (int k=0; k<MAX_CONNECTIONS; k++) {
            if (libres_threads[k])  {	
                if (!terminado_threads[k]) {
                    t[k].join();
                    terminado_threads[k] = true;
                }
                p=k; //asocia la posicion para el nuevo thread a uno libre
            }
            else {
                i++;
            }
    	}
        //aceptara todas las peticiones de cliente mientras tenga abiertas menos de 10 conexiones
          if (i<MAX_CONNECTIONS){
            client_fd = socket.Accept();            
            if(client_fd == -1) {
                cerr << "Error en el accept: " << strerror(errno) << endl;
                // Cerramos el socket
                socket.Close(socket_fd);
            } else {
				numCliente++;
                if (seguir) {
					cout<<"Cliente nuevo: cliente numero "<< numCliente <<endl;
                    libres_threads[p] = false;
                    terminado_threads[p] = false;
                    t[p] = thread(&atenderCliente, client_fd, ref(socket),ref(libres_threads[p]));
                    //t.detach();

                } else {
                	cout << "Servidor cerrado, no se aceptan peticiones" << endl;
                }
            }
        }
    }
    	cout << "Quiero Salir\n";
        f.join();
        //esperamos a que todos los threads finalicen
        cout<<"Esperando a que finalicen todos los procesos."<<endl;
        for (int k=0; k<3; k++) {			
			if(t[k].joinable()){
               t[k].join();
               cout << "finaliza la comunicacion " + to_string(k) + "\n";
			}
        }
        // Cerramos el socket del servidor
        error_code = socket.Close(socket_fd);
        if (error_code == -1) {
	        cerr << "Error cerrando el socket del servidor: " << strerror(errno)
	        << endl;
        }
        // Mensaje de despedida
        cout << "Bye bye" << endl;
        return error_code;
}
//-------------------------------------------------------------
void atenderCliente(int cliente, Socket &sck,bool &threadFinalizado) {
	int length = 100;
	string buffer;
	int consultas = 0;
	bool out = false; // Inicialmente no salir del bucle
	while (!out) {
		// Recibimos el mensaje del cliente
		int rcv_bytes = sck.Recv(cliente, buffer, MESSAGE_SIZE);
		if (rcv_bytes == -1) {
			cerr << "Error al recibir la peticion: " << strerror(errno) << endl;
			// Cerramos la conexión con el cliente
			break;
		}

		if (buffer == MENS_FIN) {
			out = true;
			//Recibir precio
			string price = to_string(consultas * precio);
			// Enviamos el resultado final de sus peticiones
			int send_bytes = sck.Send(cliente, price);
			if (send_bytes == -1) {
				cerr << "Error al enviar precio final: " << strerror(errno) << endl;
				// Cerramos el socket
				break;
			}
		} else {
			cout << "Mensaje recibido: '" << buffer << "'" << endl;

			//Parseamos el mensaje:
			//titulo*link*descripcion*categoria*fecha*icono
			array<string, 6> info;
			bool parse_ok = messageParser(buffer, info, "*");
			if (!parse_ok) {
				cout << "Error, mensaje recibido no respeta formato." << endl;
				int send_bytes = sck.Send(cliente, "-1");
				if(send_bytes==-1){
					cout << "El cliente se ha cerrado, cerrando conexión y saliendo" << endl;
					break;				
				}
			}else{
				//Creamos un monumento con la info recibida
				Monumento m(info[0], info[1], info[2], info[3], info[4], info[5],
						-200, -200);

				//Buscamos con los datos que nos da el cliente
				string msg;
				Restaurante r;
				array<Monumento, 5> mon_seleccionados;
				int resultado = busquedaMonumento(monumentos, mon_seleccionados, m);
				//Si no hay resultados
				if (resultado == 0) {
					msg = "Ningun resultado";
					int send_bytes = sck.Send(cliente, msg);
					if (send_bytes == -1) {
						cerr << "Error al enviar mensaje sin resultados: " << strerror(errno)
								<< endl;
						break;
					}
				} else {
					msg = "";
					int cuenta = 0;
					//Construimos el mensaje para el cliente con los monumentos
					for (Monumento m : mon_seleccionados) {
						msg += m.getURL() + '*' + m.getTitle() + '*';
						cuenta++;
					}

					//Enviamos los monumentos
					int send_bytes = sck.Send(cliente, msg);
					if (send_bytes == -1) {
						cerr << "Error al enviar monumentos: " << strerror(errno)
								<< endl;
						break;
					}
					//Recibimos el número de monumento que quiere
					int rcv_bytes = sck.Recv(cliente, buffer, MESSAGE_SIZE);
					if (rcv_bytes == -1 || buffer.empty()) {
						cerr << "Error al recibir numero de monumento: " << strerror(errno)
								<< endl;
						break;	
					}
					
					int auxiliar = stoi(buffer);
					if(auxiliar < 5 && auxiliar > 0 && auxiliar < cuenta){
						//Seleccionamos el monumento
						Monumento m1 = mon_seleccionados[auxiliar-1];
						//Buscamos el restaurante mas cercano
						r = BusquedaRestauranteCerc(m1, restaurantes);
						//lat;lon
						msg = to_string(r.getLat()) + ";" + to_string(r.getLon());
						//Enviamos la posicion del restaurante al cliente
						int send_bytes = sck.Send(cliente, msg);
						if (send_bytes == -1) {
							cerr << "Error al enviar posicion del restaurante: " << strerror(errno)
									<< endl;
							break;
						}
					}
					consultas++;
				}
			}
		}
	}

	// Cerramos el socket del cliente
	int error_code = sck.Close(cliente);
	if (error_code == -1) {
		cerr << "Error cerrando el socket del cliente: " << strerror(errno)
				<< endl;
	}
    threadFinalizado = true;
}

bool messageParser(const string &buf, array<string, 6> &info,
		string delimiter) {
	// 6 terminos y id cliente: titulo*link*...
	int i = 0;
	size_t pos = 0;
	string buffer = buf;
	string token;
	while ((pos = buffer.find(delimiter)) != string::npos) {
		if (i > info.size() - 1) {
			return false;
		}
		token = buffer.substr(0, pos);
		info[i] = token;
		i++;
		buffer.erase(0, pos + 1);
	}
	return true;
}

void inicializarDatos() {
	cout << "Descargando datos..." << endl;
	ImageDownloader i;
	// Para Hendrix cambiar al directorio que sea
	char c_monumentos[500]= "/home/a681417/Desktop/tNP_681417_691812_687943/datos/arte.json";
	char c_restaurantes[500] = "/home/a681417/Desktop/tNP_681417_691812_687943/datos/restaurantes.json";
	/*
	char c_monumentos[500]= "datos/restaurantes.json";
	char c_restaurantes[500] = "datos/arte.json";
	*/
	i.downloadImage(url_monumentos, c_monumentos);
	i.downloadImage(url_restaurante, c_restaurantes);
	cout << "Inicializando datos..." << endl;
	int numMon = obtenerDatos(monumentos, f_monumentos);
	int numRes = obtenerDatos(restaurantes, f_restaurante);
	cout << "Número de monumentos: " << numMon << endl;
	cout << "Número de restaurantes: " << numRes << endl;

}

/*
 * Pre: Introducimos por referencia un booleano true;
 * Post:En el booleano [seguir] gestionamos si debemos o no seguir
 * escuchando peticiones de los clientes, es decir, nos dice si intentamos
 * terminar el programa.
 * En el momento en que introduzcamos por la entrada del programa un caracter
 * igual a 'X' acabará, si introduciomos otro caracter vuelve a esperar a que
 * introduzcamos otro caracter.
 */
void seHaTerminado (bool &seguir, int SERVER_PORT){
    string caracter;
    string caracter2= "X";

    while(seguir){
        cin >> caracter;
        if(caracter.compare(caracter2)==0){
            seguir=false;
        }
    }
    //creamos una ultima conexion para que el cliente salga del Accept
    string SERVER_ADDRESS = "localhost";
    Socket socket(SERVER_ADDRESS, SERVER_PORT);

    const int MAX_ATTEMPS = 10;
    int count = 0;
    int socket_fd;
    do{
        // Conexión con el servidor
        socket_fd = socket.Connect();
        count++;

        // Si error --> esperamos 1 segundo para reconectar
        if(socket_fd == -1){
            this_thread::sleep_for(chrono::seconds(1));
        }
    } while(socket_fd == -1 && count < MAX_ATTEMPS);
        // Chequeamos si se ha realizado la conexión
    if(socket_fd == -1){
        cout << "Imposible contactar con el servidor,soy el cliente falso \n";
    }
	cout << "soy el cliente falso, cierro el servidor \n";
    socket.Close(socket_fd);
}

void prueba() {
	array<string, 6> info;
	bool ok = messageParser("Goya******", info, "*");
	//cout << ok << endl;

	Monumento m(info[0], info[1], info[2], info[3], info[4], info[5], -200,	-200);
	for(Monumento m1 : monumentos){
		if(m.compare(m1) > 0){
			cout << m1.getTitle() << endl;
		}
	}
	/*cout << m.getTitle() << endl;
	cout << m.getURL() << endl;
	cout << m.getDescripcion() << endl;
	cout << m.getIcon() << endl;
	cout << m.getCategoria() << endl;
	cout << m.getDate() << endl;*/
	array<Monumento, 5> mon_seleccionados;
	int resultado = busquedaMonumento(monumentos, mon_seleccionados, m);
	//cout << resultado << endl;
	for(Monumento m:mon_seleccionados){
		cout << m.getTitle() << endl;
	}

	exit(100);

	/*char c = '$';
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
	 cout << num << endl;*/
}
