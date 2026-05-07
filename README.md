# Compilación
El proyecto incluye un archivo `Makefile` para facilitar su compilación. Para compilar el código fuente, simplemente abre una terminal en el directorio raíz del proyecto y ejecuta:

$ make all

Esto generará el archivo ejecutable llamado `lab1`. 
Para limpiar los binarios y archivos generados posteriormente, se puede usar:

$ make clean

# Dependencias de Python
Para ejecutar los scripts de validación y visualización de resultados (como `VerificacionVisual.py`), solo se debe tener instaladas las siguientes librerías:

* numpy
* Pillow
* opencv-python
* pandas
* matplotlib

# Explicación de la Ejecución
El programa se ejecuta desde la línea de comandos pasando los parámetros necesarios mediante flags. La estructura general de ejecución es la siguiente:

$ ./lab1 -i <imagen_entrada.bin> -r <radio> -t <umbral> -o <reporte_salida.csv> [-d]

## Detalle de las Flags:
* -i : Ruta del archivo de imagen binaria de entrada (.bin).
* -r : Radio exacto de los círculos que se desean detectar (valor entero mayor a 0).
* -t : Umbral de confianza mínimo (cantidad de "votos" necesarios en el plano de Hough para considerar que un píxel es efectivamente el centro de un círculo).
* -o : Nombre del archivo CSV de salida donde se guardarán las coordenadas detectadas.
* -d : (Opcional) Flag de "Debug Dump". Si se incluye, el programa exportará dos archivos adicionales: `preprocesada.bin` (imagen sin ruido) y `ruido.bin` (diferencia entre original y preprocesada).

# Ejemplos de Ejecución (Distintos Radios y Umbrales)

$ ./lab1 -i ejemplo1.bin -t 100 -r 35 -d -o salida.csv

$ ./lab1 -i ejemplo1.bin -t 103 -r 60 -d -o salida.csv

$ ./lab1 -i ejemplo2.bin -t 103 -r 40 -d -o salida.csv

$ ./lab1 -i ejemplo2.bin -t 100 -r 60 -d -o salida.csv

$ ./lab1 -i ejemplo3.bin -t 100 -r 100 -d -o salida.csv

## Verificación Visual:
Para ver la salida gráfica de cada ejecución (superponiendo los centros detectados sobre la imagen preprocesada), se debe ejecutar el siguiente comando de Python una vez que el programa en C haya finalizado exitosamente:

$ python3 python/VerificacionVisual.py -i preprocesada.bin -c salida.csv
