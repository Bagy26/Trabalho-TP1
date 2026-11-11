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

    private:
    /**
     * @brief Identificador único do cartão de atividade.
     */
    Identificador identificador;

    /**
     * @brief Nome do cartão de atividade.
     */
    Nome nome;

    /**
     * @brief Descrição das atividades que devem ser realizadas.
     */
    Texto descricao;

    /**
     * @brief Prioridade atribuída ao cartão.
     */
    Prioridade prioridade;

    /**
     * @brief Estado atual do cartão no fluxo Kanban.
     */
    Estado estado;

    /**
     * @brief Tamanho estimado da atividade.
     */
    Tamanho tamanho;

    /**
     * @brief Data e horário de entrada do cartão na coluna A FAZER.
     */
    Timestamp entrada;

    /**
     * @brief Data e horário de início da execução da atividade.
     */
    Timestamp inicio;

    /**
     * @brief Data e horário de conclusão da atividade.
     */
    Timestamp termino;

    /**
     * @brief Email da pessoa responsável pelo desenvolvimento do cartão.
     */
    Email desenvolvedor;

    /**
     * @brief Identificador do quadro ao qual o cartão pertence.
     */
    Identificador quadro;

    /**
     * @brief Indica se o identificador do cartão já foi definido.
     */
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

    /**
     * @brief Define o desenvolvedor responsável pelo cartão.
     * @param novoDesenvolvedor Email da pessoa responsável.
     */
    void setDesenvolvedor(const Email& novoDesenvolvedor) {
        desenvolvedor = novoDesenvolvedor;
    }

    /**
     * @brief Retorna o email do desenvolvedor responsável.
     * @return Referência constante para o email do desenvolvedor.
     */
    const Email& getDesenvolvedor() const {
        return desenvolvedor;
    }

    /**
     * @brief Define o quadro ao qual o cartão pertence.
     * @param novoQuadro Identificador do quadro associado.
     */
    void setQuadro(const Identificador& novoQuadro) {
        quadro = novoQuadro;
    }

    /**
     * @brief Retorna o identificador do quadro associado.
     * @return Referência constante para o identificador do quadro.
     */
    const Identificador& getQuadro() const {
        return quadro;
    }


};

#endif
