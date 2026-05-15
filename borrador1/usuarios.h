#pragma once
#ifndef USUARIOS_H
#define USUARIOS_H

using namespace std;

//declaramos variables globales para el manejo de usuarios
extern int ID_USUARIO_ACTUAL;
extern string NOMBRE_USUARIO_ACTUAL;

//declaramos funciones	de registro de usuarios y validacion de inicio de sesion
void registroUsuario();
void iniciarSesion();

#endif
