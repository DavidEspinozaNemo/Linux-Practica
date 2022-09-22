# Proyecto 01 - Principio de Sistemas Operativos IC6600

## Integrantes
```
  David Jose Espinoza Soto - 2016012024
  Junior
```

## Introducción

El objetivo de este proyecto es comparar el rendimiento de realizar una tarea colaborativa utilizando múltiples hilos. Para ello se deberá desarrollar una versión multihilos del programa llamado grep que permite buscar palabras sobre varios archivos e imprimir el archivo en donde aparece.

En este proyecto se deben realizar múltiples pruebas para determinar la cantidad óptima de hilos para ejecutar este tipo de tarea sobre gran cantidad de archivos.

### Uso del programa


El programa grep recibe una expresión regular y una lista de archivos:

```
    grep [expresión regular] archivo1 archivo2 archivo3 ...
```

Por omisión el programa muestra la línea del archivo en donde se encuentra la expresión regular.

### Manejo de expresiones regulares

El lenguaje C por sí mismo no maneja expresiones regulares, sin embargo se puede usar la librería "regex.h" que cuenta con las funciones regcomp y regexec que permiten realizar esta tarea. La función regcomp se debe ejecutar inicialmente y recibe la expresión regular ingresada por comando, luego la función regexec se debe ejecutar sobre cada una de las líneas del archivo.

### Manejo de múltiples hilos

El programa debe contar con múltiples hilos que se encarguen de inspeccionar los diferentes archivos. Debido a que la búsqueda puede abarcar una gran cantidad de archivos no sería eficiente crear un hilo por cada archivo. Es por ello que es necesario crear un conjunto compartido de hilos (pool de hilos) que incluya solo una cantidad limitada de hilos. Como parte del proyecto se deben realizar diferentes pruebas (con muchos archivos) para determinar un tamaño "óptimo" del pool de hilos. Se utilizará el tiempo de ejecución para determinar dicho tamaño óptimo

En este caso se utilizará un pool de hilos estático, es decir, el programa creará la totalidad de los hilos al inicio del programa y únicamente utilizará estos para realizar todo el trabajo. Tal como usted debe saber, a los hilos que terminen de buscar en un archivo se les deberá asignar otro archivo para realizar la búsqueda hasta terminar con todos los archivos.

## Definición de estructuras de datos

## Descripción detallada y explicación de los componentes principales del programa

## Mecanismo de creación y comunicación de hilos

## Pruebas de rendimiento

## Conclusiones
