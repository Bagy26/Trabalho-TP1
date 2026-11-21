#ifndef TELA_PRINCIPAL_HPP
#define TELA_PRINCIPAL_HPP

#include <iostream>
#include <string>
#include "../dominio/papel.hpp"
#include <vector>
#include "../entidades/Pessoa.hpp"

class TelaPrincipal
{
private:
    Papel papelUsuario;

public:
    /**
     * @brief Define o papel da pessoa autenticada.
     * @param papel Papel do usuario.
     */
    void setPapelUsuario(const Papel &papel)
    {
        papelUsuario = papel;
    }

    /**
     * @brief Exibe o menu principal conforme o papel do usuario.
     */
    void exibirMenu()
    {
        std::cout << "\n====================================\n";
        std::cout << "          SISTEMA KANBAN\n";
        std::cout << "====================================\n";

        std::cout << "1 - Gerenciar pessoas\n";
        std::cout << "2 - Gerenciar projetos\n";
        std::cout << "3 - Gerenciar quadros\n";

        if (papelUsuario.getPapel() == "GESTOR")
        {
            std::cout << "4 - Gerenciar cartoes\n";
        }
        else
        {
            std::cout << "4 - Consultar cartoes\n";
        }

        std::cout << "0 - Sair\n";
        std::cout << "====================================\n";
        std::cout << "Escolha uma opcao: ";
    }

    /**
     * @brief Le a opcao escolhida pelo usuario.
     * @return Numero da opcao selecionada.
     */
    int lerOpcao()
    {
        int opcao;
        cin >> opcao;
        return opcao;
    }

    /**
     * @brief Exibe a lista de pessoas cadastradas.
     * @param pessoas Vetor de pessoas cadastradas.
     */
    void exibirPessoas(const std::vector<Pessoa> &pessoas)
    {
        std::cout << "\n========== PESSOAS CADASTRADAS ==========\n";

        if (pessoas.empty())
        {
            std::cout << "Nenhuma pessoa cadastrada.\n";
            return;
        }

        for (const Pessoa &pessoa : pessoas)
        {
            std::cout << "Nome: "
                      << pessoa.getNome().getNome() << "\n";

            std::cout << "Email: "
                      << pessoa.getEmail().getEmail() << "\n";

            std::cout << "Papel: "
                      << pessoa.getPapel().getPapel() << "\n";

            std::cout << "----------------------------------------\n";
        }
    }

    /**
     * @brief Exibe o menu de gerenciamento de pessoas.
     */
    void exibirMenuPessoas()
    {
        std::cout << "\n========== GERENCIAMENTO DE PESSOAS ==========\n";
        std::cout << "1 - Cadastrar pessoa\n";
        std::cout << "2 - Consultar pessoa\n";
        std::cout << "3 - Listar pessoas\n";
        std::cout << "4 - Atualizar pessoa\n";
        std::cout << "5 - Remover pessoa\n";
        std::cout << "0 - Voltar ao menu principal\n";
        std::cout << "Escolha uma opcao: ";
    }

    /**
     * @brief Exibe o menu de gerenciamento de projetos.
     */
    void exibirMenuProjetos()
    {
        std::cout << "\n========== GERENCIAMENTO DE PROJETOS ==========\n";
        std::cout << "1 - Cadastrar projeto\n";
        std::cout << "2 - Consultar projeto\n";
        std::cout << "3 - Listar projetos\n";
        std::cout << "4 - Atualizar projeto\n";
        std::cout << "5 - Remover projeto\n";
        std::cout << "0 - Voltar ao menu principal\n";
        std::cout << "Escolha uma opcao: ";
    }

    /**
     * @brief Exibe a lista de projetos cadastrados.
     * @param projetos Vetor de projetos cadastrados.
     */
    void exibirProjetos(const std::vector<Projeto> &projetos)
    {
        std::cout << "\n========== PROJETOS CADASTRADOS ==========\n";

        if (projetos.empty())
        {
            std::cout << "Nenhum projeto cadastrado.\n";
            return;
        }

        for (const Projeto &projeto : projetos)
        {
            std::cout << "Identificador: "
                      << projeto.getIdentificador().getIdentificador() << "\n";

            std::cout << "Nome: "
                      << projeto.getNome().getNome() << "\n";

            std::cout << "Descricao: "
                      << projeto.getDescricao().getTexto() << "\n";

            std::cout << "Inicio: "
                      << projeto.getInicio().getTimestamp() << "\n";

            std::cout << "Termino: "
                      << projeto.getTermino().getTimestamp() << "\n";

            std::cout << "Gestor: "
                      << projeto.getGestor().getEmail() << "\n";

            std::cout << "----------------------------------------\n";
        }
    }

    /**
     * @brief Exibe o menu de gerenciamento de quadros.
     */
    void exibirMenuQuadros()
    {
        std::cout << "\n========== GERENCIAMENTO DE QUADROS ==========\n";
        std::cout << "1 - Cadastrar quadro\n";
        std::cout << "2 - Consultar quadro\n";
        std::cout << "3 - Listar quadros\n";
        std::cout << "4 - Atualizar quadro\n";
        std::cout << "5 - Remover quadro\n";
        std::cout << "0 - Voltar ao menu principal\n";
        std::cout << "Escolha uma opcao: ";
    }
    void exibirMenuCartoes() const
    {
        std::cout << "\n========== CARTOES DE ATIVIDADE ==========\n";
        std::cout << "1 - Cadastrar cartao\n";
        std::cout << "2 - Consultar cartao\n";
        std::cout << "3 - Listar cartoes\n";
        std::cout << "4 - Atualizar cartao\n";
        std::cout << "5 - Remover cartao\n";
        std::cout << "6 - Mover cartao\n";
        std::cout << "7 - Indicadores do quadro\n";
        std::cout << "0 - Voltar\n";
        std::cout << "Escolha uma opcao: ";
    }
};

#endif