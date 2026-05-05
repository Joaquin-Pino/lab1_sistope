#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include "funciones.h"

//funcion para el control de entrada
void ejemploEntrada(){
    printf("Para iniciar correctamente el programa se necesita la siguiente estructura:\n");
    printf("./lab1 -i entrada.bin -r numero -t numero -o salida.csv -d\n");
    printf("flag -d opcional para la ejecución (exporta archivos extra)\n");
}

int main(int argc, char *argv[]){
    int option;
    //i = Ruta de la imagen de entrada (string)
    char* i = NULL;
    //r = Radio de los círculos que se desean detectar (int)
    //t = Umbral de confianza (int)
    int r=0, t=0;
    //o = Nombre del archivo de salida con resultados (string)
    char* o=NULL;
    /*
    d = Debug Dump, si está se debe exportar:
      preprocesada.bin: Imagen tras la erosión y dilatación
      ruido.bin: Imagen resultante de la resta: orignal-preprocesada
    */
    int d=0;

    //lectura de flags
    while((option = getopt(argc,argv,"i:r:t:o:d")) != -1){
        switch(option){
            case 'i':
                i = optarg;
                break;
            case 'r':
                r = atoi(optarg);
                break;
            case 't':
                t = atoi(optarg);
                break;
            case 'o':
                o = optarg;
                break;
            case 'd':
                d = 1;
                break;
        }
    }

    //debug para ver si sabemos usar flags :)
    printf("Datos recibidos de las flags:\n");
    printf("%s %d %d %s %d\n\n",i,r,t,o,d);

    //verificar existencia de cada flag para la ejecución
    if(i==NULL){
        printf("Falta la flag -i con el nombre del archivo\n");
        ejemploEntrada();
        return 0;
    }
    if(r==0){
        printf("Falta la flag -r con el radio de los círculos que se desean detectar\n");
        ejemploEntrada();
        return 1;
    }
    if(t==0){
        printf("Falta la flag -r con el umbral de confianza\n");
        ejemploEntrada();
        return 0;
    }
    if(o==NULL){
        printf("Falta la flag -o con el nombre del archivo de salida\n");
        ejemploEntrada();
        return 0;
    }

    //cargar la imagen
    Imagen* img = cargar_imagen(i);
    if(img==NULL) return 0;
    printf("Imagen cargada con exito\n");

    //aplicar erosion a la imagen
    Imagen* img_preprocesada = erosion(img);
    //aplicar dilatación a la imagen
    Imagen* img_preprocesada = dilatar(img_preprocesada);

    //guardar el ruido de la imagen normal y la preprocesada
    Imagen* ruido = get_noise(img, img_preprocesada);

    //exportar los datos de Debug si está la flag
    if(d==1){
        int preprocesada = guardar_imagen("preprocesada.bin",img_preprocesada);
        int ruido = guardar_imagen("ruido.bin",ruido);
        //en caso de no ser exportadas avisar por terminal
        if(preprocesada == 0 || ruido == 0){
            printf("No se pudo exportar una de las imagenes de la flag -d.\n");
        }
    }

    //buscar los centros deseados
    int cantidad_centros = 0;
    Punto* centros = hough(img_preprocesada,r,t,&cantidad_centros);

    //generar el reporte de los centros
    int reporte = export_report(o, centros, cantidad_centros);
    if(reporte==0){
        printf("No se pudo exportar el reporte final.\n");
        return 0;
    }

    return 1;

}