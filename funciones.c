#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "funciones.h"

/*
Entradas: 
    nombre (String: nombre del archivo a cargar)
Salidas: 
    Imagen (estructura utilizada para guardar los datos de la imagen)
Descripcion: 
    carga un archivo.bin al programa como un TDA Imagen
*/
Imagen* cargar_imagen(const char* nombre) {
    //rb lectura binaria
    FILE *archivo = fopen(nombre, "rb");
    //verificar que el archivo existe    
    if (archivo == NULL) {
        printf("Error: No se pudo abrir el archivo %s\n", nombre);
        return NULL;
    }

    //reservar memoria para la estructura de la imagen
    Imagen *img = (Imagen*)malloc(sizeof(Imagen));
    if (img == NULL) {
        printf("Error: Fallo al asignar memoria para la imagen.\n");
        fclose(archivo);
        return NULL;
    }

    //lectura de ancho y alto
    fread(&(img->ancho), sizeof(int), 1, archivo);
    fread(&(img->alto), sizeof(int), 1, archivo);

    //guardar arreglo de pixeles
    int cant_pixeles = img->ancho * img->alto;
    img->data = (unsigned char*)malloc(cant_pixeles * sizeof(unsigned char));
    if (img->data == NULL) {
        printf("Error: Fallo al asignar memoria para los píxeles.\n");
        free(img);
        fclose(archivo);
        return NULL;
    }
    //leer todos los pixeles y guardarlos
    fread(img->data, sizeof(unsigned char), cant_pixeles, archivo);

    //cerrar archivo y retornar
    fclose(archivo);
    return img;
}


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
int guardar_imagen(const char* nombre, Imagen* img) {
    //wb escritura binaria
    FILE *archivo = fopen(nombre, "wb");
    if (archivo == NULL) {
        printf("Error: No se pudo abrir el archivo %s para escribir.\n", nombre);
        return 0;
    }

    //escribimos ancho, alto y datos de la imagen
    fwrite(&(img->ancho), sizeof(int), 1, archivo);
    fwrite(&(img->alto), sizeof(int), 1, archivo);

    //escribimos todos los pixeles en el archivo
    int total_pixels = img->ancho * img->alto;
    fwrite(img->data, sizeof(unsigned char), total_pixels, archivo);

    //cerrar archivo y retornar
    fclose(archivo);
    return 1;
}

/*
Entradas: 
    img (Imagen: imagen que se desea aplicar "erosion")
Salidas:
    Imagen (resultado de aplicar "erosion" a la img de entrada)
Descripción:
    se le aplica erosion a la img de entrada,
    la cual elimina el ruido de la img
*/
Imagen* erosion(Imagen* img){
    //si no hay imagen cancela
    if (img == NULL) return NULL;

    //guardar memoria para la imagen de salida
    Imagen* salida = (Imagen*)malloc(sizeof(Imagen));
    salida->ancho = img->ancho;
    salida->alto = img->alto;
    salida->data = (unsigned char*)malloc(img->ancho * img->alto * sizeof(unsigned char));

    //elemento estructurarante dado por el enunciado
    int elEstructurante[9] = {0,1,0,1,1,1,0,1,0};

    //verificar por cada pixel
    for (int y = 0; y < img->alto; y++) {
        for (int x = 0; x < img->ancho; x++) {
            
            //bandera para ver si el pixel coincide con el elemento
            int coincide = 1;

            //recorrer el elemento 3x3
            //usamos "coincide" en la condición para detener el ciclo antes si ya fallo
            for (int ky = -1; ky <= 1 && coincide; ky++) {
                for (int kx = -1; kx <= 1; kx++) {
                    
                    //calcular indice en el elemento 3x3
                    int indice = (ky + 1) * 3 + (kx + 1);

                    //solo si hay un 1 en el elemento, verificamos la imagen
                    if (elEstructurante[indice] == 1) {
                        //coordenadas a evaluar de la imagen
                        int nx = x + kx;
                        int ny = y + ky;

                        //verificamos si se sale de los bordes O si el píxel de la imagen es 0
                        if (nx < 0 || nx >= img->ancho || ny < 0 || ny >= img->alto || 
                            img->data[ny * img->ancho + nx] == 0) {

                            coincide = 0; //no coincide, marcamos como 0
                            break; //rompemos el ciclo kx
                        }
                    }
                }
            }

            //guardar el resultado en la imagen de salida
            salida->data[y * salida->ancho + x] = coincide;
        }
    }

    return salida;
}

/*
Entradas: 
    img (Imagen: imagen que se desea aplicar "dilatar)
Salidas:
    Imagen (resultado de aplicar "dilatar" a la img de entrada)
Descripción:
    se le aplica erosion a la img de entrada, 
    la cual restaura los circulos a su tamaño idea
*/
Imagen* dilatar(Imagen* img){
    if (img == NULL) return NULL;

    Imagen* salida = (Imagen*)malloc(sizeof(Imagen));
    salida->ancho = img->ancho;
    salida->alto = img->alto;
    salida->data = (unsigned char*)malloc(img->ancho * img->alto * sizeof(unsigned char));

    //elemento estructurarante dado por el enunciado
    int elEstructurante[9] = {0,1,0,1,1,1,0,1,0};

    //verificar por cada pixel
    for (int y = 0; y < img->alto; y++) {
        for (int x = 0; x < img->ancho; x++) {
            
            //bandera para ver si el pixel coincide con el elemento
            int coincide = 0;

            //recorrer el elemento 3x3
            //usamos "coincide" en la condición para detener el ciclo antes si ya fallo
            for (int ky = -1; ky <= 1 && !coincide; ky++) {
                for (int kx = -1; kx <= 1 && !coincide; kx++) {
                    
                    //calcular indice en el elemento 3x3
                    int se_index = (ky + 1) * 3 + (kx + 1);

                    //solo si hay un 1 en el elemento, verificamos la imagen
                    if (elEstructurante[se_index] == 1) {
                        // coordenadas de la imagen a evaluar
                        int nx = x + kx;
                        int ny = y + ky;

                        //verificamos si no se sale de los bordes Y si el píxel de la imagen es 1
                        if (nx >= 0 && nx < img->ancho && ny >= 0 && ny < img->alto && 
                            img->data[ny * img->ancho + nx] == 1) {

                            coincide = 1; //coincide, marcamos como 1
                            break; //rompemos el ciclo kx
                        }
                    }
                }
            }

            //guardar el resultado en la imagen de salida
            salida->data[y * salida->ancho + x] = coincide;
        }
    }

    return salida;
}

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
Imagen* get_ruido(Imagen* original, Imagen* preprocesada){
    //preparar imagen resultante y sus datos
    Imagen* resultado = (Imagen*)malloc(sizeof(Imagen));
    resultado->ancho = original -> ancho;
    resultado->alto = original->alto;
    resultado->data = (unsigned char*)malloc(resultado->ancho * resultado->alto * sizeof(unsigned char));

    //recorrer todos los pixeles de la imagen
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
    //retornar resultado de la resta
    return resultado;
}

/*
Entradas: 
    img (Imagen: img de la cual se va a buscar los centros deseados)
    r (int: radio de los circulos que se desea buscar)
    t (int: cantidad de votos necesarios para que se considere un centro valido)
    count (int*: guarda la cantidad de centros encontrados)
Salidas:
    Punto* (retorna un arreglo de TDA Punto que guarda l)
Descripción:
    se aplica la votación de hough a cada pixel, con esto
    se buscan los centros de los circulos con el radio indicado
*/
Punto* hough(Imagen* img, int r, int t, int* count){
    //validación de entrada
    if (img == NULL || img->data == NULL || r <= 0 || count == NULL) {
        if (count != NULL) *count = 0;
        return NULL;
    }

    int ancho = img->ancho;
    int alto = img->alto;

    //memoria dinamica inicializada en 0 (calloc)
    int *acumulador = (int*)calloc(ancho * alto, sizeof(int));
    if (acumulador == NULL) {
        *count = 0;
        return NULL;
    }

    //votamos por cada pixel que sea 1 (borde)
    for (int y = 0; y < alto; y++) {
        for (int x = 0; x < ancho; x++) {

            //si hay un pixel blanco
            if (img->data[y * ancho + x] == 1) {

                //recorremos los 360 grados
                for (int theta = 0; theta < 360; theta++) {

                    double radianes = theta * PI / 180.0;

                    //redondeo simétrico al entero más cercano (en lugar de truncar)
                    int a = x - (int)lround(r * cos(radianes));
                    int b = y - (int)lround(r * sin(radianes));

                    //verificamos límites
                    if (a >= 0 && a < ancho && b >= 0 && b < alto) {
                        //sumamos el voto a ese pixel
                        acumulador[b * ancho + a]++;
                    }
                }
            }
        }
    }

    //tamaño de un cuadro a revisar por cada pixel con votos
    int win = r / 2;
    if (win < 1) win = 1;

    //copiar el arreglo con las votaciones por pixel
    int *acum_orig = (int*)malloc(ancho * alto * sizeof(int));
    if (acum_orig == NULL) {
        free(acumulador);
        *count = 0;
        return NULL;
    }
    for (int i = 0; i < ancho * alto; i++) {
        acum_orig[i] = acumulador[i];
    }

    //por cada pixel
    for (int b = 0; b < alto; b++) {
        for (int a = 0; a < ancho; a++) {
            //extraer la cantidad de votos del pixel
            int val = acum_orig[b * ancho + a];

            //no cumple la cantidad de votos exigida
            if (val < t) {
                acum_orig[b * ancho + a] = 0;//le quitamos los votos
                continue;
            }

            int es_maximo = 1;
            //ver el alrededor de ese pixel en un tamaño de (r/2)*(r/2)
            for (int ky = -win; ky <= win && es_maximo; ky++) {
                for (int kx = -win; kx <= win && es_maximo; kx++) {
                    //si estás en el centro (el mismo pixel a revisar) saltalo
                    if (kx == 0 && ky == 0) continue;
                    //coordenadas a ver al rededor del pixel deseado
                    int nb = b + ky, na = a + kx;

                    //ver que esté dentro de los limites de la imagen
                    if (nb >= 0 && nb < alto && na >= 0 && na < ancho) {
                        //extraer la cantidad de votos del pixel vecino
                        int vecino = acum_orig[nb * ancho + na];
                        //empate: pierde la celda que aparece después en orden de barrido
                        if (vecino > val || (vecino == val && (nb < b || (nb == b && na < a)))) {
                            es_maximo = 0;
                        }
                    }
                }
            }
            //si no es maximo quitarle los votos
            if (!es_maximo) acumulador[b * ancho + a] = 0;
        }
    }
    free(acum_orig);

    //contar la cantidad de centros
    int total_centros = 0;
    for (int i = 0; i < ancho * alto; i++) {
        if (acumulador[i] >= t) {
            total_centros++;
        }
    }

    //actualizamos el puntero 'count' para que lab1.c sepa el tamaño del arreglo
    *count = total_centros;

    //si no encontramos círculos, retornamos puntero null
    if (total_centros == 0) {
        free(acumulador);
        return NULL;
    }

    //asignar memoria para los resultados
    Punto* centros = (Punto*)malloc(total_centros * sizeof(Punto));
    if (centros == NULL) {
        free(acumulador);
        printf("Error: Fallo al asignar memoria para los centros detectados.\n");
        return NULL;    
    }

    //extraer las coordenadas de cada centro
    int index = 0;
    for (int i = 0; i < ancho * alto; i++) {
        if (acumulador[i] >= t) {
            centros[index].x = i % ancho; //coordenada x
            centros[index].y = i / ancho; //coordenada y
            index++;
        }
    }
    
    //liberar memoria usada y retornar los centros obtenidos
    free(acumulador);
    return centros;
}

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
int generar_reporte(const char* nombre, Punto* centros, int count) {
    FILE *file = fopen(nombre, "w");
    if (file == NULL) {
        printf("Error: No se pudo abrir el archivo %s para escribir.\n", nombre);
        return 0;
    }

    // Escribimos la cabecera del CSV
    fprintf(file, "X,Y\n");

    // Escribimos cada centro detectado
    for (int i = 0; i < count; i++) {
        fprintf(file, "%d,%d\n", centros[i].x, centros[i].y);
    }

    fclose(file);
    return 1;
}

/*
Entradas: 
    img (Imagen: dato que se desea liberar)
Salidas: 
    void
Descripcion:
    Libera la memoria de los atributos y de la propia imagen recibida
*/
void free_image(Imagen* img) {
    if (img != NULL) {
        free(img->data);
        free(img);
    }
}