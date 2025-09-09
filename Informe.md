<h1 align="center"> INFORME PROYECTO C++ </h1>
A continuación se presetara un informe detallado del proyecto de monitoreo de sistemas biometricos, en el cual se presetará la explicación de las estructuras utilizadas en el codigo, como la descripción de las funciones implemetadas.

<h2 align="center"> ESTRUCTURAS </h2>
A continuación se explicara las estructuras que se usaron con sus respectivos tipos de datos.

#### InformacionArchivoConfiguracion:
Estas estructuras fueron creadas para almacenar la informacion del archivo configuracion.txt con tres tipos de datos, un arreglo de char para almacenar el **Tipo de sensor**, y dos double **min y max** para almacenar los limites de los intervalos de cada tipo de sensor.
#### InformacionArchivoPaciente:
Esta estructura fue creada para almacenar la informacion del archivo pacientes.csv , con un conjunto de variables que representan toda la informaion de un paciente en el archivo, esta contituida por: IdPersona; tipoDeDocumento; Documento; nombre; apellido; FechaNacimiento; telefono; email; TipoDeSangre; entidadSalud; medicinaPrepagada;

### Estructura de la salas 
Estas estructuras tienen una dinamica especial ya que es como un arbol de estructuras que contienen la informacion que se necesita, es decir que existen estructuras dentro de otras estrcuturas, en este caso para la explicación de las estructuras vamos a explicar desde la estructura mas general que en este caso es **Sala** hasta la estutura mas "Pequeña" que es **Lectura sensor**. 

Estas estructuras tienen una conformación de: 
Sala ----> Maquina -----> Medicion----> LecturaSensor

#### SALA
+ ID de la sala
+ Numero de maquina
+ Arreglo de Maquinas (un puntero de la estructura de Maquina)

#### Maquinas
+ ID Maquina
+ Numero De Mediciones
+ Arreglo de Mediciones (un puntero de la estructura de mediciones)
#### Medición
+ ID del Paciente
+ Fecha y Hora
+ Numero de lecturas
+ Arreglo de lecturas  (un puntero de la estructura de Lectura Sensor)
#### LecturaSensor
+ Valor medida 1
+ Valor medida 2

####  **¿Por qué un puntero?** <br>
Se necesita que sea un apuntador ya que hay que crear arreglos con tamaño n (tamaño que nos brindara la propia estructura en **Numero de maquina** ), creamos este arreglo en memoria heap y accedemos a el por medio de este apuntador, que nos brindará el primer espacio de este arreglo o en su defecto la posición que le solicitemos gracias a su sintaxis. Esta dinamica se aplica en las demas estructuras.

<h2 align="center"> Funciones </h2>
A continuación se presentarán las funciones utilizadas y su funcionamiento en el codigo.

### 1. Trim (char str[])
Elimina espacios en blanco y caracteres de control al inicio y final de una cadena

### 2. SepararLineaConfiguracion(char linea[], char resultado[][])
Divide una línea del archivo configuracion.txt usando la coma (,) como separador. Y lo deja en resultado[i]

### 3. SepararLineaPacientes(char linea[], char resultado[][])
Divide una línea del archivo pacientes.csv usando el punto y coma (;) como separador. y lo deja en resultado[i]

### 4. CadenasAEstructuraConfiguracion(char datos[][])
Convierte un arreglo de cadenas (datos) en una estructura InformacionArchivoConfiguracion

### 5. CadenasAEstructuraPacientes(char datos[][])
Convierte un arreglo de cadenas (datos) en una estructura InformacionArchivoPaciente.

### 6. FechaMayor(const char fecha1[], const char fecha2[])
booleano que compara dos fechas con el formato: "DD/MM/AAAA HH:MM:SS.mmm", devuleve **True** si fecha1 es *Mayor* a fecha2. 

### 7. LeerArchivoConfiguracion
Lee el archivo configuracion.txt, y guarda cada configuración en un arreglo. Rellenando informacionArchivoConfiguracion

### 8. leerArchivoPacientes
ee el archivo pacientes.csv, línea por línea, y convierte cada una en InformacionArchivoPaciente.

### 9. DetectarAnomalia(const lecturaSensor&, configuraciones[], cantidad)
Booleano que Evalúa si una lectura está fuera de los rangos definidos en las configuraciones. Y retorna **TRUE** si hay anomalía, **FALSE** si está dentro del rango.

### 10. EnteroAChar(int, char[])
Convierte un número entero a cadena (char[]).

### 11. GenerarReportePaciente
Genera un archivo de texto con todas las mediciones de un paciente específico.
Ordena por fecha, clasifica cada lectura (NORMAL/ANÓMALA) y calcula estadísticas (min, max, promedio).

### 12. LeerArchivoBinario(Sala&)
Lee el archivo binario .bsf con la estructura de sala, máquinas, mediciones y lecturas.

### 13. GenerarReporteAnomaliasTodosPacientes
Genera un archivo anomalias.txt con todas las lecturas anómalas de todos los pacientes.
Ordena mediciones por fecha y lista anomalías por paciente.

### 14. AnomaliaParaECG
un booleano que evalúa lecturas de tipo ECG ('E') de un paciente y determina si superan los rangos configurados.
Devuelve **TRUE** si hay anomalías en las ondas ECG.

### 15. GenerarArchivoAnomaliaECG
Crea un archivo binario pacientes_ecg_anomalos.dat con los pacientes que presentan anomalías en ECG.

### 16. InicializarSala(Sala&)
Pone en cero y en nullptr los campos de una Sala.

### 17. LiberarSala(Sala&)

Libera toda la memoria dinámica asociada a una sala, delete.
