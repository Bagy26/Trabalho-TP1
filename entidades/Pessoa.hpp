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
 * Uma pessoa pode exercer o papel de gestor ou desenvolvedor.
 * O email é utilizado como chave primária e não pode ser
 * alterado depois de definido.
 */
class Pessoa {
private:

    Email email;
    Nome nome;
    Senha senha;
    Papel papel;

    bool emailDefinido = false;

public:
    /**
     * @brief Define o email da pessoa.
     * @param novoEmail Email validado.
     * @throw std::invalid_argument Caso o email já tenha sido definido.
     */
    void setEmail(const Email& novoEmail) {

        if (emailDefinido) {
            throw std::invalid_argument(
                "O email nao pode ser alterado."
            );
        }

        email = novoEmail;
        emailDefinido = true;
    }

    /**
     * @brief Retorna o email da pessoa.
     * @return Referência constante para o email.
     */
    const Email& getEmail() const {
        return email;
    }

    /**
     * @brief Define o nome da pessoa.
     * @param novoNome Nome validado.
     */
    void setNome(const Nome& novoNome) {
        nome = novoNome;
    }

    /**
     * @brief Retorna o nome da pessoa.
     * @return Referência constante para o nome.
     */
    const Nome& getNome() const {
        return nome;
    }

    /**
     * @brief Define a senha da pessoa.
     * @param novaSenha Senha validada.
     */
    void setSenha(const Senha& novaSenha) {
        senha = novaSenha;
    }

    /**
     * @brief Retorna a senha da pessoa.
     * @return Referência constante para a senha.
     */
    const Senha& getSenha() const {
        return senha;
    }

    /**
     * @brief Define o papel da pessoa.
     * @param novoPapel Papel validado.
     */
    void setPapel(const Papel& novoPapel) {
        papel = novoPapel;
    }

    /**
     * @brief Retorna o papel da pessoa.
     * @return Referência constante para o papel.
     */
    const Papel& getPapel() const {
        return papel;
    }
};

#endif
