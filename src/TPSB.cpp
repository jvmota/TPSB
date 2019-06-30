//#include "pch.h"
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

// metodo para atualizar o registrador pep de acordo com o resultado
int atualizaPep(int registrador) {
  int pep;
  if (registrador == 0) {
    pep = 1;
  } else if (registrador < 0) {
    pep = 2;
  } else {
    pep = 0;
  }
  return pep;
}

int main(int argc, char **argv) {
  if (argc < 2) {
    cout << "erro: Voce deve passar o nome do arquivo de entrada e o de saida!"
         << endl;
    return 0;
  }
  string nomeEntrada = argv[1]; // nome do arquivo de entrada
  string entradaVerbose;        // le a entrada de verbose
  ifstream entrada;             // arquivo de entrada
  string instrucao;             // string com instrucao corrente
  int cod_inst = 1;             // codigo da instrução lida
  int tam_programa;             // tamanho do programa em posições de memória
  int end_carreg;               // endereço de carregamento do programa
  int ap;                       // registrador ap da pilha
  int entry_point;              // endereço de inicio do programa
  int PC;                       // program counter
  int pep = 1; // registrador de estado do processador: 0 = positivo, 1 = zero,
               // 2 = negativo
  int memoria[1000];                  // memoria da MV
  int registerFile[4] = {0, 0, 0, 0}; // Banco de registradores
  int r, m;                           // Operadores de instruções
  int result;                         // variavel local para comparacao
  int verbose;               // Determina se o emulador rodara em modo verbose
  entrada.open(nomeEntrada); // abre arquivo de entrada

  // checa o modo verbose
  if ((argc > 2)) {
    entradaVerbose = argv[2];
    if (entradaVerbose == "-v") {
      verbose = 1;
    }
  } else {
    verbose = 0;
  }

  // checa se ocorreu abertura do arquivo com sucesso
  if (entrada.is_open()) {
    // checa se o arquivo é válido de acordo com o cabecalho
    if (!entrada.eof()) {
      getline(entrada, instrucao);
      if (instrucao != "MV.EXE") {
        cout << "Erro! formato invalido" << endl;
        return 0;
      }
    } else {
      cout << "Erro ao ler o arquivo" << endl;
      return 0;
    }
    // lê os atributos do programa
    entrada >> tam_programa;
    entrada >> end_carreg;
    entrada >> ap;
    entrada >> entry_point;
    // começa a copia dos dados para memoria
    for (int i = 0; i <= tam_programa; i++) {
      entrada >> memoria[end_carreg + i];
    }
    // fecha o arquivo de entrada para evitar erros
    entrada.close();
    // Carrega o valor inicial do PC
    PC = entry_point;
    // Carrega a primeira instrução
    cod_inst = memoria[PC];
    // começa a execução do programa
    while (cod_inst != 0) {
      // carrega e executa a instrução
      // Parada
      if (cod_inst == 0) {
        cout << "Para do processador! Encerrando..." << endl;
        break;
      }
      // Load: Reg[R] = Mem[PC + M]
      else if (cod_inst == 1) {
        PC++;
        r = memoria[PC];
        PC++;
        m = memoria[PC];
        PC++;
        registerFile[r] = memoria[PC + m];
      }
      // Store: Mem[m + PC] = Reg[R]
      else if (cod_inst == 2) {
        PC++;
        r = memoria[PC];
        PC++;
        m = memoria[PC];
        PC++;
        memoria[PC + m] = registerFile[r];
      }
      // Read: Reg[r] = valor lido
      else if (cod_inst == 3) {
        PC++;
        r = memoria[PC];
        PC++;
        cin >> registerFile[r];
      }
      // Write: Imprime Reg[r]
      else if (cod_inst == 4) {
        PC++;
        r = memoria[PC];
        PC++;
        cout << registerFile[r] << endl;
      }
      // Copy: Reg[r] = Reg[m]
      else if (cod_inst == 5) {
        PC++;
        r = memoria[PC];
        PC++;
        m = memoria[PC];
        PC++;
        registerFile[r] = registerFile[m];
        pep = atualizaPep(registerFile[r]);
      }
      // Push: Ap--; Mem[AP} = Reg[R]
      else if (cod_inst == 6) {
        PC++;
        r = memoria[PC];
        PC++;
        ap--;
        memoria[ap] = registerFile[r];
      }
      // Pop: Reg[R] = Mem[AP]; AP++
      else if (cod_inst == 7) {
        PC++;
        r = memoria[PC];
        PC++;
        registerFile[r] = memoria[ap];
        ap++;
      }
      // Jump: PC = PC + M
      else if (cod_inst == 8) {
        PC++;
        m = memoria[PC];
        PC = PC + m;
      }
      // JZ: PC = PC + M se 0
      else if (cod_inst == 9) {
        PC++;
        r = memoria[PC];
        if (pep == 1) {
          PC = PC + r;
        } else {
          PC++;
        }
      }
      // JNZ: PC = PC + M se não 0
      else if (cod_inst == 10) {
        PC++;
        m = memoria[PC];
        if (pep != 1) {
          PC = PC + m;
        } else {
          PC++;
        }
      }
      // JN: PC = PC + M se negativo
      else if (cod_inst == 11) {
        PC++;
        m = memoria[PC];
        if (pep == 2) {
          PC = PC + m;
        } else {
          PC = PC++;
        }
      }
      // JNN: PC = PC + M se não negativo
      else if (cod_inst == 12) {
        PC++;
        m = memoria[PC];
        if (pep != 2) {
          PC = PC + m;
        } else {
          PC++;
        }
      }
      // CALL: AP--; Mem[AP] = PC; PC = PC + M
      else if (cod_inst == 13) {
        PC++;
        m = memoria[PC];
        ap--;
        memoria[ap] = PC + 1;
        PC = PC + m;
      }
      // RET: PC = Mem[AP]; AP++
      else if (cod_inst == 14) {
        PC = memoria[ap];
        ap++;
      }
      // AND: Reg[r] = Reg[r] and Reg[m]
      else if (cod_inst == 15) {
        PC++;
        r = memoria[PC];
        PC++;
        m = memoria[PC];
        PC++;
        registerFile[r] = registerFile[r] & registerFile[m];
        pep = atualizaPep(registerFile[r]);
      }
      // OR: Reg[r] = Reg[r] and Reg[m]
      else if (cod_inst == 16) {
        PC++;
        r = memoria[PC];
        PC++;
        m = memoria[PC];
        PC++;
        registerFile[r] = registerFile[r] | registerFile[m];
        pep = atualizaPep(registerFile[r]);
      }
      // NOT: Reg[r] = not Reg[r]
      else if (cod_inst == 17) {
        PC++;
        r = memoria[PC];
        PC++;
        registerFile[r] = ~registerFile[r];
        pep = atualizaPep(registerFile[r]);
      }
      // XOR: Reg[r] = Reg[r] xor Reg[m]
      else if (cod_inst == 18) {
        PC++;
        r = memoria[PC];
        PC++;
        m = memoria[PC];
        PC++;
        registerFile[r] = registerFile[r] ^ registerFile[m];
        pep = atualizaPep(registerFile[r]);
      }
      // ADD: Reg[r] = Reg[r] + Reg[m]
      else if (cod_inst == 19) {
        PC++;
        r = memoria[PC];
        PC++;
        m = memoria[PC];
        PC++;
        registerFile[r] = registerFile[r] + registerFile[m];
        pep = atualizaPep(registerFile[r]);
      }
      // SUB: Reg[r] = Reg[r] - Reg[m]
      else if (cod_inst == 20) {
        PC++;
        r = memoria[PC];
        PC++;
        m = memoria[PC];
        PC++;
        registerFile[r] = registerFile[r] - registerFile[m];
        pep = atualizaPep(registerFile[r]);
      }
      // MUL: Reg[r] = Reg[r] * Reg[m]
      else if (cod_inst == 21) {
        PC++;
        r = memoria[PC];
        PC++;
        m = memoria[PC];
        PC++;
        registerFile[r] = registerFile[r] * registerFile[m];
        pep = atualizaPep(registerFile[r]);
      }
      // DIV: Reg[r] = Reg[r] / Reg[m]
      else if (cod_inst == 22) {
        PC++;
        r = memoria[PC];
        PC++;
        m = memoria[PC];
        PC++;
        registerFile[r] = registerFile[r] / registerFile[m];
        pep = atualizaPep(registerFile[r]);
      }
      // MOD: Reg[r] = Reg[r] resto Reg[m]
      else if (cod_inst == 23) {
        PC++;
        r = memoria[PC];
        PC++;
        m = memoria[PC];
        PC++;
        registerFile[r] = registerFile[r] % registerFile[m];
        pep = atualizaPep(registerFile[r]);
      }
      // COMP: Reg[r] - Reg[m]
      else if (cod_inst == 24) {
        PC++;
        r = memoria[PC];
        PC++;
        m = memoria[PC];
        PC++;
        result = registerFile[r] - registerFile[m];
        pep = atualizaPep(result);
      }
      // TST: Reg[r] and Reg[m]
      else if (cod_inst == 25) {
        PC++;
        r = memoria[PC];
        PC++;
        m = memoria[PC];
        PC++;
        result = registerFile[r] & registerFile[m];
        pep = atualizaPep(result);
      }
      // codigo não encontrado
      else {
        cout << "Excecao: Codigo desconhecido! Encerrando..." << endl;
        return 0;
      }
      // modo verbose
      if (verbose != 0) {
        cout << cod_inst << " " << PC << " " << ap << " " << pep << " "
             << registerFile[0] << " " << registerFile[1] << " "
             << registerFile[2] << " " << registerFile[3] << endl;
      }
      // carega a proxima instrucao
      cod_inst = memoria[PC];
    }
  } else {
    cout << "Erro ao abrir o arquivo" << endl;
  }
  cout << "Fim da execução" << endl;
  return 0;
}