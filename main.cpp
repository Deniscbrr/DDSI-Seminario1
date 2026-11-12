#include <iostream>
#include <string>
#include <limits>
#include "conexion.h"
#include "tablas.h"
#include "pedidos.h"

using namespace std;

int main() {
    conexionDB bd;
    string usuario, password;

    cout << "=======================================================" << endl;
    cout << "   DDSI - SEMINARIO 1: GESTION TRANSACCIONAL (ODBC)    " << endl;
    cout << "=======================================================" << endl;
    cout << "Usuario de Oracle (ej. x1234567): ";
    cin >> usuario;
    cout << "Password: ";
    cin >> password;

    cout << "\nConectando a oracle0.ugr.es:1521/practbd..." << endl;
    if (bd.conectarBD(usuario, password) != 0) {
        cerr << "\n[ERROR CRITICO] No se pudo establecer la conexion con Oracle." << endl;
        cerr << "Verifica tus credenciales y asegurate de tener activa la VPN de la UGR." << endl;
        return 1;
    }
    cout << "[OK] Conexion establecida con exito (Autocommit OFF)." << endl;

    int opcion = 0;
    bool salir = false;

    while (!salir) {
        cout << "\n------------------- MENU PRINCIPAL -------------------" << endl;
        cout << "1. Borrado, creacion de tablas e insercion de tuplas por defecto" << endl;
        cout << "2. Dar de alta nuevo pedido" << endl;
        cout << "3. Mostrar contenido de las tablas de la BD" << endl;
        cout << "4. Salir del programa y cerrar conexion" << endl;
        cout << "------------------------------------------------------" << endl;
        cout << "Seleccione una opcion (1-4): ";

        // Validación de entrada para evitar bucles infinitos si el usuario mete letras
        if (!(cin >> opcion)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "[ADVERTENCIA] Entrada no valida. Introduce un numero del 1 al 4." << endl;
            continue;
        }

        switch (opcion) {
            case 1:
                // Inicializa el esquema DDL y hace COMMIT interno (tablas.h)
                inicializar_tablas(bd.getConexion());
                // Mostramos las tablas para verificar las 10 tuplas de Stock
                mostrar_tablas(bd.getConexion());
                break;

            case 2:
                // Inicia la transacción del pedido con Savepoints (pedidos.h)
                altaNuevoPedido(&bd);
                break;

            case 3:
                // Lectura actual de las tablas (tablas.h)
                mostrar_tablas(bd.getConexion());
                break;

            case 4:
                // Por seguridad ACID: antes de cerrar, limpiamos cualquier transacción colgada
                bd.ejecutarRollBack();
                bd.desconectarBD();
                cout << "\nConexion cerrada de forma segura. ¡Hasta pronto!" << endl;
                salir = true;
                break;

            default:
                cout << "[ADVERTENCIA] Opcion fuera de rango. Seleccione entre 1 y 4." << endl;
                break;
        }
    }

    return 0;
}
