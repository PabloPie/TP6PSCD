//*****************************************************************
// File:    ImageDownloader.h
// Author:  PSCD-Unizar
// Date:    11/11/15
// Coms:    Programaci�n de Sistemas Concurrentes y 
//          Distribuidos Curso 2015-2016.
//          Especificaci�n de una librer�a para descarga de im�genes de Internet
//*****************************************************************
#ifndef _IMAGED_H_
#define _IMAGED_H_

class ImageDownloader {

public:

	// CONSTRUCTORES
	// PRE:
	// POST: Inicializa la librer�a de descarga de im�genes
	ImageDownloader();
	
	// DESTRUCTOR
	// PRE: 
	// POST: Libera el espacio utilizado para la descarga de im�genes. 
	//       Debe llamarse antes de acabar el programa	
	~ImageDownloader();
	
	// M�TODOS

	// PRE:  URL: URL correspondiente a una imagen
	//       path: Ruta local donde almacenar la imagen
	//		 El m�todo 'cleanup' no ha sido llamado
	// POST: Descarga una imagen de la URL y la almacena en el path local indicado
	// 		 Devuelve 0 en caso de �xito y -1 en caso de error
	int downloadImage(char* URL, char* path);
};

#endif
