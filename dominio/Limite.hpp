#ifndef LIMITE_HPP
#define LIMITE_HPP

#include <stdexcept>

class Limite {
private:
    int valor;

public:
    void setValor(int novoValor) {
        if (novoValor < 1 || novoValor > 25) {
            throw std::invalid_argument(
                "O limite WIP deve estar entre 1 e 25."
            );
        }

        valor = novoValor;
    }

    int getValor() const {
        return valor;
    }
};

#endif
