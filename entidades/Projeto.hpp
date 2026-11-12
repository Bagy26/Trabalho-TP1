#ifndef PROJETO_HPP
#define PROJETO_HPP

#include "../dominio/identificador.hpp"
#include "../dominio/nome.hpp"
#include "../dominio/texto.hpp"
#include "../dominio/timestamp.hpp"
#include "../dominio/email.hpp"

#include <stdexcept>

/**
 * @class Projeto
 * @brief Representa um projeto cadastrado no sistema Kanban.
 *
 * Cada projeto possui um identificador único, nome,
 * descrição, data de início, data de término e um gestor.
 */
class Projeto {
private:

    /**
     * @brief Identificador único do projeto.
     */
    Identificador identificador;

    /**
     * @brief Nome do projeto.
     */
    Nome nome;

    /**
     * @brief Descrição do projeto.
     */
    Texto descricao;

    /**
     * @brief Data de início do projeto.
     */
    Timestamp inicio;

    /**
     * @brief Data de término do projeto.
     */
    Timestamp termino;

    /**
     * @brief Email da pessoa responsável pelo projeto.
     */
    Email gestor;

    /**
     * @brief Indica se o identificador já foi definido.
     */
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
     * @return Referência constante para o identificador.
     */
    const Identificador& getIdentificador() const {
        return identificador;
    }

    /**
     * @brief Define o nome do projeto.
     * @param novoNome Nome validado do projeto.
     */
    void setNome(const Nome& novoNome) {
        nome = novoNome;
    }

    /**
     * @brief Retorna o nome do projeto.
     * @return Referência constante para o nome.
     */
    const Nome& getNome() const {
        return nome;
    }

    /**
     * @brief Define a descrição do projeto.
     * @param novaDescricao Descrição validada.
     */
    void setDescricao(const Texto& novaDescricao) {
        descricao = novaDescricao;
    }

    /**
     * @brief Retorna a descrição do projeto.
     * @return Referência constante para a descrição.
     */
    const Texto& getDescricao() const {
        return descricao;
    }

    /**
     * @brief Define a data de início do projeto.
     * @param novoInicio Data de início validada.
     */
    void setInicio(const Timestamp& novoInicio) {
        inicio = novoInicio;
    }

    /**
     * @brief Retorna a data de início do projeto.
     * @return Referência constante para a data de início.
     */
    const Timestamp& getInicio() const {
        return inicio;
    }

    /**
     * @brief Define a data de término do projeto.
     * @param novoTermino Data de término validada.
     */
    void setTermino(const Timestamp& novoTermino) {
        termino = novoTermino;
    }

    /**
     * @brief Retorna a data de término do projeto.
     * @return Referência constante para a data de término.
     */
    const Timestamp& getTermino() const {
        return termino;
    }

    /**
     * @brief Define o gestor responsável pelo projeto.
     * @param novoGestor Email da pessoa responsável.
     */
    void setGestor(const Email& novoGestor) {
        gestor = novoGestor;
    }

    /**
     * @brief Retorna o email do gestor do projeto.
     * @return Referência constante para o email do gestor.
     */
    const Email& getGestor() const {
        return gestor;
    }
};

#endif
