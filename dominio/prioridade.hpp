#ifndef PRIORIDADE_HPP
#define PRIORIDADE_HPP

#include <string>
#include <stdexcept>

using namespace std;

class Prioridade {
private:
    string valor;

public:
    void setPrioridade(string novaPrioridade) {
        if (novaPrioridade != "ALTA" &&
            novaPrioridade != "MEDIA" &&
            novaPrioridade != "BAIXA") {

            throw invalid_argument("Prioridade invalida");
        }

        valor = novaPrioridade;
    }

    string getPrioridade() const {
        return valor;
    }
};

#endif
