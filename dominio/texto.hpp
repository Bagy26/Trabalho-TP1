
#ifndef TEXTO_HPP
#define TEXTO_HPP

#include <string>
#include <stdexcept>

class Texto {
private:
    std::string valor;

public:
    void setTexto(std::string novoTexto) {

        // Verifica se o texto esta vazio ou excede 30 caracteres
        if (novoTexto.empty() || novoTexto.length() > 30) {
            throw std::invalid_argument("Texto invalido");
        }

        // Verifica se o primeiro caractere e uma letra maiuscula
        char primeiro = novoTexto[0];

        if (primeiro < 'A' || primeiro > 'Z') {
            throw std::invalid_argument("Texto invalido");
        }

        // Verifica se o ultimo caractere e um ponto
        if (novoTexto[novoTexto.length() - 1] != '.') {
            throw std::invalid_argument("Texto invalido");
        }

        // Percorre todos os caracteres do texto
        for (size_t i = 0; i < novoTexto.length(); i++) {

            char c = novoTexto[i];

            // Verifica letras maiusculas e minusculas
            bool letra = (c >= 'A' && c <= 'Z') ||
                         (c >= 'a' && c <= 'z');

            // Verifica digitos
            bool numero = (c >= '0' && c <= '9');

            // Verifica espaco em branco
            bool espaco = (c == ' ');

            // Verifica sinais de pontuacao permitidos
            bool pontuacao = (c == ',' || c == ';' ||
                              c == '.' || c == ':' ||
                              c == '?' || c == '!');

            // Rejeita caracteres nao permitidos
            if (!letra && !numero && !espaco && !pontuacao) {
                throw std::invalid_argument("Texto invalido");
            }

            // Verifica se um sinal de pontuacao
            // e seguido por outro sinal de pontuacao
            if (pontuacao && i + 1 < novoTexto.length()) {

                char proximo = novoTexto[i + 1];

                bool proximaPontuacao =
                    (proximo == ',' || proximo == ';' ||
                     proximo == '.' || proximo == ':' ||
                     proximo == '?' || proximo == '!');

                if (proximaPontuacao) {
                    throw std::invalid_argument("Texto invalido");
                }
            }
        }

        // Armazena o texto caso todas as validacoes passem
        valor = novoTexto;
    }

    std::string getTexto() const {
        return valor;
    }
};

#endif
