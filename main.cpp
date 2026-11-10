#include <iostream>
#include <stdexcept>

#include "dominio/nome.hpp"
#include "dominio/email.hpp"
#include "dominio/senha.hpp"
#include "dominio/papel.hpp"
#include "dominio/identificador.hpp"
#include "dominio/texto.hpp"
#include "dominio/timestamp.hpp"
#include "dominio/prioridade.hpp"
#include "dominio/Estado.hpp"
#include "entidades/Pessoa.hpp"
#include "entidades/Projeto.hpp"
#include "entidades/Quadro.hpp"
#include "entidades/CartaoAtividade.hpp"



#include <iostream>
#include "dominio/Email.hpp"

using namespace std;

int main() {

    Email email;

    try {
        email.setEmail("ana.silva_123gmail.com");

        cout << "Email valido: "
             << email.getEmail() << endl;

    } catch (const invalid_argument& erro) {
        cout << erro.what() << endl;
    }

    return 0;
}

