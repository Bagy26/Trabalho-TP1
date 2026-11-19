#ifndef TELA_PRINCIPAL_HPP
#define TELA_PRINCIPAL_HPP

#include <iostream>
#include <string>
#include "../dominio/papel.hpp"

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
};

#endif