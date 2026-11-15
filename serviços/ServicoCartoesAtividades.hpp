#ifndef SERVICOCARTOESATIVIDADES_HPP
#define SERVICOCARTOESATIVIDADES_HPP

#include "../interfaces/ICartoesAtividades.hpp"
#include "../containers/CartoesAtividades.hpp"

/**
 * @brief Implementa os servicos relacionados aos cartoes de atividades.
 */
class ServicoCartoesAtividades : public ICartoesAtividades {

private:

    /**
     * @brief Referencia ao container responsavel pela persistencia.
     */
    CartoesAtividades& repositorio;

public:

    /**
     * @brief Inicializa o servico com o container de cartoes.
     * @param cartoes Referencia ao container.
     */
    explicit ServicoCartoesAtividades(CartoesAtividades& cartoes)
        : repositorio(cartoes) {
    }

    /**
     * @brief Cadastra um cartao de atividade.
     */
    void cadastrar(const CartaoAtividade& cartao) override {
        repositorio.inserir(cartao);
    }

    /**
     * @brief Consulta um cartao pelo identificador.
     */
    CartaoAtividade consultar(
        const Identificador& identificador
    ) override {
        return repositorio.consultar(identificador);
    }

    /**
     * @brief Lista todos os cartoes cadastrados.
     */
    std::vector<CartaoAtividade> listar() override {
        return repositorio.listar();
    }

    /**
     * @brief Atualiza os dados de um cartao.
     */
    void atualizar(const CartaoAtividade& cartao) override {
        repositorio.atualizar(cartao);
    }

    /**
     * @brief Remove um cartao pelo identificador.
     */
    void remover(const Identificador& identificador) override {
        repositorio.remover(identificador);
    }

    /**
     * @brief Move um cartao para o proximo estado permitido.
     */
    void mover(
        const Identificador& identificador,
        const Estado& novoEstado,
        const Timestamp& momento
    ) override {
        repositorio.mover(identificador, novoEstado, momento);
    }

};

#endif
