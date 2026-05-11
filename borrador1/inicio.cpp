#include <iostream>
#include <mysql.h> 
#include <string>  
#include <vector>
#include <windows.h>
#include <iomanip>
#include <limits>
#include <map>

using namespace std;
// VARIABLES PARA INCIO DE SECION
int ID_USUARIO_ACTUAL = 0;
string NOMBRE_USUARIO_ACTUAL = "";

// FUNCIÓN DE CONEXIÓN
MYSQL* conectar() {
    MYSQL* conn = mysql_init(0);
    conn = mysql_real_connect(conn, "localhost", "root", "", "proyectofn", 3306, NULL, 0);
    return conn;
}

struct Macronutrientes {
    float calorias = 0;
    float proteinas = 0;
    float carbohidratos = 0;
    float grasas = 0;
    float fibra = 0;
};

// LIMPIAR PANTALLA 
void limpiarPantalla() {

#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// CABECERAS DE VENTANA 
void dibujarCabecera(string titulo) {
    limpiarPantalla();
    cout << "==============================================================================" << endl;
    cout << "      " << titulo << endl;
    cout << "==============================================================================" << endl;
}

void acumularMacronutrientes(Macronutrientes& total, float gramosConsumidos, float caloriasBase, float proteinasBase, float carbohidratosBase, float grasasBase, float fibraBase) {
    total.calorias +=
        (gramosConsumidos * caloriasBase) / 100.0;

    total.proteinas +=
        (gramosConsumidos * proteinasBase) / 100.0;

    total.carbohidratos +=
        (gramosConsumidos * carbohidratosBase) / 100.0;

    total.grasas +=
        (gramosConsumidos * grasasBase) / 100.0;

    total.fibra +=
        (gramosConsumidos * fibraBase) / 100.0;
}

// MÓDULO: REGISTRO DE ALIMENTOS 
void registrarConsumo(MYSQL* conn) {
    string fecha, hora, nombre_alimento, unidad_txt, cant_formato;
    int tiempo_op, id_cat, id_maestro, id_unidad;  
	char continuar;
    float cantidad;
    MYSQL_RES* res;
    MYSQL_ROW row;
    Macronutrientes totalMacros;

    dibujarCabecera("NUEVO REGISTRO DE INGESTA");

    // 1. Fecha
    cout << "\n [1/5] FECHA DE CONSUMO" << endl;
    cout << " > Ingrese fecha (YYYY-MM-DD) o escriba 'hoy': ";
    cin >> fecha;
    if (fecha == "hoy" || fecha == "HOY" || fecha == "Hoy") fecha = "CURRENT_DATE"; else fecha = "'" + fecha + "'";

    // 2. Hora
    cout << "\n [2/5] HORA DEL CONSUMO" << endl;
    cout << " > Ingrese hora (HH:MM) o escriba 'ahora': ";
    cin >> hora;
    if (hora == "ahora" || hora == "AHORA" || hora == "Ahora") hora = "CURRENT_TIME"; else hora = "'" + hora + ":00'";

    // 3. Tiempo de Comida
    cout << "\n [3/5] TIEMPO DE COMIDA" << endl;
    cout << " 1. Desayuno | 2. Almuerzo | 3. Cena | 4. Refaccion" << endl;
    cout << " > Seleccione una opcion: ";
    cin >> tiempo_op;
    string tiempos[] = { "Desayuno", "Almuerzo", "Cena", "Refaccion" };
    string tiempo_comida = (tiempo_op >= 1 && tiempo_op <= 4) ? tiempos[tiempo_op - 1] : "Refaccion";

    do { // Bucle para guardar registros simultaneos

        // 4. Selección de Alimento 
        cout << "\n [4/5] SELECCION DE ALIMENTO" << endl;
        mysql_query(conn, "SELECT id_categoria, nombre_categoria FROM categorias ORDER BY id_categoria ASC");
        res = mysql_store_result(conn);
        cout << " ---------------------------------------------------" << endl;
        while ((row = mysql_fetch_row(res))) {
            cout << "   " << row[0] << ". " << row[1] << endl;
        }
        mysql_free_result(res);
        cout << " ---------------------------------------------------" << endl;
        cout << " > ID de la Categoria: "; cin >> id_cat;

        // Listar alimentos de esa categoría
        string consulta = "SELECT a.id_maestro, a.nombre_alimento, u.nombre_unidad, a.calorias "
            "FROM lista_alimentos a "
            "JOIN unidades_medida u ON a.id_unidad = u.id_unidad "
            "WHERE a.id_categoria = " + to_string(id_cat) +
            " ORDER BY a.id_maestro ASC";
        mysql_query(conn, consulta.c_str());
        res = mysql_store_result(conn);

        cout << "\n" << left << setw(6) << " ID" << setw(40) << "ALIMENTO" << setw(15) << "UNIDAD" << "KCAL/100g" << endl;
        cout << " ----------------------------------------------------------------------------" << endl;
        while ((row = mysql_fetch_row(res))) {
            cout << " [" << row[0] << "] " << setw(37) << row[1] << setw(15) << row[2] << row[3] << endl;
        }
        mysql_free_result(res);

        cout << "\n > Ingrese el ID del Alimento elegido: "; cin >> id_maestro;

        // 5. Cantidad y Cálculos
        string sql_datos = "SELECT nombre_alimento, peso_equivalente_g, calorias, proteinas, carbohidratos, grasas, fibra, u.nombre_unidad, u.id_unidad "
            "FROM lista_alimentos a JOIN unidades_medida u ON a.id_unidad = u.id_unidad "
            "WHERE id_maestro = " + to_string(id_maestro);
        mysql_query(conn, sql_datos.c_str());
        res = mysql_store_result(conn);
        row = mysql_fetch_row(res);

        if (row) {
            nombre_alimento = row[0];
            float peso_eq = atof(row[1]);
            float caloriasBase = atof(row[2]);
            float proteinasBase = atof(row[3]);
            float carbohidratosBase = atof(row[4]);
            float grasasBase = atof(row[5]);
            float fibraBase = atof(row[6]);
            unidad_txt = row[7];
            id_unidad = atoi(row[8]);

            cout << "\n [5/5] CANTIDAD CONSUMIDA" << endl;
            cout << " > ¿Cuantas [" << unidad_txt << "]  de [" << nombre_alimento << "] ingirio?: ";
            cin >> cantidad;
            limpiarPantalla();
            float gramosConsumidos = cantidad * peso_eq;

            acumularMacronutrientes(totalMacros, gramosConsumidos, caloriasBase, proteinasBase, carbohidratosBase, grasasBase, fibraBase);

            // Cálculos nutricionales
            float g_totales = cantidad * peso_eq;
            float c_final = (g_totales * caloriasBase) / 100.0;
            cant_formato = to_string(cantidad);

            // Guardado en Base de Datos
            string query = "INSERT INTO registros_comida (id_usuario, nombre_alimento, cantidad_proporcional, tiempo_comida, fecha_ingesta, hora_ingesta, id_unidad, nombre_unidad) VALUES ("
                + to_string(ID_USUARIO_ACTUAL) + ", '" + nombre_alimento + "', '" + cant_formato + "', '" + tiempo_comida + "', " + fecha + ", " + hora + ", " + to_string(id_unidad) + ", '" + unidad_txt + "')";

            if (mysql_query(conn, query.c_str()) == 0) {
                cout << "\n +---------------------------------------------------------+" << endl;
                cout << " |   ¡REGISTRO GUARDADO CON EXITO EN TU HISTORIAL!         |" << endl;
                cout << " +---------------------------------------------------------+" << endl;
                cout << "   Resumen: " << g_totales << "g de " << nombre_alimento << endl;
                cout << "   Energia: " << fixed << setprecision(2) << c_final << " kcal" << endl;
                cout << " +---------------------------------------------------------+" << endl;
            }
            else {
                cout << "\n [!] ERROR AL GUARDAR: " << mysql_error(conn) << endl;
            }
        }
        else {
            cout << "\n [!] El ID de alimento no existe." << endl;
        }
        mysql_free_result(res);

        cout << "\n========== ACUMULADO ACTUAL ==========" << endl;
        cout << " Calorias: " << fixed << setprecision(2) << totalMacros.calorias << " kcal" << endl;
        cout << " Proteinas: " << totalMacros.proteinas << " g" << endl;
        cout << " Carbohidratos: " << totalMacros.carbohidratos << " g" << endl;
        cout << " Grasas: " << totalMacros.grasas << " g" << endl;
        cout << " Fibra: " << totalMacros.fibra << " g" << endl;

        cout << "\n ¿Desea continuar con el registro de consumo? (s/n): ";
        cin >> continuar;

        if (continuar == 'n' || continuar == 'N') {

            cout << "\n Finaisando registro de consumo... Presione ENTER para regresar al menu...";
            cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
            cin.get();
            return; // Salir de la función sin registrar
        }
        limpiarPantalla();
	} while (continuar == 's' || continuar == 'S');
}

// REGISTRO DE USUARIO
void registrarUsuario() {
    MYSQL* conn = conectar();
    if (!conn) return;

    string nombreCompleto, usuario, password, genero, departamento, municipio, actividadFisica, ocupacion;
    int edad, opGenero, opRegion, opDepartamento, opMunicipio, opActividad;
    double peso, altura;

    dibujarCabecera("CREAR NUEVA CUENTA DE USUARIO");

    // ----------- ESTRUCTURA DE DATOS -----------
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

    // ----------- DATOS GENERALES -----------
    cout << " > Nombre Completo: "; cin.ignore(); getline(cin, nombreCompleto);
    cout << " > Edad: "; cin >> edad;

    cout << " > Genero (1. Masc / 2. Fem): "; cin >> opGenero;
    genero = (opGenero == 1) ? "Masculino" : "Femenino";

    // ----------- REGION -----------
    cout << "\nSeleccione Region:\n";
    for (int i = 0; i < regiones.size(); i++) {
        cout << i + 1 << ". " << regiones[i] << endl;
    }
    cin >> opRegion;

    string regionSeleccionada = regiones[opRegion - 1];

    // ----------- DEPARTAMENTO -----------
    cout << "\nSeleccione Departamento:\n";
    for (int i = 0; i < departamentos[regionSeleccionada].size(); i++) {
        cout << i + 1 << ". " << departamentos[regionSeleccionada][i] << endl;
    }
    cin >> opDepartamento;

    departamento = departamentos[regionSeleccionada][opDepartamento - 1];

    // ----------- MUNICIPIO -----------
    cout << "\nSeleccione Municipio:\n";
    for (int i = 0; i < municipios[departamento].size(); i++) {
        cout << i + 1 << ". " << municipios[departamento][i] << endl;
    }
    cin >> opMunicipio;

    municipio = municipios[departamento][opMunicipio - 1];

    // ----------- RESTO DE DATOS -----------
    cout << " > Peso (kg): "; cin >> peso;
    cout << " > Altura (m): "; cin >> altura;

    cout << " > Actividad (1. Sedentaria / 2. Ligera / 3. Moderada / 4. Intensa): ";
    cin >> opActividad;
    string acts[] = { "Sedentaria", "Ligera", "Moderada", "Intensa" };
    actividadFisica = acts[opActividad - 1];

    cout << " > Ocupacion: "; cin.ignore(); getline(cin, ocupacion);
    cout << " > Usuario unico: "; cin >> usuario;
    cout << " > Contrasena: "; cin >> password;

   
    string query = "INSERT INTO usuarios (nombre_completo, edad, genero, departamento, municipio, peso, altura, actividad_fisica, ocupacion, usuario, password) VALUES ('"
        + nombreCompleto + "', " + to_string(edad) + ", '" + genero + "', '" + departamento + "', '" + municipio + "', " + to_string(peso) + ", " + to_string(altura) + ", '" + actividadFisica + "', '" + ocupacion + "', '" + usuario + "', '" + password + "')";

    if (mysql_query(conn, query.c_str()) == 0) {
        cout << "\n [OK] Perfil creado con exito. Presione ENTER para continuar...";
    }
    else {
        cout << "\n [!] Error al registrar: " << mysql_error(conn) << endl;
    }

    mysql_close(conn);
    cin.ignore(); cin.get();
}

// DETALLES DEL DIA
void verDetalleDia(MYSQL* conn, int id_usuario, string fecha) {
    if (conn == NULL) return;

    string consulta =
        "SELECT r.tiempo_comida, r.nombre_alimento, r.cantidad_proporcional, r.nombre_unidad, "
        "((r.cantidad_proporcional * l.peso_equivalente_g * l.calorias) / 100) AS kcal, "
        "((r.cantidad_proporcional * l.peso_equivalente_g * l.proteinas) / 100) AS prot, "
        "((r.cantidad_proporcional * l.peso_equivalente_g * l.carbohidratos) / 100) AS carb, "
        "((r.cantidad_proporcional * l.peso_equivalente_g * l.grasas) / 100) AS gras, "
        "((r.cantidad_proporcional * l.peso_equivalente_g * l.fibra) / 100) AS fibra "
        "FROM registros_comida r "
        "JOIN lista_alimentos l ON r.nombre_alimento = l.nombre_alimento "
        "WHERE r.id_usuario = " + to_string(id_usuario) + " AND r.fecha_ingesta = '" + fecha + "' "
        "ORDER BY FIELD(r.tiempo_comida, 'Desayuno', 'Almuerzo', 'Cena', 'Refaccion')";

    if (mysql_query(conn, consulta.c_str())) {
        cout << "\n[!] Error en Detalle: " << mysql_error(conn) << endl;
        return;
    }

    MYSQL_RES* res = mysql_store_result(conn);
    if (!res) return;

    MYSQL_ROW row;

    cout << "\n====================================================================================================" << endl;
    cout << "                             DETALLE DE INGESTA: " << fecha << endl;
    cout << "====================================================================================================" << endl;
    cout << left << setw(12) << "TIEMPO"
        << setw(22) << "ALIMENTO"
        << setw(16) << "CANTIDAD"
        << right << setw(8) << "KCAL"
        << setw(8) << "P(g)"
        << setw(8) << "C(g)"
        << setw(8) << "G(g)"
        << setw(8) << "F(g)" << endl;
    cout << "----------------------------------------------------------------------------------------------------" << endl;

    bool encontro = false;
    while ((row = mysql_fetch_row(res))) {
        encontro = true;

        string cantStr = (row[2] ? row[2] : "0");
        string unidStr = (row[3] ? row[3] : "");
        string cantUnidad = cantStr + " " + unidStr;

        cout << left << " [" << setw(9) << row[0] << "] "
            << setw(22) << (strlen(row[1]) > 21 ? string(row[1]).substr(0, 18) + "..." : row[1])
            << setw(16) << cantUnidad
            << right << setw(8) << fixed << setprecision(1) << (row[4] ? atof(row[4]) : 0.0)
            << setw(8) << (row[5] ? atof(row[5]) : 0.0)
            << setw(8) << (row[6] ? atof(row[6]) : 0.0)
            << setw(8) << (row[7] ? atof(row[7]) : 0.0)
            << setw(8) << (row[8] ? atof(row[8]) : 0.0) << endl;
    }

    if (!encontro) {
        cout << " [!] No se encontraron detalles para la fecha: " << fecha << endl;
        cout << " Nota: Verifique que el nombre del alimento en 'registros_comida' coincida con 'lista_alimentos'." << endl;
    }

    mysql_free_result(res);
    cout << "====================================================================================================" << endl;
    cout << "\nPresione Enter para regresar...";

    cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
    cin.get();
}

//CALENDARIO DE CONSUMO Y ESPESIFICACIONES
void calendario(MYSQL* conn, int id_usuario) {
    int anio, mes;

    cout << "\n==============================================================================" << endl;
    cout << "                         HISTORIAL NUTRICIONAL MENSUAL" << endl;
    cout << "==============================================================================" << endl;
    cout << " Ingrese el año: "; cin >> anio;
    cout << " Ingrese el mes (1-12): "; cin >> mes;

    // CONSULTA SQL: Suma proporcional de todos los nutrientes por día
    string consulta =
        "SELECT r.fecha_ingesta, "
        "SUM((r.cantidad_proporcional * l.peso_equivalente_g * l.calorias) / 100) AS kcal, "
        "SUM((r.cantidad_proporcional * l.peso_equivalente_g * l.proteinas) / 100) AS prot, "
        "SUM((r.cantidad_proporcional * l.peso_equivalente_g * l.carbohidratos) / 100) AS carb, "
        "SUM((r.cantidad_proporcional * l.peso_equivalente_g * l.grasas) / 100) AS grasa, "
        "SUM((r.cantidad_proporcional * l.peso_equivalente_g * l.fibra) / 100) AS fibra "
        "FROM registros_comida r "
        "JOIN lista_alimentos l ON r.nombre_alimento = l.nombre_alimento "
        "WHERE r.id_usuario = " + to_string(id_usuario) +
        " AND YEAR(r.fecha_ingesta) = " + to_string(anio) +
        " AND MONTH(r.fecha_ingesta) = " + to_string(mes) +
        " GROUP BY r.fecha_ingesta "
        "ORDER BY r.fecha_ingesta ASC";

    if (mysql_query(conn, consulta.c_str())) {
        cout << "\n[!] Error de MySQL: " << mysql_error(conn) << endl;
        return;
    }

    MYSQL_RES* res = mysql_store_result(conn);
    if (!res) return;

    MYSQL_ROW row;
    cout << "\n " << left << setw(12) << "FECHA"
        << setw(10) << "KCAL"
        << setw(10) << "PROT(g)"
        << setw(10) << "CARB(g)"
        << setw(10) << "GRAS(g)"
        << "FIBRA(g)" << endl;
    cout << " ------------------------------------------------------------------------------" << endl;

    bool hayDatos = false;
    while ((row = mysql_fetch_row(res))) {
        hayDatos = true;
        // row[0]=fecha, row[1]=kcal, row[2]=prot, row[3]=carb, row[4]=grasa, row[5]=fibra
        cout << "  " << left << setw(11) << row[0]
            << right << setw(7) << fixed << setprecision(1) << (row[1] ? atof(row[1]) : 0.0) << "  "
            << setw(8) << (row[2] ? atof(row[2]) : 0.0) << "  "
            << setw(8) << (row[3] ? atof(row[3]) : 0.0) << "  "
            << setw(8) << (row[4] ? atof(row[4]) : 0.0) << "  "
            << setw(8) << (row[5] ? atof(row[5]) : 0.0) << endl;
    }

    if (!hayDatos) {
        cout << "  [!] No hay registros para " << anio << "-" << mes << endl;
    }

    mysql_free_result(res);

    if (hayDatos) {
        string fecha_elegida;
        cout << " ------------------------------------------------------------------------------" << endl;
        cout << " Escriba la FECHA [YYYY-MM-DD] para ver detalle (o '0' para salir): ";
        cin >> fecha_elegida;

        if (fecha_elegida != "0") {
            verDetalleDia(conn, id_usuario, fecha_elegida);
        }
    }
    else {
        cout << "\nPresione Enter para continuar...";
        cin.ignore(); cin.get();
    }
}


void analizarMacronutriente(
    string nombre,
    double consumido,
    double recomendado,
    string unidad
) {

    double minimo = recomendado * 0.90;
    double maximo = recomendado * 1.10;

    cout << "\n--------------------------------------------------" << endl;
    cout << " " << nombre << endl;
    cout << " " << endl;

    cout << " Recomendado: "
        << fixed << setprecision(2)
        << recomendado << " " << unidad << endl;

    cout << " Consumido:   "
        << consumido << " " << unidad << endl;

    if (consumido < minimo) {

        cout << " ESTADO: CONSUMO BAJO" << endl;
        cout << " Deficit aproximado: "
            << (recomendado - consumido)
            << " " << unidad << endl;
    }
    else if (consumido > maximo) {

        cout << " ESTADO: EXCESO DE CONSUMO" << endl;
        cout << " Exceso aproximado: "
            << (consumido - recomendado)
            << " " << unidad << endl;
    }
    else {

        cout << " ESTADO: CONSUMO ADECUADO" << endl;
    }
}

void mostrarAnalisisUsuario(MYSQL* conn, string user) {
    if (!conn) return;

    dibujarCabecera("ANALISIS DE PERFIL NUTRICIONAL");

    string query =
        "SELECT "
        "nombre_completo, "
        "edad, "
        "genero, "
        "peso, "
        "altura, "
        "actividad_fisica, "
        "departamento, "
        "municipio "
        "FROM usuarios "
        "WHERE usuario = '" + user + "'";


    if (mysql_query(conn, query.c_str())) {

        cout << "Error SQL: "
            << mysql_error(conn)
            << endl;

        return;
    }

    MYSQL_RES* res = mysql_store_result(conn);
    MYSQL_ROW row = mysql_fetch_row(res);

    if (!row) {

        cout << "\n[!] Usuario no encontrado." << endl;

        if (res)
            mysql_free_result(res);

        cin.ignore();
        cin.get();

        return;
    }

    string nombre = row[0] ? row[0] : "Sin nombre";
    int edad = row[1] ? atoi(row[1]) : 0;
    string genero = row[2] ? row[2] : "M";
    double peso = row[3] ? atof(row[3]) : 0;
    double altura = row[4] ? atof(row[4]) : 0;
    string actividad = row[5] ? row[5] : "Sedentaria";
    string departamento = row[6] ? row[6] : "N/A";
    string municipio = row[7] ? row[7] : "N/A";

    mysql_free_result(res);

    // CALCULOS DE SALUD

    double imc = 0;

    if (altura > 0) {

        imc = peso / (altura * altura);
    }


    string estadoIMC;

    if (imc < 18.5)
        estadoIMC = "Bajo peso";

    else if (imc < 25)
        estadoIMC = "Normal";

    else if (imc < 30)
        estadoIMC = "Sobrepeso";

    else
        estadoIMC = "Obesidad";


    // TMB

    double altura_cm = altura * 100;
    double tmb = 0;

    if (genero == "Masculino" || genero == "1") {

        tmb =
            (10 * peso) +
            (6.25 * altura_cm) -
            (5 * edad) + 5;
    }
    else {

        tmb =
            (10 * peso) +
            (6.25 * altura_cm) -
            (5 * edad) - 161;
    }

    // FACTOR DE ACTIVIDAD

    double factorActividad = 1.2;

    if (actividad == "Ligera")
        factorActividad = 1.375;

    else if (actividad == "Moderada")
        factorActividad = 1.55;

    else if (actividad == "Intensa")
        factorActividad = 1.725;

    // CALORIAS RECOMENDADAS

    double caloriasRecomendadas =
        tmb * factorActividad;

    // MACRONUTRIENTES RECOMENDADOS

    double proteinasRecomendadas =
        peso * 1.6;

    double carbohidratosRecomendados =
        (caloriasRecomendadas * 0.50) / 4;

    double grasasRecomendadas =
        (caloriasRecomendadas * 0.25) / 9;

    double fibraRecomendada = 30;

    // CONSULTA DE CONSUMO

    string consultaConsumo =
        "SELECT "
        "SUM((r.cantidad_proporcional * l.peso_equivalente_g * l.calorias) / 100), "
        "SUM((r.cantidad_proporcional * l.peso_equivalente_g * l.proteinas) / 100), "
        "SUM((r.cantidad_proporcional * l.peso_equivalente_g * l.carbohidratos) / 100), "
        "SUM((r.cantidad_proporcional * l.peso_equivalente_g * l.grasas) / 100), "
        "SUM((r.cantidad_proporcional * l.peso_equivalente_g * l.fibra) / 100) "
        "FROM registros_comida r "
        "INNER JOIN lista_alimentos l "
        "ON r.nombre_alimento = l.nombre_alimento "
        "WHERE r.id_usuario = " +
        to_string(ID_USUARIO_ACTUAL);


    if (mysql_query(conn, consultaConsumo.c_str())) {

        cout << "Error SQL nutricional: "
            << mysql_error(conn)
            << endl;

        return;
    }


    MYSQL_RES* res2 = mysql_store_result(conn);
    MYSQL_ROW row2 = mysql_fetch_row(res2);

    // CONSUMO TOTAL

    double caloriasConsumidas =
        (row2 && row2[0]) ? atof(row2[0]) : 0;

    double proteinasConsumidas =
        (row2 && row2[1]) ? atof(row2[1]) : 0;

    double carbohidratosConsumidos =
        (row2 && row2[2]) ? atof(row2[2]) : 0;

    double grasasConsumidas =
        (row2 && row2[3]) ? atof(row2[3]) : 0;

    double fibrasConsumidas =
        (row2 && row2[4]) ? atof(row2[4]) : 0;


    mysql_free_result(res2);

    // IMPRESION DE DATOS
    cout << " " << endl;
    cout << " DATOS DEL USUARIO" << endl;
    cout << "==================================================" << endl;

    cout << " Nombre:       " << nombre << endl;
    cout << " Edad:         " << edad << endl;
    cout << " Genero:       " << genero << endl;
    cout << " Peso:         " << peso << " kg" << endl;
    cout << " Altura:       " << altura << " m" << endl;
    cout << " Actividad:    " << actividad << endl;
    cout << " Ubicacion:    "
        << departamento
        << ", "
        << municipio
        << endl;

    // ANALISIS DE SALUD
    cout << " " << endl;
    cout << " ANALISIS DE SALUD" << endl;
    cout << "==================================================" << endl;

    cout << " IMC: "
        << fixed << setprecision(2)
        << imc
        << " ("
        << estadoIMC
        << ")"
        << endl;

    cout << " TMB: "
        << tmb
        << " kcal"
        << endl;

    // ANALISIS NUTRICIONAL
    cout << " " << endl;
    cout << " ANALISIS NUTRICIONAL" << endl;

    analizarMacronutriente(
        "CALORIAS",
        caloriasConsumidas,
        caloriasRecomendadas,
        "kcal"
    );

    analizarMacronutriente(
        "PROTEINAS",
        proteinasConsumidas,
        proteinasRecomendadas,
        "g"
    );

    analizarMacronutriente(
        "CARBOHIDRATOS",
        carbohidratosConsumidos,
        carbohidratosRecomendados,
        "g"
    );

    analizarMacronutriente(
        "GRASAS",
        grasasConsumidas,
        grasasRecomendadas,
        "g"
    );

    analizarMacronutriente(
        "FIBRAS",
        fibrasConsumidas,
        fibraRecomendada,
        "g"
    );

    cout << "\n==================================================" << endl;
    cout << "Presione Enter para regresar...";
    cout << endl;

    cin.ignore((numeric_limits<streamsize>::max)(), '\n');
    cin.get();
}

//  INICIO DE SESIÓN 
void iniciarSesion() {
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


// MENÚ PRINCIPAL
int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    int opcion;
    do {
        dibujarCabecera("SISTEMA DE GESTION NUTRICIONAL PROYECTOFN");
        cout << "\n          [1] INICIAR SESION" << endl;
        cout << "          [2] CREAR CUENTA NUEVA" << endl;
        cout << "          [3] SALIR DEL SISTEMA" << endl;
        cout << "\n  Seleccione una opcion: ";

        if (!(cin >> opcion)) {
            cin.clear();
            cin.ignore(1000, '\n');
            continue;
        }

        switch (opcion) {
        case 1: iniciarSesion(); break;
        case 2: registrarUsuario(); break;
        case 3:
            limpiarPantalla();
            cout << "\n GRACIAS POR USAR EL SISTEMA. HASTA PRONTO.\n" << endl;
            break;
        }
    } while (opcion != 3);

    return 0;
}