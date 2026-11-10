
#ifndef IDENTIFICADOR_HPP
#define IDENTIFICADOR_HPP

#include <string>
#include <stdexcept>

class Identificador {
private:
    std::string valor;

public:
    void setIdentificador(std::string novoIdentificador) {

        if (novoIdentificador.length() != 6) {
            throw std::invalid_argument("Identificador invalido");
        }

        for (int i = 0; i < 3; i++) {
            char c = novoIdentificador[i];

            if (!((c >= 'A' && c <= 'Z') ||
                  (c >= 'a' && c <= 'z'))) {
                throw std::invalid_argument("Identificador invalido");
            }
        }

        for (int i = 3; i < 6; i++) {
            char c = novoIdentificador[i];

            if (c < '0' || c > '9') {
                throw std::invalid_argument("Identificador invalido");
            }
        }

        valor = novoIdentificador;

    } 

    std::string getIdentificador() const {
        return valor;
    } 

}; 

#endif
