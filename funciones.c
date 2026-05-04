#include <stdio.h>
#include <stdlib.h>
#include "funciones.h"

Imagen* cargar_imagen(const char* filename) {
  FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        printf("Error: No se pudo abrir el archivo %s\n", filename);
        return NULL;
    }

    Imagen *img = (Imagen*)malloc(sizeof(Imagen));
    if (img == NULL) {
        printf("Error: Fallo al asignar memoria para la imagen.\n");
        fclose(file);
        return NULL;
    }

    fread(&(img->ancho), sizeof(int), 1, file);
    fread(&(img->alto), sizeof(int), 1, file);

    int total_pixels = img->ancho * img->alto;
    img->data = (unsigned char*)malloc(total_pixels * sizeof(unsigned char));
    
    if (img->data == NULL) {
        printf("Error: Fallo al asignar memoria para los píxeles.\n");
        free(img);
        fclose(file);
        return NULL;
    }
    fread(img->data, sizeof(unsigned char), total_pixels, file);

    fclose(file);
    return img;
}