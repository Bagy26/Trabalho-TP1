#ifndef QUADRO_HPP
#define QUADRO_HPP

#include "../dominio/identificador.hpp"
#include "../dominio/nome.hpp"
#include "../dominio/Limite.hpp"

#include <stdexcept>

/**
 * @class Quadro
 * @brief Representa um quadro Kanban pertencente a um projeto.
 *
 * Cada quadro possui um identificador único, um nome
 * e um limite máximo de atividades simultâneas na coluna FAZENDO.
 */
class Quadro {
private:

    Identificador identificador;
    Nome nome;
    Limite limite;

    bool identificadorDefinido = false;

public:

    /**
     * @brief Define o identificador do quadro.
     * @param novoIdentificador Identificador validado.
     * @throw std::invalid_argument Caso já tenha sido definido.
     */
    void setIdentificador(const Identificador& novoIdentificador) {

        if (identificadorDefinido) {
            throw std::invalid_argument(
                "O identificador nao pode ser alterado."
            );
        }

        identificador = novoIdentificador;
        identificadorDefinido = true;
    }

    /**
     * @brief Retorna o identificador do quadro.
     */
    const Identificador& getIdentificador() const {
        return identificador;
    }

    /**
     * @brief Define o nome do quadro.
     */
    void setNome(const Nome& novoNome) {
        nome = novoNome;
    }

    /**
     * @brief Retorna o nome do quadro.
     */
    const Nome& getNome() const {
        return nome;
    }

    /**
     * @brief Define o limite de atividades simultâneas.
     */
    void setLimite(const Limite& novoLimite) {
        limite = novoLimite;
    }

    /**
     * @brief Retorna o limite de atividades simultâneas.
     */
    const Limite& getLimite() const {
        return limite;
    }
};

#endif
