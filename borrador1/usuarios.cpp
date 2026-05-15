#include "conexion.h"
#include "utilidades.h"
#include "usuarios.h"
#include "alimentos.h"
#include "historial.h"
#include "analisis.h"


#include <iostream>
#include <string>
#include <vector>
#include <map>

using namespace std;

//funcion para registrar un nuevo usuario
void registroUsuario() {
    MYSQL* conn = conectar();
    if (!conn) return;

    string nombreCompleto, usuario, password, genero, departamento, municipio, actividadFisica, ocupacion;
    int edad, opGenero, opRegion, opDepartamento, opMunicipio, opActividad;
    double peso, altura;

    dibujarCabecera("CREAR NUEVA CUENTA DE USUARIO");

	//estructuras de regiones, departamentos y municipios para la seleccion del usuario
    vector<string> regiones = { "Metropolitana", "Norte", "Nororiente", "Suroriente", "Central", "Occidente", "Peten" };

    map<string, vector<string>> departamentos = {
        {"Metropolitana", {"Guatemala"}},
        {"Norte", {"Alta Verapaz", "Baja Verapaz"}},
        {"Nororiente", {"Zacapa", "El Progreso", "Izabal", "Chiquimula"}},
        {"Suroriente", {"Jalapa", "Santa Rosa", "Jutiapa"}},
        {"Central", {"Sacatepequez", "Chimaltenango", "Escuintla"}},
        {"Occidente", {"Quetzaltenango", "Huehuetenango", "San Marcos", "Sololá", "Totonicapán", "Suchitepéquez", "Retalhuleu", "Quiche"}},
        {"Peten", {"Peten"}}
    };

	//estrucutura de municipios por departamento
    map<string, vector<string>> municipios = {
        {"Guatemala", {
                "Guatemala","Santa Catarina Pinula","San Jose Pinula","San Jose del Golfo",
                "Palencia","Chinautla","San Pedro Ayampuc","Mixco",
                "San Pedro Sacatepequez","San Juan Sacatepequez","San Raymundo",
                "Chuarrancho","Fraijanes","Amatitlan","Villa Nueva",
                "Villa Canales","Petapa"
            }},
        {"El Progreso", {
                "Guastatoya","Morazan","San Agustin Acasaguastlan",
                "San Cristobal Acasaguastlan","El Jicaro","Sansare",
                "Sanarate","San Antonio La Paz"
            }},
        {"Sacatepequez", {
                "Antigua Guatemala","Jocotenango","Pastores","Sumpango",
                "Santo Domingo Xenacoj","Santiago Sacatepequez",
                "San Bartolome Milpas Altas","San Lucas Sacatepequez",
                "Santa Lucia Milpas Altas","Magdalena Milpas Altas",
                "Santa Maria de Jesus","Ciudad Vieja","San Miguel Dueñas",
                "Alotenango"
            }},
        {"Chimaltenango", {
                "Chimaltenango","San Jose Poaquil","San Martin Jilotepeque",
                "San Juan Comalapa","Santa Apolonia","Tecpan Guatemala",
                "Patzun","Pochuta","Patzicia","Santa Cruz Balanya",
                "Acatenango","Yepocapa","San Andres Itzapa",
                "Parramos","Zaragoza","El Tejar"
            }},
        {"Escuintla", {
                "Escuintla","Santa Lucia Cotzumalguapa","La Democracia",
                "Siquinala","Masagua","Tiquisate","La Gomera",
                "Guanagazapa","San Jose","Iztapa","Palin",
                "San Vicente Pacaya","Nueva Concepcion"
            }},
        {"Santa Rosa", {
                "Cuilapa","Barberena","Santa Rosa de Lima","Casillas",
                "San Rafael Las Flores","Oratorio","San Juan Tecuaco",
                "Chiquimulilla","Taxisco","Santa Maria Ixhuatan",
                "Guazacapan","Santa Cruz Naranjo","Pueblo Nuevo Viñas",
                "Nueva Santa Rosa"
            }},
        {"Solola", {
                "Solola","San Jose Chacaya","Santa Maria Visitacion",
                "Santa Lucia Utatlan","Nahuala","Santa Catarina Ixtahuacan",
                "Santa Clara La Laguna","Concepcion","San Andres Semetabaj",
                "Panajachel","Santa Catarina Palopo","San Antonio Palopo",
                "San Lucas Toliman","Santa Cruz La Laguna",
                "San Pablo La Laguna","San Marcos La Laguna",
                "San Juan La Laguna","San Pedro La Laguna",
                "Santiago Atitlan"
            }},
        {"Totonicapan", {
                "Totonicapan","San Cristobal Totonicapan",
                "San Francisco El Alto","San Andres Xecul",
                "Momostenango","Santa Maria Chiquimula",
                "Santa Lucia La Reforma","San Bartolo"
            }},
        {"Quetzaltenango", {
                "Quetzaltenango","Salcaja","Olintepeque","San Carlos Sija",
                "Sibilia","Cabrican","Cajola","San Miguel Siguila",
                "Ostuncalco","San Mateo","Concepcion Chiquirichapa",
                "San Martin Sacatepequez","Almolonga","Cantel","Huitan",
                "Zunil","Colomba","San Francisco La Union",
                "El Palmar","Coatepeque","Genova",
                "Flores Costa Cuca","La Esperanza",
                "Palestina de Los Altos"
            }},
        {"Suchitepequez", {
                "Mazatenango","Cuyotenango","San Francisco Zapotitlan",
                "San Bernardino","San Jose El Idolo",
                "Santo Domingo Suchitepequez","San Lorenzo",
                "Samayaque","San Pablo Jocopilas",
                "San Antonio Suchitepequez","San Miguel Panan",
                "San Gabriel","Chicacao","Patulul","Santa Barbara",
                "San Juan Bautista","Santo Tomas La Union",
                "Zunilito","Pueblo Nuevo","Rio Bravo"
            }},
        {"Retalhuleu", {
                "Retalhuleu","San Sebastian","Santa Cruz Mulua",
                "San Martin Zapotitlan","San Felipe",
                "San Andres Villa Seca","Champerico",
                "Nuevo San Carlos","El Asintal"
            }},
        {"San Marcos", {
                "San Marcos","San Pedro Sacatepequez",
                "San Antonio Sacatepequez","Comitancillo",
                "San Miguel Ixtahuacan","Concepcion Tutuapa",
                "Tacana","Sibinal","Tajumulco","Tejutla",
                "San Rafael Pie de la Cuesta","Nuevo Progreso",
                "El Tumbador","El Rodeo","Malacatan","Catarina",
                "Ayutla","Ocos","San Pablo","El Quetzal",
                "La Reforma","Pajapita","Ixchiguan",
                "San Jose Ojetenan","San Cristobal Cucho",
                "Sipacapa","Esquipulas Palo Gordo",
                "Rio Blanco","San Lorenzo"
            }},
        {"Huehuetenango", {
                "Huehuetenango","Chiantla","Malacatancito","Cuilco",
                "Nenton","San Pedro Necta","Jacaltenango",
                "San Pedro Soloma","San Ildefonso Ixtahuacan",
                "Santa Barbara","La Libertad","La Democracia",
                "San Miguel Acatan","San Rafael La Independencia",
                "Todos Santos Cuchumatan","San Juan Atitan",
                "Santa Eulalia","San Mateo Ixtatan","Colotenango",
                "San Sebastian Huehuetenango","Tectitan",
                "Concepcion Huista","San Juan Ixcoy",
                "San Antonio Huista","San Sebastian Coatan",
                "Barillas","Aguacatan","San Rafael Petzal",
                "San Gaspar Ixchil","Santiago Chimaltenango",
                "Santa Ana Huista","Union Cantinil"
            }},
        { "Quiche", {
                "Santa Cruz del Quiche","Chiche","Chinique",
                "Zacualpa","Chajul","Santo Tomas Chichicastenango",
                "Patzite","San Antonio Ilotenango","San Pedro Jocopilas",
                "Cunen","San Juan Cotzal","Joyabaj","Nebaj",
                "San Andres Sajcabaja","Uspantan","Sacapulas",
                "San Bartolome Jocotenango","Canilla","Chicaman",
                "Ixcán","Pachalum"
            } },
        { "Baja Verapaz", {
                "Salama","San Miguel Chicaj","Rabinal","Cubulco",
                "Granados","Santa Cruz El Chol","San Jeronimo",
                "Purulha"
            } },
        { "Alta Verapaz", {
                "Coban","Santa Cruz Verapaz","San Cristobal Verapaz",
                "Tactic","Tamahu","Tucuru","Panzos",
                "Senahu","San Pedro Carcha","San Juan Chamelco",
                "Lanquin","Santa Maria Cahabon","Chisec",
                "Chahal","Fray Bartolome de las Casas",
                "Santa Catalina La Tinta","Raxruha"
            } },
        { "Peten", {
                "Flores","San Jose","San Benito","San Andres",
                "La Libertad","San Francisco","Santa Ana",
                "Dolores","San Luis","Sayaxche","Melchor de Mencos",
                "Poptun","Las Cruces","El Chal"
            } },
        { "Izabal", {
                "Puerto Barrios","Livingston","El Estor",
                "Morales","Los Amates"
            } },
        { "Zacapa", {
                "Zacapa","Estanzuela","Rio Hondo","Gualan",
                "Teculutan","Usumatlan","Cabanas",
                "San Diego","La Union","Huite"
            } },
        { "Chiquimula", {
                "Chiquimula","San Jose La Arada","San Juan Ermita",
                "Jocotan","Camotan","Olopa","Esquipulas",
                "Concepcion Las Minas","Quetzaltepeque","San Jacinto",
                "Ipala"
            } },
        { "Jalapa", {
                "Jalapa","San Pedro Pinula","San Luis Jilotepeque",
                "San Manuel Chaparron","San Carlos Alzatate",
                "Monjas","Mataquescuintla"
            } },
        {"Jutiapa", {
                "Jutiapa","El Progreso","Santa Catarina Mita",
                "Agua Blanca","Asuncion Mita","Yupiltepeque",
                "Atescatempa","Jerez","El Adelanto",
                "Zapotitlan","Comapa","Jalpatagua",
                "Conguaco","Moyuta","Pasaco","San Jose Acatempa",
                "Quesada"
            }}

    };

	//datos basicos
    cout << " > Nombre Completo: "; cin.ignore(); getline(cin, nombreCompleto);
    cout << " > Edad: "; cin >> edad;

    cout << " > Genero (1. Masc / 2. Fem): "; cin >> opGenero;
    genero = (opGenero == 1) ? "Masculino" : "Femenino";

    //regios de residencia
    cout << "\nSeleccione Region:\n";
    for (int i = 0; i < regiones.size(); i++) {
        cout << i + 1 << ". " << regiones[i] << endl;
    }
    cin >> opRegion;

    string regionSeleccionada = regiones[opRegion - 1];

	//departamentos por region
    cout << "\nSeleccione Departamento:\n";
    for (int i = 0; i < departamentos[regionSeleccionada].size(); i++) {
        cout << i + 1 << ". " << departamentos[regionSeleccionada][i] << endl;
    }
    cin >> opDepartamento;

    departamento = departamentos[regionSeleccionada][opDepartamento - 1];

	//municipios por departamento
    cout << "\nSeleccione Municipio:\n";
    for (int i = 0; i < municipios[departamento].size(); i++) {
        cout << i + 1 << ". " << municipios[departamento][i] << endl;
    }
    cin >> opMunicipio;

    municipio = municipios[departamento][opMunicipio - 1];

	//datos sobre salud y estilo de vida
    cout << " > Peso (kg): "; cin >> peso;
    cout << " > Altura (m): "; cin >> altura;

    cout << " > Actividad (1. Sedentaria / 2. Ligera / 3. Moderada / 4. Intensa): ";
    cin >> opActividad;
    string acts[] = { "Sedentaria", "Ligera", "Moderada", "Intensa" };
    actividadFisica = acts[opActividad - 1];

    cout << " > Ocupacion: "; cin.ignore(); getline(cin, ocupacion);

	//credenciales de acceso
    cout << " > Usuario unico: "; cin >> usuario;
    cout << " > Contrasena: "; cin >> password;

	//consulta SQL para insertar el nuevo usuario en la base de datos
    string query = "INSERT INTO usuarios (nombre_completo, edad, genero, departamento, municipio, peso, altura, actividad_fisica, ocupacion, usuario, password) VALUES ('"
        + nombreCompleto + "', " + to_string(edad) + ", '" + genero + "', '" + departamento + "', '" + municipio + "', " + to_string(peso) + ", " + to_string(altura) + ", '" + actividadFisica + "', '" + ocupacion + "', '" + usuario + "', '" + password + "')";

	//validacion de la consulta SQL y mensaje de resultado
    if (mysql_query(conn, query.c_str()) == 0) {
        cout << "\n [OK] Perfil creado con exito. Presione ENTER para continuar...";
    }
    else {
        cout << "\n [!] Error al registrar: " << mysql_error(conn) << endl;
    }

    mysql_close(conn);
    cin.ignore(); cin.get();
}

//funcion para iniciar sesion y validar credenciales
void iniciarSecion() {
    MYSQL* conn = conectar();
    string usuario, password;

    dibujarCabecera("ACCESO AL SISTEMA");

    cout << "\n  USUARIO: "; cin >> usuario;
    cout << "  CONTRASEÑA: "; cin >> password;

    string query = "SELECT id, nombre_completo FROM usuarios WHERE usuario = '" + usuario + "' AND password = '" + password + "'";
    mysql_query(conn, query.c_str());
    MYSQL_RES* res = mysql_store_result(conn);

    //  MENU DE USUARIO}
    if (res && mysql_num_rows(res) > 0) {
        MYSQL_ROW row = mysql_fetch_row(res);
        ID_USUARIO_ACTUAL = stoi(row[0]);
        NOMBRE_USUARIO_ACTUAL = row[1];
        mysql_free_result(res);

        int op;
        do {
            dibujarCabecera("PANEL DE CONTROL - " + NOMBRE_USUARIO_ACTUAL);
            cout << "\n  1. REGISTRAR CONSUMO DE ALIMENTO" << endl;
            cout << "  2. CALENDARIO DE REGISTROS" << endl;
            cout << "  3. ANALISIS DE PERFIL NUTRICIONAL" << endl;
            cout << "  4. CERRAR SESION" << endl;
            cout << "\n  Seleccione una opcion: ";
            cin >> op;

            switch (op) {
            case 1: registrarConsumo(conn); break;
            case 2: calendario(conn, ID_USUARIO_ACTUAL); break;
            case 3: mostrarAnalisisUsuario(conn, usuario); break;
            case 4:
                limpiarPantalla();
                cout << "\n GRACIAS POR USAR EL SISTEMA. HASTA PRONTO.\n" << endl;
                break;
            }


        } while (op != 4);

        mysql_close(conn);
    }
    else {
        cout << "\n [!] Credenciales incorrectas. Intente de nuevo." << endl;
        mysql_close(conn);
        cin.ignore(); cin.get();
    }
}