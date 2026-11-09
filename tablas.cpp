#include "tablas.h"
#include <iostream>
using namespace std;

static void mostrar_error(SQLHSTMT st) {
    SQLCHAR estado[6], msg[512]; SQLINTEGER nativo; SQLSMALLINT len;
    if (SQLGetDiagRec(SQL_HANDLE_STMT, st, 1, estado, &nativo, msg, sizeof(msg), &len) == SQL_SUCCESS)
        cout << "Error SQL [" << estado << "]: " << msg << endl;
}

static void ejecutar(SQLHDBC hdbc, const char* sql, bool ignorar_error = false) {
    SQLHSTMT st;
    SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &st);
    SQLRETURN r = SQLExecDirect(st, (SQLCHAR*)sql, SQL_NTS);
    if (!SQL_SUCCEEDED(r) && !ignorar_error) mostrar_error(st);
    SQLFreeHandle(SQL_HANDLE_STMT, st);
}

static void mostrar_consulta(SQLHDBC hdbc, const char* titulo, const char* sql) {
    cout << "\n--- " << titulo << " ---" << endl;
    SQLHSTMT st;
    SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &st);
    if (!SQL_SUCCEEDED(SQLExecDirect(st, (SQLCHAR*)sql, SQL_NTS))) {
        mostrar_error(st);
        SQLFreeHandle(SQL_HANDLE_STMT, st);
        return;
    }
    SQLSMALLINT ncols;
    SQLNumResultCols(st, &ncols);
    while (SQL_SUCCEEDED(SQLFetch(st))) {
        for (SQLSMALLINT i = 1; i <= ncols; i++) {
            char buf[256]; SQLLEN ind;
            SQLGetData(st, i, SQL_C_CHAR, buf, sizeof(buf), &ind);
            cout << (ind == SQL_NULL_DATA ? "NULL" : buf) << "\t";
        }
        cout << endl;
    }
    SQLFreeHandle(SQL_HANDLE_STMT, st);
}

void inicializar_tablas(SQLHDBC hdbc) {
    ejecutar(hdbc, "DROP TABLE Detalle_Pedido", true);
    ejecutar(hdbc, "DROP TABLE Pedido", true);
    ejecutar(hdbc, "DROP TABLE Stock", true);

    ejecutar(hdbc, "CREATE TABLE Stock (Cproducto NUMBER PRIMARY KEY, Cantidad NUMBER)");
    ejecutar(hdbc, "CREATE TABLE Pedido (Cpedido NUMBER PRIMARY KEY, Ccliente NUMBER, Fecha_pedido DATE)");
    ejecutar(hdbc, "CREATE TABLE Detalle_Pedido (Cpedido NUMBER REFERENCES Pedido(Cpedido), "
                   "Cproducto NUMBER REFERENCES Stock(Cproducto), Cantidad NUMBER, "
                   "PRIMARY KEY (Cpedido, Cproducto))");

    // Cambia estos valores por los del enunciado
    for (int i = 1; i <= 10; i++) {
        string sql = "INSERT INTO Stock VALUES (" + to_string(i) + ", " + to_string(i * 10) + ")";
        ejecutar(hdbc, sql.c_str());
    }
    SQLEndTran(SQL_HANDLE_DBC, hdbc, SQL_COMMIT);
    cout << "Tablas inicializadas." << endl;
}

void mostrar_tablas(SQLHDBC hdbc) {
    mostrar_consulta(hdbc, "Stock", "SELECT * FROM Stock ORDER BY Cproducto");
    mostrar_consulta(hdbc, "Pedido", "SELECT * FROM Pedido ORDER BY Cpedido");
    mostrar_consulta(hdbc, "Detalle_Pedido", "SELECT * FROM Detalle_Pedido ORDER BY Cpedido, Cproducto");
}
