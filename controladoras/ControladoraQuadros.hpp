#ifndef CONTROLADORA_QUADROS_HPP
#define CONTROLADORA_QUADROS_HPP

#include "../interfaces/IQuadros.hpp"
#include "../dominio/papel.hpp"

#include <stdexcept>
#include <vector>

/**
 * @class ControladoraQuadros
 * @brief Controla as operações relacionadas aos quadros.
 *
 * Responsável por verificar as permissões dos usuários
 * e encaminhar as operações para o serviço de quadros.
 *
 * O gestor pode cadastrar, consultar, atualizar, remover
 * e listar quadros.
 *
 * O desenvolvedor pode consultar e listar quadros.
 */
class ControladoraQuadros
{
private:
    /**
     * @brief Referência para a interface do serviço de quadros.
     */
    IQuadros &servico;

    /**
     * @brief Verifica se o usuário possui papel de gestor.
     * @param papel Papel do usuário autenticado.
     * @throw std::runtime_error Caso o usuário não seja gestor.
     */
    void exigirGestor(const Papel &papel)
    {
        if (papel.getPapel() != "GESTOR")
        {
            throw std::runtime_error(
                "Operacao permitida somente para o gestor.");
        }
    }

    /**
     * @brief Verifica se o usuário possui um papel válido.
     * @param papel Papel do usuário autenticado.
     * @throw std::runtime_error Caso o papel não seja reconhecido.
     */
    void exigirUsuarioValido(const Papel &papel)
    {
        if (papel.getPapel() != "GESTOR" &&
            papel.getPapel() != "DESENVOLVEDOR")
        {
            throw std::runtime_error("Usuario sem permissao.");
        }
    }

public:
    /**
     * @brief Construtor da controladora de quadros.
     * @param servicoQuadros Interface do serviço de quadros.
     */
    explicit ControladoraQuadros(IQuadros &servicoQuadros)
        : servico(servicoQuadros) {}

    /**
     * @brief Cadastra um novo quadro.
     * @param papel Papel do usuário autenticado.
     * @param quadro Quadro que será cadastrado.
     * @throw std::runtime_error Caso o usuário não seja gestor.
     */
    void cadastrar(
        const Papel &papel,
        const Quadro &quadro)
    {
        exigirGestor(papel);
        servico.cadastrar(quadro);
    }

    /**
     * @brief Consulta um quadro pelo identificador.
     * @param papel Papel do usuário autenticado.
     * @param identificador Identificador do quadro.
     * @return Quadro encontrado.
     * @throw std::runtime_error Caso o usuário não possua permissão.
     */
    Quadro consultar(
        const Papel &papel,
        const Identificador &identificador)
    {
        exigirUsuarioValido(papel);
        return servico.consultar(identificador);
    }

    /**
     * @brief Lista os quadros cadastrados.
     * @param papel Papel do usuário autenticado.
     * @return Vetor contendo os quadros cadastrados.
     * @throw std::runtime_error Caso o usuário não possua permissão.
     */
    std::vector<Quadro> listar(const Papel &papel)
    {
        exigirUsuarioValido(papel);
        return servico.listar();
    }

    /**
     * @brief Atualiza os dados de um quadro.
     * @param papel Papel do usuário autenticado.
     * @param quadro Quadro com os dados atualizados.
     * @throw std::runtime_error Caso o usuário não seja gestor.
     */
    void atualizar(
        const Papel &papel,
        const Quadro &quadro)
    {
        exigirGestor(papel);
        servico.atualizar(quadro);
    }

    /**
     * @brief Remove um quadro pelo identificador.
     * @param papel Papel do usuário autenticado.
     * @param identificador Identificador do quadro.
     * @throw std::runtime_error Caso o usuário não seja gestor.
     */
    void remover(
        const Papel &papel,
        const Identificador &identificador)
    {
        exigirGestor(papel);
        servico.remover(identificador);
    }
};

#endif
