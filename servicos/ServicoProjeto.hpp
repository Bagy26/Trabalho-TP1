
#ifndef SERVICOPROJETOS_HPP
#define SERVICOPROJETOS_HPP

#include "../interfaces/IProjetos.hpp"
#include "../containers/Projetos.hpp"

/**
 * @brief Implementa os servicos relacionados aos projetos.
 */
class ServicoProjetos : public IProjetos {

private:

    /**
     * @brief Referencia ao container responsavel pela persistencia.
     */
    Projetos& repositorio;

public:

    /**
     * @brief Inicializa o servico com o container de projetos.
     * @param projetos Referencia ao container.
     */
    explicit ServicoProjetos(Projetos& projetos)
        : repositorio(projetos) {
    }

    /**
     * @brief Cadastra um projeto.
     */
    void cadastrar(const Projeto& projeto) override {
        repositorio.inserir(projeto);
    }

    /**
     * @brief Consulta um projeto pelo identificador.
     */
    Projeto consultar(const Identificador& identificador) override {
        return repositorio.consultar(identificador);
    }

    /**
     * @brief Lista todos os projetos.
     */
    std::vector<Projeto> listar() override {
        return repositorio.listar();
    }

    /**
     * @brief Atualiza um projeto.
     */
    void atualizar(const Projeto& projeto) override {
        repositorio.atualizar(projeto);
    }

    /**
     * @brief Remove um projeto pelo identificador.
     */
    void remover(const Identificador& identificador) override {
        repositorio.remover(identificador);
    }

};

#endif
