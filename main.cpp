#include <iostream>
#include "sqlite/sqlite3.h"
#include "containers/Pessoas.hpp"
#include <vector>
#include "containers/Projetos.hpp"

int main() {

    sqlite3* banco = nullptr;

    int resultado = sqlite3_open("kanban.db", &banco);

    if (resultado != SQLITE_OK) {
        std::cout << "Erro ao abrir banco: "
                  << sqlite3_errmsg(banco) << std::endl;
        sqlite3_close(banco);
        return 1;
    }
    if (sqlite3_exec(banco, "PRAGMA foreign_keys = ON;",
                 nullptr, nullptr, nullptr) != SQLITE_OK) {
    std::cerr << "Erro ao ativar chaves estrangeiras: "
              << sqlite3_errmsg(banco) << std::endl;
    sqlite3_close(banco);
    return 1;
}
    try {
        Pessoas pessoas(banco);

        pessoas.criarTabela();

        // Criacao dos atributos de dominio
        Email email;
        email.setEmail("teste@gmail.com");

        Nome nome;
        nome.setNome("Maria Silva");

        Senha senha;
        senha.setSenha("Abc12");

        Papel papel;
        papel.setPapel("GESTOR");

        // Criacao da entidade Pessoa
        Pessoa pessoa;
        pessoa.setEmail(email);
        pessoa.setNome(nome);
        pessoa.setSenha(senha);
        pessoa.setPapel(papel);
        Projetos projetos(banco);

        projetos.criarTabela();
        std::cout << "Tabela Projetos criada com sucesso!" << std::endl;

    // Listagem de todas as pessoas cadastradas
    std::vector<Pessoa> pessoasCadastradas = pessoas.listar();

    std::cout << "\n--- Pessoas cadastradas ---\n";

    for (const Pessoa& pessoa : pessoasCadastradas) {
        std::cout << "Email: "
                << pessoa.getEmail().getEmail() << std::endl;

        std::cout << "Nome: "
                << pessoa.getNome().getNome() << std::endl;

        std::cout << "Papel: "
                << pessoa.getPapel().getPapel() << std::endl;

        std::cout << "--------------------------\n";
    }

    // Cadastro no banco
    // Consulta da pessoa cadastrada anteriormente
    Email emailConsulta;
    emailConsulta.setEmail("teste@gmail.com");

    Pessoa pessoaEncontrada = pessoas.consultar(emailConsulta);

    std::cout << "Pessoa encontrada!" << std::endl;

    std::cout << "Email: "
            << pessoaEncontrada.getEmail().getEmail()
            << std::endl;

    std::cout << "Nome: "
            << pessoaEncontrada.getNome().getNome()
            << std::endl;

    std::cout << "Papel: "
            << pessoaEncontrada.getPapel().getPapel()
            << std::endl;

        } catch (const std::exception& erro) {
            std::cout << "Erro: " << erro.what() << std::endl;
        }

        sqlite3_close(banco);

        return 0;
    }