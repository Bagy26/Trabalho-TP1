#ifndef PESSOA_HPP
#define PESSOA_HPP

#include "../dominio/email.hpp"
#include "../dominio/nome.hpp"
#include "../dominio/senha.hpp"
#include "../dominio/papel.hpp"

#include <stdexcept>

/**
 * @class Pessoa
 * @brief Representa uma pessoa cadastrada no sistema Kanban.
 *
 * Cada pessoa possui um email único, nome, senha e papel
 * no sistema (GESTOR ou DESENVOLVEDOR).
 */
class Pessoa
{
private:
    /**
     * @brief Email único da pessoa.
     */
    Email email;

    /**
     * @brief Nome da pessoa.
     */
    Nome nome;

    /**
     * @brief Senha de acesso da pessoa.
     */
    Senha senha;

    /**
     * @brief Papel da pessoa no sistema.
     */
    Papel papel;

    /**
     * @brief Indica se o email já foi definido.
     */
    bool emailDefinido = false;

public:
    /**
     * @brief Define o email da pessoa.
     * @param novoEmail Email validado.
     * @throw std::invalid_argument Caso já tenha sido definido.
     */
    void setEmail(const Email &novoEmail)
    {

        if (emailDefinido)
        {
            throw std::invalid_argument(
                "O email nao pode ser alterado.");
        }

        email = novoEmail;
        emailDefinido = true;
    }

    /**
     * @brief Retorna o email da pessoa.
     * @return Referência constante para o email.
     */
    const Email &getEmail() const
    {
        return email;
    }

    /**
     * @brief Define o nome da pessoa.
     * @param novoNome Nome validado.
     */
    void setNome(const Nome &novoNome)
    {
        nome = novoNome;
    }

    /**
     * @brief Retorna o nome da pessoa.
     * @return Referência constante para o nome.
     */
    const Nome &getNome() const
    {
        return nome;
    }

    /**
     * @brief Define a senha da pessoa.
     * @param novaSenha Senha validada.
     */
    void setSenha(const Senha &novaSenha)
    {
        senha = novaSenha;
    }

    /**
     * @brief Retorna a senha da pessoa.
     * @return Referência constante para a senha.
     */
    const Senha &getSenha() const
    {
        return senha;
    }

    /**
     * @brief Define o papel da pessoa no sistema.
     * @param novoPapel Papel validado.
     */
    void setPapel(const Papel &novoPapel)
    {
        papel = novoPapel;
    }

    /**
     * @brief Retorna o papel da pessoa.
     * @return Referência constante para o papel.
     */
    const Papel &getPapel() const
    {
        return papel;
    }
};

#endif
