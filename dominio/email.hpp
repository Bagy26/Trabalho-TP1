#ifndef EMAIL_HPP
#define EMAIL_HPP

#include <string>
#include <regex>
#include <stdexcept>

using namespace std;

class Email {
private:
    string valor;

public:
    void setEmail(string novoEmail) {

        // Verifica se o e-mail está vazio
        if (novoEmail.empty()) {
            throw invalid_argument("Email invalido");
        }

        // Verifica se existe o caractere @
        size_t posicaoArroba = novoEmail.find('@');

        if (posicaoArroba == string::npos) {
            throw invalid_argument("Email invalido");
        }

        // Verifica o tamanho da parte local (maximo 64)
        string parteLocal = novoEmail.substr(0, posicaoArroba);

        if (parteLocal.empty() || parteLocal.length() > 64) {
            throw invalid_argument("Email invalido");
        }

        // Expressao regular para validar o formato
        const regex padrao(
            R"(^[A-Za-z0-9_]+([.-][A-Za-z0-9][A-Za-z0-9_]*)*@([A-Za-z0-9]([A-Za-z0-9-]{0,253}[A-Za-z0-9])?)(\.[A-Za-z0-9]([A-Za-z0-9-]{0,253}[A-Za-z0-9])?)+$)"
        );

        if (!regex_match(novoEmail, padrao)) {
            throw invalid_argument("Email invalido");
        }

        // Armazena o email caso seja valido
        valor = novoEmail;
    }

    string getEmail() const {
        return valor;
    }
};

#endif
