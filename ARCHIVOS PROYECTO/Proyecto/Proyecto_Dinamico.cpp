
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <sstream>
#include <cstdio>
#include <cstddef>
#include <cstdint>
#include <cmath>
#include "libs/Nuevas_Estructuras.hh"
using namespace std;

const int MAX_VALORES = 10;
const int MAX_CARACTERES = 50;


void trim(char str[])
{
    if (!str) return;
    int len = strlen(str);
    int start = 0;
    while (start < len && (str[start] == ' ' || str[start] == '\t' || str[start] == '\r' || str[start] == '\n'))
        start++;
    int end = len - 1;
    while (end >= start && (str[end] == ' ' || str[end] == '\t' || str[end] == '\r' || str[end] == '\n'))
        end--;
    int j = 0;
    for (int i = start; i <= end; ++i, ++j)
        str[j] = str[i];
    str[j] = '\0';
}


void SepararLineaConfiguracion(char linea[], char resultado[MAX_VALORES][MAX_CARACTERES])
{
    if (linea[0] == '\0') return;
    string sline(linea);
    stringstream ss(sline);
    string token;
    int token_count = 0;
    char delim = ',';

    while (getline(ss, token, delim) && token_count < MAX_VALORES) {
        strncpy(resultado[token_count], token.c_str(), MAX_CARACTERES - 1);
        resultado[token_count][MAX_CARACTERES - 1] = '\0';
        trim(resultado[token_count]);
        token_count++;
    }
    for (int i = token_count; i < MAX_VALORES; ++i)
        resultado[i][0] = '\0';
}

void SepararLineaPacientes(char linea[], char resultado[MAX_VALORES][MAX_CARACTERES])
{
    stringstream ss(linea);
    string token;
    int token_count = 0;
    char delim = ';';

    while (getline(ss, token, delim) && token_count < 11) {
        strcpy(resultado[token_count], token.c_str());
        trim(resultado[token_count]);
        token_count++;
    }
}

InformacionArchivoConfiguracion CadenasAEstructuraConfiguracion(char datos[MAX_VALORES][MAX_CARACTERES])
{
    InformacionArchivoConfiguracion ConfiguracionOficial;
    strcpy(ConfiguracionOficial.TipoDeSensor, datos[0]);
    ConfiguracionOficial.Min = atof(datos[1]);
    ConfiguracionOficial.Max = atof(datos[2]);
    return ConfiguracionOficial;
}

InformacionArchivoPaciente CadenasAEstructuraPacientes(char datos[MAX_VALORES][MAX_CARACTERES]) {
    InformacionArchivoPaciente P;
    P.IdPersona = atoi(datos[0]);
    auto copy_safe = [](char* dst, const char* src, size_t cap) {
        if (!src) { dst[0] = '\0'; return; }
        strncpy(dst, src, cap - 1);
        dst[cap - 1] = '\0';
    };
    copy_safe(P.tipoDeDocumento,   datos[1],  sizeof(P.tipoDeDocumento));
    copy_safe(P.Documento,         datos[2],  sizeof(P.Documento));
    copy_safe(P.nombre,            datos[3],  sizeof(P.nombre));
    copy_safe(P.apellido,          datos[4],  sizeof(P.apellido));
    copy_safe(P.FechaNacimiento,   datos[5],  sizeof(P.FechaNacimiento));
    copy_safe(P.telefono,          datos[6],  sizeof(P.telefono));
    copy_safe(P.email,             datos[7],  sizeof(P.email));
    copy_safe(P.TipoDeSangre,      datos[8],  sizeof(P.TipoDeSangre));
    copy_safe(P.entidadSalud,      datos[9],  sizeof(P.entidadSalud));
    copy_safe(P.medicinaPrepagada, datos[10], sizeof(P.medicinaPrepagada));
    return P;
}

bool FechaMayor(const char fecha1[], const char fecha2[])
{
    int dia1, mes1, anio1, hora1, min1, seg1, mseg1;
    int dia2, mes2, anio2, hora2, min2, seg2, mseg2;

    sscanf(fecha1, "%d/%d/%d %d:%d:%d.%d", &dia1, &mes1, &anio1, &hora1, &min1, &seg1, &mseg1);
    sscanf(fecha2, "%d/%d/%d %d:%d:%d.%d", &dia2, &mes2, &anio2, &hora2, &min2, &seg2, &mseg2);

    if (anio1 != anio2) return anio1 > anio2;
    if (mes1 != mes2) return mes1 > mes2;
    if (dia1 != dia2) return dia1 > dia2;
    if (hora1 != hora2) return hora1 > hora2;
    if (min1 != min2) return min1 > min2;
    if (seg1 != seg2) return seg1 > seg2;
    return mseg1 > mseg2;
}

void LeerArchivoConfiguracion(InformacionArchivoConfiguracion configuraciones[], int &cantidad, int cap)
{
    fstream archivo("Data/configuracion.txt", ios::in);
    char linea[200];
    cantidad = 0;

    if (!archivo.is_open()) {
        cout << "No se pudo abrir el archivo de configuración.\n";
        return;
    }

    while (archivo.getline(linea, sizeof(linea))) {
        trim(linea);
        if (linea[0] == '\0' || linea[0] == '#') continue;

        char datos[MAX_VALORES][MAX_CARACTERES];
        for (int t = 0; t < MAX_VALORES; ++t) datos[t][0] = '\0';

        SepararLineaConfiguracion(linea, datos);
        if (strlen(datos[0]) == 0) continue;

        InformacionArchivoConfiguracion conf = CadenasAEstructuraConfiguracion(datos);
        trim(conf.TipoDeSensor);

        if (cantidad >= cap) {
            cout << "Se excedió la capacidad de configuraciones (" << cap << "). Ignorando líneas extras.\n";
            break;
        }

        strncpy(configuraciones[cantidad].TipoDeSensor, conf.TipoDeSensor,
                sizeof(configuraciones[cantidad].TipoDeSensor) - 1);
        configuraciones[cantidad].TipoDeSensor[sizeof(configuraciones[cantidad].TipoDeSensor) - 1] = '\0';
        configuraciones[cantidad].Min = conf.Min;
        configuraciones[cantidad].Max = conf.Max;

        cantidad++;
    }

    archivo.close();
}


void LeerArchivoPacientes(InformacionArchivoPaciente pacientes[], int &cantidad, int cap) {
    fstream archivo("Data/pacientes.csv", ios::in);
    char linea[512];
    cantidad = 0;

    if (!archivo.is_open()) {
        cout << "No se pudo abrir el archivo de pacientes.\n";
        return;
    }

    while (archivo.getline(linea, sizeof(linea))) {
        char tmp[512];
        strncpy(tmp, linea, sizeof(tmp)-1); tmp[sizeof(tmp)-1] = '\0';
        trim(tmp);
        if (tmp[0] == '\0' || tmp[0] == '#') continue;

        if (cantidad >= cap) {
            cout << "[WARN] Se excedió la capacidad de pacientes (" << cap << "). Ignorando filas extras.\n";
            break;
        }

        char datos[MAX_VALORES][MAX_CARACTERES];
        for (int t = 0; t < MAX_VALORES; ++t) datos[t][0] = '\0';

        SepararLineaPacientes(tmp, datos);

        pacientes[cantidad] = CadenasAEstructuraPacientes(datos);
        cantidad++;
    }

    archivo.close();
}


bool DetectarAnomalia(const lecturaSensor &lectura, InformacionArchivoConfiguracion configuraciones[], int cantidadConfig)
{
    if (lectura.tipodesensor == 'E') {
        return false;
    }

    if (lectura.tipodesensor == 'P') {
        bool anomSis = false, anomDia = false;

        for (int i = 0; i < cantidadConfig; ++i) {
            trim(configuraciones[i].TipoDeSensor);

            if (strcmp(configuraciones[i].TipoDeSensor, "P_SIS") == 0) {
                if (lectura.ValordeMedida1 < configuraciones[i].Min ||
                    lectura.ValordeMedida1 > configuraciones[i].Max) {
                    anomSis = true;
                }
            }

            if (strcmp(configuraciones[i].TipoDeSensor, "P_DIA") == 0) {
                if (lectura.ValordeMedida2 < configuraciones[i].Min ||
                    lectura.ValordeMedida2 > configuraciones[i].Max) {
                    anomDia = true;
                }
            }
        }
        return anomSis || anomDia;
    }

    for (int i = 0; i < cantidadConfig; ++i) {
        trim(configuraciones[i].TipoDeSensor);
        if (strlen(configuraciones[i].TipoDeSensor) == 1 &&
            configuraciones[i].TipoDeSensor[0] == lectura.tipodesensor) {
            return lectura.ValordeMedida1 < configuraciones[i].Min ||
                   lectura.ValordeMedida1 > configuraciones[i].Max;
        }
    }

    return false;
}

void EnteroAChar(int numeroEntero, char numeroChar[])
{
    sprintf(numeroChar, "%d", numeroEntero);
}


void GenerarReportePaciente(
    int IdPacienteEntero,
    Sala &sala,
    InformacionArchivoConfiguracion configuraciones[],
    int totalConfig)
{
    
    char idPaciente[20];
    EnteroAChar(IdPacienteEntero, idPaciente);

    
    int totalFiltradas = 0;
    for (int i = 0; i < static_cast<int>(static_cast<unsigned char>(sala.NumerodeMaquinas)); i++) {
        Maquina &maq = sala.ArregloMaquina[i];
        for (int j = 0; j < maq.NumeroDeMediciones; j++) {
            if (strcmp(maq.ArregloDeMediciones[j].IDdelPaciente, idPaciente) == 0)
                totalFiltradas++;
        }
    }

    if (totalFiltradas == 0) {
        cout << "No se encontraron mediciones para el paciente " << idPaciente << endl;
        return;
    }

    
    Medicion* filtradas = new Medicion[totalFiltradas];
    int idx = 0;
    for (int i = 0; i < static_cast<int>(static_cast<unsigned char>(sala.NumerodeMaquinas)); i++) {
        Maquina &maq = sala.ArregloMaquina[i];
        for (int j = 0; j < maq.NumeroDeMediciones; j++) {
            if (strcmp(maq.ArregloDeMediciones[j].IDdelPaciente, idPaciente) == 0) {
                filtradas[idx++] = maq.ArregloDeMediciones[j]; 
            }
        }
    }

  
    for (int i = 0; i < totalFiltradas - 1; i++) {
        for (int j = 0; j < totalFiltradas - i - 1; j++) {
            if (FechaMayor(filtradas[j].FechaYHora, filtradas[j + 1].FechaYHora)) {
                Medicion temp = filtradas[j];
                filtradas[j] = filtradas[j + 1];
                filtradas[j + 1] = temp;
            }
        }
    }

    
    char nombreArchivo[100];
    sprintf(nombreArchivo, "mediciones_paciente_%s.txt", idPaciente);
    ofstream archivo(nombreArchivo);

    if (!archivo.is_open()) {
        cout << "Error al crear el archivo de reporte.\n";
        delete[] filtradas;
        return;
    }

    archivo << "==== Reporte de Mediciones - Paciente: " << idPaciente << " ====\n\n";

   
    char sensoresProcesados[20] = {0};
    int cantSensoresProcesados = 0;

    for (int s = 0; s < totalFiltradas; s++) {
        for (int l = 0; l < filtradas[s].NumeroDeLecturas; l++) {
            char tipo = filtradas[s].lecturas[l].tipodesensor;

            bool yaProcesado = false;
            for (int k = 0; k < cantSensoresProcesados; k++) {
                if (sensoresProcesados[k] == tipo) { yaProcesado = true; break; }
            }
            if (yaProcesado) continue;

            sensoresProcesados[cantSensoresProcesados++] = tipo;
            archivo << ">> Sensor " << tipo << " <<\n";

            if (tipo == 'P') {
                
                double p_sis_min = 0.0, p_sis_max = 0.0;
                double p_dia_min = 0.0, p_dia_max = 0.0;
                bool hasP_SIS = false, hasP_DIA = false;
                for (int c = 0; c < totalConfig; c++) {
                    if (strcmp(configuraciones[c].TipoDeSensor, "P_SIS") == 0) {
                        p_sis_min = configuraciones[c].Min;
                        p_sis_max = configuraciones[c].Max;
                        hasP_SIS = true;
                    } else if (strcmp(configuraciones[c].TipoDeSensor, "P_DIA") == 0) {
                        p_dia_min = configuraciones[c].Min;
                        p_dia_max = configuraciones[c].Max;
                        hasP_DIA = true;
                    }
                }

                double minS = 1e9, maxS = -1e9, sumS = 0.0;
                double minD = 1e9, maxD = -1e9, sumD = 0.0;
                int cntS = 0, cntD = 0;

                for (int i = 0; i < totalFiltradas; i++) {
                    for (int l2 = 0; l2 < filtradas[i].NumeroDeLecturas; l2++) {
                        if (filtradas[i].lecturas[l2].tipodesensor == 'P') {
                            double sist = filtradas[i].lecturas[l2].ValordeMedida1;
                            double dia =  filtradas[i].lecturas[l2].ValordeMedida2;

                            bool anomSis = false, anomDia = false;
                            if (hasP_SIS && (sist < p_sis_min || sist > p_sis_max)) anomSis = true;
                            if (hasP_DIA && (dia  < p_dia_min || dia  > p_dia_max)) anomDia = true;

                            if (!hasP_SIS && !hasP_DIA) {
                                bool anom = DetectarAnomalia(filtradas[i].lecturas[l2], configuraciones, totalConfig);
                                archivo << filtradas[i].FechaYHora << " = " << sist << " / " << dia
                                        << " [" << (anom ? "ANOMALO" : "NORMAL") << "]\n";
                            } else {
                                if (anomSis && anomDia)
                                    archivo << filtradas[i].FechaYHora << " = " << sist << " / " << dia << " [ANOMALO: SIS & DIA]\n";
                                else if (anomSis)
                                    archivo << filtradas[i].FechaYHora << " = " << sist << " / " << dia << " [ANOMALO: SIS]\n";
                                else if (anomDia)
                                    archivo << filtradas[i].FechaYHora << " = " << sist << " / " << dia << " [ANOMALO: DIA]\n";
                                else
                                    archivo << filtradas[i].FechaYHora << " = " << sist << " / " << dia << " [NORMAL]\n";
                            }

                            if (!isnan(sist)) { if (sist < minS) minS = sist; if (sist > maxS) maxS = sist; sumS += sist; cntS++; }
                            if (!isnan(dia))  { if (dia  < minD) minD = dia;  if (dia  > maxD) maxD = dia;  sumD += dia;  cntD++; }
                        }
                    }
                }

                archivo << "  SISTÓLICA -> Min: " << (cntS ? minS : 0)
                        << "  Max: " << (cntS ? maxS : 0)
                        << "  Prom: " << (cntS ? (sumS / cntS) : 0) << "\n";
                archivo << "  DIASTÓLICA -> Min: " << (cntD ? minD : 0)
                        << "  Max: " << (cntD ? maxD : 0)
                        << "  Prom: " << (cntD ? (sumD / cntD) : 0) << "\n\n";
            } else {
                double minVal = 1e9, maxVal = -1e9, sumaVal = 0.0;
                int contador = 0;

                for (int i = 0; i < totalFiltradas; i++) {
                    for (int l2 = 0; l2 < filtradas[i].NumeroDeLecturas; l2++) {
                        if (filtradas[i].lecturas[l2].tipodesensor == tipo) {
                            double valor = filtradas[i].lecturas[l2].ValordeMedida1;

                            archivo << filtradas[i].FechaYHora << " = " << valor;
                            if (tipo != 'E') {
                                bool anomalia = DetectarAnomalia(filtradas[i].lecturas[l2], configuraciones, totalConfig);
                                archivo << " [" << (anomalia ? "ANOMALO" : "NORMAL") << "]";
                            }
                            archivo << "\n";

                            if (valor < minVal) minVal = valor;
                            if (valor > maxVal) maxVal = valor;
                            sumaVal += valor;
                            contador++;
                        }
                    }
                }

                archivo << "Min: " << (contador ? minVal : 0) << endl;
                archivo << "Max: " << (contador ? maxVal : 0) << endl;
                archivo << "Promedio: " << (contador > 0 ? sumaVal / contador : 0) << endl;
                archivo << "\n";
            }
        }
    }

    archivo.close();
    delete[] filtradas;
    cout << "Reporte generado en: " << nombreArchivo << endl;
}


void LeerArchivoBinario(Sala &SalaFromBinaryFile)
{
    fstream binary_file("Data/patient_readings_simulation_small.bsf", ios::in | ios::binary);
    if (!binary_file.is_open()) {
        cout << "Error al abrir el archivo Binario" << endl;
        return;
    }

    
    LiberarSala(SalaFromBinaryFile);
    InicializarSala(SalaFromBinaryFile);

    binary_file.read(reinterpret_cast<char*>(&SalaFromBinaryFile.IDdelaSala),
                     sizeof(SalaFromBinaryFile.IDdelaSala));
    binary_file.read(reinterpret_cast<char*>(&SalaFromBinaryFile.NumerodeMaquinas),
                     sizeof(SalaFromBinaryFile.NumerodeMaquinas));

   
    SalaFromBinaryFile.ArregloMaquina =
        new Maquina[static_cast<unsigned char>(SalaFromBinaryFile.NumerodeMaquinas)];

    
    for (int i = 0; i < static_cast<int>(static_cast<unsigned char>(SalaFromBinaryFile.NumerodeMaquinas)); i++)
    {
        Maquina& M = SalaFromBinaryFile.ArregloMaquina[i];

        binary_file.read(reinterpret_cast<char*>(&M.IDMaquina), sizeof(M.IDMaquina));
        binary_file.read(reinterpret_cast<char*>(&M.NumeroDeMediciones), sizeof(M.NumeroDeMediciones));

      
        M.ArregloDeMediciones = new Medicion[M.NumeroDeMediciones];

       
        for (int j = 0; j < M.NumeroDeMediciones; j++)
        {
            Medicion& med = M.ArregloDeMediciones[j];

           
            binary_file.read(med.IDdelPaciente, 11);
            med.IDdelPaciente[10] = '\0';

        
            binary_file.read(med.FechaYHora, 24);
            med.FechaYHora[23] = '\0';

            binary_file.read(reinterpret_cast<char*>(&med.NumeroDeLecturas),
                             sizeof(med.NumeroDeLecturas));

           
            med.lecturas = new lecturaSensor[med.NumeroDeLecturas];

          
            for (int k = 0; k < med.NumeroDeLecturas; k++)
            {
                lecturaSensor& lec = med.lecturas[k];

            
                binary_file.read(reinterpret_cast<char*>(&lec.tipodesensor), 1);

                if (lec.tipodesensor == 'P') {
                    unsigned int sist, diast;
                    binary_file.read(reinterpret_cast<char*>(&sist), sizeof(sist));
                    binary_file.read(reinterpret_cast<char*>(&diast), sizeof(diast));
                    lec.ValordeMedida1 = static_cast<double>(sist);
                    lec.ValordeMedida2 = static_cast<double>(diast);
                } else {
                    double valor;
                    binary_file.read(reinterpret_cast<char*>(&valor), sizeof(valor));
                    lec.ValordeMedida1 = valor;
                    lec.ValordeMedida2 = 0.0;
                }
            }
        }
    }

    binary_file.close();
    cout << "Archivo binario leído correctamente " << endl;
}

void GenerarReporteAnomaliasTodosPacientes(
    Sala &sala,
    InformacionArchivoConfiguracion configuraciones[],
    int totalConfig)
{
    ofstream archivo("anomalias.txt");
    if (!archivo.is_open()) {
        cout << "Error al crear anomalias.txt" << endl;
        return;
    }

    archivo << "==== REPORTE DE ANOMALÍAS DE TODOS LOS PACIENTES ====\n\n";

    
    char pacientesProcesados[1000][20];
    for (int z = 0; z < 1000; ++z) pacientesProcesados[z][0] = '\0';
    int cantPacientes = 0;

    for (int i = 0; i < static_cast<int>(static_cast<unsigned char>(sala.NumerodeMaquinas)); i++) {
        Maquina &maq = sala.ArregloMaquina[i];
        for (int j = 0; j < maq.NumeroDeMediciones; j++) {
            Medicion &med = maq.ArregloDeMediciones[j];

            bool yaExiste = false;
            for (int k = 0; k < cantPacientes; k++) {
                if (strcmp(pacientesProcesados[k], med.IDdelPaciente) == 0) { yaExiste = true; break; }
            }
            if (!yaExiste && cantPacientes < 1000) {
                strncpy(pacientesProcesados[cantPacientes], med.IDdelPaciente, 19);
                pacientesProcesados[cantPacientes][19] = '\0';
                cantPacientes++;
            }
        }
    }

    
    int totalAnomaliasEscritasGlobal = 0;

    for (int p = 0; p < cantPacientes; p++)
    {
        char *idPaciente = pacientesProcesados[p];
        archivo << "Paciente ID: " << idPaciente << "\n";

       
        int totalMed = 0;
        for (int i = 0; i < static_cast<int>(static_cast<unsigned char>(sala.NumerodeMaquinas)); i++) {
            Maquina &maq = sala.ArregloMaquina[i];
            for (int j = 0; j < maq.NumeroDeMediciones; j++) {
                if (strcmp(maq.ArregloDeMediciones[j].IDdelPaciente, idPaciente) == 0) totalMed++;
            }
        }
        
        Medicion* medicionesPaciente = new Medicion[totalMed];
        int pos = 0;
        for (int i = 0; i < static_cast<int>(static_cast<unsigned char>(sala.NumerodeMaquinas)); i++) {
            Maquina &maq = sala.ArregloMaquina[i];
            for (int j = 0; j < maq.NumeroDeMediciones; j++) {
                if (strcmp(maq.ArregloDeMediciones[j].IDdelPaciente, idPaciente) == 0) {
                    medicionesPaciente[pos++] = maq.ArregloDeMediciones[j];
                }
            }
        }

        
        for (int i = 0; i < totalMed - 1; i++) {
            for (int j = 0; j < totalMed - i - 1; j++) {
                if (FechaMayor(medicionesPaciente[j].FechaYHora, medicionesPaciente[j + 1].FechaYHora)) {
                    Medicion temp = medicionesPaciente[j];
                    medicionesPaciente[j] = medicionesPaciente[j + 1];
                    medicionesPaciente[j + 1] = temp;
                }
            }
        }

       
        bool encontroAnomalia = false;

        for (int i = 0; i < totalMed; i++) {
            Medicion &med = medicionesPaciente[i];
            for (int l = 0; l < med.NumeroDeLecturas; l++) {
                lecturaSensor &lec = med.lecturas[l];

                if (lec.tipodesensor == 'E') continue;

                bool esAnomalo = DetectarAnomalia(lec, configuraciones, totalConfig);

                
                if (esAnomalo) {
                    encontroAnomalia = true;
                    totalAnomaliasEscritasGlobal++;

                    if (lec.tipodesensor == 'P') {
                        archivo << "   " << med.FechaYHora
                                << " | Sensor P | "
                                << static_cast<int>(lec.ValordeMedida1) << "/"
                                << static_cast<int>(lec.ValordeMedida2)
                                << "  [ANOMALO]\n";
                    } else {
                        archivo << "   " << med.FechaYHora
                                << " | Sensor " << lec.tipodesensor
                                << " | " << lec.ValordeMedida1
                                << "  [ANOMALO]\n";
                    }
                }
            }
        }

        if (!encontroAnomalia) {
            archivo << "   (No se encontraron anomalías)\n";
        }

        archivo << "\n";
        delete[] medicionesPaciente;
    }

    archivo.close();
    cout << "Reporte generado en anomalias.txt. Total anomalías escritas: "
         << totalAnomaliasEscritasGlobal << endl;
}


bool AnomaliaParaECG(Medicion Mediciones[], int numeroMediciones, char idPaciente[],
                     InformacionArchivoConfiguracion configuraciones[], int TotalConfiguracion)
{
    double ValorMin = 1e9;
    double ValorMax = -1e9;
    double ValorAux = 0;

    for (int i = 0; i < numeroMediciones; i++) {
        if (strcmp(Mediciones[i].IDdelPaciente, idPaciente) == 0) {
            for (int j = 0; j < Mediciones[i].NumeroDeLecturas; j++) {
                if (Mediciones[i].lecturas[j].tipodesensor == 'E') {
                    ValorAux = Mediciones[i].lecturas[j].ValordeMedida1;
                    if (ValorAux < ValorMin) ValorMin = ValorAux;
                    if (ValorAux > ValorMax) ValorMax = ValorAux;
                }
            }
        }
    }

    if (ValorMin == 1e9 || ValorMax == -1e9) return false;

    double ConfiECGMinima = 0, ConfiECGMaxima = 0;
    for (int k = 0; k < TotalConfiguracion; k++) {
        if (strcmp(configuraciones[k].TipoDeSensor, "E") == 0) {
            ConfiECGMinima = configuraciones[k].Min;
            ConfiECGMaxima = configuraciones[k].Max;
            break;
        }
    }

    if (ValorMax < 0) ValorMax = -ValorMax;
    if (ValorMin < 0) ValorMin = -ValorMin;

    double SumaLectura = ValorMax + ValorMin;

    if (ConfiECGMaxima < 0) ConfiECGMaxima = -ConfiECGMaxima;
    if (ConfiECGMinima < 0) ConfiECGMinima = -ConfiECGMinima;

    double SumaConfiECG = ConfiECGMaxima + ConfiECGMinima;

    return (SumaLectura > SumaConfiECG);
}


void GenerarArchivoAnomaliaECG(Sala &sala, InformacionArchivoConfiguracion configuraciones[], int TotalConfiguraciones)
{
    fstream Anomalias_Binary_File("pacientes_ecg_anomalos.dat", ios::out | ios::binary);
    if (!Anomalias_Binary_File) {
        cout << "No se pudo crear el Archivo Correctamente";
        return;
    }

    for (int i = 0; i < static_cast<int>(static_cast<unsigned char>(sala.NumerodeMaquinas)); i++) {
        for (int j = 0; j < sala.ArregloMaquina[i].NumeroDeMediciones; j++) {

            
            if (AnomaliaParaECG(sala.ArregloMaquina[i].ArregloDeMediciones,
                                sala.ArregloMaquina[i].NumeroDeMediciones,
                                sala.ArregloMaquina[i].ArregloDeMediciones[j].IDdelPaciente,
                                configuraciones, TotalConfiguraciones))
            {
               
                Anomalias_Binary_File.write(sala.ArregloMaquina[i].ArregloDeMediciones[j].IDdelPaciente, 11);

                int numeroECG = 0;
                for (int k = 0; k < sala.ArregloMaquina[i].NumeroDeMediciones; k++) {
                    if (strcmp(sala.ArregloMaquina[i].ArregloDeMediciones[k].IDdelPaciente,
                               sala.ArregloMaquina[i].ArregloDeMediciones[j].IDdelPaciente) == 0)
                    {
                        for (int l = 0; l < sala.ArregloMaquina[i].ArregloDeMediciones[k].NumeroDeLecturas; l++) {
                            if (sala.ArregloMaquina[i].ArregloDeMediciones[k].lecturas[l].tipodesensor == 'E') {
                                numeroECG++;
                            }
                        }
                    }
                }

                Anomalias_Binary_File.write(reinterpret_cast<char*>(&numeroECG), 4);

                for (int m = 0; m < sala.ArregloMaquina[i].NumeroDeMediciones; m++) {
                    if (strcmp(sala.ArregloMaquina[i].ArregloDeMediciones[m].IDdelPaciente,
                               sala.ArregloMaquina[i].ArregloDeMediciones[j].IDdelPaciente) == 0)
                    {
                        for (int n = 0; n < sala.ArregloMaquina[i].ArregloDeMediciones[m].NumeroDeLecturas; n++) {
                            if (sala.ArregloMaquina[i].ArregloDeMediciones[m].lecturas[n].tipodesensor == 'E') {
                                Anomalias_Binary_File.write(sala.ArregloMaquina[i].ArregloDeMediciones[m].FechaYHora, 24);
                                Anomalias_Binary_File.write(reinterpret_cast<char*>(
                                    &sala.ArregloMaquina[i].ArregloDeMediciones[m].lecturas[n].ValordeMedida1), 8);
                            }
                        }
                    }
                }
            }
        }
    }
    Anomalias_Binary_File.close();
}


int main()
{
    InformacionArchivoConfiguracion configuraciones[5]; 
    InformacionArchivoPaciente     pacientes[1000];       
    int totalConfig = 0, totalPacientes = 0;
    int IdPacienteParaArchivo;

    Sala prueba;
    InicializarSala(prueba);

    LeerArchivoBinario(prueba);
    LeerArchivoConfiguracion(configuraciones, totalConfig, 5);

    // Menu
    cout << "Ingrese el numeral que quiere realizar" << endl;
    cout << "1. Cargar archivo de configuracion y datos de pacientes\n";
    cout << "2. Leer archivo .bsf\n";
    cout << "3. Generar reporte de anomalias\n";
    cout << "4. Calcular estadisticas\n";
    cout << "5. Exportar datos procesados\n";
    cout << "6. Salir\n";

    cout << "Quiero realizar el numeral:  " << endl;
    int Item;
    cin >> Item; 

    switch (Item)
    {
    case 1:
    {
        LeerArchivoConfiguracion(configuraciones, totalConfig, 5);
        LeerArchivoPacientes(pacientes, totalPacientes, 1000);
        cout << "Los datos se guardaron correctamente" << endl;

        cout << "=== CONFIG CARGADA (" << totalConfig << ") ===\n";
        for (int i = 0; i < totalConfig; ++i) {
            cout << i << ") [" << configuraciones[i].TipoDeSensor
                 << "] min=" << configuraciones[i].Min
                 << " max=" << configuraciones[i].Max << "\n";
        }
        break;
    }
    case 2:
    {
        LeerArchivoBinario(prueba);
        cout << "El archivo se leyo perfectamente" << endl;
        break;
    }
    case 3:
    {
        GenerarReporteAnomaliasTodosPacientes(prueba, configuraciones, totalConfig);
        cout << "Reporte de anomalias generado" << endl;
        break;
    }
    case 4:
    {
        cout << "Ingrese el id del paciente que quiere obtener reporte: " << endl;
        cout << "Id Paciente: ";
        cin >> IdPacienteParaArchivo;

        GenerarReportePaciente(IdPacienteParaArchivo, prueba, configuraciones, totalConfig);
        break;
    }
    case 5:
    {
        GenerarArchivoAnomaliaECG(prueba, configuraciones, totalConfig);
        cout << "Archivo ANOMALIAS ECG generado" << endl;
        break;
    }
    case 6:
    default:
    {
        cout << "Saliendo....." << endl;
        break;
    }
    }

    LiberarSala(prueba);
    return 0;
}

