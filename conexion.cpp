#include "conexion.h"

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
    else
        return 1;
}
