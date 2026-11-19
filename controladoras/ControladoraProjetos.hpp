#ifndef CONTROLADORA_PROJETOS_HPP
#define CONTROLADORA_PROJETOS_HPP

#include "../interfaces/IProjetos.hpp"
#include "../dominio/papel.hpp"

#include <stdexcept>
#include <vector>

/**
 * @class ControladoraProjetos
 * @brief Controla as operações relacionadas aos projetos.
 *
 * Responsável por verificar as permissões dos usuários
 * e encaminhar as operações para o serviço de projetos.
 *
 * O gestor pode cadastrar, consultar, atualizar, remover
 * e listar projetos.
 *
 * O desenvolvedor pode consultar e listar projetos.
 */
class ControladoraProjetos
{
private:
    /**
     * @brief Referência para a interface do serviço de projetos.
     */
    IProjetos &servico;

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
     * @brief Construtor da controladora de projetos.
     * @param servicoProjetos Interface do serviço de projetos.
     */
    explicit ControladoraProjetos(IProjetos &servicoProjetos)
        : servico(servicoProjetos) {}

    /**
     * @brief Cadastra um novo projeto.
     * @param papel Papel do usuário autenticado.
     * @param projeto Projeto que será cadastrado.
     * @throw std::runtime_error Caso o usuário não seja gestor.
     */
    void cadastrar(
        const Papel &papel,
        const Projeto &projeto)
    {
        exigirGestor(papel);
        servico.cadastrar(projeto);
    }

    /**
     * @brief Consulta um projeto pelo identificador.
     * @param papel Papel do usuário autenticado.
     * @param identificador Identificador do projeto.
     * @return Projeto encontrado.
     * @throw std::runtime_error Caso o usuário não possua permissão.
     */
    Projeto consultar(
        const Papel &papel,
        const Identificador &identificador)
    {
        exigirUsuarioValido(papel);
        return servico.consultar(identificador);
    }

    /**
     * @brief Lista os projetos cadastrados.
     * @param papel Papel do usuário autenticado.
     * @return Vetor contendo os projetos cadastrados.
     * @throw std::runtime_error Caso o usuário não possua permissão.
     */
    std::vector<Projeto> listar(const Papel &papel)
    {
        exigirUsuarioValido(papel);
        return servico.listar();
    }

    /**
     * @brief Atualiza os dados de um projeto.
     * @param papel Papel do usuário autenticado.
     * @param projeto Projeto com os dados atualizados.
     * @throw std::runtime_error Caso o usuário não seja gestor.
     */
    void atualizar(
        const Papel &papel,
        const Projeto &projeto)
    {
        exigirGestor(papel);
        servico.atualizar(projeto);
    }

    /**
     * @brief Remove um projeto pelo identificador.
     * @param papel Papel do usuário autenticado.
     * @param identificador Identificador do projeto.
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
