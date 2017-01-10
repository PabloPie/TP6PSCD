/*
 * Autores:
 * Nip:
 * Fecha de entrega: 10/01/2017
 */

#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include "../headers/Socket.h"

using namespace std;

const int MESSAGE_SIZE = 4001; //mensajes de no más 4000 caracteres
const string MENS_FIN("END OF SERVICE");
const string TERMINOS[] {"Nombre","URL","Descripcion","Categoria","Fecha"};

void elegirTerminos(int& contador, bool* correcto){
    int tabla [contador];
    int borrar;
   
    while(contador>5){
        int num = 1;
        int i = 0;
        cout<<"Ha elegido demasiados terminos\n";
        while(num<=contador&&i<6){
            if(correcto[i]){
                cout<<"\t" + to_string(num)+ ". " + TERMINOS[i] +".\n";
                tabla[num-1]=i;
                num++;
            }
            i++;
        }
        cout<<"Indique cual quiere dejar de buscar: "<<flush;
        cin>>borrar;
        correcto[tabla[borrar-1]];
        contador--;
    }
}

string generarPeticion(int& contador, bool* correcto,string* termino){
    int i=0;
    string peticion = "";
    while(i<contador){
        if(correcto[i]){
            peticion = peticion + termino[i];
        }
        peticion=peticion + "*";
    }
    return peticion;
}

int elegirMonumento(){
    int respuesta;
    char c;
    bool continuar = true;
    while(continuar){
        cout<<"¿Quiere elegir un restaurante cercano? (Y/N): "<<flush;
        cin>>c;
        if(c == 'Y'){
        	respuesta=0;
            continuar=false;
        }
        else if(c == 'N'){
            respuesta = -1;
            continuar = false;
        }
        else{
            cout<<"No le he entendido bien\n";     
        }
    }
    return respuesta;
}

bool messageParser(string buffer, array<string, 5> &url, array<string,5>& nom, string delimiter) {
	// 8 terminos y id cliente: titulo*link*...*idcliente
	int i = 0;
	size_t pos = 0;
	string token;
	while ((pos = buffer.find_first_of(delimiter)) != string::npos) {
		if (i > url.size()) {
			return false;
		}
		token = buffer.substr(0, pos);
		url[i] = token;
		if (i > nom.size()) {
			return false;
		}
		buffer.erase(0, pos + 1);
		pos = buffer.find_first_of(delimiter);
		if(pos!=string::npos){
			token = buffer.substr(0, pos);
			nom[i] = token;
		}
		i++;
	}
	return true;
}

int main(int argc, char* argv[]) {
    if (argc<3) {
            cout << "Uso: ./Cliente [direccion] [puerto]\n";
            cout << "Siga los pasos indicados correctamente.\n";
            exit(1);
    }
    srand(time(NULL));
    // Dirección y número donde escucha el proceso servidor
    string SERVER_ADDRESS;
    int SERVER_PORT;
    string id;
    
    SERVER_ADDRESS = argv[1];
    SERVER_PORT = atoi(argv[2]);
    
    cout << SERVER_ADDRESS + "\n";
    cout << to_string(SERVER_PORT) + "\n";
    
    // Creación del socket con el que se llevará a cabo
    // la comunicación con el servidor.
    Socket socket(SERVER_ADDRESS, SERVER_PORT);

    // Conectamos con el servidor. Probamos varias conexiones
    const int MAX_ATTEMPS = 10;
    int count = 0;
    int socket_fd;
    do
    {
        // Conexión con el servidor
        socket_fd = socket.Connect();
        count++;
        // Si error --> esperamos 1 segundo para reconectar
        if(socket_fd == -1){
            this_thread::sleep_for(chrono::seconds(1));
        }
    } while(socket_fd == -1 && count < MAX_ATTEMPS);

    // Comprobamos si se ha realizado la conexión
    if(socket_fd == -1){
    cout << "Imposible contactar con el servidor \n";
        return socket_fd;
    }
    string posibilidades[6]={"titulo","URL","descripcion","categoria","fecha","icono"};
    string termino[6];
    bool correcto[6];
    int contador = 0;
    const char* message;
    string cadena;
    bool out=false;
    while(!out){
        for(int i=0; i<6; i++){
                cout << "Inserte"<< posibilidades[i] <<"( * si no desea buscar "<< posibilidades[i] <<"): ";
                scanf("%s",termino[i]);
                cout << "\n";
                correcto[i]=false;
                if(termino[i].find_first_not_of(" \t")==string::npos){	//El usuario no ha introducido nada
                        cout << "No puede introducir una cadena vacía.\n";
                        i--; //Para repetir la pregunta
                }
                if(termino[i]!="*"){ //Si hay alg�n t�rmino
                        contador++;
                        correcto[i]=true;
                }
        }
        if(contador==0){
                cerr << "No hay suficientes términos para hacer la búsqueda: " << strerror(errno) << endl;
                exit(1);
        }
        else if(contador>5){
                elegirTerminos(ref(contador),correcto);
        }

        contador = 0;

        string peticion = generarPeticion(ref(contador),correcto,termino) + id;
        cout << "Enviando peticion al servidor: " + peticion + "\n";
        message = peticion.c_str();

        //enviamos la peticion
        int send_bytes = socket.Send(socket_fd, message);
        if(send_bytes == -1)
        {
                cerr << "Error al enviar datos: " << strerror(errno) << endl;
                // Cerramos el socket
                socket.Close(socket_fd);
                exit(1);
        }

        // Buffer para almacenar la respuesta
        char buffer[MESSAGE_SIZE];

        // Recibimos la respuesta del servidor
        int read_bytes = socket.Recv(socket_fd, buffer, MESSAGE_SIZE);
        if (read_bytes == -1) {
                cerr << "Error al recibir datos: " << strerror(errno)<< endl;
                 // Cerramos el socket
                socket.Close(socket_fd);
                exit(1);
        }

        string aux = buffer;
        // Mostramos la respuesta
        cout << "La respuesta del servidor es: " + aux + "\n";

        if(aux!="-1"){    //Si es -1 le hemos pasado mal los datos antes
            if(aux != "Ningun resultado"){
                array<string, 5> URL;
                array<string, 5> nom;
                bool parse_ok = messageParser(aux, URL, nom, "*");
                if (!parse_ok) {
                        cerr << "Error al recibir datos." << endl;
                        // Cerramos el socket
                        socket.Close(socket_fd);
                        exit(1);
                }
                for(int i=0;i<URL.size();i++){
                        string cmd("firefox" + URL[i]);
                        //"system" requiere un "char *", que es lo que nos da el operador "c_str()" de la clase string de C++
                        int resCall = system(cmd.c_str());
                        if(resCall != 0){
                                 cerr << "Ha habido algún problema al abrir el navegador" << endl;
                                 return 1;
                        }
                }
                int eleccion= elegirMonumento();
                if(eleccion != -1){
                    string busqueda;
                    cout << "Elija una URL para obtener el restaurante mas cercano al monumento:" <<endl;

                    for(int i=0; i < nom.size();i++){
                            cout << to_string(i) << ". " << nom[i] << "\n";
                    }
                    //Ha elegido un monumento
                    cin >> eleccion;
                    busqueda = to_string(eleccion);
                    message = busqueda.c_str();

                    //Enviamos la nueva busqueda
                    int send_bytes = socket.Send(socket_fd, message);
                    if(send_bytes == -1){
                            cerr << "Error al enviar datos: " << strerror(errno) << endl;
                            // Cerramos el socket
                            socket.Close(socket_fd);
                            exit(1);
                    }
                    //Recibimos la Lat y Lon del restaurante
                    read_bytes = socket.Recv(socket_fd, buffer, MESSAGE_SIZE);
                    if (read_bytes == -1) {
                            cerr << "Error al recibir datos: " << strerror(errno)<< endl;
                             // Cerramos el socket
                            socket.Close(socket_fd);
                            exit(1);
                    }
                    string restaurante = buffer;
                    //Mostramos la respuesta
                    string lat,lon;
                    lat=restaurante.substr(0,restaurante.find_first_of(';'));
                    lon=restaurante.substr(restaurante.find_first_of(';')+1);


                    string cmd("firefox https://www.google.com/maps/place/"+lat+","+lon);
                    //"system" requiere un "char *", que es lo que nos da el operador "c_str()" de la clase string de C++
                    int resCall = system(cmd.c_str());
                    if(resCall != 0){
                             cerr << "Ha habido algún problema al abrir el navegador" << endl;
                             return 1;
                    }
                }
            }
            cout << "Si desea finalizar la ejecuci�n escriba TERMINAR, en caso contrario escriba OK:	";
            cin >> cadena;
            if(cadena == "TERMINAR"){
                    out=true;
                    cadena = "END OF SERVICE";
                    int send_bytes = socket.Send(socket_fd, cadena);
                    if(send_bytes == -1)
                    {
                            cerr << "Error al enviar datos: " << strerror(errno) << endl;
                            // Cerramos el socket
                            socket.Close(socket_fd);
                            exit(1);
                    }
                    //Recibimos el precio de las consultas realizadas
                    int read_bytes = socket.Recv(socket_fd, cadena, MESSAGE_SIZE);
                    if (read_bytes == -1) {
                            cerr << "Error al recibir datos: " << strerror(errno)<< endl;
                            // Cerramos el socket
                            socket.Close(socket_fd);
                            exit(1);
                    }
                    string precio = buffer;
                    //Mostramos la respuesta
                    cout << "El precio por las consultas realizadas es de: " + precio + "\n";
            }
        }
        else{
                cout<<"No le hemos enviado bien el monumento\n";
        }
    }

    // Cerramos el socket
    int error_code = socket.Close(socket_fd);
    if(error_code == -1){
             cerr << "Error cerrando el socket: " << strerror(errno) << endl;
             sleep(5);               //Espermos 5 segundos antes de terminar
             return error_code;
    }
}
