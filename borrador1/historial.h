#pragma once
#ifndef HISTORIAL_H
#define HISTORIAL_H

#include <string>
#include "conexion.cpp"

using namespace std;

//declaracion de funciones para el historial de consumo
void verDetalleDia(MYSQL* conn, int id_usuario, string fecha);
void calendario(MYSQL* conn, int id_usuario);

#endif
