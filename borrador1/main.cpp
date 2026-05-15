#include <iostream>

#include "usuarios.h"
#include "utilidades.h"

using namespace std;

int main() {

	int opcion;

	do {

		dibujarCabecera("SISTEMA DE CONTROL NUTRICIONAL");

		cout << " 1. Iniciar Sesion" << endl;
		cout << " 2. Crear cuante" << endl;
		cout << " 3. Salir" << endl;

		cin >> opcion;

		if (!(cin >> opcion)) {
			cin.clear();
			cin.ignore(1000, '\n');
			continue;
		}

		switch (opcion) {
		case 1:
			iniciarSesion();
			break;
		case 2:
			registroUsuario();
		case 3:
			limpiarPantalla();
			cout << "\n GRACIAS POR USAR EL SISTEMA. HASTA PRONTO.\n" << endl;
			break;
		}
	} while (opcion != 3);
	return 0;
}