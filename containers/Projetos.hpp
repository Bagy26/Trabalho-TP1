
#ifndef PROJETOS_HPP
#define PROJETOS_HPP

#include "../sqlite/sqlite3.h"
#include "../entidades/Projeto.hpp"

#include <stdexcept>
#include <string>

/**
 * @brief Gerencia o armazenamento de projetos no banco SQLite.
 */
class Projetos {
private:

    /**
     * @brief Conexao compartilhada com o banco de dados.
     */
    sqlite3* banco;

public:

    /**
     * @brief Inicializa o container com uma conexao SQLite.
     * @param conexao Ponteiro para uma conexao aberta.
     */
    explicit Projetos(sqlite3* conexao) {
        if (conexao == nullptr) {
            throw std::invalid_argument("Conexao com o banco invalida.");
        }

        banco = conexao;
    }

    /**
     * @brief Cria a tabela de projetos no banco de dados.
     * @throw std::runtime_error Caso ocorra erro na criacao.
     */
    void criarTabela() {

        const char* sql =
            "CREATE TABLE IF NOT EXISTS Projetos ("
            "identificador TEXT PRIMARY KEY NOT NULL,"
            "nome TEXT NOT NULL,"
            "descricao TEXT NOT NULL,"
            "inicio TEXT NOT NULL,"
            "termino TEXT NOT NULL,"
            "gestor TEXT NOT NULL,"
            "FOREIGN KEY (gestor) REFERENCES Pessoas(email) "
            "ON DELETE RESTRICT"
            ");";

        char* mensagemErro = nullptr;

        int resultado = sqlite3_exec(
            banco,
            sql,
            nullptr,
            nullptr,
            &mensagemErro
        );

        if (resultado != SQLITE_OK) {
            std::string mensagem = mensagemErro
                ? mensagemErro
                : "Erro ao criar tabela Projetos.";

            sqlite3_free(mensagemErro);

            throw std::runtime_error(mensagem);
        }
    }

    /**
     * @brief Insere um projeto no banco de dados.
     * @param projeto Entidade Projeto que sera cadastrada.
     * @throw std::runtime_error Caso ocorra erro durante a insercao.
     */
    void inserir(const Projeto& projeto) {

        const char* sql =
            "INSERT INTO Projetos "
            "(identificador, nome, descricao, inicio, termino, gestor) "
            "VALUES (?, ?, ?, ?, ?, ?);";

        sqlite3_stmt* comando = nullptr;

        int resultado = sqlite3_prepare_v2(
            banco, sql, -1, &comando, nullptr
        );

        if (resultado != SQLITE_OK) {
            throw std::runtime_error(sqlite3_errmsg(banco));
        }

        resultado = sqlite3_bind_text(
            comando, 1,
            projeto.getIdentificador().getIdentificador().c_str(),
            -1, SQLITE_TRANSIENT
        );

        if (resultado == SQLITE_OK) {
            resultado = sqlite3_bind_text(
                comando, 2,
                projeto.getNome().getNome().c_str(),
                -1, SQLITE_TRANSIENT
            );
        }

        if (resultado == SQLITE_OK) {
            resultado = sqlite3_bind_text(
                comando, 3,
                projeto.getDescricao().getTexto().c_str(),
                -1, SQLITE_TRANSIENT
            );
        }

        if (resultado == SQLITE_OK) {
            resultado = sqlite3_bind_text(
                comando, 4,
                projeto.getInicio().getTimestamp().c_str(),
                -1, SQLITE_TRANSIENT
            );
        }

        if (resultado == SQLITE_OK) {
            resultado = sqlite3_bind_text(
                comando, 5,
                projeto.getTermino().getTimestamp().c_str(),
                -1, SQLITE_TRANSIENT
            );
        }

        if (resultado == SQLITE_OK) {
            resultado = sqlite3_bind_text(
                comando, 6,
                projeto.getGestor().getEmail().c_str(),
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
                mensagem = "Projeto duplicado ou gestor inexistente.";
            } else {
                mensagem = sqlite3_errmsg(banco);
            }

            sqlite3_finalize(comando);
            throw std::runtime_error(mensagem);
        }

        sqlite3_finalize(comando);
    }
};

#endif
