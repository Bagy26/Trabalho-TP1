#ifndef TIMESTAMP_HPP
#define TIMESTAMP_HPP

#include <string>
#include <stdexcept>

class Timestamp {
private:
    std::string valor;

    // Verifica se o ano e bissexto
    bool bissexto(int ano) const {
        return (ano % 400 == 0) ||
               (ano % 4 == 0 && ano % 100 != 0);
    }

public:
    void setTimestamp(std::string novoTimestamp) {

        if (novoTimestamp.length() != 17) {
            throw std::invalid_argument("Timestamp invalido");
        }

        if (novoTimestamp[2] != '-' ||
            novoTimestamp[6] != '-' ||
            novoTimestamp[11] != '-' ||
            novoTimestamp[14] != ':') {

            throw std::invalid_argument("Formato invalido");
        }

        int posicoes[] = {
            0, 1, 7, 8, 9, 10, 12, 13, 15, 16
        };

        for (int pos : posicoes) {
            char c = novoTimestamp[pos];

            if (c < '0' || c > '9') {
                throw std::invalid_argument("Timestamp invalido");
            }
        }

        int dia = std::stoi(novoTimestamp.substr(0, 2));
        std::string mes = novoTimestamp.substr(3, 3);
        int ano = std::stoi(novoTimestamp.substr(7, 4));
        int hora = std::stoi(novoTimestamp.substr(12, 2));
        int minuto = std::stoi(novoTimestamp.substr(15, 2));

        std::string meses[] = {
            "JAN", "FEV", "MAR", "ABR",
            "MAI", "JUN", "JUL", "AGO",
            "SET", "OUT", "NOV", "DEZ"
        };

        int diasMes[] = {
            31, 28, 31, 30, 31, 30,
            31, 31, 30, 31, 30, 31
        };

        int indiceMes = -1;

        for (int i = 0; i < 12; i++) {
            if (mes == meses[i]) {
                indiceMes = i;
                break;
            }
        }

        if (indiceMes == -1 ||
            ano == 0 ||
            hora > 23 ||
            minuto > 59) {

            throw std::invalid_argument("Timestamp invalido");
        }

        if (indiceMes == 1 && bissexto(ano)) {
            diasMes[1] = 29;
        }

        if (dia < 1 || dia > diasMes[indiceMes]) {
            throw std::invalid_argument("Data invalida");
        }

        valor = novoTimestamp;
    }

    std::string getTimestamp() const {
        return valor;
    }
};

#endif
