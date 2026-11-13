
#ifndef QUADROS_HPP
#define QUADROS_HPP

#include "../sqlite/sqlite3.h"
#include "../entidades/Quadro.hpp"

#include <stdexcept>
#include <string>
#include <vector>

/**
 * @class Quadros
 * @brief Gerencia o armazenamento de quadros Kanban.
 */
class Quadros {

private:

    sqlite3* banco;

public:

    /**
     * @brief Inicializa o container.
     */
    explicit Quadros(sqlite3* conexao) {

        if (conexao == nullptr) {
            throw std::invalid_argument(
                "Conexao com o banco invalida."
            );
        }

        banco = conexao;
    }

    /**
     * @brief Cria a tabela de quadros.
     */
    void criarTabela() {

        const char* sql =
            "CREATE TABLE IF NOT EXISTS Quadros ("
            "identificador TEXT PRIMARY KEY NOT NULL,"
            "nome TEXT NOT NULL,"
            "limite INTEGER NOT NULL,"
            "projeto TEXT NOT NULL,"
            "FOREIGN KEY (projeto) REFERENCES Projetos(identificador) "
            "ON DELETE RESTRICT"
            ");";

        char* mensagemErro = nullptr;

        int resultado = sqlite3_exec(
            banco, sql, nullptr, nullptr, &mensagemErro
        );

        if (resultado != SQLITE_OK) {

            std::string mensagem = mensagemErro
                ? mensagemErro
                : "Erro ao criar tabela Quadros.";

            sqlite3_free(mensagemErro);

            throw std::runtime_error(mensagem);
        }
    }

    /**
     * @brief Insere um quadro no banco.
     */
    void inserir(const Quadro& quadro) {

        const char* sql =
            "INSERT INTO Quadros "
            "(identificador, nome, limite, projeto) "
            "VALUES (?, ?, ?, ?);";

        sqlite3_stmt* comando = nullptr;

        int resultado = sqlite3_prepare_v2(
            banco, sql, -1, &comando, nullptr
        );

        if (resultado != SQLITE_OK) {
            throw std::runtime_error(sqlite3_errmsg(banco));
        }

        resultado = sqlite3_bind_text(
            comando, 1,
            quadro.getIdentificador().getIdentificador().c_str(),
            -1, SQLITE_TRANSIENT
        );

        if (resultado == SQLITE_OK) {
            resultado = sqlite3_bind_text(
                comando, 2,
                quadro.getNome().getNome().c_str(),
                -1, SQLITE_TRANSIENT
            );
        }

        if (resultado == SQLITE_OK) {
            resultado = sqlite3_bind_int(
                comando, 3,
                quadro.getLimite().getValor()
            );
        }

        if (resultado == SQLITE_OK) {
            resultado = sqlite3_bind_text(
                comando, 4,
                quadro.getProjeto().getIdentificador().c_str(),
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
                "Erro ao inserir quadro: " + mensagem
            );
        }

        sqlite3_finalize(comando);
    }

    /**
     * @brief Consulta um quadro pelo identificador.
     */
    Quadro consultar(const Identificador& identificador) {

        const char* sql =
            "SELECT identificador, nome, limite, projeto "
            "FROM Quadros WHERE identificador = ?;";

        sqlite3_stmt* comando = nullptr;

        int resultado = sqlite3_prepare_v2(
            banco, sql, -1, &comando, nullptr
        );

        if (resultado != SQLITE_OK) {
            throw std::runtime_error(sqlite3_errmsg(banco));
        }

        resultado = sqlite3_bind_text(
            comando, 1,
            identificador.getIdentificador().c_str(),
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
                ? "Quadro nao encontrado."
                : sqlite3_errmsg(banco);

            sqlite3_finalize(comando);

            throw std::runtime_error(mensagem);
        }

        Quadro quadro;

        Identificador id;
        id.setIdentificador(
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

        Limite limite;
        limite.setValor(sqlite3_column_int(comando, 2));

        Identificador projeto;
        projeto.setIdentificador(
            reinterpret_cast<const char*>(
                sqlite3_column_text(comando, 3)
            )
        );

        quadro.setIdentificador(id);
        quadro.setNome(nome);
        quadro.setLimite(limite);
        quadro.setProjeto(projeto);

        sqlite3_finalize(comando);

        return quadro;
    }

    /**
     * @brief Lista todos os quadros cadastrados.
     */
    std::vector<Quadro> listar() {

        const char* sql =
            "SELECT identificador, nome, limite, projeto "
            "FROM Quadros;";

        sqlite3_stmt* comando = nullptr;

        int resultado = sqlite3_prepare_v2(
            banco, sql, -1, &comando, nullptr
        );

        if (resultado != SQLITE_OK) {
            throw std::runtime_error(sqlite3_errmsg(banco));
        }

        std::vector<Quadro> quadros;

        while ((resultado = sqlite3_step(comando)) == SQLITE_ROW) {

            Quadro quadro;

            Identificador id;
            id.setIdentificador(
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

            Limite limite;
            limite.setValor(sqlite3_column_int(comando, 2));

            Identificador projeto;
            projeto.setIdentificador(
                reinterpret_cast<const char*>(
                    sqlite3_column_text(comando, 3)
                )
            );

            quadro.setIdentificador(id);
            quadro.setNome(nome);
            quadro.setLimite(limite);
            quadro.setProjeto(projeto);

            quadros.push_back(quadro);
        }

        if (resultado != SQLITE_DONE) {

            std::string mensagem = sqlite3_errmsg(banco);

            sqlite3_finalize(comando);

            throw std::runtime_error(mensagem);
        }

        sqlite3_finalize(comando);

        return quadros;
    }

    /**
     * @brief Atualiza os dados de um quadro.
     * O identificador nao pode ser alterado.
     */
    void atualizar(const Quadro& quadro) {

        const char* sql =
            "UPDATE Quadros SET nome = ?, limite = ? "
            "WHERE identificador = ?;";

        sqlite3_stmt* comando = nullptr;

        int resultado = sqlite3_prepare_v2(
            banco, sql, -1, &comando, nullptr
        );

        if (resultado != SQLITE_OK) {
            throw std::runtime_error(sqlite3_errmsg(banco));
        }

        resultado = sqlite3_bind_text(
            comando, 1,
            quadro.getNome().getNome().c_str(),
            -1, SQLITE_TRANSIENT
        );

        if (resultado == SQLITE_OK) {
            resultado = sqlite3_bind_int(
                comando, 2,
                quadro.getLimite().getValor()
            );
        }

        if (resultado == SQLITE_OK) {
            resultado = sqlite3_bind_text(
                comando, 3,
                quadro.getIdentificador().getIdentificador().c_str(),
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

            throw std::runtime_error(mensagem);
        }

        int alteracoes = sqlite3_changes(banco);

        sqlite3_finalize(comando);

        if (alteracoes == 0) {
            throw std::runtime_error(
                "Quadro nao encontrado para atualizacao."
            );
        }
    }

    /**
     * @brief Remove um quadro pelo identificador.
     */
    void remover(const Identificador& identificador) {

        const char* sql =
            "DELETE FROM Quadros WHERE identificador = ?;";

        sqlite3_stmt* comando = nullptr;

        int resultado = sqlite3_prepare_v2(
            banco, sql, -1, &comando, nullptr
        );

        if (resultado != SQLITE_OK) {
            throw std::runtime_error(sqlite3_errmsg(banco));
        }

        resultado = sqlite3_bind_text(
            comando, 1,
            identificador.getIdentificador().c_str(),
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
                "Nao foi possivel remover o quadro: " + mensagem
            );
        }

        int alteracoes = sqlite3_changes(banco);

        sqlite3_finalize(comando);

        if (alteracoes == 0) {
            throw std::runtime_error("Quadro nao encontrado.");
        }
    }
};

#endif
