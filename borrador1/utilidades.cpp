#include "utilidades.h"

#include <iostream>
#include <windows.h>
#include <cstdlib>

using namespace std;

//funcion encargada de limpiar patalla cada que sea necesario
void limpiarPantalla() {

#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

//funcion para realizar la cabezera de cada ventana
void dibujarCabecera(string titulo) {

    limpiarPantalla();
    cout << "==============================================================================" << endl;
    cout << "      " << titulo << endl;
    cout << "==============================================================================" << endl;
}
