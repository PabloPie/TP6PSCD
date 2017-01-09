/*
 * Autores:
 * Nip:
 * Fecha de entrega: 10/01/2017
 */

#include <iostream>
#include <chrono>
#include <thread>
#include "../headers/Socket.h"

using namespace std;

const int MESSAGE_SIZE = 4001; //mensajes de no más 4000 caracteres
const string MENS_FIN("END OF SERVICE");
const string TERMINOS {"Nombre","URL","Descripcion","Categoria","Fecha"};

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

//CAMBIO: Creo que no se pasa asi
int elegirMonumento(array<string>& m){
    int respuesta;
    char c;
    bool continuar = true;
    while(continuar){
        cout<<"¿Quiere elegir un restaurante cercano? (Y/N): "<<flush;
        cin>>c;
        if(c == 'Y'){
            for(int i=0; i<m.size(); i++){
                cout<< to_string(i+1) + ". " + m[i] +".\n";
            }
            cout<<"Elija la URL del monumento: "<<flush
            cin>>respuesta;
            respuesta--;
            continuar = false;
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

int main(int argc, char* argv[]) {
    if (argc!=10) {
            cout << "Uso: ./Cliente [direccion] [puerto] [id] [title] [link] [descripcion] [categoria] [fecha] [icono] \n";
            cout<<"Si no quieres añadir uno de los terminos del monumento añadir un \"*\"\n";
            exit(1);
    }
    srand(time(NULL));
    // Dirección y número donde escucha el proceso servidor
    string SERVER_ADDRESS;
    int SERVER_PORT;
    string id;
    // string nombre, URL, descripcion, categoria, fecha, icono;
    
    SERVER_ADDRESS = argv[1];
    SERVER_PORT = atoi(argv[2]);
    id = argv[3];
    /*
    nombre = argv[4];
    URL = argv[5];
    descripcion = argv[6];
    categoria = argv[7];
    fecha = argv[8];
    icono = argv[9];
    */
    string termino[6];
    bool correcto[6];
    int contador = 0;
    for(int i=0; i<6; i++){
        termino[i]=argv[i+4];
        correcto[i]=false;
        if(termino[i]!="*"){
            contador++;
            correcto[i]=true;
        }
    }
    if(contador==0){
        cerr << "No hay suficientes terminos para hacer la busqueda: " << strerror(errno) << endl;
        exit(1);
    }
    else if(contador>5){
        elegirTerminos(ref(contador),correcto);
    }
    
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
    int contador = 0;
    
    string peticion = generarPeticion(ref(contador),correcto,termino) + id;
    cout << "Enviando peticion al servidor: " + peticion + "\n";
    const char* message = peticion.c_str();

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
    //CAMBIO: No estoy muy segura de si seria asi
    if(aux!=-1){    //Si es -1 le hemos pasado mal los datos antes
        if(aux != "Ningun resultado"){
            //CAMBIO: Necesitamos el numero de monumentos?
            array<string, 5> m;
            bool parse_ok = messageParser(aux, m, "*");
            if (!parse_ok) {
                // Cerramos el socket
                socket.Close(socket_fd);
                exit(1);
            }
            //CAMBIO: mirar si el parser sirve en este caso
            //CAMBIO: (Elegir monumento) -> "0-4" o (Volver a consultar o END OF SERVICE) ->"-1"
            int eleccion= elegirMonumento(m);
            string busqueda;
            if(eleccion != -1){
                //Ha elegido un monumento
                busqueda = "Buscar restaurante " + to_string(eleccion);
                message = busqueda.c_str();

                //Enviamos la nueva busqueda
                int send_bytes = socket.Send(socket_fd, message);
                if(send_bytes == -1){
                    cerr << "Error al enviar datos: " << strerror(errno) << endl;
                    // Cerramos el socket
                    socket.Close(socket_fd);
                    exit(1);
                }
                //Recibimos la URL del restaurante
                read_bytes = socket.Recv(socket_fd, buffer, MESSAGE_SIZE);
                if (read_bytes == -1) {
                    cerr << "Error al recibir datos: " << strerror(errno)<< endl;
                     // Cerramos el socket
                    socket.Close(socket_fd);
                    exit(1);
                }
                string restaurante = buffer;
                //CAMBIO: abrir pagina del restaurante
                //Mostramos la respuesta
                cout << "La URL del restaurante es: " + restaurante + "\n";
            }
            
            //CAMBIO: Volver a consultar o END OF SERVICE
            if(rand()%3 == 0){ //END OF SERVICE
                peticion = "END OF SERVICE";
                message = peticion.c_str();

                //Enviamos la peticion de acabar la comunicacion
                int send_bytes = socket.Send(socket_fd, message);
                if(send_bytes == -1){
                    cerr << "Error al enviar datos: " << strerror(errno) << endl;
                    // Cerramos el socket
                    socket.Close(socket_fd);
                    exit(1);
                }
                //Recibimos el precio de las consultas realizadas
                read_bytes = socket.Recv(socket_fd, buffer, MESSAGE_SIZE);
                if (read_bytes == -1) {
                   cerr << "Error al recibir datos: " << strerror(errno)<< endl;
                    // Cerramos el socket
                   socket.Close(socket_fd);
                   exit(1);
                }
                string precio = buffer;
                //Mostramos la respuesta
                cout << "El precio por las consultas realizadas es de: " + precio + "\n";
                //CAMBIO: Las consultas estarían en un bucle, indicar que sale de este para cerrar el socket
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
    else{
        cout<<"No le hemos enviado bien el monumento\n";
    }
}
