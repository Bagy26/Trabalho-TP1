#ifndef CARTOES_ATIVIDADES_HPP
#define CARTOES_ATIVIDADES_HPP

#include "../sqlite/sqlite3.h"
#include "../entidades/CartaoAtividade.hpp"

#include <stdexcept>
#include <string>
#include <vector>

/**
 * @class CartoesAtividades
 * @brief Gerencia os cartoes de atividades no banco SQLite.
 */
class CartoesAtividades {

private:

    sqlite3* banco;

    /**
     * @brief Reconstrói uma entidade a partir de uma linha do SQLite.
     */
    CartaoAtividade construirEntidade(sqlite3_stmt* comando) {

        CartaoAtividade cartao;

        Identificador identificador;
        identificador.setIdentificador(
            reinterpret_cast<const char*>(sqlite3_column_text(comando, 0))
        );

        Nome nome;
        nome.setNome(
            reinterpret_cast<const char*>(sqlite3_column_text(comando, 1))
        );

        Texto descricao;
        descricao.setTexto(
            reinterpret_cast<const char*>(sqlite3_column_text(comando, 2))
        );

        Prioridade prioridade;
        prioridade.setPrioridade(
            reinterpret_cast<const char*>(sqlite3_column_text(comando, 3))
        );

        Estado estado;
        estado.setEstado(
            reinterpret_cast<const char*>(sqlite3_column_text(comando, 4))
        );

        Tamanho tamanho;
        tamanho.setTamanho(
            reinterpret_cast<const char*>(sqlite3_column_text(comando, 5))
        );

        Timestamp entrada;
        entrada.setTimestamp(
            reinterpret_cast<const char*>(sqlite3_column_text(comando, 6))
        );

        Timestamp inicio;
        inicio.setTimestamp(
            reinterpret_cast<const char*>(sqlite3_column_text(comando, 7))
        );

        Timestamp termino;
        termino.setTimestamp(
            reinterpret_cast<const char*>(sqlite3_column_text(comando, 8))
        );

        Email desenvolvedor;
        desenvolvedor.setEmail(
            reinterpret_cast<const char*>(sqlite3_column_text(comando, 9))
        );

        Identificador quadro;
        quadro.setIdentificador(
            reinterpret_cast<const char*>(sqlite3_column_text(comando, 10))
        );

        cartao.setIdentificador(identificador);
        cartao.setNome(nome);
        cartao.setDescricao(descricao);
        cartao.setPrioridade(prioridade);
        cartao.setEstado(estado);
        cartao.setTamanho(tamanho);
        cartao.setEntrada(entrada);
        cartao.setInicio(inicio);
        cartao.setTermino(termino);
        cartao.setDesenvolvedor(desenvolvedor);
        cartao.setQuadro(quadro);

        return cartao;
    }

    /**
     * @brief SQL utilizado para recuperar os dados dos cartões.
     */
    static const char* consultaCompleta() {
        return
            "SELECT identificador, nome, descricao, prioridade, "
            "estado, tamanho, entrada, inicio, termino, "
            "desenvolvedor, quadro FROM CartoesAtividades";
    }

public:

    /**
     * @brief Inicializa o container.
     */
    explicit CartoesAtividades(sqlite3* conexao) {

        if (conexao == nullptr) {
            throw std::invalid_argument(
                "Conexao com o banco invalida."
            );
        }

        banco = conexao;
    }

    /**
     * @brief Cria a tabela de cartões.
     */
    void criarTabela() {

        const char* sql =
            "CREATE TABLE IF NOT EXISTS CartoesAtividades ("
            "identificador TEXT PRIMARY KEY NOT NULL,"
            "nome TEXT NOT NULL,"
            "descricao TEXT NOT NULL,"
            "prioridade TEXT NOT NULL,"
            "estado TEXT NOT NULL,"
            "tamanho TEXT NOT NULL,"
            "entrada TEXT NOT NULL,"
            "inicio TEXT NOT NULL,"
            "termino TEXT NOT NULL,"
            "desenvolvedor TEXT NOT NULL,"
            "quadro TEXT NOT NULL,"
            "FOREIGN KEY (desenvolvedor) REFERENCES Pessoas(email) "
            "ON DELETE RESTRICT,"
            "FOREIGN KEY (quadro) REFERENCES Quadros(identificador) "
            "ON DELETE RESTRICT"
            ");";

        char* mensagemErro = nullptr;

        int resultado = sqlite3_exec(
            banco, sql, nullptr, nullptr, &mensagemErro
        );

        if (resultado != SQLITE_OK) {

            std::string mensagem = mensagemErro
                ? mensagemErro
                : "Erro ao criar tabela CartoesAtividades.";

            sqlite3_free(mensagemErro);

            throw std::runtime_error(mensagem);
        }
    }

    /**
     * @brief Insere um cartão no banco.
     */
    void inserir(const CartaoAtividade& cartao) {

        const char* sql =
            "INSERT INTO CartoesAtividades "
            "(identificador, nome, descricao, prioridade, estado, "
            "tamanho, entrada, inicio, termino, desenvolvedor, quadro) "
            "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);";

        sqlite3_stmt* comando = nullptr;

        int resultado = sqlite3_prepare_v2(
            banco, sql, -1, &comando, nullptr
        );

        if (resultado != SQLITE_OK) {
            throw std::runtime_error(sqlite3_errmsg(banco));
        }

        resultado = sqlite3_bind_text(comando, 1,
            cartao.getIdentificador().getIdentificador().c_str(),
            -1, SQLITE_TRANSIENT);

        if (resultado == SQLITE_OK)
            resultado = sqlite3_bind_text(comando, 2,
                cartao.getNome().getNome().c_str(), -1, SQLITE_TRANSIENT);

        if (resultado == SQLITE_OK)
            resultado = sqlite3_bind_text(comando, 3,
                cartao.getDescricao().getTexto().c_str(), -1, SQLITE_TRANSIENT);

        if (resultado == SQLITE_OK)
            resultado = sqlite3_bind_text(comando, 4,
                cartao.getPrioridade().getPrioridade().c_str(), -1, SQLITE_TRANSIENT);

        if (resultado == SQLITE_OK)
            resultado = sqlite3_bind_text(comando, 5,
                cartao.getEstado().getEstado().c_str(), -1, SQLITE_TRANSIENT);

        if (resultado == SQLITE_OK)
            resultado = sqlite3_bind_text(comando, 6,
                cartao.getTamanho().getTamanho().c_str(), -1, SQLITE_TRANSIENT);

        if (resultado == SQLITE_OK)
            resultado = sqlite3_bind_text(comando, 7,
                cartao.getEntrada().getTimestamp().c_str(), -1, SQLITE_TRANSIENT);

        if (resultado == SQLITE_OK)
            resultado = sqlite3_bind_text(comando, 8,
                cartao.getInicio().getTimestamp().c_str(), -1, SQLITE_TRANSIENT);

        if (resultado == SQLITE_OK)
            resultado = sqlite3_bind_text(comando, 9,
                cartao.getTermino().getTimestamp().c_str(), -1, SQLITE_TRANSIENT);

        if (resultado == SQLITE_OK)
            resultado = sqlite3_bind_text(comando, 10,
                cartao.getDesenvolvedor().getEmail().c_str(), -1, SQLITE_TRANSIENT);

        if (resultado == SQLITE_OK)
            resultado = sqlite3_bind_text(comando, 11,
                cartao.getQuadro().getIdentificador().c_str(), -1, SQLITE_TRANSIENT);

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
                "Erro ao inserir cartao: " + mensagem
            );
        }

        sqlite3_finalize(comando);
    }

    /**
     * @brief Consulta um cartão pelo identificador.
     */
    CartaoAtividade consultar(const Identificador& identificador) {

        std::string sql = std::string(consultaCompleta()) +
                          " WHERE identificador = ?;";

        sqlite3_stmt* comando = nullptr;

        int resultado = sqlite3_prepare_v2(
            banco, sql.c_str(), -1, &comando, nullptr
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

            std::string mensagem = resultado == SQLITE_DONE
                ? "Cartao nao encontrado."
                : sqlite3_errmsg(banco);

            sqlite3_finalize(comando);
            throw std::runtime_error(mensagem);
        }

        CartaoAtividade cartao = construirEntidade(comando);

        sqlite3_finalize(comando);

        return cartao;
    }

    /**
     * @brief Lista todos os cartões cadastrados.
     */
    std::vector<CartaoAtividade> listar() {

        std::string sql = std::string(consultaCompleta()) + ";";

        sqlite3_stmt* comando = nullptr;

        int resultado = sqlite3_prepare_v2(
            banco, sql.c_str(), -1, &comando, nullptr
        );

        if (resultado != SQLITE_OK) {
            throw std::runtime_error(sqlite3_errmsg(banco));
        }

        std::vector<CartaoAtividade> cartoes;

        while ((resultado = sqlite3_step(comando)) == SQLITE_ROW) {
            cartoes.push_back(construirEntidade(comando));
        }

        if (resultado != SQLITE_DONE) {

            std::string mensagem = sqlite3_errmsg(banco);
            sqlite3_finalize(comando);

            throw std::runtime_error(mensagem);
        }

        sqlite3_finalize(comando);

        return cartoes;
    }

    /**
     * @brief Atualiza os dados de um cartão sem alterar sua chave primária.
     */
    void atualizar(const CartaoAtividade& cartao) {

        const char* sql =
            "UPDATE CartoesAtividades SET "
            "nome = ?, descricao = ?, prioridade = ?, estado = ?, "
            "tamanho = ?, entrada = ?, inicio = ?, termino = ?, "
            "desenvolvedor = ?, quadro = ? "
            "WHERE identificador = ?;";

        sqlite3_stmt* comando = nullptr;

        int resultado = sqlite3_prepare_v2(
            banco, sql, -1, &comando, nullptr
        );

        if (resultado != SQLITE_OK) {
            throw std::runtime_error(sqlite3_errmsg(banco));
        }

        resultado = sqlite3_bind_text(comando, 1,
            cartao.getNome().getNome().c_str(), -1, SQLITE_TRANSIENT);

        if (resultado == SQLITE_OK)
            resultado = sqlite3_bind_text(comando, 2,
                cartao.getDescricao().getTexto().c_str(), -1, SQLITE_TRANSIENT);

        if (resultado == SQLITE_OK)
            resultado = sqlite3_bind_text(comando, 3,
                cartao.getPrioridade().getPrioridade().c_str(), -1, SQLITE_TRANSIENT);

        if (resultado == SQLITE_OK)
            resultado = sqlite3_bind_text(comando, 4,
                cartao.getEstado().getEstado().c_str(), -1, SQLITE_TRANSIENT);

        if (resultado == SQLITE_OK)
            resultado = sqlite3_bind_text(comando, 5,
                cartao.getTamanho().getTamanho().c_str(), -1, SQLITE_TRANSIENT);

        if (resultado == SQLITE_OK)
            resultado = sqlite3_bind_text(comando, 6,
                cartao.getEntrada().getTimestamp().c_str(), -1, SQLITE_TRANSIENT);

        if (resultado == SQLITE_OK)
            resultado = sqlite3_bind_text(comando, 7,
                cartao.getInicio().getTimestamp().c_str(), -1, SQLITE_TRANSIENT);

        if (resultado == SQLITE_OK)
            resultado = sqlite3_bind_text(comando, 8,
                cartao.getTermino().getTimestamp().c_str(), -1, SQLITE_TRANSIENT);

        if (resultado == SQLITE_OK)
            resultado = sqlite3_bind_text(comando, 9,
                cartao.getDesenvolvedor().getEmail().c_str(), -1, SQLITE_TRANSIENT);

        if (resultado == SQLITE_OK)
            resultado = sqlite3_bind_text(comando, 10,
                cartao.getQuadro().getIdentificador().c_str(), -1, SQLITE_TRANSIENT);

        if (resultado == SQLITE_OK)
            resultado = sqlite3_bind_text(comando, 11,
                cartao.getIdentificador().getIdentificador().c_str(),
                -1, SQLITE_TRANSIENT);

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
                "Erro ao atualizar cartao: " + mensagem
            );
        }

        int alteracoes = sqlite3_changes(banco);

        sqlite3_finalize(comando);

        if (alteracoes == 0) {
            throw std::runtime_error(
                "Cartao nao encontrado para atualizacao."
            );
        }
    }

    /**
     * @brief Remove um cartão pelo identificador.
     */
    void remover(const Identificador& identificador) {

        const char* sql =
            "DELETE FROM CartoesAtividades WHERE identificador = ?;";

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
                "Erro ao remover cartao: " + mensagem
            );
        }

        int alteracoes = sqlite3_changes(banco);

        sqlite3_finalize(comando);

        if (alteracoes == 0) {
            throw std::runtime_error("Cartao nao encontrado.");
        }
    }

    /**
     * @brief Move um cartão para o próximo estado permitido.
     * @param identificador Identificador do cartão.
     * @param novoEstado Novo estado do cartão.
     * @param momento Timestamp da movimentação.
     */
    void mover(
        const Identificador& identificador,
        const Estado& novoEstado,
        const Timestamp& momento
    ) {

        CartaoAtividade cartao = consultar(identificador);

        std::string estadoAtual = cartao.getEstado().getEstado();
        std::string estadoDestino = novoEstado.getEstado();

        if (estadoAtual == "A FAZER" && estadoDestino == "FAZENDO") {

            cartao.setEstado(novoEstado);
            cartao.setInicio(momento);

        } else if (
            estadoAtual == "FAZENDO" && estadoDestino == "FEITO"
        ) {

            cartao.setEstado(novoEstado);
            cartao.setTermino(momento);

        } else {

            throw std::invalid_argument(
                "Movimentacao nao permitida."
            );
        }

        atualizar(cartao);
    }
};

#endif
