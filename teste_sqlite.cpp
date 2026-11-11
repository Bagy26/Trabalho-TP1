#include <iostream>
#include "sqlite/sqlite3.h"

int main() {
    sqlite3* banco;

    int resultado = sqlite3_open("kanban.db", &banco);

    if (resultado == SQLITE_OK) {
        std::cout << "Banco de dados conectado com sucesso!" << std::endl;
    } else {
        std::cout << "Erro ao conectar: "
                  << sqlite3_errmsg(banco) << std::endl;
    }

    sqlite3_close(banco);

    return 0;
}