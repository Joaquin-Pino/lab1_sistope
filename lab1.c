#include <stdio.h>
#include <stdlib.h>
#include "funciones.h"

int main(){
    const char* filename = "imagen_ruido.bin";
    Imagen* img = cargar_imagen(filename);
    if (img == NULL) {
        return 1; // Error al cargar la imagen
    }
    Imagen* img_erosionada = erosion(img);
    guardar_imagen("salida.bin", img_erosionada);

    free_image(img); // Liberar memoria de la imagen cargada
    free_image(img_erosionada); // Liberar memoria de la imagen erosionada
    return 0;
}