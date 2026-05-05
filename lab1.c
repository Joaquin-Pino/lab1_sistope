#include <stdio.h>
#include <stdlib.h>
#include "funciones.h"

int main(){
    const char* filename = "imagen_ruido.bin";
    Imagen* img = cargar_imagen(filename);
    if (img == NULL) {
        return 1; // Error al cargar la imagen
    }

    guardar_imagen("salida.bin", img);

    free_image(img); // Liberar memoria de la imagen cargada
    return 0;
}