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
        // 4. TELA INICIAL, LOGIN E CADASTRO
        // =========================================

        Pessoa pessoaEncontrada;
        bool loginRealizado = false;
        int opcaoInicial;

        // Funcao para realizar o login.
        auto realizarLogin = [&]() -> bool
        {
            try
            {
                std::string emailInformado;
                std::string senhaInformada;

                std::cout << "\n========== LOGIN ==========\n";

                std::cout << "Email: ";
                std::cin >> emailInformado;

                std::cout << "Senha: ";
                std::cin >> senhaInformada;

                Email email;
                email.setEmail(emailInformado);

                Pessoa usuario = pessoas.consultar(email);

                if (usuario.getSenha().getSenha() != senhaInformada)
                {
                    std::cout << "\nEmail ou senha incorretos.\n";
                    return false;
                }

                pessoaEncontrada = usuario;

                std::cout << "\nLogin realizado com sucesso!\n";
                std::cout << "Bem-vindo(a), "
                          << pessoaEncontrada.getNome().getNome()
                          << "!\n";

                return true;
            }
            catch (const std::exception &erro)
            {
                std::cout << "\nErro no login: "
                          << erro.what() << "\n";

                return false;
            }
        };

        // Tela inicial.
        do
        {
            std::cout << "\n========== KANBAN ==========\n";
            std::cout << "1 - Entrar\n";
            std::cout << "2 - Cadastrar usuario\n";
            std::cout << "0 - Sair\n";
            std::cout << "Escolha uma opcao: ";

            std::cin >> opcaoInicial;

            switch (opcaoInicial)
            {
            case 1:
            {
                loginRealizado = realizarLogin();
                break;
            }

            case 2:
            {
                try
                {
                    std::string emailInformado;
                    std::string nomeInformado;
                    std::string senhaInformada;
                    std::string papelInformado;

                    Pessoa novaPessoa;

                    std::cout << "\n========== CADASTRO DE USUARIO ==========\n";

                    std::cout << "Email: ";
                    std::cin >> emailInformado;

                    Email email;
                    email.setEmail(emailInformado);
                    novaPessoa.setEmail(email);

                    std::cout << "Nome: ";
                    std::getline(std::cin >> std::ws, nomeInformado);

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

                    // Cadastro sem exigir autenticacao.
                    pessoas.inserir(novaPessoa);

                    std::cout << "\nUsuario cadastrado com sucesso!\n";
                    std::cout << "Agora voce pode entrar usando seu email e senha.\n";
                }
                catch (const std::exception &erro)
                {
                    std::cout << "\nErro no cadastro: "
                              << erro.what() << "\n";
                }

                break;
            }

            case 0:
                std::cout << "\nEncerrando o sistema...\n";
                break;

            default:
                std::cout << "\nOpcao invalida.\n";
            }

        } while (!loginRealizado && opcaoInicial != 0);

        if (!loginRealizado)
        {
            sqlite3_close(banco);
            return 0;
        }

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
                    {
                        try
                        {
                            std::vector<Quadro> quadrosCadastrados =
                                controladoraQuadros.listar(pessoaEncontrada.getPapel());

                            std::cout << "\n========== QUADROS CADASTRADOS ==========\n";

                            if (quadrosCadastrados.empty())
                            {
                                std::cout << "Nenhum quadro cadastrado.\n";
                            }
                            else
                            {
                                for (const Quadro &quadro : quadrosCadastrados)
                                {
                                    std::cout << "\nIdentificador: "
                                              << quadro.getIdentificador().getIdentificador()
                                              << "\n";

                                    std::cout << "Nome: "
                                              << quadro.getNome().getNome()
                                              << "\n";

                                    std::cout << "Limite WIP: "
                                              << quadro.getLimite().getValor()
                                              << "\n";

                                    std::cout << "Projeto associado: "
                                              << quadro.getProjeto().getIdentificador()
                                              << "\n";

                                    std::cout << "----------------------------------------\n";
                                }
                            }
                        }
                        catch (const std::exception &erro)
                        {
                            std::cout << "\nErro ao listar quadros: "
                                      << erro.what() << "\n";
                        }

                        break;
                    }

                    case 4:
                    {
                        try
                        {
                            std::string valorIdentificador;
                            std::string novoNome;
                            int novoLimite;

                            std::cout << "\n========== ATUALIZAR QUADRO ==========\n";

                            std::cout << "Identificador do quadro: ";
                            std::getline(std::cin >> std::ws, valorIdentificador);

                            Identificador identificador;
                            identificador.setIdentificador(valorIdentificador);

                            // Busca o quadro existente.
                            Quadro quadroAtualizado =
                                controladoraQuadros.consultar(
                                    pessoaEncontrada.getPapel(),
                                    identificador);

                            std::cout << "\nQuadro encontrado: "
                                      << quadroAtualizado.getNome().getNome()
                                      << "\n";

                            std::cout << "Novo nome: ";
                            std::getline(std::cin, novoNome);

                            std::cout << "Novo limite WIP: ";
                            std::cin >> novoLimite;

                            // Valida os novos dados.
                            Nome nome;
                            nome.setNome(novoNome);

                            Limite limite;
                            limite.setValor(novoLimite);

                            // Altera somente nome e limite.
                            quadroAtualizado.setNome(nome);
                            quadroAtualizado.setLimite(limite);

                            controladoraQuadros.atualizar(
                                pessoaEncontrada.getPapel(),
                                quadroAtualizado);

                            std::cout << "\nQuadro atualizado com sucesso!\n";
                        }
                        catch (const std::exception &erro)
                        {
                            std::cout << "\nErro ao atualizar quadro: "
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

                            std::cout << "\n========== REMOVER QUADRO ==========\n";

                            std::cout << "Identificador do quadro: ";
                            std::getline(std::cin >> std::ws, valorIdentificador);

                            Identificador identificador;
                            identificador.setIdentificador(valorIdentificador);

                            // Consulta o quadro antes de removê-lo.
                            Quadro quadroEncontrado =
                                controladoraQuadros.consultar(
                                    pessoaEncontrada.getPapel(),
                                    identificador);

                            std::cout << "\nQuadro encontrado: "
                                      << quadroEncontrado.getNome().getNome()
                                      << "\n";

                            std::cout << "Deseja realmente remover este quadro? (S/N): ";
                            std::cin >> confirmacao;

                            if (confirmacao == 'S' || confirmacao == 's')
                            {
                                controladoraQuadros.remover(
                                    pessoaEncontrada.getPapel(),
                                    identificador);

                                std::cout << "\nQuadro removido com sucesso!\n";
                            }
                            else
                            {
                                std::cout << "\nRemocao cancelada.\n";
                            }
                        }
                        catch (const std::exception &erro)
                        {
                            std::cout << "\nErro ao remover quadro: "
                                      << erro.what() << "\n";
                        }

                        break;
                    }

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
            {
                int opcaoCartoes;

                do
                {
                    tela.exibirMenuCartoes();
                    opcaoCartoes = tela.lerOpcao();

                    switch (opcaoCartoes)
                    {
                    case 1:
                    {
                        try
                        {
                            std::string valorIdentificador;
                            std::string valorNome;
                            std::string valorDescricao;
                            std::string valorPrioridade;
                            std::string valorTamanho;
                            std::string valorDesenvolvedor;
                            std::string valorQuadro;
                            std::string valorEntrada;

                            std::cout << "\n========== CADASTRAR CARTAO ==========\n";

                            std::cout << "Identificador: ";
                            std::getline(std::cin >> std::ws, valorIdentificador);

                            std::cout << "Nome: ";
                            std::getline(std::cin, valorNome);

                            std::cout << "Descricao: ";
                            std::getline(std::cin, valorDescricao);

                            std::cout << "Prioridade (ALTA, MEDIA, BAIXA): ";
                            std::getline(std::cin, valorPrioridade);

                            std::cout << "Tamanho (GRANDE, MEDIO, PEQUENO): ";
                            std::getline(std::cin, valorTamanho);

                            std::cout << "Email do desenvolvedor responsavel: ";
                            std::getline(std::cin, valorDesenvolvedor);

                            std::cout << "Identificador do quadro: ";
                            std::getline(std::cin, valorQuadro);

                            std::cout << "Data e hora de entrada (DD-MMM-AAAA-HH:MM): ";
                            std::getline(std::cin, valorEntrada);

                            // Validacao dos dados de dominio.
                            Identificador identificador;
                            identificador.setIdentificador(valorIdentificador);

                            Nome nome;
                            nome.setNome(valorNome);

                            Texto descricao;
                            descricao.setTexto(valorDescricao);

                            Prioridade prioridade;
                            prioridade.setPrioridade(valorPrioridade);

                            Tamanho tamanho;
                            tamanho.setTamanho(valorTamanho);

                            Email desenvolvedor;
                            desenvolvedor.setEmail(valorDesenvolvedor);

                            Identificador quadro;
                            quadro.setIdentificador(valorQuadro);

                            Timestamp entrada;
                            entrada.setTimestamp(valorEntrada);

                            // Estado inicial obrigatorio.
                            Estado estado;
                            estado.setEstado("A FAZER");

                            // Montagem da entidade.
                            CartaoAtividade novoCartao;

                            novoCartao.setIdentificador(identificador);
                            novoCartao.setNome(nome);
                            novoCartao.setDescricao(descricao);
                            novoCartao.setPrioridade(prioridade);
                            novoCartao.setEstado(estado);
                            novoCartao.setTamanho(tamanho);
                            novoCartao.setEntrada(entrada);
                            novoCartao.setDesenvolvedor(desenvolvedor);
                            novoCartao.setQuadro(quadro);

                            // Cadastro pela controladora.
                            controladoraCartoes.cadastrar(
                                pessoaEncontrada.getPapel(),
                                novoCartao);

                            std::cout << "\nCartao cadastrado com sucesso!\n";
                        }
                        catch (const std::exception &erro)
                        {
                            std::cout << "\nErro ao cadastrar cartao: "
                                      << erro.what() << "\n";
                        }

                        break;
                    }

                    case 2:
                    {
                        try
                        {
                            std::string valorIdentificador;

                            std::cout << "\n========== CONSULTAR CARTAO ==========\n";

                            std::cout << "Identificador do cartao: ";
                            std::getline(std::cin >> std::ws, valorIdentificador);

                            Identificador identificador;
                            identificador.setIdentificador(valorIdentificador);

                            CartaoAtividade cartaoEncontrado =
                                controladoraCartoes.consultar(
                                    pessoaEncontrada.getPapel(),
                                    identificador);

                            std::cout << "\n========== DADOS DO CARTAO ==========\n";

                            std::cout << "Identificador: "
                                      << cartaoEncontrado.getIdentificador().getIdentificador()
                                      << "\n";

                            std::cout << "Nome: "
                                      << cartaoEncontrado.getNome().getNome()
                                      << "\n";

                            std::cout << "Descricao: "
                                      << cartaoEncontrado.getDescricao().getTexto()
                                      << "\n";

                            std::cout << "Prioridade: "
                                      << cartaoEncontrado.getPrioridade().getPrioridade()
                                      << "\n";

                            std::cout << "Estado: "
                                      << cartaoEncontrado.getEstado().getEstado()
                                      << "\n";

                            std::cout << "Tamanho: "
                                      << cartaoEncontrado.getTamanho().getTamanho()
                                      << "\n";

                            std::cout << "Entrada: "
                                      << cartaoEncontrado.getEntrada().getTimestamp()
                                      << "\n";

                            std::cout << "Inicio: "
                                      << cartaoEncontrado.getInicio().getTimestamp()
                                      << "\n";

                            std::cout << "Termino: "
                                      << cartaoEncontrado.getTermino().getTimestamp()
                                      << "\n";

                            std::cout << "Desenvolvedor: "
                                      << cartaoEncontrado.getDesenvolvedor().getEmail()
                                      << "\n";

                            std::cout << "Quadro: "
                                      << cartaoEncontrado.getQuadro().getIdentificador()
                                      << "\n";
                        }
                        catch (const std::exception &erro)
                        {
                            std::cout << "\nErro ao consultar cartao: "
                                      << erro.what() << "\n";
                        }

                        break;
                    }
                    case 3:
                    {
                        try
                        {
                            std::vector<CartaoAtividade> cartoesCadastrados =
                                controladoraCartoes.listar(
                                    pessoaEncontrada.getPapel());

                            std::cout << "\n========== CARTOES CADASTRADOS ==========\n";

                            if (cartoesCadastrados.empty())
                            {
                                std::cout << "Nenhum cartao cadastrado.\n";
                            }
                            else
                            {
                                for (const CartaoAtividade &cartao : cartoesCadastrados)
                                {
                                    std::cout << "\nIdentificador: "
                                              << cartao.getIdentificador().getIdentificador()
                                              << "\n";

                                    std::cout << "Nome: "
                                              << cartao.getNome().getNome()
                                              << "\n";

                                    std::cout << "Descricao: "
                                              << cartao.getDescricao().getTexto()
                                              << "\n";

                                    std::cout << "Prioridade: "
                                              << cartao.getPrioridade().getPrioridade()
                                              << "\n";

                                    std::cout << "Estado: "
                                              << cartao.getEstado().getEstado()
                                              << "\n";

                                    std::cout << "Tamanho: "
                                              << cartao.getTamanho().getTamanho()
                                              << "\n";

                                    std::cout << "Entrada: "
                                              << cartao.getEntrada().getTimestamp()
                                              << "\n";

                                    std::cout << "Inicio: "
                                              << cartao.getInicio().getTimestamp()
                                              << "\n";

                                    std::cout << "Termino: "
                                              << cartao.getTermino().getTimestamp()
                                              << "\n";

                                    std::cout << "Desenvolvedor: "
                                              << cartao.getDesenvolvedor().getEmail()
                                              << "\n";

                                    std::cout << "Quadro: "
                                              << cartao.getQuadro().getIdentificador()
                                              << "\n";

                                    std::cout << "----------------------------------------\n";
                                }
                            }
                        }
                        catch (const std::exception &erro)
                        {
                            std::cout << "\nErro ao listar cartoes: "
                                      << erro.what() << "\n";
                        }

                        break;
                    }

                    case 4:
                    {
                        try
                        {
                            string id;
                            cout << "Informe o identificador do cartao: ";
                            cin >> id;

                            Identificador identificador;
                            identificador.setIdentificador(id);

                            CartaoAtividade cartao =
                                controladoraCartoes.consultar(
                                    pessoaEncontrada.getPapel(),
                                    identificador);

                            cout << "\nCartao encontrado: "
                                 << cartao.getNome().getNome() << endl;

                            string nome, descricao, prioridade, tamanho, email;

                            cout << "Novo nome: ";
                            cin.ignore();
                            getline(cin, nome);

                            cout << "Nova descricao: ";
                            getline(cin, descricao);

                            cout << "Nova prioridade (ALTA, MEDIA, BAIXA): ";
                            cin >> prioridade;

                            cout << "Novo tamanho (GRANDE, MEDIO, PEQUENO): ";
                            cin >> tamanho;

                            cout << "Novo email do desenvolvedor: ";
                            cin >> email;

                            Nome novoNome;
                            novoNome.setNome(nome);

                            Texto novaDescricao;
                            novaDescricao.setTexto(descricao);

                            Prioridade novaPrioridade;
                            novaPrioridade.setPrioridade(prioridade);

                            Tamanho novoTamanho;
                            novoTamanho.setTamanho(tamanho);

                            Email novoEmail;
                            novoEmail.setEmail(email);

                            cartao.setNome(novoNome);
                            cartao.setDescricao(novaDescricao);
                            cartao.setPrioridade(novaPrioridade);
                            cartao.setTamanho(novoTamanho);
                            cartao.setDesenvolvedor(novoEmail);

                            controladoraCartoes.atualizar(
                                pessoaEncontrada.getPapel(),
                                cartao);

                            cout << "\nCartao atualizado com sucesso!\n";
                        }
                        catch (const exception &erro)
                        {
                            cout << "\nErro: " << erro.what() << endl;
                        }

                        break;
                    }

                    case 5:
                    {
                        try
                        {
                            string id;
                            char confirmacao;

                            cout << "Informe o identificador do cartao: ";
                            cin >> id;

                            Identificador identificador;
                            identificador.setIdentificador(id);

                            CartaoAtividade cartao =
                                controladoraCartoes.consultar(
                                    pessoaEncontrada.getPapel(),
                                    identificador);

                            cout << "\nCartao encontrado: "
                                 << cartao.getNome().getNome() << endl;

                            cout << "Deseja realmente remover este cartao? (S/N): ";
                            cin >> confirmacao;

                            if (confirmacao == 'S' || confirmacao == 's')
                            {
                                controladoraCartoes.remover(
                                    pessoaEncontrada.getPapel(),
                                    identificador);

                                cout << "\nCartao removido com sucesso!\n";
                            }
                            else
                            {
                                cout << "\nRemocao cancelada.\n";
                            }
                        }
                        catch (const exception &erro)
                        {
                            cout << "\nErro: " << erro.what() << endl;
                        }

                        break;
                    }

                    case 6:
                    {
                        try
                        {
                            string id, estado, dataHora;

                            cout << "Informe o identificador do cartao: ";
                            cin >> id;

                            Identificador identificador;
                            identificador.setIdentificador(id);

                            CartaoAtividade cartao =
                                controladoraCartoes.consultar(
                                    pessoaEncontrada.getPapel(),
                                    identificador);

                            cout << "Estado atual: "
                                 << cartao.getEstado().getEstado() << endl;

                            cout << "Novo estado (FAZENDO ou FEITO): ";
                            cin.ignore();
                            getline(cin, estado);

                            Estado novoEstado;
                            novoEstado.setEstado(estado);

                            cout << "Informe a data e hora da movimentacao ";
                            cout << "(DD-MMM-AAAA-HH:MM): ";
                            cin >> dataHora;

                            Timestamp momento;
                            momento.setTimestamp(dataHora);

                            controladoraCartoes.mover(
                                pessoaEncontrada.getPapel(),
                                identificador,
                                novoEstado,
                                momento);

                            cout << "\nCartao movimentado com sucesso!\n";
                        }
                        catch (const exception &erro)
                        {
                            cout << "\nErro: " << erro.what() << endl;
                        }

                        break;
                    }

                    case 7:
                    {
                        try
                        {
                            string id;

                            cout << "Informe o identificador do quadro: ";
                            cin >> id;

                            Identificador identificador;
                            identificador.setIdentificador(id);

                            int wip = controladoraCartoes.calcularWIP(
                                pessoaEncontrada.getPapel(),
                                identificador);

                            double leadTime =
                                controladoraCartoes.calcularLeadTimeMedio(
                                    pessoaEncontrada.getPapel(),
                                    identificador);

                            double cycleTime =
                                controladoraCartoes.calcularCycleTimeMedio(
                                    pessoaEncontrada.getPapel(),
                                    identificador);

                            cout << "\n========== INDICADORES DO QUADRO ==========\n";
                            cout << "WIP atual: " << wip << endl;
                            cout << "Lead Time medio: " << leadTime << " minutos\n";
                            cout << "Cycle Time medio: " << cycleTime << " minutos\n";
                        }
                        catch (const exception &erro)
                        {
                            cout << "\nErro: " << erro.what() << endl;
                        }

                        break;
                    }

                    case 0:
                        std::cout << "\nVoltando ao menu principal...\n";
                        break;

                    default:
                        std::cout << "\nOpcao invalida.\n";
                        break;
                    }

                } while (opcaoCartoes != 0);

                break;
            }

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
