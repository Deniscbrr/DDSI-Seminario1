#ifndef PEDIDOS_H
#define PEDIDOS_H

#include "conexion.h"

//Da de alta a un nuevo pedido con sus detalles, gestionando la transacción mediante savepoint, rollback y commit.

void altaNuevoPedido(conexionDB* bd);

#endif 
