# Compilación
El proyecto incluye un archivo `Makefile` para facilitar su compilación. Para compilar el código fuente, simplemente abre una terminal en el directorio raíz del proyecto y ejecutar:

$ make all

Esto generará el archivo ejecutable llamado `lab1`. 
Para limpiar los binarios y archivos generados posteriormente, se puede usar:

$ make clean

# Entorno Virtual y Dependencias
Para ejecutar los scripts de validación y visualización de resultados (como `VerificacionVisual.py`), es necesario configurar un entorno de Python con las librerías adecuadas.

1. **Crear el entorno virtual**:
   $ python3 -m venv venv

2. **Activar el entorno**:
   $ source venv/bin/activate

3. **Instalar dependencias**:
   Una vez activado el entorno, instalar los paquetes necesarios usando el archivo `requirements.txt`:
   $ pip install -r requirements.txt

# Explicación de la Ejecución
El programa se ejecuta desde la línea de comandos pasando los parámetros necesarios mediante flags. La estructura general de ejecución es la siguiente:

$ ./lab1 -i <imagen_entrada.bin> -r  <radio>  -t  <umbral>  -o  <reporte_salida.csv>  [-d]

## Detalle de las Flags:
* -i : Ruta del archivo de imagen binaria de entrada (.bin).
* -r : Radio exacto de los círculos que se desean detectar (valor entero mayor a 0).
* -t : Umbral de confianza mínimo (cantidad de "votos" necesarios en el plano de Hough para considerar que un píxel es efectivamente el centro de un círculo).
* -o : Nombre del archivo CSV de salida donde se guardarán las coordenadas detectadas.
* -d : (Opcional) Flag de "Debug Dump". Si se incluye, el programa exportará dos archivos adicionales: `preprocesada.bin` (imagen sin ruido) y `ruido.bin` (diferencia entre original y preprocesada).


# Ejemplos de Ejecución (Distintos Radios y Umbrales)
./lab1 -i ejemplo1.bin -t 100 -r 35 -d -o salida.csv

./lab1 -i ejemplo1.bin -t 103 -r 60 -d -o salida.csv

./lab1 -i ejemplo2.bin -t 103 -r 40 -d -o salida.csv

./lab1 -i ejemplo2.bin -t 100 -r 60 -d -o salida.csv

./lab1 -i ejemplo3.bin  -t 100 -r 100 -d -o salida.csv

## para ver la salida de cada ejecucion se debe usar (debe estar activado el venv):
python python/VerificacionVisual.py -i preprocesada.bin -c salida.csv