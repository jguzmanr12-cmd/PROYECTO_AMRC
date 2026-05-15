#include "conexion.h"

//función para establecer la conexión a la base de datos MySQL
MYSQL* conectar() {

    MYSQL* conn = mysql_init(0);

    conn = mysql_real_connect(
        conn,
        "localhost",
        "root",
        "",
        "proyectofn",
        3306,
        NULL,
        0
    );

    return conn;
}