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

        ServicoCartoesAtividades servicoCartoes(cartoes, quadros);

        // =========================================
        // CRIACAO DOS DEMAIS SERVICOS
        // =========================================

        ServicoPessoas servicoPessoas(pessoas);
        ServicoProjetos servicoProjetos(projetos);
        ServicoQuadros servicoQuadros(quadros);

        // =========================================
        // CRIACAO DAS CONTROLADORAS
        // =========================================

        ControladoraPessoas controladoraPessoas(servicoPessoas);
        ControladoraProjetos controladoraProjetos(servicoProjetos);
        ControladoraQuadros controladoraQuadros(servicoQuadros);
        ControladoraCartoesAtividades controladoraCartoes(servicoCartoes);

        // =========================================
        // CRIAÇÃO DO QUADRO DE TESTE
        // =========================================

        Identificador identificadorQuadro;
        identificadorQuadro.setIdentificador("QDR001");

        Nome nomeQuadro;
        nomeQuadro.setNome("Quadro Teste");

        Limite limiteWIP;
        limiteWIP.setValor(3);

        Identificador projetoDoQuadro;
        projetoDoQuadro.setIdentificador("PRJ001");

        Quadro quadro;
        quadro.setIdentificador(identificadorQuadro);
        quadro.setNome(nomeQuadro);
        quadro.setLimite(limiteWIP);
        quadro.setProjeto(projetoDoQuadro);

        // Evita cadastrar novamente um quadro existente.
        bool quadroExiste = false;

        std::vector<Quadro> quadrosCadastrados = quadros.listar();

        for (const Quadro &existente : quadrosCadastrados)
        {
            if (existente.getIdentificador().getIdentificador() == "QDR001")
            {
                quadroExiste = true;
                break;
            }
        }

        if (!quadroExiste)
        {
            quadros.inserir(quadro);
            std::cout << "Quadro cadastrado com sucesso!" << std::endl;
        }
        else
        {
            std::cout << "Utilizando quadro ja cadastrado!" << std::endl;
        }

        // =========================================
        // CRIAÇÃO DO PRIMEIRO CARTÃO DE TESTE
        // =========================================

        Identificador identificadorCartao;
        identificadorCartao.setIdentificador("CAR001");

        Nome nomeCartao;
        nomeCartao.setNome("Cartao Teste");

        Texto descricaoCartao;
        descricaoCartao.setTexto("Testar indicador WIP.");

        Prioridade prioridadeCartao;
        prioridadeCartao.setPrioridade("ALTA");

        Estado estadoCartao;
        estadoCartao.setEstado("A FAZER");

        Tamanho tamanhoCartao;
        tamanhoCartao.setTamanho("PEQUENO");

        Timestamp entradaCartao;
        entradaCartao.setTimestamp("15-NOV-2026-08:00");

        Timestamp inicioCartao;
        inicioCartao.setTimestamp("15-NOV-2026-08:00");

        Timestamp terminoCartao;
        terminoCartao.setTimestamp("15-NOV-2026-08:00");

        Email desenvolvedorCartao;
        desenvolvedorCartao.setEmail("teste@gmail.com");

        CartaoAtividade cartao;
        cartao.setIdentificador(identificadorCartao);
        cartao.setNome(nomeCartao);
        cartao.setDescricao(descricaoCartao);
        cartao.setPrioridade(prioridadeCartao);
        cartao.setEstado(estadoCartao);
        cartao.setTamanho(tamanhoCartao);
        cartao.setEntrada(entradaCartao);
        cartao.setInicio(inicioCartao);
        cartao.setTermino(terminoCartao);
        cartao.setDesenvolvedor(desenvolvedorCartao);
        cartao.setQuadro(identificadorQuadro);

        // Verifica se o cartão já existe.
        bool cartaoExiste = false;

        std::vector<CartaoAtividade> cartoesCadastrados =
            cartoes.listar();

        for (const CartaoAtividade &existente : cartoesCadastrados)
        {
            if (existente.getIdentificador().getIdentificador() == "CAR001")
            {
                cartaoExiste = true;
                break;
            }
        }

        if (!cartaoExiste)
        {
            servicoCartoes.cadastrar(cartao);
            std::cout << "Cartao CAR001 cadastrado!" << std::endl;
        }
        else
        {
            std::cout << "Cartao CAR001 ja cadastrado!" << std::endl;
        }

        // =========================================
        // TESTE DE MOVIMENTAÇÃO E WIP
        // =========================================

        Estado estadoFazendo;
        estadoFazendo.setEstado("FAZENDO");

        Timestamp momentoInicio;
        momentoInicio.setTimestamp("15-NOV-2026-09:00");

        CartaoAtividade cartaoAtual =
            servicoCartoes.consultar(identificadorCartao);

        if (cartaoAtual.getEstado().getEstado() == "A FAZER")
        {
            servicoCartoes.mover(
                identificadorCartao,
                estadoFazendo,
                momentoInicio);

            std::cout << "Cartao movido para FAZENDO!" << std::endl;
        }
        else
        {
            std::cout << "Cartao ja foi movimentado anteriormente."
                      << std::endl;
        }

        std::cout << "WIP atual do quadro: "
                  << servicoCartoes.calcularWIP(identificadorQuadro)
                  << std::endl;

        // =========================================
        // TESTE DE BLOQUEIO DO LIMITE WIP
        // =========================================

        Identificador idQuartoCartao;
        idQuartoCartao.setIdentificador("CAR004");

        Nome nomeQuartoCartao;
        nomeQuartoCartao.setNome("Cartao Quatro");

        Texto descricaoQuartoCartao;
        descricaoQuartoCartao.setTexto("Testar limite WIP.");

        Prioridade prioridadeQuartoCartao;
        prioridadeQuartoCartao.setPrioridade("ALTA");

        Estado estadoQuartoCartao;
        estadoQuartoCartao.setEstado("A FAZER");

        Tamanho tamanhoQuartoCartao;
        tamanhoQuartoCartao.setTamanho("PEQUENO");

        Timestamp dataQuartoCartao;
        dataQuartoCartao.setTimestamp("19-NOV-2026-08:00");

        CartaoAtividade quartoCartao;
        quartoCartao.setIdentificador(idQuartoCartao);
        quartoCartao.setNome(nomeQuartoCartao);
        quartoCartao.setDescricao(descricaoQuartoCartao);
        quartoCartao.setPrioridade(prioridadeQuartoCartao);
        quartoCartao.setEstado(estadoQuartoCartao);
        quartoCartao.setTamanho(tamanhoQuartoCartao);
        quartoCartao.setEntrada(dataQuartoCartao);
        quartoCartao.setInicio(dataQuartoCartao);
        quartoCartao.setTermino(dataQuartoCartao);
        quartoCartao.setDesenvolvedor(desenvolvedorCartao);
        quartoCartao.setQuadro(identificadorQuadro);

        // Cadastra somente se ainda nao existir.
        bool quartoExiste = false;

        std::vector<CartaoAtividade> todosCartoes = cartoes.listar();

        for (const CartaoAtividade &existente : todosCartoes)
        {
            if (existente.getIdentificador().getIdentificador() == "CAR004")
            {
                quartoExiste = true;
                break;
            }
        }

        if (!quartoExiste)
        {
            servicoCartoes.cadastrar(quartoCartao);
        }

        Estado novoEstado;
        novoEstado.setEstado("FAZENDO");

        Timestamp momentoMovimentacao;
        momentoMovimentacao.setTimestamp("19-NOV-2026-12:00");

        try
        {
            servicoCartoes.mover(
                idQuartoCartao,
                novoEstado,
                momentoMovimentacao);

            std::cout << "ERRO: o limite WIP nao foi respeitado!"
                      << std::endl;
        }
        catch (const std::exception &erro)
        {
            std::cout << "Bloqueio esperado: "
                      << erro.what() << std::endl;
        }

        std::cout << "WIP apos tentativa: "
                  << servicoCartoes.calcularWIP(identificadorQuadro)
                  << std::endl;

        // =========================================
        // TESTE DE LEAD TIME E CYCLE TIME
        // =========================================

        Estado estadoFeito;
        estadoFeito.setEstado("FEITO");

        // Identificador do CAR002
        Identificador idCartaoDois;
        idCartaoDois.setIdentificador("CAR002");

        // Consulta os cartões
        CartaoAtividade primeiro = servicoCartoes.consultar(
            identificadorCartao);

        CartaoAtividade segundo = servicoCartoes.consultar(
            idCartaoDois);

        // Conclui CAR001
        if (primeiro.getEstado().getEstado() == "FAZENDO")
        {
            Timestamp terminoUm;
            terminoUm.setTimestamp("19-NOV-2026-13:00");

            servicoCartoes.mover(
                identificadorCartao,
                estadoFeito,
                terminoUm);

            std::cout << "CAR001 concluido!" << std::endl;
        }

        // Conclui CAR002
        if (segundo.getEstado().getEstado() == "FAZENDO")
        {
            Timestamp terminoDois;
            terminoDois.setTimestamp("19-NOV-2026-14:00");

            servicoCartoes.mover(
                idCartaoDois,
                estadoFeito,
                terminoDois);

            std::cout << "CAR002 concluido!" << std::endl;
        }

        // Calcula os indicadores
        std::cout << "\n--- INDICADORES DO QUADRO ---" << std::endl;

        std::cout << "WIP: "
                  << servicoCartoes.calcularWIP(identificadorQuadro)
                  << std::endl;

        std::cout << "Lead Time medio (minutos): "
                  << servicoCartoes.calcularLeadTimeMedio(identificadorQuadro)
                  << std::endl;

        std::cout << "Cycle Time medio (minutos): "
                  << servicoCartoes.calcularCycleTimeMedio(identificadorQuadro)
                  << std::endl;

        // =========================================
        // CADASTRO DOS CARTOES CAR002 E CAR003
        // =========================================

        for (int numero = 2; numero <= 3; numero++)
        {

            std::string id = "CAR00" + std::to_string(numero);

            Identificador identificador;
            identificador.setIdentificador(id);

            Nome nome;

            if (numero == 2)
            {
                nome.setNome("Cartao Dois");
            }
            else
            {
                nome.setNome("Cartao Tres");
            }
            Texto descricao;
            descricao.setTexto("Testar limite WIP.");

            Prioridade prioridade;
            prioridade.setPrioridade("MEDIA");

            Estado estado;
            estado.setEstado("A FAZER");

            Tamanho tamanho;
            tamanho.setTamanho("PEQUENO");

            Timestamp entrada;
            entrada.setTimestamp("15-NOV-2026-08:00");

            Timestamp inicio;
            inicio.setTimestamp("15-NOV-2026-08:00");

            Timestamp termino;
            termino.setTimestamp("15-NOV-2026-08:00");

            Email desenvolvedor;
            desenvolvedor.setEmail("teste@gmail.com");

            CartaoAtividade novoCartao;
            novoCartao.setIdentificador(identificador);
            novoCartao.setNome(nome);
            novoCartao.setDescricao(descricao);
            novoCartao.setPrioridade(prioridade);
            novoCartao.setEstado(estado);
            novoCartao.setTamanho(tamanho);
            novoCartao.setEntrada(entrada);
            novoCartao.setInicio(inicio);
            novoCartao.setTermino(termino);
            novoCartao.setDesenvolvedor(desenvolvedor);
            novoCartao.setQuadro(identificadorQuadro);

            bool existe = false;

            std::vector<CartaoAtividade> cadastrados =
                cartoes.listar();

            for (const CartaoAtividade &existente : cadastrados)
            {
                if (existente.getIdentificador().getIdentificador() == id)
                {
                    existe = true;
                    break;
                }
            }

            if (!existe)
            {
                servicoCartoes.cadastrar(novoCartao);
                std::cout << id << " cadastrado!" << std::endl;
            }
            else
            {
                std::cout << id << " ja cadastrado!" << std::endl;
            }

            // Movimenta o cartão para FAZENDO.
            CartaoAtividade atual = servicoCartoes.consultar(identificador);

            if (atual.getEstado().getEstado() == "A FAZER")
            {
                Estado fazendo;
                fazendo.setEstado("FAZENDO");

                Timestamp momento;
                momento.setTimestamp(
                    numero == 2
                        ? "15-NOV-2026-10:00"
                        : "15-NOV-2026-11:00");

                servicoCartoes.mover(
                    identificador,
                    fazendo,
                    momento);

                std::cout << id << " movido para FAZENDO!" << std::endl;
            }
        }

        std::cout << "\nWIP final do quadro: "
                  << servicoCartoes.calcularWIP(identificadorQuadro)
                  << std::endl;

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
        for (const Projeto &projetoCadastrado : projetosCadastrados)
        {
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
        for (const Pessoa &pessoaCadastrada : pessoasCadastradas)
        {
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
    catch (const std::exception &erro)
    {

        std::cerr << "Erro: " << erro.what() << std::endl;
    }

    // Fechamento do banco de dados
    sqlite3_close(banco);

    return 0;
}
