
#ifndef IQUADROS_HPP
#define IQUADROS_HPP

#include "../entidades/Quadro.hpp"
#include <vector>

/**
 * @brief Interface para operacoes relacionadas a quadros.
 */
class IQuadros {
public:
    virtual ~IQuadros() = default;

    virtual void cadastrar(const Quadro& quadro) = 0;

    virtual Quadro consultar(const Identificador& identificador) = 0;

    virtual std::vector<Quadro> listar() = 0;

    virtual void atualizar(const Quadro& quadro) = 0;

    virtual void remover(const Identificador& identificador) = 0;
};

#endif
