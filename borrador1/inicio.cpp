#include <iostream>
#include <mysql.h> 
#include <string>  
#include <vector>
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

// MÓDULO: REGISTRO DE ALIMENTOS 
void registrarConsumo(MYSQL* conn) {
    string fecha, hora, nombre_alimento, unidad_txt, cant_formato;
    int tiempo_op, id_cat, id_maestro, id_unidad;
    float cantidad;
    MYSQL_RES* res;
    MYSQL_ROW row;

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
    string sql_datos = "SELECT nombre_alimento, peso_equivalente_g, calorias, u.nombre_unidad, u.id_unidad "
        "FROM lista_alimentos a JOIN unidades_medida u ON a.id_unidad = u.id_unidad "
        "WHERE id_maestro = " + to_string(id_maestro);
    mysql_query(conn, sql_datos.c_str());
    res = mysql_store_result(conn);
    row = mysql_fetch_row(res);

    if (row) {
        nombre_alimento = row[0];
        float peso_eq = atof(row[1]);
        float cal_base = atof(row[2]);
        unidad_txt = row[3];
        id_unidad = atof(row[4]);

        cout << "\n [5/5] CANTIDAD CONSUMIDA" << endl;
        cout << " > ¿Cuantas [" << unidad_txt << "] ingirio?: "; 
        cin >> cantidad;

        // Cálculos nutricionales
        float g_totales = cantidad * peso_eq;
        float c_final = (g_totales * cal_base) / 100.0;
        cant_formato = to_string((int)cantidad) ;

        // Guardado en Base de Datos
        string query = "INSERT INTO registros_comida (id_usuario, nombre_alimento, cantidad_proporcional, tiempo_comida, fecha_ingesta, hora_ingesta, id_unidad, nombre_unidad) VALUES ("
            + to_string(ID_USUARIO_ACTUAL) + ", '" + nombre_alimento + "', '" + cant_formato + "', '" + tiempo_comida + "', " + fecha + ", " + hora + ", " + to_string(id_unidad) + ", '" + unidad_txt +"')";

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
    cout << "\n Presione ENTER para volver al menu...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
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
    vector<string> regiones = { "Metropolitana", "Central", "Occidente" };

    map<string, vector<string>> departamentos = {
        {"Metropolitana", {"Guatemala"}},
        {"Central", {"Sacatepequez", "Chimaltenango", "Escuintla"}},
        {"Occidente", {"Quetzaltenango", "Huehuetenango", "San Marcos"}}
    };

    map<string, vector<string>> municipios = {
        {"Guatemala", {"Guatemala", "Mixco", "Villa Nueva"}},
        {"Sacatepequez", {"Antigua Guatemala", "Ciudad Vieja", "Jocotenango"}},
        {"Chimaltenango", {"Chimaltenango", "El Tejar", "Patzun"}},
        {"Escuintla", {"Escuintla", "Santa Lucia Cotzumalguapa", "La Democracia"}},
        {"Quetzaltenango", {"Quetzaltenango", "Olintepeque", "Salcaja"}},
        {"Huehuetenango", {"Huehuetenango", "Chiantla", "Malacatancito"}},
        {"San Marcos", {"San Marcos", "Malacatan", "Ayutla"}}
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
    actividadFisica = acts[opActividad];

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

    // CORRECCIÓN: Se cambió 'nombre_unitad' por 'nombre_unidad' 
    // y se ajustaron los nombres de las columnas para que coincidan con tu DB
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

        // Formateamos cantidad + unidad (ej: "100 Gramos")
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

    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

//CALENDARIO DE CONSUMO Y ESPESIFICACIONES
void calendario(MYSQL* conn, int id_usuario) {
    int anio, mes;

    cout << "\n==============================================================================" << endl;
    cout << "                         HISTORIAL NUTRICIONAL MENSUAL" << endl;
    cout << "==============================================================================" << endl;
    cout << " Ingrese el anio: "; cin >> anio;
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

void mostrarAnalisisUsuario(MYSQL* conn, string user) { // Ahora recibe conn
    if (!conn) return;

    dibujarCabecera("ANALISIS DE PERFIL NUTRICIONAL");

    // --- PRIMERA CONSULTA: DATOS USUARIO ---
    string query = "SELECT nombre_completo, edad, genero, peso, altura, actividad_fisica, departamento, municipio FROM usuarios WHERE usuario = '" + user + "'";
    if (mysql_query(conn, query.c_str())) {
        cout << "Error: " << mysql_error(conn) << endl;
        return;
    }

    MYSQL_RES* res = mysql_store_result(conn);
    MYSQL_ROW row = mysql_fetch_row(res);

    if (!row) {
        cout << "\n [!] No se encontraron datos para el usuario: " << user << endl;
        if (res) mysql_free_result(res);
        cout << "\nPresione Enter para volver...";
        cin.ignore(); cin.get();
        return;
    }

    // Mapeo seguro de datos
    string nombre = row[0] ? row[0] : "Sin nombre";
    int edad = row[1] ? atoi(row[1]) : 0;
    string genero = row[2] ? row[2] : "M";
    double peso = row[3] ? atof(row[3]) : 0;
    double altura = row[4] ? atof(row[4]) : 0;
    string actividad = row[5] ? row[5] : "Sedentaria";
    string departamento = row[6] ? row[6] : "N/A";
    string municipio = row[7] ? row[7] : "N/A";

    mysql_free_result(res);

    // --- CÁLCULOS SALUD ---
    double imc = (altura > 0) ? (peso / (altura * altura)) : 0;
    string estadoIMC = (imc < 18.5) ? "Bajo peso" : (imc < 25) ? "Normal" : (imc < 30) ? "Sobrepeso" : "Obesidad";

    double altura_cm = altura * 100;
    double tmb = (genero == "Masculino" || genero == "1") ?
        (10 * peso + 6.25 * altura_cm - 5 * edad + 5) :
        (10 * peso + 6.25 * altura_cm - 5 * edad - 161);

    double factor = 1.2;
    if (actividad == "Ligera") factor = 1.375;
    else if (actividad == "Moderada") factor = 1.55;
    else if (actividad == "Intensa") factor = 1.725;

    double caloriasRecomendadas = tmb * factor;

    // --- SEGUNDA CONSULTA: CALORÍAS CONSUMIDAS (Basado en imagen_956b3b.png) ---
    // IMPORTANTE: Asegurate que la tabla se llame 'registros_comida' o 'registro_diario' 
    // segun tu DB. Aqui uso 'registros_comida' porque asi esta en tu registrarConsumo.
    string q2 = "SELECT SUM((r.cantidad_proporcional * l.peso_equivalente_g * l.calorias) / 100) "
        "FROM registros_comida r "
        "JOIN lista_alimentos l ON r.nombre_alimento = l.nombre_alimento "
        "WHERE r.id_usuario = " + to_string(ID_USUARIO_ACTUAL);

    mysql_query(conn, q2.c_str());
    MYSQL_RES* res2 = mysql_store_result(conn);
    MYSQL_ROW row2 = mysql_fetch_row(res2);

    double caloriasConsumidas = (row2 && row2[0]) ? atof(row2[0]) : 0;
    if (res2) mysql_free_result(res2);

    // --- IMPRESIÓN DE RESULTADOS ---
    cout << "\n > Nombre:      " << nombre << endl;
    cout << " > Edad:        " << edad << " años" << endl;
    cout << " > Genero:      " << genero << endl;
    cout << " > Ubicacion:   " << departamento << ", " << municipio << endl;
    cout << " > Actividad:   " << actividad << endl;

    cout << "\n --- ANALISIS DE SALUD ---" << endl;
    cout << " > IMC:         " << fixed << setprecision(2) << imc << " (" << estadoIMC << ")" << endl;
    cout << " > TMB:         " << tmb << " kcal (Gasto base)" << endl;

    cout << "\n --- BALANCE CALORICO ---" << endl;
    cout << " > Recomendadas: " << caloriasRecomendadas << " kcal" << endl;
    cout << " > Consumidas:   " << caloriasConsumidas << " kcal" << endl;
    cout << " ---------------------------------------------------" << endl;

    if (caloriasConsumidas > caloriasRecomendadas)
        cout << " ESTADO: EXCESO CALORICO" << endl;
    else if (caloriasConsumidas < (caloriasRecomendadas * 0.8))
        cout << " ESTADO: DEFICIT CALORICO" << endl;
    else
        cout << " ESTADO: BALANCE ADECUADO" << endl;

    cout << "\n==============================================================================" << endl;
    cout << "Presione Enter para regresar al menu...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
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