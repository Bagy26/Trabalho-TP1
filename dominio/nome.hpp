#ifndef NOME_HPP
#define NOME_HPP

#include <string>
#include <stdexcept>

class Nome {
private:
    std::string valor;

public:
    void setNome(std::string novoNome) {

        if (novoNome.empty() || novoNome.length() > 15) {
            throw std::invalid_argument("Nome invalido");
        }

        // Verifica se o primeiro ou ultimo caractere e espaço
        if (novoNome[0] == ' ' ||
            novoNome[novoNome.length() - 1] == ' ') {

            throw std::invalid_argument("Nome invalido");
        }

        for (size_t i = 0; i < novoNome.length(); i++) {

            char c = novoNome[i];

            bool letra = (c >= 'A' && c <= 'Z') ||
                         (c >= 'a' && c <= 'z');

            if (!letra && c != ' ') {
                throw std::invalid_argument("Nome invalido");
            }

            if (c == ' ') {

                char proximo = novoNome[i + 1];

                bool proximaLetra =
                    (proximo >= 'A' && proximo <= 'Z') ||
                    (proximo >= 'a' && proximo <= 'z');

                if (!proximaLetra) {
                    throw std::invalid_argument("Nome invalido");
                }
            }
        }

        valor = novoNome;
    }

    std::string getNome() const {
        return valor;
    }
};

#endif

