#ifndef SERVICOPESSOAS_HPP
#define SERVICOPESSOAS_HPP

#include "../interfaces/IPessoas.hpp"
#include "../containers/Pessoas.hpp"

/**
 * @brief Implementa os servicos relacionados a pessoas.
 */
class ServicoPessoas : public IPessoas {
private:

    // Container responsavel pelo acesso ao banco de dados.
    Pessoas& repositorio;

public:

    /**
     * @brief Recebe o container de pessoas.
     */
    explicit ServicoPessoas(Pessoas& pessoas)
        : repositorio(pessoas) {
    }

    /**
     * @brief Cadastra uma pessoa.
     */
    void cadastrar(const Pessoa& pessoa) override {
        repositorio.inserir(pessoa);
    }

    /**
     * @brief Consulta uma pessoa pelo email.
     */
    Pessoa consultar(const Email& email) override {
        return repositorio.consultar(email);
    }

    /**
     * @brief Lista todas as pessoas.
     */
    std::vector<Pessoa> listar() override {
        return repositorio.listar();
    }

    /**
     * @brief Atualiza os dados de uma pessoa.
     */
    void atualizar(const Pessoa& pessoa) override {
        repositorio.atualizar(pessoa);
    }

    /**
     * @brief Remove uma pessoa pelo email.
     */
    void remover(const Email& email) override {
        repositorio.remover(email);
    }
};

#endif
