#ifndef FUNCIONES_H
#define FUNCIONES_H

#include <stdint.h>
#include <stdio.h>

// Estructura para manejar la imagen en memoria
typedef struct {
    int ancho;// 4 bytes: Ancho de la imagen
    int alto;// 4 bytes: Alto de la imagen
    unsigned char *data; // Arreglo unidimensional de (width * height) bytes
} Imagen;

// Estructura para almacenar las coordenadas detectadas
typedef struct {
    int x;
    int y;
} Punto;

// Entradas: const char* filename (ruta del archivo binario a leer)
// Salidas: Imagen* (puntero a la estructura de la imagen cargada, o NULL si falla)
// Descripción: Lee la cabecera (ancho y alto) y los píxeles de un archivo .bin, reservando la memoria necesaria.
Imagen* cargar_imagen(const char* filename);

int guardar_imagen(const char* filename, Imagen* img);

Imagen* erosion(Imagen* img);

// Entradas: Imagen* img (imagen erosionada)
// Salidas: Imagen* (nueva imagen con la operación de dilatación aplicada)
// Descripción: Aplica el filtro de dilatación utilizando un elemento estructurante de 3x3 para restaurar la forma.
Imagen* dilatar(Imagen* img);

// Entradas: Imagen* original, Imagen* preprocessed
// Salidas: Imagen* (imagen resultante del ruido)
// Descripción: Resta la imagen preprocesada a la imagen original para obtener los artefactos eliminados (ruido).
Imagen* get_noise(Imagen* original, Imagen* preprocessed);


// Entradas: Imagen* img (imagen preprocesada), int r (radio a buscar), int t (umbral de votos)
// Salidas: Punto* (arreglo dinámico con los centros detectados) y por referencia int* count (cantidad de centros)
// Descripción: Genera el plano acumulador, realiza la votación de Hough basándose en la ecuación paramétrica del círculo y filtra los centros que superen el umbral t.
Punto* hough_transform(Imagen* img, int r, int t, int* count);

// Entradas: const char* filename, Punto* centers, int count
// Salidas: int (1 si fue exitoso, 0 si hubo error)
// Descripción: Exporta la lista de centros detectados (X, Y) a un archivo CSV.
int export_report(const char* filename, Punto* centers, int count);

// Entradas: Imagen* img
// Salidas: void
// Descripción: Libera la memoria dinámica asignada para los píxeles y la estructura de la imagen.
void free_image(Imagen* img);

#endif // FUNCIONES_H