#pragma once
#ifndef ANALISIS_H
#define ANALISIS_H

#include "conexion.cpp"
#include <string>
  
using namespace std;

//declaracion de funciones para el analisis de datos
void analizarMacronutriente(string nombre, double consumido, double recomendado, string unidad);
void  mostrarAnalisisUsuario(MYSQL* conn, string user);


#endif
