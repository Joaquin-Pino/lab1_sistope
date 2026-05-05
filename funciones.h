#ifndef FUNCIONES_H
#define FUNCIONES_H

#include <stdint.h>
#include <stdio.h>
#include <math.h>

#define PI 3.14159265358979323846

//estructura para manejar la imagen en memoria
typedef struct {
    int ancho;
    int alto;
    unsigned char *data; //arreglo [ancho*alto]
} Imagen;

//estructura para almacenar las coordenadas
typedef struct {
    int x;
    int y;
} Punto;

/*
Entradas: 
    nombre (String: nombre del archivo a cargar)
Salidas: 
    Imagen (estructura utilizada para guardar los datos de la imagen)
Descripcion: 
    carga un archivo.bin al programa como un TDA Imagen
*/
Imagen* cargar_imagen(const char* filename);

/*
Entradas: 
    nombre (String: nombre deseado para la imagen.bin)
    img (Imagen: datos de la imagen que se desea guardar)
Salidas:
    int (0: no se pudo guardar, 1: se guardó correctamente)
Descripción:
    Se guarda la estructura Imagen recibida como un 
    archivo binario con el nombre recibido
*/
int guardar_imagen(const char* filename, Imagen* img);

/*
Entradas: 
    img (Imagen: imagen que se desea aplicar "erosion")
Salidas:
    Imagen (resultado de aplicar "erosion" a la img de entrada)
Descripción:
    se le aplica erosion a la img de entrada,
    la cual elimina el ruido de la img
*/
Imagen* erosion(Imagen* img);

/*
Entradas: 
    img (Imagen: imagen que se desea aplicar "dilatar)
Salidas:
    Imagen (resultado de aplicar "dilatar" a la img de entrada)
Descripción:
    se le aplica erosion a la img de entrada, 
    la cual restaura los circulos a su tamaño idea
*/
Imagen* dilatar(Imagen* img);

/*
Entradas: 
    img (Imagen: imagen original)
    preprocesada (Imagen: imagen después de aplicar erosion y dilatar)
Salidas:
    Imagen (resultado de la resta de ambas imagenes, es el "ruido")
Descripción:
    a la imagen original se le resta la preprocesada
    para obtener el rudio de la imagen original
*/
Imagen* get_ruido(Imagen* original, Imagen* preprocessed);

/*
Entradas: 
    img (Imagen: img de la cual se va a buscar los centros deseados)
    r (int: radio de los circulos que se desea buscar)
    t (int: cantidad de votos necesarios para que se considere un centro valido)
    count (int*: guarda la cantidad de centros encontrados)
Salidas:
    Punto* (retorna un arreglo de TDA Punto que guarda l)
Descripción:
    se le aplica erosion a la img de entrada, 
    la cual restaura los circulos a su tamaño idea
*/
Punto* hough(Imagen* img, int r, int t, int* count);

/*
Entradas: 
    nombre (String: nombre del archivo.csv de salida)
    centros (Punto*: arreglo de TDA Punto, los centros encontrados)
    count (int: cantidad de centros en el arreglo centros)
Salidas: 
    int (0: si no se pudo guardar el archivo, 1: se guardó el archivo correctamente)
Descripcion:
    genera el archivo .csv con el reporte de todos los centros encontrados
*/
int generar_reporte(const char* nombre, Punto* centros, int count);

/*
Entradas: 
    img (Imagen: dato que se desea liberar)
Salidas: 
    void
Descripcion:
    Libera la memoria de los atributos y de la propia imagen recibida
*/
void free_image(Imagen* img);

#endif //FUNCIONES_H