#ifndef CONEXIONDB_H
#define CONEXIONDB_H
#include <string>
#include <sql.h>
#include <sqlext.h>

using namespace std;

class conexionDB{
    public:
    // Inicializa el entorno ODBC, desactiva el autocommit y establece la conexión 
    // con la base de datos Oracle (practbd).
    int conectarBD(string user, string passwd);

    // Cierra la sesión con Oracle de forma segura y libera la memoria de los manejadores ODBC.
    void desconectarBD();

    // Confirma la transacción actual. Hace que todos los INSERT, UPDATE o DELETE 
    // ejecutados desde el inicio de la transacción sean permanentes y visibles.
    void ejecutarCommit();

    // Cancela la transacción actual por completo. Deshace todos los cambios realizados 
    // en la base de datos desde el último Commit o desde el inicio de la conexión.
    void ejecutarRollBack();

    // Crea un punto de guardado intermedio dentro de la transacción actual.
    // Útil para poder deshacer solo una parte del proceso sin cancelar todo el pedido.
    //SVP es SavePoint
    int crearSavepoint( string nombreSVP);

    // Deshace los cambios realizados únicamente hasta el punto de guardado indicado, 
    // manteniendo el resto de la transacción intacta y activa.
    //SVP es SavePoint
    int rollbackASavepoint( string nombreSVP);

    private:
    SQLHENV entorno;
    SQLHDBC conexion;
};
#endif 
