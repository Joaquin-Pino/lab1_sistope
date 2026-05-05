#include <stdio.h>
#include <stdlib.h>
#include "funciones.h"

Imagen* cargar_imagen(const char* filename) {
  FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        printf("Error: No se pudo abrir el archivo %s\n", filename);
        return NULL;
    }

    // Reservar memoria para la estructura de la imagen
    Imagen *img = (Imagen*)malloc(sizeof(Imagen));
    if (img == NULL) {
        printf("Error: Fallo al asignar memoria para la imagen.\n");
        fclose(file);
        return NULL;
    }

    // lectura de ancho y alto
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

void free_image(Imagen* img) {
    if (img != NULL) {
        free(img->data);
        free(img);
    }
}

int guardar_imagen(const char* filename, Imagen* img) {
    FILE *archivo = fopen(filename, "wb");
    if (archivo == NULL) {
        printf("Error: No se pudo abrir el archivo %s para escribir.\n", filename);
        return 0;
    }

    //escribimos ancho, alto y datos de la imagen
    fwrite(&(img->ancho), sizeof(int), 1, archivo);
    fwrite(&(img->alto), sizeof(int), 1, archivo);

    int total_pixels = img->ancho * img->alto;
    fwrite(img->data, sizeof(unsigned char), total_pixels, archivo);

    fclose(archivo);
    return 1;
}
    