#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
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

Imagen* get_ruido(Imagen* original, Imagen* preprocesada){
    //preparar imagen resultante y sus datos
    Imagen* resultado = (Imagen*)malloc(sizeof(Imagen));
    resultado->ancho = original -> ancho;
    resultado->alto = original->alto;
    resultado->data = (unsigned char*)malloc(resultado->ancho * resultado->alto * sizeof(unsigned char));


    for(int i = 0; i < original->alto; i++){
        for(int j = 0; j< original->ancho; j++){
            int indice = i * original->ancho + j;

            //aplicar la resta
            if(original->data[indice]==1){
                //ambas imagenes tienen ese pixel, no es ruido
                if(preprocesada->data[indice]==1){
                    resultado->data[indice] = 0;
                }
                //solo la original lo tiene, es ruido
                else{
                    resultado->data[indice] = 1;
                }
            }
        }
    }
    return resultado;
}

Punto* hough(Imagen* img, int r, int t, int* count){
    // Validación de entrada
    if (img == NULL || img->data == NULL || r <= 0 || count == NULL) {
        if (count != NULL) *count = 0;
        return NULL;
    }

    int ancho = img->ancho;
    int alto = img->alto;

    // Memoria dinamica inicializada en 0 (calloc)
    int *acumulador = (int*)calloc(ancho * alto, sizeof(int));
    if (acumulador == NULL) {
        *count = 0;
        return NULL;
    }

    // Votamos por cada pixel que sea 1 (borde)
    for (int y = 0; y < alto; y++) {
        for (int x = 0; x < ancho; x++) {

            if (img->data[y * ancho + x] == 1) {

                // Recorremos los 360 grados
                for (int theta = 0; theta < 360; theta++) {

                    double radianes = theta * PI / 180.0;

                    // Redondeo simétrico al entero más cercano (en lugar de truncar)
                    int a = x - (int)lround(r * cos(radianes));
                    int b = y - (int)lround(r * sin(radianes));

                    // Verificamos límites
                    if (a >= 0 && a < ancho && b >= 0 && b < alto) {
                        acumulador[b * ancho + a]++; // Sumamos el voto
                    }
                }
            }
        }
    }

    // Non-Maximum Suppression: keep only local maxima within a window of radius win.
    // Usamos una copia de los valores originales para evitar leer celdas ya suprimidas
    // durante el barrido (bug de modificación in-place con empates en plateau).
    int win = r / 2;
    if (win < 1) win = 1;

    int *acum_orig = (int*)malloc(ancho * alto * sizeof(int));
    if (acum_orig == NULL) {
        free(acumulador);
        *count = 0;
        return NULL;
    }
    memcpy(acum_orig, acumulador, ancho * alto * sizeof(int));

    for (int b = 0; b < alto; b++) {
        for (int a = 0; a < ancho; a++) {
            int val = acum_orig[b * ancho + a];
            if (val < t) continue;
            int es_maximo = 1;
            for (int ky = -win; ky <= win && es_maximo; ky++) {
                for (int kx = -win; kx <= win && es_maximo; kx++) {
                    if (kx == 0 && ky == 0) continue;
                    int nb = b + ky, na = a + kx;

                    if (nb >= 0 && nb < alto && na >= 0 && na < ancho) {
                        int vecino = acum_orig[nb * ancho + na];
                        // empate: pierde la celda que aparece después en orden de barrido
                        if (vecino > val || (vecino == val && (nb < b || (nb == b && na < a)))) {
                            es_maximo = 0;
                        }
                    }
                }
            }
            if (!es_maximo) acumulador[b * ancho + a] = 0;
        }
    }
    free(acum_orig);

    int total_centros = 0;
    for (int i = 0; i < ancho * alto; i++) {
        if (acumulador[i] >= t) {
            total_centros++;
        }
    }

    // Actualizamos el puntero 'count' para que lab1.c sepa el tamaño del arreglo
    *count = total_centros;

    // Si no detectamos ningún círculo, limpiamos y retornamos NULL
    if (total_centros == 0) {
        free(acumulador);
        return NULL;
    }

    // Asignar memoria exacta para los resultados
    Punto* centros = (Punto*)malloc(total_centros * sizeof(Punto));
    if (centros == NULL) {
        free(acumulador);
        printf("Error: Fallo al asignar memoria para los centros detectados.\n");
        return NULL;    
    }

    // Extraer las coordenadas
    int index = 0;
    for (int i = 0; i < ancho * alto; i++) {
        if (acumulador[i] >= t) {
            centros[index].x = i % ancho; // Coordenada x mágica
            centros[index].y = i / ancho; // Coordenada y mágica
            index++;
        }
    }
    
    free(acumulador);
    return centros;
}

int generar_reporte(const char* filename, Punto* centers, int count) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error: No se pudo abrir el archivo %s para escribir.\n", filename);
        return 0;
    }

    // Escribimos la cabecera del CSV
    fprintf(file, "X,Y\n");

    // Escribimos cada centro detectado
    for (int i = 0; i < count; i++) {
        fprintf(file, "%d,%d\n", centers[i].x, centers[i].y);
    }

    fclose(file);
    return 1;
}