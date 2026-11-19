
#ifndef SERVICOCARTOESATIVIDADES_HPP
#define SERVICOCARTOESATIVIDADES_HPP

#include "../interfaces/ICartoesAtividades.hpp"
#include "../containers/CartoesAtividades.hpp"
#include "../containers/Quadros.hpp"

#include <stdexcept>
#include <string>
#include <vector>
#include <ctime>

class ServicoCartoesAtividades : public ICartoesAtividades
{
private:
    CartoesAtividades &repositorio;
    Quadros &repositorioQuadros;

    void validarLimiteWIP(
        const CartaoAtividade &cartao,
        const std::string &ignorarIdentificador = "")
    {
        Quadro quadro = repositorioQuadros.consultar(cartao.getQuadro());
        int limite = quadro.getLimite().getValor();
        int quantidade = 0;

        std::vector<CartaoAtividade> cartoes = repositorio.listar();

        for (const CartaoAtividade &existente : cartoes)
        {
            bool mesmoQuadro =
                existente.getQuadro().getIdentificador() ==
                cartao.getQuadro().getIdentificador();

            bool estaFazendo =
                existente.getEstado().getEstado() == "FAZENDO";

            bool naoIgnorar =
                existente.getIdentificador().getIdentificador() !=
                ignorarIdentificador;

            if (mesmoQuadro && estaFazendo && naoIgnorar)
            {
                quantidade++;
            }
        }

        if (quantidade >= limite)
        {
            throw std::runtime_error("Limite de WIP do quadro atingido.");
        }
    }

    long long converterParaMinutos(const Timestamp &timestamp)
    {
        std::string valor = timestamp.getTimestamp();

        int dia = std::stoi(valor.substr(0, 2));
        std::string mes = valor.substr(3, 3);
        int ano = std::stoi(valor.substr(7, 4));
        int hora = std::stoi(valor.substr(12, 2));
        int minuto = std::stoi(valor.substr(15, 2));

        int numeroMes;

        if (mes == "JAN")
            numeroMes = 0;
        else if (mes == "FEV")
            numeroMes = 1;
        else if (mes == "MAR")
            numeroMes = 2;
        else if (mes == "ABR")
            numeroMes = 3;
        else if (mes == "MAI")
            numeroMes = 4;
        else if (mes == "JUN")
            numeroMes = 5;
        else if (mes == "JUL")
            numeroMes = 6;
        else if (mes == "AGO")
            numeroMes = 7;
        else if (mes == "SET")
            numeroMes = 8;
        else if (mes == "OUT")
            numeroMes = 9;
        else if (mes == "NOV")
            numeroMes = 10;
        else if (mes == "DEZ")
            numeroMes = 11;
        else
        {
            throw std::invalid_argument("Mes invalido.");
        }

        std::tm data = {};
        data.tm_mday = dia;
        data.tm_mon = numeroMes;
        data.tm_year = ano - 1900;
        data.tm_hour = hora;
        data.tm_min = minuto;
        data.tm_isdst = -1;

        std::time_t resultado = std::mktime(&data);

        if (resultado == static_cast<std::time_t>(-1))
        {
            throw std::runtime_error("Nao foi possivel converter a data.");
        }

        return static_cast<long long>(resultado / 60);
    }

    double calcularTempoMedio(
        const Identificador &quadro,
        bool ciclo)
    {
        std::vector<CartaoAtividade> cartoes = repositorio.listar();

        long long soma = 0;
        int quantidade = 0;

        for (const CartaoAtividade &cartao : cartoes)
        {
            bool mesmoQuadro =
                cartao.getQuadro().getIdentificador() ==
                quadro.getIdentificador();

            bool concluido =
                cartao.getEstado().getEstado() == "FEITO";

            if (!mesmoQuadro || !concluido)
            {
                continue;
            }

            Timestamp inicio;

            if (ciclo)
            {
                inicio = cartao.getInicio();
            }
            else
            {
                inicio = cartao.getEntrada();
            }

            long long momentoInicial = converterParaMinutos(inicio);
            long long momentoFinal =
                converterParaMinutos(cartao.getTermino());

            if (momentoFinal < momentoInicial)
            {
                throw std::runtime_error(
                    "Data de conclusao anterior a data inicial.");
            }

            soma += momentoFinal - momentoInicial;
            quantidade++;
        }

        if (quantidade == 0)
        {
            return 0.0;
        }

        return static_cast<double>(soma) / quantidade;
    }

public:
    explicit ServicoCartoesAtividades(
        CartoesAtividades &cartoes,
        Quadros &quadros) : repositorio(cartoes), repositorioQuadros(quadros) {}

    void cadastrar(const CartaoAtividade &cartao) override
    {
        if (cartao.getEstado().getEstado() != "A FAZER")
        {
            throw std::invalid_argument(
                "Todo cartao deve ser cadastrado em A FAZER.");
        }

        repositorio.inserir(cartao);
    }

    CartaoAtividade consultar(
        const Identificador &identificador) override
    {
        return repositorio.consultar(identificador);
    }

    std::vector<CartaoAtividade> listar() override
    {
        return repositorio.listar();
    }

    void atualizar(const CartaoAtividade &cartao) override
    {
        CartaoAtividade atual =
            repositorio.consultar(cartao.getIdentificador());

        if (atual.getEstado().getEstado() !=
            cartao.getEstado().getEstado())
        {
            throw std::invalid_argument(
                "Utilize o metodo mover para alterar o estado.");
        }

        repositorio.atualizar(cartao);
    }
    void remover(const Identificador &identificador) override
    {
        repositorio.remover(identificador);
    }

    void mover(
        const Identificador &identificador,
        const Estado &novoEstado,
        const Timestamp &momento) override
    {
        CartaoAtividade cartao = repositorio.consultar(identificador);

        bool entrandoEmFazendo =
            cartao.getEstado().getEstado() == "A FAZER" &&
            novoEstado.getEstado() == "FAZENDO";

        if (entrandoEmFazendo)
        {
            validarLimiteWIP(
                cartao,
                identificador.getIdentificador());
        }

        repositorio.mover(identificador, novoEstado, momento);
    }

    int calcularWIP(const Identificador &quadro) override
    {
        std::vector<CartaoAtividade> cartoes = repositorio.listar();

        int quantidade = 0;

        for (const CartaoAtividade &cartao : cartoes)
        {
            bool mesmoQuadro =
                cartao.getQuadro().getIdentificador() ==
                quadro.getIdentificador();

            bool estaFazendo =
                cartao.getEstado().getEstado() == "FAZENDO";

            if (mesmoQuadro && estaFazendo)
            {
                quantidade++;
            }
        }

        return quantidade;
    }

    double calcularLeadTimeMedio(
        const Identificador &quadro) override
    {
        return calcularTempoMedio(quadro, false);
    }

    double calcularCycleTimeMedio(
        const Identificador &quadro) override
    {
        return calcularTempoMedio(quadro, true);
    }
};

#endif
