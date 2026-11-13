
#ifndef IPESSOAS_HPP
#define IPESSOAS_HPP

#include "../entidades/Pessoa.hpp"
#include <vector>

/**
 * @brief Interface para operacoes relacionadas a pessoas.
 */
class IPessoas {
public:
    virtual ~IPessoas() = default;

    virtual void cadastrar(const Pessoa& pessoa) = 0;

    virtual Pessoa consultar(const Email& email) = 0;

    virtual std::vector<Pessoa> listar() = 0;

    virtual void atualizar(const Pessoa& pessoa) = 0;

    virtual void remover(const Email& email) = 0;
};

#endif
