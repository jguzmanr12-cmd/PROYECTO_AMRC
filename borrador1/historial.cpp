#include "historial.h"

#include <iostream>
#include <iomanip>

using namespace std;

//funcion para los detalles del dia 
void verDetallesDia(MYSQL* conn, int id_usuario, string fecha) {
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

//funcion para el calendario del historial
void calendario(MYSQL* conn, int id_usuario) {
    int anio, mes;

    cout << "\n==============================================================================" << endl;
    cout << "                         HISTORIAL NUTRICIONAL MENSUAL" << endl;
    cout << "==============================================================================" << endl;
    cout << " Ingrese el año: "; cin >> anio;
    cout << " Ingrese el mes (1-12): "; cin >> mes;

    //suma proporcional de todos los nutrientes por día
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