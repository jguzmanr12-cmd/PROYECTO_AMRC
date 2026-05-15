#include "alimentos.h"
#include "estructuras.h"
#include "conexion.h"
#include "utilidades.h"
#include "usuarios.h"
#include "estructuras.h"

#include <iostream>
#include <iomanip>

using namespace std;

//funcion para el registro de alimentos y su consumo
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

//funcion para el acumulado de macronutrientes
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