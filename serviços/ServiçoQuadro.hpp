#ifndef SERVICOQUADROS_HPP
#define SERVICOQUADROS_HPP

#include "../interfaces/IQuadros.hpp"
#include "../containers/Quadros.hpp"

/**
 * @brief Implementa os servicos relacionados aos quadros.
 */
class ServicoQuadros : public IQuadros {

private:

    /**
     * @brief Referencia ao container responsavel pela persistencia.
     */
    Quadros& repositorio;

public:

    /**
     * @brief Inicializa o servico com o container de quadros.
     * @param quadros Referencia ao container.
     */
    explicit ServicoQuadros(Quadros& quadros)
        : repositorio(quadros) {
    }

    /**
     * @brief Cadastra um quadro.
     */
    void cadastrar(const Quadro& quadro) override {
        repositorio.inserir(quadro);
    }

    /**
     * @brief Consulta um quadro pelo identificador.
     */
    Quadro consultar(const Identificador& identificador) override {
        return repositorio.consultar(identificador);
    }

    /**
     * @brief Lista todos os quadros.
     */
    std::vector<Quadro> listar() override {
        return repositorio.listar();
    }

    /**
     * @brief Atualiza um quadro.
     */
    void atualizar(const Quadro& quadro) override {
        repositorio.atualizar(quadro);
    }

    /**
     * @brief Remove um quadro pelo identificador.
     */
    void remover(const Identificador& identificador) override {
        repositorio.remover(identificador);
    }

};

#endif
