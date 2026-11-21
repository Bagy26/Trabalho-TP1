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
#include "entidades/Quadro.hpp"

#include "containers/Pessoas.hpp"
#include "containers/Projetos.hpp"
#include "containers/Quadros.hpp"
#include "containers/CartoesAtividades.hpp"

#include "servicos/ServicoCartoesAtividades.hpp"
#include "servicos/ServicoPessoas.hpp"
#include "servicos/ServicoProjetos.hpp"
#include "servicos/ServicoQuadros.hpp"

#include "controladoras/ControladoraPessoas.hpp"
#include "controladoras/ControladoraProjetos.hpp"
#include "controladoras/ControladoraQuadros.hpp"
#include "controladoras/ControladoraCartoesAtividades.hpp"

#include "apresentacao/TelaPrincipal.hpp"

int main()
{

    // Abertura do banco de dados
    sqlite3 *banco = nullptr;

    int resultado = sqlite3_open("kanban.db", &banco);

    if (resultado != SQLITE_OK)
    {
        std::cerr << "Erro ao abrir banco: "
                  << sqlite3_errmsg(banco) << std::endl;

        sqlite3_close(banco);
        return 1;
    }

    // Ativação das chaves estrangeiras
    if (sqlite3_exec(banco, "PRAGMA foreign_keys = ON;",
                     nullptr, nullptr, nullptr) != SQLITE_OK)
    {

        std::cerr << "Erro ao ativar chaves estrangeiras: "
                  << sqlite3_errmsg(banco) << std::endl;

        sqlite3_close(banco);
        return 1;
    }

    try
    {

        // =========================================
        // 1. CRIACAO DOS CONTAINERS E DAS TABELAS
        // =========================================

        Pessoas pessoas(banco);
        pessoas.criarTabela();

        Projetos projetos(banco);
        projetos.criarTabela();

        Quadros quadros(banco);
        quadros.criarTabela();

        CartoesAtividades cartoes(banco);
        cartoes.criarTabela();

        // =========================================
        // 2. CRIACAO DOS SERVICOS
        // =========================================

        ServicoPessoas servicoPessoas(pessoas);
        ServicoProjetos servicoProjetos(projetos);
        ServicoQuadros servicoQuadros(quadros);
        ServicoCartoesAtividades servicoCartoes(cartoes, quadros);

        // =========================================
        // 3. CRIACAO DAS CONTROLADORAS
        // =========================================

        ControladoraPessoas controladoraPessoas(servicoPessoas);
        ControladoraProjetos controladoraProjetos(servicoProjetos);
        ControladoraQuadros controladoraQuadros(servicoQuadros);
        ControladoraCartoesAtividades controladoraCartoes(servicoCartoes);

        // =========================================
        // 4. RECUPERACAO DO USUARIO
        // =========================================

        Email emailUsuario;
        emailUsuario.setEmail("teste@gmail.com");

        Pessoa pessoaEncontrada = pessoas.consultar(emailUsuario);

        // =========================================
        // 5. EXIBICAO DO MENU PRINCIPAL
        // =========================================

        TelaPrincipal tela;
        tela.setPapelUsuario(pessoaEncontrada.getPapel());

        int opcao;

        do
        {
            tela.exibirMenu();
            opcao = tela.lerOpcao();

            switch (opcao)
            {
            case 1:
            {
                int opcaoPessoas;

                do
                {
                    tela.exibirMenuPessoas();
                    opcaoPessoas = tela.lerOpcao();

                    switch (opcaoPessoas)
                    {
                    case 1:
                    {
                        std::string emailInformado;
                        std::string nomeInformado;
                        std::string senhaInformada;
                        std::string papelInformado;

                        try
                        {
                            Pessoa novaPessoa;

                            std::cout << "\n========== CADASTRO DE PESSOA ==========\n";

                            std::cout << "Email: ";
                            std::cin >> emailInformado;

                            Email email;
                            email.setEmail(emailInformado);
                            novaPessoa.setEmail(email);

                            std::cin.ignore();

                            std::cout << "Nome: ";
                            std::getline(std::cin, nomeInformado);

                            Nome nome;
                            nome.setNome(nomeInformado);
                            novaPessoa.setNome(nome);

                            std::cout << "Senha (5 caracteres): ";
                            std::cin >> senhaInformada;

                            Senha senha;
                            senha.setSenha(senhaInformada);
                            novaPessoa.setSenha(senha);

                            std::cout << "Papel (GESTOR ou DESENVOLVEDOR): ";
                            std::cin >> papelInformado;

                            Papel papel;
                            papel.setPapel(papelInformado);
                            novaPessoa.setPapel(papel);

                            controladoraPessoas.cadastrar(
                                pessoaEncontrada.getPapel(), novaPessoa);

                            std::cout << "\nPessoa cadastrada com sucesso!\n";
                        }
                        catch (const std::exception &erro)
                        {
                            std::cout << "\nErro no cadastro: "
                                      << erro.what() << std::endl;
                        }

                        break;
                    }

                    case 2:
                    {
                        std::string emailInformado;

                        std::cout << "\nDigite o email da pessoa: ";
                        std::cin >> emailInformado;

                        try
                        {
                            Email email;
                            email.setEmail(emailInformado);

                            Pessoa pessoaConsultada =
                                controladoraPessoas.consultar(
                                    pessoaEncontrada.getPapel(), email);

                            std::cout << "\n========== PESSOA ENCONTRADA ==========\n";
                            std::cout << "Nome: "
                                      << pessoaConsultada.getNome().getNome() << "\n";
                            std::cout << "Email: "
                                      << pessoaConsultada.getEmail().getEmail() << "\n";
                            std::cout << "Papel: "
                                      << pessoaConsultada.getPapel().getPapel() << "\n";
                        }
                        catch (const std::exception &erro)
                        {
                            std::cout << "Erro: " << erro.what() << std::endl;
                        }

                        break;
                    }

                    case 3:
                    {
                        std::vector<Pessoa> pessoasCadastradas =
                            controladoraPessoas.listar(pessoaEncontrada.getPapel());

                        tela.exibirPessoas(pessoasCadastradas);
                        break;
                    }

                    case 4:
                    {
                        std::string emailInformado;
                        std::string novoNome;
                        std::string novaSenha;
                        std::string novoPapel;

                        try
                        {
                            std::cout << "\n========== ATUALIZAR PESSOA ==========\n";

                            std::cout << "Digite o email da pessoa: ";
                            std::cin >> emailInformado;

                            Email email;
                            email.setEmail(emailInformado);

                            Pessoa pessoaAtualizada =
                                controladoraPessoas.consultar(
                                    pessoaEncontrada.getPapel(), email);

                            std::cin.ignore();

                            std::cout << "Novo nome: ";
                            std::getline(std::cin, novoNome);

                            Nome nome;
                            nome.setNome(novoNome);
                            pessoaAtualizada.setNome(nome);

                            std::cout << "Nova senha: ";
                            std::cin >> novaSenha;

                            Senha senha;
                            senha.setSenha(novaSenha);
                            pessoaAtualizada.setSenha(senha);

                            std::cout << "Novo papel (GESTOR ou DESENVOLVEDOR): ";
                            std::cin >> novoPapel;

                            Papel papel;
                            papel.setPapel(novoPapel);
                            pessoaAtualizada.setPapel(papel);

                            controladoraPessoas.atualizar(
                                pessoaEncontrada.getPapel(), pessoaAtualizada);

                            std::cout << "\nPessoa atualizada com sucesso!\n";
                        }
                        catch (const std::exception &erro)
                        {
                            std::cout << "\nErro na atualizacao: "
                                      << erro.what() << std::endl;
                        }

                        break;
                    }

                    case 5:
                    {
                        std::string emailInformado;
                        char confirmacao;

                        try
                        {
                            std::cout << "\n========== REMOVER PESSOA ==========\n";

                            std::cout << "Digite o email da pessoa: ";
                            std::cin >> emailInformado;

                            Email email;
                            email.setEmail(emailInformado);

                            Pessoa pessoaRemover =
                                controladoraPessoas.consultar(
                                    pessoaEncontrada.getPapel(), email);

                            std::cout << "\nPessoa encontrada:\n";
                            std::cout << "Nome: "
                                      << pessoaRemover.getNome().getNome() << "\n";
                            std::cout << "Email: "
                                      << pessoaRemover.getEmail().getEmail() << "\n";

                            std::cout << "\nDeseja realmente remover? (S/N): ";
                            std::cin >> confirmacao;

                            if (confirmacao == 'S' || confirmacao == 's')
                            {
                                controladoraPessoas.remover(
                                    pessoaEncontrada.getPapel(), email);

                                std::cout << "\nPessoa removida com sucesso!\n";
                            }
                            else
                            {
                                std::cout << "\nRemocao cancelada.\n";
                            }
                        }
                        catch (const std::exception &erro)
                        {
                            std::cout << "\nErro na remocao: "
                                      << erro.what() << std::endl;
                        }

                        break;
                    }

                    case 0:
                        std::cout << "Voltando ao menu principal..."
                                  << std::endl;
                        break;

                    default:
                        std::cout << "Opcao invalida!"
                                  << std::endl;
                    }

                } while (opcaoPessoas != 0);

                break;
            }
            case 2:
            {
                int opcaoProjetos;

                do
                {
                    tela.exibirMenuProjetos();
                    opcaoProjetos = tela.lerOpcao();

                    switch (opcaoProjetos)
                    {
                    case 1:
                    {
                        try
                        {
                            std::string valorIdentificador;
                            std::string valorNome;
                            std::string valorDescricao;
                            std::string valorInicio;
                            std::string valorTermino;

                            std::cout << "\n========== CADASTRO DE PROJETO ==========\n";

                            std::cout << "Identificador do projeto: ";
                            std::getline(std::cin >> std::ws, valorIdentificador);

                            std::cout << "Nome do projeto: ";
                            std::getline(std::cin, valorNome);

                            std::cout << "Descricao do projeto: ";
                            std::getline(std::cin, valorDescricao);

                            std::cout << "Data de inicio (DD-MMM-AAAA-HH:MM): ";
                            std::getline(std::cin, valorInicio);

                            std::cout << "Data de termino (DD-MMM-AAAA-HH:MM): ";
                            std::getline(std::cin, valorTermino);

                            // Cria e valida os dados do projeto.
                            Identificador identificador;
                            identificador.setIdentificador(valorIdentificador);

                            Nome nome;
                            nome.setNome(valorNome);

                            Texto descricao;
                            descricao.setTexto(valorDescricao);

                            Timestamp inicio;
                            inicio.setTimestamp(valorInicio);

                            Timestamp termino;
                            termino.setTimestamp(valorTermino);

                            // Monta a entidade Projeto.
                            Projeto novoProjeto;
                            novoProjeto.setIdentificador(identificador);
                            novoProjeto.setNome(nome);
                            novoProjeto.setDescricao(descricao);
                            novoProjeto.setInicio(inicio);
                            novoProjeto.setTermino(termino);
                            novoProjeto.setGestor(pessoaEncontrada.getEmail());

                            // Encaminha o cadastro para a controladora.
                            controladoraProjetos.cadastrar(
                                pessoaEncontrada.getPapel(),
                                novoProjeto);

                            std::cout << "\nProjeto cadastrado com sucesso!\n";
                        }
                        catch (const std::exception &erro)
                        {
                            std::cout << "\nErro ao cadastrar projeto: "
                                      << erro.what() << "\n";
                        }

                        break;
                    }

                    case 2:
                    {
                        try
                        {
                            std::string valorIdentificador;

                            std::cout << "\n========== CONSULTAR PROJETO ==========\n";

                            std::cout << "Informe o identificador do projeto: ";
                            std::getline(std::cin >> std::ws, valorIdentificador);

                            Identificador identificador;
                            identificador.setIdentificador(valorIdentificador);

                            Projeto projetoEncontrado =
                                controladoraProjetos.consultar(
                                    pessoaEncontrada.getPapel(),
                                    identificador);

                            std::cout << "\n========== DADOS DO PROJETO ==========\n";

                            std::cout << "Identificador: "
                                      << projetoEncontrado.getIdentificador().getIdentificador() << "\n";

                            std::cout << "Nome: "
                                      << projetoEncontrado.getNome().getNome() << "\n";

                            std::cout << "Descricao: "
                                      << projetoEncontrado.getDescricao().getTexto() << "\n";

                            std::cout << "Inicio: "
                                      << projetoEncontrado.getInicio().getTimestamp() << "\n";

                            std::cout << "Termino: "
                                      << projetoEncontrado.getTermino().getTimestamp() << "\n";

                            std::cout << "Gestor: "
                                      << projetoEncontrado.getGestor().getEmail() << "\n";
                        }
                        catch (const std::exception &erro)
                        {
                            std::cout << "\nErro ao consultar projeto: "
                                      << erro.what() << "\n";
                        }

                        break;
                    }

                    case 3:
                    {
                        std::vector<Projeto> projetosCadastrados =
                            controladoraProjetos.listar(pessoaEncontrada.getPapel());

                        tela.exibirProjetos(projetosCadastrados);
                        break;
                    }

                    case 4:
                    {
                        try
                        {
                            std::string valorIdentificador;
                            std::string valorNome;
                            std::string valorDescricao;
                            std::string valorInicio;
                            std::string valorTermino;

                            std::cout << "\n========== ATUALIZAR PROJETO ==========\n";

                            std::cout << "Informe o identificador do projeto: ";
                            std::getline(std::cin >> std::ws, valorIdentificador);

                            Identificador identificador;
                            identificador.setIdentificador(valorIdentificador);

                            // Busca o projeto existente no banco.
                            Projeto projetoAtualizado =
                                controladoraProjetos.consultar(
                                    pessoaEncontrada.getPapel(),
                                    identificador);

                            std::cout << "\nInforme os novos dados:\n";

                            std::cout << "Novo nome: ";
                            std::getline(std::cin, valorNome);

                            std::cout << "Nova descricao: ";
                            std::getline(std::cin, valorDescricao);

                            std::cout << "Nova data de inicio (DD-MMM-AAAA-HH:MM): ";
                            std::getline(std::cin, valorInicio);

                            std::cout << "Nova data de termino (DD-MMM-AAAA-HH:MM): ";
                            std::getline(std::cin, valorTermino);

                            // Valida os novos dados.
                            Nome nome;
                            nome.setNome(valorNome);

                            Texto descricao;
                            descricao.setTexto(valorDescricao);

                            Timestamp inicio;
                            inicio.setTimestamp(valorInicio);

                            Timestamp termino;
                            termino.setTimestamp(valorTermino);

                            // Atualiza somente os campos permitidos.
                            projetoAtualizado.setNome(nome);
                            projetoAtualizado.setDescricao(descricao);
                            projetoAtualizado.setInicio(inicio);
                            projetoAtualizado.setTermino(termino);

                            controladoraProjetos.atualizar(
                                pessoaEncontrada.getPapel(),
                                projetoAtualizado);

                            std::cout << "\nProjeto atualizado com sucesso!\n";
                        }
                        catch (const std::exception &erro)
                        {
                            std::cout << "\nErro ao atualizar projeto: "
                                      << erro.what() << "\n";
                        }

                        break;
                    }

                    case 5:
                    {
                        try
                        {
                            std::string valorIdentificador;
                            char confirmacao;

                            std::cout << "\n========== REMOVER PROJETO ==========\n";

                            std::cout << "Informe o identificador do projeto: ";
                            std::getline(std::cin >> std::ws, valorIdentificador);

                            Identificador identificador;
                            identificador.setIdentificador(valorIdentificador);

                            // Consulta o projeto antes de solicitar a remocao.
                            Projeto projetoEncontrado =
                                controladoraProjetos.consultar(
                                    pessoaEncontrada.getPapel(),
                                    identificador);

                            std::cout << "\nProjeto encontrado: "
                                      << projetoEncontrado.getNome().getNome() << "\n";

                            std::cout << "Deseja realmente remover este projeto? (S/N): ";
                            std::cin >> confirmacao;

                            if (confirmacao == 'S' || confirmacao == 's')
                            {
                                controladoraProjetos.remover(
                                    pessoaEncontrada.getPapel(),
                                    identificador);

                                std::cout << "\nProjeto removido com sucesso!\n";
                            }
                            else
                            {
                                std::cout << "\nRemocao cancelada.\n";
                            }
                        }
                        catch (const std::exception &erro)
                        {
                            std::cout << "\nErro ao remover projeto: "
                                      << erro.what() << "\n";
                        }

                        break;
                    }

                    case 0:
                        std::cout << "Voltando ao menu principal..."
                                  << std::endl;
                        break;

                    default:
                        std::cout << "Opcao invalida!"
                                  << std::endl;
                    }

                } while (opcaoProjetos != 0);

                break;
            }

            case 3:
            {
                int opcaoQuadros;

                do
                {
                    tela.exibirMenuQuadros();
                    opcaoQuadros = tela.lerOpcao();

                    switch (opcaoQuadros)
                    {
                    case 1:
                    {
                        try
                        {
                            std::string valorIdentificador;
                            std::string valorNome;
                            std::string valorProjeto;
                            int valorLimite;

                            std::cout << "\n========== CADASTRO DE QUADRO ==========\n";

                            std::cout << "Identificador do quadro: ";
                            std::getline(std::cin >> std::ws, valorIdentificador);

                            std::cout << "Nome do quadro: ";
                            std::getline(std::cin, valorNome);

                            std::cout << "Limite de atividades simultaneas (WIP): ";
                            std::cin >> valorLimite;
                            std::cin.ignore(10000, '\n');

                            std::cout << "Identificador do projeto associado: ";
                            std::getline(std::cin, valorProjeto);

                            Identificador identificador;
                            identificador.setIdentificador(valorIdentificador);

                            Nome nome;
                            nome.setNome(valorNome);

                            Limite limite;
                            limite.setValor(valorLimite);

                            Identificador identificadorProjeto;
                            identificadorProjeto.setIdentificador(valorProjeto);

                            // Verifica se o projeto existe.
                            controladoraProjetos.consultar(
                                pessoaEncontrada.getPapel(),
                                identificadorProjeto);

                            Quadro novoQuadro;
                            novoQuadro.setIdentificador(identificador);
                            novoQuadro.setNome(nome);
                            novoQuadro.setLimite(limite);
                            novoQuadro.setProjeto(identificadorProjeto);

                            controladoraQuadros.cadastrar(
                                pessoaEncontrada.getPapel(),
                                novoQuadro);

                            std::cout << "\nQuadro cadastrado com sucesso!\n";
                        }
                        catch (const std::exception &erro)
                        {
                            std::cout << "\nErro ao cadastrar quadro: "
                                      << erro.what() << "\n";
                        }

                        break;
                    }

                    case 2:
                    {
                        try
                        {
                            std::string valorIdentificador;

                            std::cout << "\n========== CONSULTAR QUADRO ==========\n";

                            std::cout << "Informe o identificador do quadro: ";
                            std::getline(std::cin >> std::ws, valorIdentificador);

                            Identificador identificador;
                            identificador.setIdentificador(valorIdentificador);

                            Quadro quadroEncontrado =
                                controladoraQuadros.consultar(
                                    pessoaEncontrada.getPapel(),
                                    identificador);

                            std::cout << "\n========== DADOS DO QUADRO ==========\n";

                            std::cout << "Identificador: "
                                      << quadroEncontrado.getIdentificador().getIdentificador() << "\n";

                            std::cout << "Nome: "
                                      << quadroEncontrado.getNome().getNome() << "\n";

                            std::cout << "Limite WIP: "
                                      << quadroEncontrado.getLimite().getValor() << "\n";

                            std::cout << "Projeto associado: "
                                      << quadroEncontrado.getProjeto().getIdentificador() << "\n";
                        }
                        catch (const std::exception &erro)
                        {
                            std::cout << "\nErro ao consultar quadro: "
                                      << erro.what() << "\n";
                        }

                        break;
                    }

                    case 3:
                        std::cout << "Opcao de listagem de quadros.\n";
                        break;

                    case 4:
                        std::cout << "Opcao de atualizacao de quadro.\n";
                        break;

                    case 5:
                        std::cout << "Opcao de remocao de quadro.\n";
                        break;

                    case 0:
                        std::cout << "Retornando ao menu principal.\n";
                        break;

                    default:
                        std::cout << "Opcao invalida.\n";
                    }

                } while (opcaoQuadros != 0);

                break;
            }
            case 4:
                std::cout << "Opcao de gerenciamento de cartoes."
                          << std::endl;
                break;

            case 0:
                std::cout << "Encerrando o sistema..."
                          << std::endl;
                break;

            default:
                std::cout << "Opcao invalida!"
                          << std::endl;
            }

        } while (opcao != 0);
    }

    catch (const std::exception &erro)
    {

        std::cerr << "Erro: " << erro.what() << std::endl;
    }

    // Fechamento do banco de dados
    sqlite3_close(banco);

    return 0;
}
