#include <iostream>
#include <vector>
#include <exception>

#include "sqlite/sqlite3.h"

#include "dominio/identificador.hpp"
#include "dominio/nome.hpp"
#include "dominio/texto.hpp"
#include "dominio/timestamp.hpp"
#include "dominio/email.hpp"

#include "entidades/Projeto.hpp"

#include "containers/Pessoas.hpp"
#include "containers/Projetos.hpp"
#include "containers/Quadros.hpp"
#include "containers/CartoesAtividades.hpp"

int main() {

    // Abertura do banco de dados
    sqlite3* banco = nullptr;

    int resultado = sqlite3_open("kanban.db", &banco);

    if (resultado != SQLITE_OK) {
        std::cerr << "Erro ao abrir banco: "
                  << sqlite3_errmsg(banco) << std::endl;

        sqlite3_close(banco);
        return 1;
    }

    // Ativação das chaves estrangeiras
    if (sqlite3_exec(banco, "PRAGMA foreign_keys = ON;",
                     nullptr, nullptr, nullptr) != SQLITE_OK) {

        std::cerr << "Erro ao ativar chaves estrangeiras: "
                  << sqlite3_errmsg(banco) << std::endl;

        sqlite3_close(banco);
        return 1;
    }

    try {

        // =========================================
        // 1. CRIAÇÃO DOS CONTAINERS E DAS TABELAS
        // =========================================

        Pessoas pessoas(banco);
        pessoas.criarTabela();

        Projetos projetos(banco);
        projetos.criarTabela();

        std::cout << "Tabelas criadas com sucesso!" << std::endl;


        // =========================================
        // 2. CRIAÇÃO DA ENTIDADE PESSOA (GESTOR)
        // =========================================

        Email email;
        email.setEmail("teste@gmail.com");

        Nome nome;
        nome.setNome("Maria Silva");

        Senha senha;
        senha.setSenha("Abc12");

        Papel papel;
        papel.setPapel("GESTOR");

        Pessoa pessoa;
        pessoa.setEmail(email);
        pessoa.setNome(nome);
        pessoa.setSenha(senha);
        pessoa.setPapel(papel);

        Quadros quadros(banco);
        quadros.criarTabela();

        CartoesAtividades cartoes(banco);
        cartoes.criarTabela();


        // =========================================
        // 3. CADASTRO DA PESSOA
        // =========================================
        // A pessoa já está cadastrada no banco.
        // Vamos utilizar o cadastro existente como gestor.
        std::cout << "Utilizando pessoa gestora já cadastrada!"
          << std::endl;

        // =========================================
        // 4. CRIAÇÃO DA ENTIDADE PROJETO
        // =========================================

        Identificador identificadorProjeto;
        identificadorProjeto.setIdentificador("PRJ001");

        Nome nomeProjeto;
        nomeProjeto.setNome("Projeto Kanban");

        Texto descricaoProjeto;
        descricaoProjeto.setTexto("Desenvolver sistema Kanban.");

        Timestamp inicioProjeto;
        inicioProjeto.setTimestamp("01-JAN-2026-08:00");

        Timestamp terminoProjeto;
        terminoProjeto.setTimestamp("30-DEZ-2026-18:00");

        Email emailGestor;
        emailGestor.setEmail("teste@gmail.com");

        Projeto projeto;
        projeto.setIdentificador(identificadorProjeto);
        projeto.setNome(nomeProjeto);
        projeto.setDescricao(descricaoProjeto);
        projeto.setInicio(inicioProjeto);
        projeto.setTermino(terminoProjeto);
        projeto.setGestor(emailGestor);


        // =========================================
        // 5. CADASTRO DO PROJETO
        // =========================================

       // 5. CADASTRO DO PROJETO

        // O projeto PRJ001 já está cadastrado no banco.
        // Vamos testar sua recuperação pelo método listar().

        std::cout << "Consultando projetos cadastrados..."
          << std::endl;
        // Listagem de todos os projetos
        std::vector<Projeto> projetosCadastrados = projetos.listar();

        std::cout << "\n--- Projetos cadastrados ---\n";
            for (const Projeto& projetoCadastrado : projetosCadastrados) {
         std::cout << "Identificador: "
            << projetoCadastrado.getIdentificador().getIdentificador()
            << std::endl;
            std::cout << "Nome: "
            << projetoCadastrado.getNome().getNome()
            << std::endl;
         std::cout << "Descricao: "
            << projetoCadastrado.getDescricao().getTexto()
            << std::endl;
         std::cout << "Gestor: "
          << projetoCadastrado.getGestor().getEmail()
          << std::endl;
          std::cout << "--------------------------\n";
}


        // =========================================
        // 6. LISTAGEM DAS PESSOAS
        // =========================================

        std::vector<Pessoa> pessoasCadastradas = pessoas.listar();

        std::cout << "\n--- Pessoas cadastradas ---\n";
        for (const Pessoa& pessoaCadastrada : pessoasCadastradas) {
        std::cout << "Email: "
            << pessoaCadastrada.getEmail().getEmail()
            << std::endl;
        std::cout << "Nome: "
            << pessoaCadastrada.getNome().getNome()
            << std::endl;
        std::cout << "Papel: "
            << pessoaCadastrada.getPapel().getPapel()
            << std::endl;
        std::cout << "--------------------------\n";
        }


        // =========================================
        // 7. CONSULTA DA PESSOA
        // =========================================

        Email emailConsulta;
        emailConsulta.setEmail("teste@gmail.com");

        Pessoa pessoaEncontrada = pessoas.consultar(emailConsulta);

        std::cout << "\n--- Pessoa encontrada ---\n";

        std::cout << "Email: "
            << pessoaEncontrada.getEmail().getEmail()
            << std::endl;
        std::cout << "Nome: "
            << pessoaEncontrada.getNome().getNome()
            << std::endl;
        std::cout << "Papel: "
            << pessoaEncontrada.getPapel().getPapel()
            << std::endl;

    }
    catch (const std::exception& erro) {

        std::cerr << "Erro: " << erro.what() << std::endl;
    }

        // Fechamento do banco de dados
        sqlite3_close(banco);

    return 0;
}
