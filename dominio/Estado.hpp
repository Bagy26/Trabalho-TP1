
#ifndef ESTADO_HPP
#define ESTADO_HPP

#include <string>
#include <stdexcept>

class Estado {
private:
    std::string valor;

public:
    void setEstado(std::string novoEstado) {
        if (novoEstado != "A FAZER" &&
            novoEstado != "FAZENDO" &&
            novoEstado != "FEITO") {

            throw std::invalid_argument("Estado invalido");
        }

        valor = novoEstado;
    }

    std::string getEstado() const {
        return valor;
    }
};

#endif