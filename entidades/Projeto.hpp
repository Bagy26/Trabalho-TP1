#ifndef PROJETO_HPP
#define PROJETO_HPP

#include "../dominio/identificador.hpp"
#include "../dominio/nome.hpp"
#include "../dominio/texto.hpp"
#include "../dominio/timestamp.hpp"

#include <stdexcept>

/**
 * @class Projeto
 * @brief Representa um projeto cadastrado no sistema Kanban.
 *
 * Cada projeto possui um identificador único, nome,
 * descrição, data de início e data de término.
 */
class Projeto {
private:

    Identificador identificador;
    Nome nome;
    Texto descricao;
    Timestamp inicio;
    Timestamp termino;

    bool identificadorDefinido = false;

public:

    /**
     * @brief Define o identificador do projeto.
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
     * @brief Retorna o identificador do projeto.
     */
    const Identificador& getIdentificador() const {
        return identificador;
    }

    /**
     * @brief Define o nome do projeto.
     */
    void setNome(const Nome& novoNome) {
        nome = novoNome;
    }

    /**
     * @brief Retorna o nome do projeto.
     */
    const Nome& getNome() const {
        return nome;
    }

    /**
     * @brief Define a descrição do projeto.
     */
    void setDescricao(const Texto& novaDescricao) {
        descricao = novaDescricao;
    }

    /**
     * @brief Retorna a descrição do projeto.
     */
    const Texto& getDescricao() const {
        return descricao;
    }

    /**
     * @brief Define a data de início do projeto.
     */
    void setInicio(const Timestamp& novoInicio) {
        inicio = novoInicio;
    }

    /**
     * @brief Retorna a data de início do projeto.
     */
    const Timestamp& getInicio() const {
        return inicio;
    }

    /**
     * @brief Define a data de término do projeto.
     */
    void setTermino(const Timestamp& novoTermino) {
        termino = novoTermino;
    }

    /**
     * @brief Retorna a data de término do projeto.
     */
    const Timestamp& getTermino() const {
        return termino;
    }
};

#endif
