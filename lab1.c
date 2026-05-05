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
    Imagen* img_dilatada = dilatar(img_erosionada);
    guardar_imagen("salida_erosionada.bin", img_erosionada);
    guardar_imagen("salida_dilatada.bin", img_dilatada);

    free_image(img); // Liberar memoria de la imagen cargada
    free_image(img_erosionada); // Liberar memoria de la imagen erosionada
    free_image(img_dilatada); // Liberar memoria de la imagen dilatada
    return 0;
}