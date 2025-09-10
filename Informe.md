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
Elimina espacios en blanco y caracteres de control al inicio y final de una cadena.
#### Como funciona? <br>
+ Revisa que la cadena exista.
+ Calcula su longitud.
+ Encuentra el primer carácter válido desde el inicio.
+ Encuentra el último carácter válido desde el final.
+ Copia solo ese contenido “útil” al inicio.
+ Cierra la cadena limpia.


### 2. SepararLineaConfiguracion(char linea[], char resultado[][])
Divide una línea del archivo configuracion.txt usando la coma (,) como separador. Y lo deja en resultado[i]
#### Como funciona?
+ Revisa si la línea está vacía, no hace nada
+ Convierte la línea a un flujo de texto para poder partirla fácil.
+ Va leyendo fragmentos separados por comas mediante getline 
+ Guarda cada fragmento en resultado y le aplica trim para limpiar espacios.
+ Completa lo que falte llenando las posiciones vacías del arreglo con cadenas vacías ("").

### 3. SepararLineaPacientes(char linea[], char resultado[][])
Divide una línea del archivo pacientes.csv usando el punto y coma (;) como separador. y lo deja en resultado[i]
#### Como funciona? 
+ Revisa si la línea está vacía, no hace nada.
+ Convierte la línea en un flujo de texto para poder dividirla fácilmente.
+ Va leyendo fragmentos separados por punto y coma ( ; ).
+ Guarda cada fragmento en el arreglo resultado y lo limpia con trim para quitar espacios sobrantes.
+ Se detiene después de 11 fragmentos, porque esa es la cantidad de campos que tiene la estructura de pacientes.

### 4. CadenasAEstructuraConfiguracion(char datos[][])
Convierte un arreglo de cadenas (datos) en una estructura InformacionArchivoConfiguracion, en otras palabras Convierte los tokens de texto que salieron de SepararLineaConfiguracion en una estructura InformacionArchivoConfiguracion lista para usarse en el programa.

#### Como funciona?
+ Crea una variable local de tipo InformacionArchivoConfiguracion
+ Copia el primer token en TipoDeSensor
+ Convierte el segundo token a número decimal (double) usando la funcion Atof
+ Convierte el tercer token a número decimal (double) tambien usando atof
+ Devuelve la estructura completa con los tres campos ya inicializados.

### 5. CadenasAEstructuraPacientes(char datos[][])
Convierte un arreglo de cadenas (datos) en una estructura InformacionArchivoPaciente.
#### Como funciona? 
+ Crea una variable local P de tipo InformacionArchivoPaciente.
+ Convierte el primer token (datos[0]) a número entero mediante atoi
+ Define una mini-función interna llamada copy_safe
+ Usa copy_safe para llenar cada campo de texto en la estructura
+ Devuelve la estructura P ya llena.

##### Como y para que funciona Copy_safe 
La función copy_safe es una pequeña utilidad que se usa para copiar cadenas de manera segura dentro de los campos de la estructura de pacientes. Su objetivo principal es evitar errores de desbordamiento de memoria y asegurar que todas las cadenas terminen correctamente en \0.

Funciona de la siguiente manera: recibe un destino, una cadena de origen y la capacidad máxima del campo. Si la cadena de origen existe, copia como máximo capacidad – 1 caracteres para no sobrepasar el espacio disponible, y al final coloca el carácter nulo que marca el fin de la cadena. Si la cadena de origen no existe, simplemente deja el destino vacío.

### 6. FechaMayor(const char fecha1[], const char fecha2[])
booleano que compara dos fechas con el formato: "DD/MM/AAAA HH:MM:SS.mmm", devuleve **True** si fecha1 es *Mayor* a fecha2. 

#### Como funciona? 
+ Crea grupos de enteros para guardar los componentes de cada fecha: día, mes, año, hora, minuto, segundo y milisegundo.
+ Extrae cada parte con sscanf (Sirve para leer datos desde un texto (string) siguiendo un formato específico y guardarlos en variables)
+ Compara de mayor a menor jerarquía
+ Retorna true apenas encuentra un valor mayor en fecha1, o false si es menor.

### 7. LeerArchivoConfiguracion
Lee el archivo configuracion.txt, y guarda cada configuración en un arreglo. Rellenando informacionArchivoConfiguracion
+ Abre el archivo en modo lectura ios::in
+ Inicializa contador
+ Lee el archivo línea por línea
+ Limpia la línea con TRIM
+ Separa la línea en tokens mediante separar linea configuración
+ Convierte los tokens en estructura mediante Cadenas A estructuras Configuración
+ Copia los valores al arreglo de salida
+ Cierra el archivo

### 8. Leer Pacientes
Para esta tarea se usaron 3 distintas funciones: 

#### 8.1 ContarPacientesVaidos
Cuenta cuántas filas válidas hay en pacientes.csv mediante getline para luego reservar memoria exacta en heap. Ignora líneas vacías y comentarios.

#### 8.2 leerArchivoPacientes
Carga todas las filas válidas de pacientes.csv en un arreglo dinámico (heap) y devuelve el puntero a ese arreglo. Además, te deja en cantidad cuántos pacientes se cargaron.

#### 8.3 Liberar Pacientes
Libera el espacio de memoria de el arreglo creado en heap 

### 9. DetectarAnomalia(const lecturaSensor&, configuraciones[], cantidad)
Booleano que Evalúa si una lectura está fuera de los rangos definidos en las configuraciones. Y retorna **TRUE** si hay anomalía, **FALSE** si está dentro del rango.

#### Como funciona? 
+ Recibe una lectura de sensor con tipo y valor/es, configuraciones y cantidad de configuraciones
+ Compara si los datos de la lectura sobrepasan los limites de configuracion
+ Retorna True cuando se sobrepasan 

### 10. EnteroAChar(int, char[])
Convierte un número entero a cadena (char[]), por medio de sprintf que funciona con formato 

### 11. GenerarReportePaciente
Genera un archivo de texto con todas las mediciones de un paciente específico.
Ordena por fecha, clasifica cada lectura (NORMAL/ANÓMALA) y calcula estadísticas (min, max, promedio).

#### Como funciona? 
+ Convierte el ID a texto, filtra todas las mediciones del paciente y las ordena por fecha (antiguas → recientes).
+ abre un archivo mediciones_paciente_<ID>.txt.
+ Por cada tipo de sensor detecta anomalias y calcula promedios. 
+ Escribe un resumen por sensor, cierra el archivo y libera memoria temporal.

### 12. LeerArchivoBinario(Sala&)
Lee el archivo binario .bsf con la estructura de sala, máquinas, mediciones y lecturas.
#### Como funciona? 
+ Abre el archivo .bsf en modo binario
+ Limpia la Sala (libera e inicializa) y lee cabecera: IDdelaSala y número de máquinas.
+ Reserva el arreglo de Máquinas y, para cada una, lee su ID y número de mediciones; luego reserva esas mediciones.
+ Para cada Medición lee IDdelPaciente, FechaYHora y NumeroDeLecturas
+ Para cada lectura lee tipo de sensor y valor 1 , en el caso de que TipoSensor == 'P' tambien lee valor 2, si no este vale 0.0
+ Cierra el archivo y reporta.

### 13. GenerarReporteAnomaliasTodosPacientes
Genera un archivo anomalias.txt con todas las lecturas anómalas de todos los pacientes.
Ordena mediciones por fecha y lista anomalías por paciente.

#### Como Funciona? 
+ Abre anomalias.txt y escribe un encabezado.
+ Recorre todas las máquinas/mediciones y arma una lista de IDs de pacientes únicos (máx. 1000).
+ Para cada paciente: junta todas sus mediciones, las ordena por fecha.
+ Recorre cada lectura (omite ECG ‘E’), usa DetectarAnomalia y escribe las que salgan anómalas (para ‘P’ muestra sist/dia).
+ Si un paciente no tiene anomalías, lo indica.
+ Cierra el archivo y muestra el total de anomalías escritas.

### 14. AnomaliaParaECG
un booleano que evalúa lecturas de tipo ECG ('E') de un paciente y determina si superan los rangos configurados.
Devuelve **TRUE** si hay anomalías en las ondas ECG.

#### Como funciona?
+ Recorre las mediciones del paciente y obtiene el mínimo y máximo de lecturas ECG (‘E’) (usa ValordeMedida1).
+ Si no hay ECG, retorna false.
+ Lee de la configuración los límites min/max para 'E'.
+ Compara |max| + |min| (amplitud medida) contra |confMax| + |confMin|.
+ Devuelve true si la amplitud medida supera la configurada (anómalo), si no, false.

### 15. GenerarArchivoAnomaliaECG
Crea un archivo binario pacientes_ecg_anomalos.dat con los pacientes que presentan anomalías en ECG.

#### Como funciona? 
+ Abre pacientes_ecg_anomalos.dat en binario para escribir.
+ Recorre todas las máquinas y sus mediciones; para cada ID de paciente, llama AnomaliaParaECG (por máquina).
+ Si el ECG del paciente es anómalo, escribe en el binario: Id del paciente, Cantidad de lecturas, y por cada lectura la fechayhora y el Valor de medidad
+ Cierra el archivo

### 16. InicializarSala(Sala&)
Pone en cero y en nullptr los campos de una Sala.

### 17. LiberarSala(Sala&)

Libera toda la memoria dinámica asociada a una sala, delete.
