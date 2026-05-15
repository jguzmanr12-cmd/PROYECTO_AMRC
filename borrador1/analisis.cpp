#include "analisis.h"
#include "utilidades.h"
#include "usuarios.h"

#include <iomanip>

using namespace std;

//funcion para analizar el consumo de un macronutriente
void analizarMacronutriente(string nombre, double consumido, double recomendado, string unidad) {
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

//funcion para mostrar el analisis de consumo del usuario
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

	//consulta para obtener el consumo total del usuario

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
    cout << " Ubicacion:    "<< departamento<< ", "<< municipio<< endl;

	//analisis de salud
    cout << " " << endl;
    cout << " ANALISIS DE SALUD" << endl;
    cout << "==================================================" << endl;

    cout << " IMC: " << fixed << setprecision(2) << imc << " (" << estadoIMC << ")" << endl;

    cout << " TMB: "<< tmb<< " kcal"<< endl;

	//analisis de consumo
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
