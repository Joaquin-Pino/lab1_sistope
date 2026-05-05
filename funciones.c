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

Imagen* erosion(Imagen* img){
if (img == NULL) return NULL;

    Imagen* out = (Imagen*)malloc(sizeof(Imagen));
    out->ancho = img->ancho;
    out->alto = img->alto;
    out->data = (unsigned char*)malloc(img->ancho * img->alto * sizeof(unsigned char));

    int elemEstruct[9] = {0,1,0,1,1,1,0,1,0};

    // Recorremos toda la imagen
    for (int y = 0; y < img->alto; y++) {
        for (int x = 0; x < img->ancho; x++) {
            
            int coincide = 1;

            // Recorremos la ventana 3x3 alrededor del pixel (x, y)
            // Usamos 'coincide' en la condición para detener el ciclo antes si ya fallo
            for (int ky = -1; ky <= 1 && coincide; ky++) {
                for (int kx = -1; kx <= 1; kx++) {
                    
                    // Calculamos el indice (0 al 8) del arreglo del elemento estructurante
                    int se_index = (ky + 1) * 3 + (kx + 1);

                    // Solo evaluamos se nos exige un '1' en esta pos
                    if (elemEstruct[se_index] == 1) {
                        // coordenadas de la imagen a evaluar
                        int nx = x + kx;
                        int ny = y + ky;

                        // Verificamos si se sale de los bordes O si el píxel de la imagen es 0
                        if (nx < 0 || nx >= img->ancho || ny < 0 || ny >= img->alto || 
                            img->data[ny * img->ancho + nx] == 0) {

                            coincide = 0; // No coincide, marcamos como 0
                            break;      // Rompemos el ciclo kx
                        }
                    }
                }
            }

            // Asignamos el resultado a la nueva imagen
            out->data[y * out->ancho + x] = coincide;
        }
    }

    return out;
}

Imagen* dilatar(Imagen* img){
    if (img == NULL) return NULL;

    Imagen* out = (Imagen*)malloc(sizeof(Imagen));
    out->ancho = img->ancho;
    out->alto = img->alto;
    out->data = (unsigned char*)malloc(img->ancho * img->alto * sizeof(unsigned char));

    int elemEstruct[9] = {0,1,0,1,1,1,0,1,0};

    // Recorremos toda la imagen
    for (int y = 0; y < img->alto; y++) {
        for (int x = 0; x < img->ancho; x++) {
            
            int coincide = 0;

            // Recorremos la ventana 3x3 alrededor del pixel (x, y)
            // Usamos 'coincide' en la condición para detener el ciclo antes si ya encontro un '1'
            for (int ky = -1; ky <= 1 && !coincide; ky++) {
                for (int kx = -1; kx <= 1 && !coincide; kx++) {
                    
                    // Calculamos el indice (0 al 8) del arreglo del elemento estructurante
                    int se_index = (ky + 1) * 3 + (kx + 1);

                    // Solo evaluamos se nos exige un '1' en esta pos
                    if (elemEstruct[se_index] == 1) {
                        // coordenadas de la imagen a evaluar
                        int nx = x + kx;
                        int ny = y + ky;

                        // Verificamos si no se sale de los bordes Y si el píxel de la imagen es 1
                        if (nx >= 0 && nx < img->ancho && ny >= 0 && ny < img->alto && 
                            img->data[ny * img->ancho + nx] == 1) {

                            coincide = 1; // Coincide, marcamos como 1
                            break;      // Rompemos el ciclo kx
                        }
                    }
                }
            }

            // Asignamos el resultado a la nueva imagen
            out->data[y * out->ancho + x] = coincide;
        }
    }

    return out;
}