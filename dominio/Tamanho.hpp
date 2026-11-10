#ifndef TAMANHO_HPP
#define TAMANHO_HPP

#include <string>
#include <stdexcept>

class Tamanho {
private:
    std::string valor;

public:
    void setTamanho(std::string novoTamanho) {

        if (novoTamanho != "GRANDE" &&
            novoTamanho != "MEDIO" &&
            novoTamanho != "PEQUENO") {

            throw std::invalid_argument("Tamanho invalido");
        }

        valor = novoTamanho;
    }

    std::string getTamanho() const {
        return valor;
    }
};

#endif
