// Aquí va la lógica de las transacciones
#include <iostream>
#include <string>
#include "pedidos.h"
#include "conexion.h"
#include "tablas.h"

using namespace std;

//Función auxiliar para lanzar los INSERT y UPDATE mediante ODNC
static void ejecutarDML(SQLHDBC hdbc, const string& sql) {
    SQLHSTMT stmt;
    SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &stmt);
    SQLExecDirect(stmt, (SQLCHAR*)sql.c_str(), SQL_NTS);
    SQLFreeHandle(SQL_HANDLE_STMT, stmt);
}

void altaNuevoPedido(conexionDB* bd) {
    SQLHDBC hdbc = bd->getConexion();

    int cpedido, ccliente;
    string fecha;

    //Datos del pedido
    cout << "\n---ALTA DE NUEVO PEDIDO ---" << endl;
    cout << "Codigo de pedido: ";
    cin >> cpedido;
    cout << "Codigo de cliente: ";
    cin >> ccliente;
    cout << "Fecha del pedido: ";
    cin >> ws;
    getline(cin, fecha);


    //Insertar pedido
    string sqlInsertPedido = "INSERT INTO Pedido (Cpedido, Ccliente, Fecha_pedido) VALUES (" + 
                              to_string(cpedido) + ", " + to_string(ccliente) + ", TO_DATE('" + fecha + "', 'DD/MM/YYYY'))";

    ejecutarDML(hdbc, sqlInsertPedido);


    //Establecemos el punto guardado
    bd->crearSavepoint("inicio_detalles");

    int opcionSubmenu = 0;
    while (true) {
        cout << "\n--- GESTION DE DETALLES DEL PEDIDO " << cpedido << " ---" << endl;
        cout << "1. Añadir detalle de producto" << endl;
        cout << "2. Eliminar todos los detalles de producto" << endl;
        cout << "3. Cancelar pedido" << endl;
        cout << "4. Finalizar pedido" << endl;
        cout << "Selecciona una opcion: ";
        cin >> opcionSubmenu;

        if (opcionSubmenu == 1) {
            int cproducto, cantidad;
            cout << "Codigo del producto: ";
            cin >> cproducto;
            cout << "cantidad solicitada: ";
            cin >> cantidad;

            //Insertamos el detalle en la base de datos
            string sqlInsertDetalle = "INSERT INTO Detalle_Pedido (Cpedido, Cproducto, Cantidad) VALUES (" + 
                                      to_string(cpedido) + ", " + to_string(cproducto) + ", " + to_string(cantidad) + ")";

            ejecutarDML(hdbc, sqlInsertDetalle);

            //Actualizamos restando el stock
            string sqlUpdateStock = "UPDATE Stock SET Cantidad = Cantidad - " + to_string(cantidad) +
                                     " WHERE Cproducto = " + to_string(cproducto);
                
            ejecutarDML(hdbc, sqlUpdateStock);

            cout << "Detalle añadido. Stock actualizado." << endl;

        }else if (opcionSubmenu == 2) {
            //Revertimos hasta el savepoint
            bd->rollbackASavepoint("inicio_detalles");

            //Volvemos a fijarlo
            bd->crearSavepoint("inicio_detalles");

            cout << "Detalles eliminados. Puedes volver a insertar." << endl;

        }else if (opcionSubmenu == 3) {
            //Cancelación total
            bd->ejecutarRollBack();

            cout << "Pedido cancelado por completo. Cambios revertidos." << endl;
            break;

        }else if (opcionSubmenu == 4) {
            //Confirmación permanente 
            bd->ejecutarCommit();
            cout << "Pedido finalizado y consolidado." << endl;
            break;

        }else {
            cout << "Opcion invalida." << endl;
        }
    }

    //Imprimimos el estado
    mostrar_tablas(hdbc);
    
}