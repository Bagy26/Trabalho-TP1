
#ifndef PESSOAS_HPP
#define PESSOAS_HPP

#include "../sqlite/sqlite3.h"
#include "../entidades/Pessoa.hpp"

#include <stdexcept>
#include <string>
#include <vector>

/**
 * @brief Gerencia o armazenamento de pessoas no banco de dados.
 */
class Pessoas {
private:

    /**
     * @brief Conexao compartilhada com o banco de dados SQLite.
     */
    sqlite3* banco;

public:

    /**
     * @brief Construtor do container de pessoas.
     * @param conexao Ponteiro para a conexao SQLite aberta.
     */
    explicit Pessoas(sqlite3* conexao) {
        if (conexao == nullptr) {
            throw std::invalid_argument("Conexao com o banco invalida.");
        }

        banco = conexao;
    }

    /**
     * @brief Cria a tabela de pessoas no banco de dados.
     */
    void criarTabela() {

        const char* sql =
            "CREATE TABLE IF NOT EXISTS Pessoas ("
            "email TEXT PRIMARY KEY NOT NULL,"
            "nome TEXT NOT NULL,"
            "senha TEXT NOT NULL,"
            "papel TEXT NOT NULL"
            ");";

        char* mensagemErro = nullptr;

        int resultado = sqlite3_exec(
            banco, sql, nullptr, nullptr, &mensagemErro
        );

        if (resultado != SQLITE_OK) {
            std::string mensagem = mensagemErro
                ? mensagemErro
                : "Erro ao criar tabela Pessoas.";

            sqlite3_free(mensagemErro);
            throw std::runtime_error(mensagem);
        }
    }

    /**
     * @brief Insere uma pessoa no banco de dados.
     */
    void inserir(const Pessoa& pessoa) {

        const char* sql =
            "INSERT INTO Pessoas (email, nome, senha, papel) "
            "VALUES (?, ?, ?, ?);";

        sqlite3_stmt* comando = nullptr;

        int resultado = sqlite3_prepare_v2(
            banco, sql, -1, &comando, nullptr
        );

        if (resultado != SQLITE_OK) {
            throw std::runtime_error(sqlite3_errmsg(banco));
        }

        resultado = sqlite3_bind_text(
            comando, 1, pessoa.getEmail().getEmail().c_str(),
            -1, SQLITE_TRANSIENT
        );

        if (resultado == SQLITE_OK) {
            resultado = sqlite3_bind_text(
                comando, 2, pessoa.getNome().getNome().c_str(),
                -1, SQLITE_TRANSIENT
            );
        }

        if (resultado == SQLITE_OK) {
            resultado = sqlite3_bind_text(
                comando, 3, pessoa.getSenha().getSenha().c_str(),
                -1, SQLITE_TRANSIENT
            );
        }

        if (resultado == SQLITE_OK) {
            resultado = sqlite3_bind_text(
                comando, 4, pessoa.getPapel().getPapel().c_str(),
                -1, SQLITE_TRANSIENT
            );
        }

        if (resultado != SQLITE_OK) {
            std::string mensagem = sqlite3_errmsg(banco);
            sqlite3_finalize(comando);
            throw std::runtime_error(mensagem);
        }

        resultado = sqlite3_step(comando);

        if (resultado != SQLITE_DONE) {
            std::string mensagem;

            if (resultado == SQLITE_CONSTRAINT) {
                mensagem = "Ja existe uma pessoa cadastrada com esse email.";
            } else {
                mensagem = sqlite3_errmsg(banco);
            }

            sqlite3_finalize(comando);
            throw std::runtime_error(mensagem);
        }

        sqlite3_finalize(comando);
    }

    /**
     * @brief Consulta uma pessoa pelo email.
     */
    Pessoa consultar(const Email& email) {

        const char* sql =
            "SELECT email, nome, senha, papel "
            "FROM Pessoas WHERE email = ?;";

        sqlite3_stmt* comando = nullptr;

        int resultado = sqlite3_prepare_v2(
            banco, sql, -1, &comando, nullptr
        );

        if (resultado != SQLITE_OK) {
            throw std::runtime_error(sqlite3_errmsg(banco));
        }

        resultado = sqlite3_bind_text(
            comando, 1, email.getEmail().c_str(),
            -1, SQLITE_TRANSIENT
        );

        if (resultado != SQLITE_OK) {
            std::string mensagem = sqlite3_errmsg(banco);
            sqlite3_finalize(comando);
            throw std::runtime_error(mensagem);
        }

        resultado = sqlite3_step(comando);

        if (resultado != SQLITE_ROW) {
            std::string mensagem =
                resultado == SQLITE_DONE
                ? "Pessoa nao encontrada."
                : sqlite3_errmsg(banco);

            sqlite3_finalize(comando);
            throw std::runtime_error(mensagem);
        }

        Pessoa pessoa;

        Email emailEncontrado;
        emailEncontrado.setEmail(
            reinterpret_cast<const char*>(
                sqlite3_column_text(comando, 0)
            )
        );

        Nome nome;
        nome.setNome(
            reinterpret_cast<const char*>(
                sqlite3_column_text(comando, 1)
            )
        );

        Senha senha;
        senha.setSenha(
            reinterpret_cast<const char*>(
                sqlite3_column_text(comando, 2)
            )
        );

        Papel papel;
        papel.setPapel(
            reinterpret_cast<const char*>(
                sqlite3_column_text(comando, 3)
            )
        );

        pessoa.setEmail(emailEncontrado);
        pessoa.setNome(nome);
        pessoa.setSenha(senha);
        pessoa.setPapel(papel);

        sqlite3_finalize(comando);

        return pessoa;
    }

    /**
     * @brief Lista todas as pessoas cadastradas.
     */
    std::vector<Pessoa> listar() {

        const char* sql =
            "SELECT email, nome, senha, papel FROM Pessoas;";

        sqlite3_stmt* comando = nullptr;

        int resultado = sqlite3_prepare_v2(
            banco, sql, -1, &comando, nullptr
        );

        if (resultado != SQLITE_OK) {
            throw std::runtime_error(sqlite3_errmsg(banco));
        }

        std::vector<Pessoa> pessoasEncontradas;

        while ((resultado = sqlite3_step(comando)) == SQLITE_ROW) {

            Pessoa pessoa;

            Email email;
            email.setEmail(reinterpret_cast<const char*>(
                sqlite3_column_text(comando, 0)));

            Nome nome;
            nome.setNome(reinterpret_cast<const char*>(
                sqlite3_column_text(comando, 1)));

            Senha senha;
            senha.setSenha(reinterpret_cast<const char*>(
                sqlite3_column_text(comando, 2)));

            Papel papel;
            papel.setPapel(reinterpret_cast<const char*>(
                sqlite3_column_text(comando, 3)));

            pessoa.setEmail(email);
            pessoa.setNome(nome);
            pessoa.setSenha(senha);
            pessoa.setPapel(papel);

            pessoasEncontradas.push_back(pessoa);
        }

        if (resultado != SQLITE_DONE) {
            std::string mensagem = sqlite3_errmsg(banco);
            sqlite3_finalize(comando);
            throw std::runtime_error(mensagem);
        }

        sqlite3_finalize(comando);

        return pessoasEncontradas;
    }

    /**
     * @brief Atualiza os dados de uma pessoa.
     * O email, que e a chave primaria, nao pode ser alterado.
     */
    void atualizar(const Pessoa& pessoa) {

        const char* sql =
            "UPDATE Pessoas SET nome = ?, senha = ?, papel = ? "
            "WHERE email = ?;";

        sqlite3_stmt* comando = nullptr;

        int resultado = sqlite3_prepare_v2(
            banco, sql, -1, &comando, nullptr
        );

        if (resultado != SQLITE_OK) {
            throw std::runtime_error(sqlite3_errmsg(banco));
        }

        resultado = sqlite3_bind_text(
            comando, 1, pessoa.getNome().getNome().c_str(),
            -1, SQLITE_TRANSIENT
        );

        if (resultado == SQLITE_OK) {
            resultado = sqlite3_bind_text(
                comando, 2, pessoa.getSenha().getSenha().c_str(),
                -1, SQLITE_TRANSIENT
            );
        }

        if (resultado == SQLITE_OK) {
            resultado = sqlite3_bind_text(
                comando, 3, pessoa.getPapel().getPapel().c_str(),
                -1, SQLITE_TRANSIENT
            );
        }

        if (resultado == SQLITE_OK) {
            resultado = sqlite3_bind_text(
                comando, 4, pessoa.getEmail().getEmail().c_str(),
                -1, SQLITE_TRANSIENT
            );
        }

        if (resultado != SQLITE_OK) {
            std::string mensagem = sqlite3_errmsg(banco);
            sqlite3_finalize(comando);
            throw std::runtime_error(mensagem);
        }

        resultado = sqlite3_step(comando);

        if (resultado != SQLITE_DONE) {
            std::string mensagem = sqlite3_errmsg(banco);
            sqlite3_finalize(comando);
            throw std::runtime_error(
                "Erro ao atualizar pessoa: " + mensagem
            );
        }

        int alteracoes = sqlite3_changes(banco);

        sqlite3_finalize(comando);

        if (alteracoes == 0) {
            throw std::runtime_error(
                "Pessoa nao encontrada ou nenhum dado foi alterado."
            );
        }
    }

    /**
     * @brief Remove uma pessoa pelo email.
     * A exclusao sera impedida se houver projetos ou cartoes
     * associados a essa pessoa.
     */
    void remover(const Email& email) {

        const char* sql =
            "DELETE FROM Pessoas WHERE email = ?;";

        sqlite3_stmt* comando = nullptr;

        int resultado = sqlite3_prepare_v2(
            banco, sql, -1, &comando, nullptr
        );

        if (resultado != SQLITE_OK) {
            throw std::runtime_error(sqlite3_errmsg(banco));
        }

        resultado = sqlite3_bind_text(
            comando, 1, email.getEmail().c_str(),
            -1, SQLITE_TRANSIENT
        );

        if (resultado != SQLITE_OK) {
            std::string mensagem = sqlite3_errmsg(banco);
            sqlite3_finalize(comando);
            throw std::runtime_error(mensagem);
        }

        resultado = sqlite3_step(comando);

        if (resultado != SQLITE_DONE) {
            std::string mensagem = sqlite3_errmsg(banco);
            sqlite3_finalize(comando);
            throw std::runtime_error(
                "Erro ao remover pessoa: " + mensagem
            );
        }

        int alteracoes = sqlite3_changes(banco);

        sqlite3_finalize(comando);

        if (alteracoes == 0) {
            throw std::runtime_error("Pessoa nao encontrada.");
        }
    }
};

#endif
