#include "conexion.h"
#include <iostream>
using namespace std;

int conexionDB::conectarBD(string user, string passwd){
    SQLRETURN salida;
    //reserva espacio para el entorno 
    salida = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &entorno);
    //decirle el protocolo de comunicacion que va a usar
    salida = SQLSetEnvAttr(entorno,SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3,0);
    // reserva espacio para la conexion
    salida = SQLAllocHandle(SQL_HANDLE_DBC,entorno,&conexion);
    // desactivar el autocommit para que funcionen los safe point y los rollback
    salida = SQLSetConnectAttr(conexion, SQL_ATTR_AUTOCOMMIT, (SQLPOINTER)SQL_AUTOCOMMIT_OFF, 0);

    string conn_str = "Driver={Oracle};DBQ=oracle0.ugr.es:1521/practbd;Uid=" + user + ";Pwd=" + passwd + ";";

    SQLCHAR outstr[1024];
    SQLSMALLINT outstrlen;

    salida = SQLDriverConnect(conexion,NULL,(SQLCHAR*)conn_str.c_str(),SQL_NTS, outstr, sizeof(outstr), &outstrlen, SQL_DRIVER_NOPROMPT);
    
    if(salida == SQL_SUCCESS || salida == SQL_SUCCESS_WITH_INFO)
        return 0;
    else {
        SQLFreeHandle(SQL_HANDLE_DBC, conexion); 
        SQLFreeHandle(SQL_HANDLE_ENV, entorno);
        return 1;
    }
}

void conexionDB::desconectarBD(){
    SQLDisconnect(conexion);
    SQLFreeHandle(SQL_HANDLE_DBC,conexion);
    SQLFreeHandle(SQL_HANDLE_ENV,entorno);
}

void conexionDB::ejecutarCommit() {
    SQLEndTran(SQL_HANDLE_DBC, conexion, SQL_COMMIT);
}

void conexionDB::ejecutarRollBack(){
    SQLEndTran(SQL_HANDLE_DBC, conexion, SQL_ROLLBACK);
}

int conexionDB::crearSavepoint( string nombreSVP) {
    SQLHSTMT stmt; 
    SQLRETURN salida;
    //stmt es un manejador de sentencia, con esta funcion le pedimos un espacio de memoria para una senticia asociada a conexion 
    salida = SQLAllocHandle(SQL_HANDLE_STMT, conexion, &stmt);

    if (salida != SQL_SUCCESS && salida != SQL_SUCCESS_WITH_INFO) {
        cout << "Error: No se pudo reservar memoria para la sentencia." << endl;
        return 1; 
    }
    // El comando que se va a mandar
    string comando = "SAVEPOINT " + nombreSVP;

    // Manda el comando para ejecutarlo
    salida = SQLExecDirect(stmt, (SQLCHAR*)comando.c_str(), SQL_NTS);

    if (salida != SQL_SUCCESS && salida != SQL_SUCCESS_WITH_INFO) {
        cout << "Error: Fallo al crear SavePoint" << endl;
        SQLFreeHandle(SQL_HANDLE_STMT, stmt);
        return 1; 
    }

    //Libera la memoria del manejador que habiamos cogido antes
    SQLFreeHandle(SQL_HANDLE_STMT, stmt);

    return 0;
}

int conexionDB::rollbackASavepoint( string nombreSVP){
    SQLHSTMT stmt; 
    SQLRETURN salida;
    //stmt es un manejador de sentencia, con esta funcion le pedimos un espacio de memoria para una senticia asociada a conexion 
    salida = SQLAllocHandle(SQL_HANDLE_STMT, conexion, &stmt);

    if (salida != SQL_SUCCESS && salida != SQL_SUCCESS_WITH_INFO) {
        cout << "Error: No se pudo reservar memoria para la sentencia." << endl;
        return 1; 
    }

    // El comando que se va a mandar
    string comando = "ROLLBACK TO " + nombreSVP;

    // Manda el comando para ejecutarlo
    salida = SQLExecDirect(stmt, (SQLCHAR*)comando.c_str(), SQL_NTS);

    if (salida != SQL_SUCCESS && salida != SQL_SUCCESS_WITH_INFO) {
        cout << "Error: Fallo al ejecutar RollBack" << endl;
        SQLFreeHandle(SQL_HANDLE_STMT, stmt);
        return 1; 
    }

    //Libera la memoria del manejador que habiamos cogido antes
    SQLFreeHandle(SQL_HANDLE_STMT, stmt);

    return 0;
}
