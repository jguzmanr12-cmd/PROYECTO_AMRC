#pragma once
#ifndef ALIMENTOS_H
#define ALIMENTOS_H

#include "conexion.cpp"

using namespace std;

//estrustura de macros
struct Macronutrientes;

//declaramos las funciones para el registro de alimentos y su consumo
void acumularMacronutrientes(float gramosConsumidos, float caloriasBase, float proteinasBase, float carbohidratosBase, float grasasBase, float fibraBase);
void registrarConsumo(MYSQL* conn);

#endif
