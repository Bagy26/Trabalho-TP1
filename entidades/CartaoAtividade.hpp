#ifndef CARTAO_ATIVIDADE_HPP
#define CARTAO_ATIVIDADE_HPP

#include "../dominio/identificador.hpp"
#include "../dominio/nome.hpp"
#include "../dominio/texto.hpp"
#include "../dominio/prioridade.hpp"
#include "../dominio/Estado.hpp"
#include "../dominio/Tamanho.hpp"
#include "../dominio/timestamp.hpp"

#include <stdexcept>

/**
 * @class CartaoAtividade
 * @brief Representa uma atividade de um quadro Kanban.
 *
 * Armazena os dados da atividade, sua prioridade,
 * seu estado e os timestamps utilizados nos cálculos
 * de tempo de ciclo e lead time.
 */
class CartaoAtividade {
private:

    Identificador identificador;
    Nome nome;
    Texto descricao;
    Prioridade prioridade;
    Estado estado;
    Tamanho tamanho;
    Timestamp entrada;
    Timestamp inicio;
    Timestamp termino;

    bool identificadorDefinido = false;

public:

    /**
     * @brief Define o identificador do cartão.
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
     * @brief Retorna o identificador do cartão.
     */
    const Identificador& getIdentificador() const {
        return identificador;
    }

    /**
     * @brief Define o nome da atividade.
     */
    void setNome(const Nome& novoNome) {
        nome = novoNome;
    }

    /**
     * @brief Retorna o nome da atividade.
     */
    const Nome& getNome() const {
        return nome;
    }

    /**
     * @brief Define a descrição da atividade.
     */
    void setDescricao(const Texto& novaDescricao) {
        descricao = novaDescricao;
    }

    /**
     * @brief Retorna a descrição da atividade.
     */
    const Texto& getDescricao() const {
        return descricao;
    }

    /**
     * @brief Define a prioridade da atividade.
     */
    void setPrioridade(const Prioridade& novaPrioridade) {
        prioridade = novaPrioridade;
    }

    /**
     * @brief Retorna a prioridade da atividade.
     */
    const Prioridade& getPrioridade() const {
        return prioridade;
    }

    /**
     * @brief Define o estado da atividade.
     */
    void setEstado(const Estado& novoEstado) {
        estado = novoEstado;
    }

    /**
     * @brief Retorna o estado da atividade.
     */
    const Estado& getEstado() const {
        return estado;
    }

    /**
     * @brief Define o tamanho da atividade.
     */
    void setTamanho(const Tamanho& novoTamanho) {
        tamanho = novoTamanho;
    }

    /**
     * @brief Retorna o tamanho da atividade.
     */
    const Tamanho& getTamanho() const {
        return tamanho;
    }

    /**
     * @brief Define o timestamp de entrada no quadro.
     */
    void setEntrada(const Timestamp& novaEntrada) {
        entrada = novaEntrada;
    }

    /**
     * @brief Retorna o timestamp de entrada no quadro.
     */
    const Timestamp& getEntrada() const {
        return entrada;
    }

    /**
     * @brief Define o timestamp de início da atividade.
     */
    void setInicio(const Timestamp& novoInicio) {
        inicio = novoInicio;
    }

    /**
     * @brief Retorna o timestamp de início da atividade.
     */
    const Timestamp& getInicio() const {
        return inicio;
    }

    /**
     * @brief Define o timestamp de término da atividade.
     */
    void setTermino(const Timestamp& novoTermino) {
        termino = novoTermino;
    }

    /**
     * @brief Retorna o timestamp de término da atividade.
     */
    const Timestamp& getTermino() const {
        return termino;
    }
};

#endif
