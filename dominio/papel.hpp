#ifndef PAPEL_HPP
#define PAPEL_HPP

#include <string>
#include <stdexcept>

using namespace std;

class Papel {
private:
    string valor;

public:
    void setPapel(string novoPapel) {
        if (novoPapel != "GESTOR" &&
            novoPapel != "DESENVOLVEDOR") {

            throw invalid_argument("Papel invalido");
        }

        valor = novoPapel;
    }

    string getPapel() const {
        return valor;
    }
};

#endif
