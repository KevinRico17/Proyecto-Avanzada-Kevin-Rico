<h1 align="center"> Sistemas de Monitoreo Biomédico </h1>
<h2 align="center"> Guía paso a paso para pobrar el codigo </h2>

En este proyecto se desarrollarán componentes esenciales para un sistema de procesamiento de datos de sensores biomédicos. El proyecto simula un escenario real donde un ingeniero debe procesar lecturas de múltiples sensores para evaluar los signos vitales de pacientes en una unidad de cuidados intensivos. En esta simulación usted deberá seguir los siguientes pasos para comprobar el funcionamiento del proyecto.

## Pre-requisitos para compilar el programa ## 
Se necesita que cree un conjunto de carpetas con la siguiente estructura: 

Proyecto/ <br> 
├── Data/ <br>
&nbsp; &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; ├── configuracion.txt <br>
&nbsp; &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;   ├── pacientes.csv <br>
&nbsp; &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;  └── patient_readings_simulation.bsf <br>
├── libs/ <br>
&nbsp; &nbsp;&nbsp;&nbsp;&nbsp;&nbsp; &nbsp;&nbsp; └── Nuevas_Estructuras.hh <br>
├── Proyecto_KR.cpp <br>

Los archivos los podrá encontrar en la carpeta de: "ARCHIVOS PROYECTO", asegurese de que los archivos tengan los nombres correctos.

## Compilación del Programa ##
En la parte superior del compilador encontrará un icono de "Play", con este podra correr el programa, cuando empiece la ejecución dirijase a la parte inferior de su pantalla al apartado de "Terminal", ahí podrá ver el Menú del programa el cual tiene la siguiente estructura: 

1. Cargar archivo de configuracion y datos de pacientes
2. Leer archivo .bsf
3. Generar reporte de anomalias 
4. Calcular estadisticas
5. Exportar datos procesados
6. Salir

A continuación solo deberá introducir el numeral de la instrucción que quiere realizar mediante la consola, sin embargo se le expliacará el funcionamiento de cada numeral para que tenga clara la dinamica del programa 

<h2 align="center"> ¿Que hace cada item? </h2>

### 1.Cargar archivo de configuracion y datos de pacientes ###
Carga todos los archivos necesarios para inicializar la base de datos y estructuras internas.

### 2.Leer archivo .bsf ###
Lee el Archivo Binario donde se encuentra toda la información de las lecturas de los pacientes.

### 3.Generar reporte de anomalias  ###
Crea un archivo llamdado "anomalias.txt" donde se encuentran las lecturas anómalas (excepto ECG), de **TODOS LOS PACIENTES** agrupadas por paciente y por orden cronologico.

### 4.Calcular estadisticas ###
Solicita el ID algún paciente (En numero entero) y genera un archivo llamado "mediciones_paciente_<ID>.txt" (<ID> es el numero que ingresaste anteriormente) con sus lecturas ordenadas y clasificadas. (Debes de saber el ID del paciente al cual quieras generar el reporte)

### 5. Exportar datos procesados ###
Detecta pacientes con lecturas ECG fuera de los rangos normales y los exporta a un archivo llamado "pacientes_ecg_anomalos.dat." este archivo no se podrá leer normalmente porque es un archivo de tipo **Binario**.

### 6. Salir ###
Finaliza la ejecución del programa.

Se espera que el proyecto se halla ejecutado de manera correcta :3

## Autor

**Kevin Santiago Rico Garcia** <br>
**Proyecto Programación avanzada**<br>
**Pontificia Universidad Javeriana**<br>
