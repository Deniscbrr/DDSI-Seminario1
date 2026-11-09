#ifndef TABLAS_H
#define TABLAS_H
#include <sql.h>
#include <sqlext.h>

void inicializar_tablas(SQLHDBC hdbc);   // opción 1 del menú
void mostrar_tablas(SQLHDBC hdbc);       // muestra el contenido actual

#endif
