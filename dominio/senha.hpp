#ifndef SENHA_HPP
#define SENHA_HPP

#include <string>
#include <stdexcept>

using namespace std;

class Senha {
private:
    string valor;

public:
    void setSenha(string novaSenha) {

        if (novaSenha.length() != 5) {
            throw invalid_argument("Senha invalida");
        }

        bool temLetra = false;
        bool temNumero = false;

        for (char c : novaSenha) {

            if ((c >= 'A' && c <= 'Z') ||
                (c >= 'a' && c <= 'z')) {
                temLetra = true;
            }
            else if (c >= '0' && c <= '9') {
                temNumero = true;
            }
            else {
                throw invalid_argument("Senha invalida");
            }
        }

        if (!temLetra || !temNumero) {
            throw invalid_argument("Senha invalida");
        }

        valor = novaSenha;
    }

    string getSenha() const {
        return valor;
    }
};

#endif
