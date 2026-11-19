#ifndef CONTROLADORA_PESSOAS_HPP
#define CONTROLADORA_PESSOAS_HPP

#include "../interfaces/IPessoas.hpp"
#include "../dominio/papel.hpp"
#include "../entidades/Pessoa.hpp"

#include <stdexcept>
#include <vector>

/**
 * @class ControladoraPessoas
 * @brief Controla as operações relacionadas às pessoas.
 *
 * Encaminha as operações para o serviço de pessoas
 * e verifica se o usuário possui um papel válido.
 *
 * Gestores e desenvolvedores podem cadastrar, consultar,
 * atualizar, remover e listar pessoas.
 */
class ControladoraPessoas
{
private:
    /**
     * @brief Referência para a interface do serviço de pessoas.
     */
    IPessoas &servico;

    /**
     * @brief Verifica se o usuário possui um papel válido.
     * @param papel Papel do usuário autenticado.
     * @throw std::runtime_error Caso o papel não seja reconhecido.
     */
    void exigirUsuarioValido(const Papel &papel)
    {
        std::string papelUsuario = papel.getPapel();

        if (papelUsuario != "GESTOR" &&
            papelUsuario != "DESENVOLVEDOR")
        {
            throw std::runtime_error("Usuario sem permissao.");
        }
    }

public:
    /**
     * @brief Construtor da controladora de pessoas.
     * @param servicoPessoas Interface do serviço de pessoas.
     */
    explicit ControladoraPessoas(IPessoas &servicoPessoas)
        : servico(servicoPessoas) {}

    /**
     * @brief Cadastra uma pessoa.
     * @param papel Papel do usuário autenticado.
     * @param pessoa Pessoa que será cadastrada.
     */
    void cadastrar(const Papel &papel, const Pessoa &pessoa)
    {
        exigirUsuarioValido(papel);
        servico.cadastrar(pessoa);
    }

    /**
     * @brief Consulta uma pessoa pelo e-mail.
     * @param papel Papel do usuário autenticado.
     * @param email E-mail da pessoa.
     * @return Pessoa encontrada.
     */
    Pessoa consultar(const Papel &papel, const Email &email)
    {
        exigirUsuarioValido(papel);
        return servico.consultar(email);
    }

    /**
     * @brief Lista todas as pessoas cadastradas.
     * @param papel Papel do usuário autenticado.
     * @return Vetor contendo as pessoas cadastradas.
     */
    std::vector<Pessoa> listar(const Papel &papel)
    {
        exigirUsuarioValido(papel);
        return servico.listar();
    }

    /**
     * @brief Atualiza os dados de uma pessoa.
     * @param papel Papel do usuário autenticado.
     * @param pessoa Pessoa com os dados atualizados.
     */
    void atualizar(const Papel &papel, const Pessoa &pessoa)
    {
        exigirUsuarioValido(papel);
        servico.atualizar(pessoa);
    }

    /**
     * @brief Remove uma pessoa pelo e-mail.
     * @param papel Papel do usuário autenticado.
     * @param email E-mail da pessoa.
     */
    void remover(const Papel &papel, const Email &email)
    {
        exigirUsuarioValido(papel);
        servico.remover(email);
    }
};

#endif