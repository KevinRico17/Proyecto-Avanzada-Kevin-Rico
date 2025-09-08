
struct InformacionArchivoConfiguracion {
    char TipoDeSensor[10];
    double Min;
    double Max;
};

struct InformacionArchivoPaciente {
    int  IdPersona;
    char tipoDeDocumento[4];
    char Documento[11];
    char nombre[11];
    char apellido[11];
    char FechaNacimiento[11];
    char telefono[12];
    char email[31];
    char TipoDeSangre[4];
    char entidadSalud[11];
    char medicinaPrepagada[11];
};

struct lecturaSensor {
    char   tipodesensor;     
    double ValordeMedida1;   
    double ValordeMedida2;   
};

struct Medicion {
    char IDdelPaciente[11];  
    char FechaYHora[24];     
    int  NumeroDeLecturas;
    lecturaSensor* lecturas;
};

struct Maquina {
    char IDMaquina;             
    int  NumeroDeMediciones;      
    Medicion* ArregloDeMediciones; 
};

struct Sala {
    char    IDdelaSala;        
    char    NumerodeMaquinas; 
    Maquina* ArregloMaquina;   
};


inline void InicializarSala(Sala& s) {
    s.IDdelaSala = 0;
    s.NumerodeMaquinas = 0;
    s.ArregloMaquina = nullptr;
}

inline void LiberarSala(Sala& s) {
    if (!s.ArregloMaquina) return;
    for (int i = 0; i < static_cast<int>(s.NumerodeMaquinas); ++i) {
        Maquina& m = s.ArregloMaquina[i];
        if (m.ArregloDeMediciones) {
            for (int j = 0; j < m.NumeroDeMediciones; ++j) {
                Medicion& med = m.ArregloDeMediciones[j];
                delete[] med.lecturas;
                med.lecturas = nullptr;
            }
            delete[] m.ArregloDeMediciones;
            m.ArregloDeMediciones = nullptr;
        }
    }
    delete[] s.ArregloMaquina;
    s.ArregloMaquina = nullptr;
    s.NumerodeMaquinas = 0;
}
