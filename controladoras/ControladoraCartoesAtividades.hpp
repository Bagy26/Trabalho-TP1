#ifndef CONTROLADORA_CARTOES_ATIVIDADES_HPP
#define CONTROLADORA_CARTOES_ATIVIDADES_HPP

#include "../interfaces/ICartoesAtividades.hpp"
#include "../dominio/papel.hpp"

#include <stdexcept>
#include <vector>

class ControladoraCartoesAtividades
{
private:
    ICartoesAtividades &servico;

    void exigirGestor(const Papel &papel)
    {
        if (papel.getPapel() != "GESTOR")
        {
            throw std::runtime_error(
                "Operacao permitida somente para o gestor.");
        }
    }

    void exigirDesenvolvedor(const Papel &papel)
    {
        if (papel.getPapel() != "DESENVOLVEDOR")
        {
            throw std::runtime_error(
                "Operacao permitida somente para o desenvolvedor.");
        }
    }

public:
    explicit ControladoraCartoesAtividades(
        ICartoesAtividades &servicoCartoes) : servico(servicoCartoes) {}

    void cadastrar(
        const Papel &papel,
        const CartaoAtividade &cartao)
    {
        exigirGestor(papel);
        servico.cadastrar(cartao);
    }

    CartaoAtividade consultar(
        const Papel &papel,
        const Identificador &identificador)
    {
        if (papel.getPapel() != "GESTOR" &&
            papel.getPapel() != "DESENVOLVEDOR")
        {
            throw std::runtime_error("Papel sem permissao.");
        }

        return servico.consultar(identificador);
    }

    std::vector<CartaoAtividade> listar(const Papel &papel)
    {
        if (papel.getPapel() != "GESTOR" &&
            papel.getPapel() != "DESENVOLVEDOR")
        {
            throw std::runtime_error("Papel sem permissao.");
        }

        return servico.listar();
    }

    void atualizar(
        const Papel &papel,
        const CartaoAtividade &cartao)
    {
        exigirGestor(papel);
        servico.atualizar(cartao);
    }

    void remover(
        const Papel &papel,
        const Identificador &identificador)
    {
        exigirGestor(papel);
        servico.remover(identificador);
    }

    void mover(
        const Papel &papel,
        const Identificador &identificador,
        const Estado &novoEstado,
        const Timestamp &momento)
    {
        exigirDesenvolvedor(papel);
        servico.mover(identificador, novoEstado, momento);
    }

    int calcularWIP(
        const Papel &papel,
        const Identificador &quadro)
    {
        exigirDesenvolvedor(papel);
        return servico.calcularWIP(quadro);
    }

    double calcularLeadTimeMedio(
        const Papel &papel,
        const Identificador &quadro)
    {
        exigirDesenvolvedor(papel);
        return servico.calcularLeadTimeMedio(quadro);
    }

    double calcularCycleTimeMedio(
        const Papel &papel,
        const Identificador &quadro)
    {
        exigirDesenvolvedor(papel);
        return servico.calcularCycleTimeMedio(quadro);
    }
};

#endif
