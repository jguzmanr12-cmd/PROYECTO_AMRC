#include <iostream>
#include <mysql.h> 
#include <string>  
#include <vector>
#include <iomanip>
#include <limits>

using namespace std;

// Variable global para rastrear al usuario que inició sesión
int ID_USUARIO_ACTUAL = 0;
string NOMBRE_USUARIO_ACTUAL = "";

// --- FUNCIÓN DE CONEXIÓN ---
MYSQL* conectar() {
    MYSQL* conn = mysql_init(0);
    conn = mysql_real_connect(conn, "localhost", "root", "", "proyectofn", 3306, NULL, 0);
    return conn;
}

// --- UTILIDAD: LIMPIAR PANTALLA ---
void limpiarPantalla() {
    // "cls" para Windows, "clear" para Linux/Mac
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// --- UTILIDAD: CABECERAS DE VENTANA ---
void dibujarCabecera(string titulo) {
    limpiarPantalla();
    cout << "==============================================================================" << endl;
    cout << "      " << titulo << endl;
    cout << "==============================================================================" << endl;
}

// --- MÓDULO: REGISTRO DE ALIMENTOS (FLUJO COMPLETO) ---
void registrarConsumo(MYSQL* conn) {
    string fecha, hora, nombre_alimento, unidad_txt, cant_formato;
    int tiempo_op, id_cat, id_maestro;
    float cantidad;
    MYSQL_RES* res;
    MYSQL_ROW row;

    dibujarCabecera("NUEVO REGISTRO DE INGESTA");

    // 1. Preguntar Fecha
    cout << "\n [1/5] FECHA DE CONSUMO" << endl;
    cout << " > Ingrese fecha (YYYY-MM-DD) o escriba 'hoy': ";
    cin >> fecha;
    if (fecha == "hoy") fecha = "CURRENT_DATE"; else fecha = "'" + fecha + "'";

    // 2. Preguntar Hora
    cout << "\n [2/5] HORA DEL CONSUMO" << endl;
    cout << " > Ingrese hora (HH:MM) o escriba 'ahora': ";
    cin >> hora;
    if (hora == "ahora") hora = "CURRENT_TIME"; else hora = "'" + hora + ":00'";

    // 3. Preguntar Tiempo de Comida
    cout << "\n [3/5] TIEMPO DE COMIDA" << endl;
    cout << " 1. Desayuno | 2. Almuerzo | 3. Cena | 4. Refaccion" << endl;
    cout << " > Seleccione una opcion: ";
    cin >> tiempo_op;
    string tiempos[] = { "Desayuno", "Almuerzo", "Cena", "Refaccion" };
    string tiempo_comida = (tiempo_op >= 1 && tiempo_op <= 4) ? tiempos[tiempo_op - 1] : "Refaccion";

    // 4. Selección de Alimento (Navegación completa)
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
        "WHERE a.id_categoria = " + to_string(id_cat);
    mysql_query(conn, consulta.c_str());
    res = mysql_store_result(conn);

    cout << "\n" << left << setw(6) << " ID" << setw(40) << "ALIMENTO" << setw(15) << "UNIDAD" << "KCAL/100g" << endl;
    cout << " ----------------------------------------------------------------------------" << endl;
    while ((row = mysql_fetch_row(res))) {
        cout << " [" << row[0] << "] " << setw(37) << row[1] << setw(15) << row[2] << row[3] << endl;
    }
    mysql_free_result(res);

    cout << "\n > Ingrese el ID del Alimento elegido: "; cin >> id_maestro;

    // 5. Cantidad y Cálculos Finales
    string sql_datos = "SELECT nombre_alimento, peso_equivalente_g, calorias, u.nombre_unidad "
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

        cout << "\n [5/5] CANTIDAD CONSUMIDA" << endl;
        cout << " > ¿Cuantas [" << unidad_txt << "] ingirio?: "; cin >> cantidad;

        // Cálculos nutricionales proporcionales
        float g_totales = cantidad * peso_eq;
        float c_final = (g_totales * cal_base) / 100.0;
        cant_formato = to_string((int)cantidad) + " " + unidad_txt;

        // Guardado en Base de Datos
        string query = "INSERT INTO registros_comida (id_usuario, nombre_alimento, cantidad_proporcional, tiempo_comida, fecha_ingesta, hora_ingesta) VALUES ("
            + to_string(ID_USUARIO_ACTUAL) + ", '" + nombre_alimento + "', '" + cant_formato + "', '" + tiempo_comida + "', " + fecha + ", " + hora + ")";

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

// --- MÓDULO: REGISTRO DE USUARIO COMPLETO ---
void registrarUsuario() {
    MYSQL* conn = conectar();
    if (!conn) return;

    string n, u, p, gen, dep, mun, act, ocu;
    int ed, opG, opR, opA;
    double pes, alt;

    dibujarCabecera("CREAR NUEVA CUENTA DE USUARIO");

    cout << " > Nombre Completo: "; cin.ignore(); getline(cin, n);
    cout << " > Edad: "; cin >> ed;
    cout << " > Genero (1. Masc / 2. Fem): "; cin >> opG;
    gen = (opG == 1) ? "Masculino" : "Femenino";

    cout << " > Region (1. Metropolitana / 2. Central / 3. Occidente): "; cin >> opR;
    dep = (opR == 1) ? "Guatemala" : (opR == 2) ? "Sacatepequez" : "Quetzaltenango";
    mun = "Cabecera";

    cout << " > Peso (kg): "; cin >> pes;
    cout << " > Altura (m): "; cin >> alt;
    cout << " > Actividad (1. Sedentaria / 2. Ligera / 3. Moderada / 4. Intensa): "; cin >> opA;
    string acts[] = { "Sedentaria", "Ligera", "Moderada", "Intensa" }; act = acts[opA - 1];

    cout << " > Ocupacion: "; cin.ignore(); getline(cin, ocu);
    cout << " > Usuario unico: "; cin >> u;
    cout << " > Contrasena: "; cin >> p;

    string query = "INSERT INTO usuarios (nombre_completo, edad, genero, departamento, municipio, peso, altura, actividad_fisica, ocupacion, usuario, password) VALUES ('"
        + n + "', " + to_string(ed) + ", '" + gen + "', '" + dep + "', '" + mun + "', " + to_string(pes) + ", " + to_string(alt) + ", '" + act + "', '" + ocu + "', '" + u + "', '" + p + "')";

    if (mysql_query(conn, query.c_str()) == 0) {
        cout << "\n [OK] Perfil creado con exito. Presione ENTER para continuar...";
    }
    else {
        cout << "\n [!] Error al registrar: " << mysql_error(conn) << endl;
    }
    mysql_close(conn);
    cin.ignore(); cin.get();
}

// --- MÓDULO: INICIO DE SESIÓN ---
void iniciarSesion() {
    MYSQL* conn = conectar();
    string u, p;

    dibujarCabecera("ACCESO AL SISTEMA");

    cout << "\n  USUARIO: "; cin >> u;
    cout << "  CONTRASEÑA: "; cin >> p;

    string query = "SELECT id, nombre_completo FROM usuarios WHERE usuario = '" + u + "' AND password = '" + p + "'";
    mysql_query(conn, query.c_str());
    MYSQL_RES* res = mysql_store_result(conn);

    if (res && mysql_num_rows(res) > 0) {
        MYSQL_ROW row = mysql_fetch_row(res);
        ID_USUARIO_ACTUAL = stoi(row[0]);
        NOMBRE_USUARIO_ACTUAL = row[1];
        mysql_free_result(res);

        int op;
        do {
            dibujarCabecera("PANEL DE CONTROL - " + NOMBRE_USUARIO_ACTUAL);
            cout << "\n  1. REGISTRAR CONSUMO DE ALIMENTO" << endl;
            cout << "  2. CERRAR SESION" << endl;
            cout << "\n  Seleccione una opcion: ";
            cin >> op;

            if (op == 1) registrarConsumo(conn);
        } while (op != 2);

        mysql_close(conn);
    }
    else {
        cout << "\n [!] Credenciales incorrectas. Intente de nuevo." << endl;
        mysql_close(conn);
        cin.ignore(); cin.get();
    }
}

// --- FUNCIÓN PRINCIPAL (MENÚ DE ENTRADA) ---
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