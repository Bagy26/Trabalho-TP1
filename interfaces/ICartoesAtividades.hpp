
#ifndef ICARTOESATIVIDADES_HPP
#define ICARTOESATIVIDADES_HPP

#include "../entidades/CartaoAtividade.hpp"
#include <vector>

/**
 * @brief Interface para operacoes relacionadas aos cartoes.
 */
class ICartoesAtividades {
public:
    virtual ~ICartoesAtividades() = default;

    virtual void cadastrar(const CartaoAtividade& cartao) = 0;

    virtual CartaoAtividade consultar(
        const Identificador& identificador
    ) = 0;

    virtual std::vector<CartaoAtividade> listar() = 0;

    virtual void atualizar(const CartaoAtividade& cartao) = 0;

    virtual void remover(const Identificador& identificador) = 0;

    virtual void mover(
        const Identificador& identificador,
        const Estado& novoEstado,
        const Timestamp& momento
    ) = 0;
};

#endif
