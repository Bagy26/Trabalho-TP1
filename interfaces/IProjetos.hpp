
#ifndef IPROJETOS_HPP
#define IPROJETOS_HPP

#include "../entidades/Projeto.hpp"
#include <vector>

/**
 * @brief Interface para operacoes relacionadas a projetos.
 */
class IProjetos {
public:
    virtual ~IProjetos() = default;

    virtual void cadastrar(const Projeto& projeto) = 0;

    virtual Projeto consultar(const Identificador& identificador) = 0;

    virtual std::vector<Projeto> listar() = 0;

    virtual void atualizar(const Projeto& projeto) = 0;

    virtual void remover(const Identificador& identificador) = 0;
};

#endif
