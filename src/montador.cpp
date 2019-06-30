//#include "pch.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

// funcao que calcula quanto o PC deve ser atualizado a cada instrucao
int countPC(string codInst) {
  int PC = 0;
  if (codInst == "HALT") {
    PC = 0;
  } else if (codInst == "LOAD") {
    PC = 3;
  } else if (codInst == "STORE") {
    PC = 3;
  } else if (codInst == "READ") {
    PC = 2;
  } else if (codInst == "WRITE") {
    PC = 2;
  } else if (codInst == "COPY") {
    PC = 3;
  } else if (codInst == "PUSH") {
    PC = 2;
  } else if (codInst == "POP") {
    PC = 2;
  } else if (codInst == "JUMP") {
    PC = 2;
  } else if (codInst == "JZ") {
    PC = 2;
  } else if (codInst == "JNZ") {
    PC = 2;
  } else if (codInst == "JN") {
    PC = 2;
  } else if (codInst == "JNN") {
    PC = 2;
  } else if (codInst == "CALL") {
    PC = 2;
  } else if (codInst == "RET") {
    PC = 1;
  } else if (codInst == "AND") {
    PC = 3;
  } else if (codInst == "OR") {
    PC = 3;
  } else if (codInst == "NOT") {
    PC = 2;
  } else if (codInst == "XOR") {
    PC = 3;
  } else if (codInst == "ADD") {
    PC = 3;
  } else if (codInst == "SUB") {
    PC = 3;
  } else if (codInst == "MUL") {
    PC = 3;
  } else if (codInst == "DIV") {
    PC = 3;
  } else if (codInst == "MOD") {
    PC = 3;
  } else if (codInst == "CMP") {
    PC = 3;
  } else if (codInst == "TST") {
    PC = 3;
  } else if (codInst == "WORD") {
    PC = 1;
  } else if (codInst == "END") {
    PC = 1;
  }
  return PC;
}

// funcao para decodificar registrador a partir do nome deste
int decodeReg(string registrador) {
  int reg;
  if (registrador == "R0") {
    return 0;
  } else if (registrador == "R1") {
    return 1;
  } else if (registrador == "R2") {
    return 2;
  } else if (registrador == "R3") {
    return 3;
  } else {
    cout << "erro: Registrador invalido!" << endl;
    return 0;
  }
}

int main(int argc, char **argv) {
  if (argc < 3) {
    cout << "Erro: argumentos invalidos! Passe o nome do arquivo de entrada e "
            "o de saida!"
         << endl;
    return 0;
  }
  string nomeEntrada = argv[1];
  string nomeSaida = argv[2];
  // definição de estrutura label, onde salva o pc da instrucao;
  struct label {
    int PC;      // PC da label
    string name; // nome da label
  };
  ifstream entrada;             // arquivo de entrada;
  ofstream saida;               // arquivo de saida
  string linha;                 // cada linha do arquivo de entrada
  istringstream instrucao("a"); // linha auxiliar para extracao de operadores
  string op = "teste";          // operadores da instrucao
  ostringstream programa; // string com os comandos a serem escritos no programa
  int PC = 0;             // contador de operador do programa
  label tabelaLabel[100]; // tabela de labels (assumindo que não irá passar de
                          // 100)
  int labelAtual = 0;     // inteiro para controlar a posicao dos labels
  string labelLido;

  // abre o arquivo de entrada
  entrada.open(nomeEntrada);

  // checa se o arquivo abriu
  if (!entrada.is_open()) {
    cout << "erro ao ler arquivo" << endl;
    entrada.close();
    return 0;
  }

  // primeira passagem para criação da tabela de simbolos

  while (!entrada.eof()) {
    // le a linha
    getline(entrada, linha);

    // se a linha estiver escrita:
    if (linha.length()) {
      // le o primeiro operador
      istringstream instrucao(linha);
      instrucao >> op;
      // checa se é label
      if (op[op.length() - 1] == ':') {
        // se sim, salva o PC da label e o nome da label
        tabelaLabel[labelAtual] = {PC + 1, op.substr(0, op.size() - 1)};
        labelAtual++;
        // pega a proxima instrucao
        instrucao >> op;
      }
      PC += countPC(op);
    }
  }

  // for (int i = 0; i < labelAtual; i++) {
  //	cout << tabelaLabel[i].name << " " << tabelaLabel[i].PC << endl;
  //}

  // fecha a entrada
  entrada.close();

  // abre a entrada novamente
  entrada.open(nomeEntrada);

  // abre o arquivo de saida;
  saida.open(nomeSaida);

  if (!saida.is_open()) {
    cout << "erro ao abrir o arquivo de saida" << endl;
    entrada.close();
    saida.close();
    return 0;
  }

  // grava o cabecalho na saida
  saida << "MV.EXE\n";

  // zera o contador
  PC = 0;

  // loop de leitura de instrucoes
  while (!entrada.eof()) {
    // le uma linha
    getline(entrada, linha);
    // se a linha não estiver em branco
    if (linha.length()) {
      // se a linha nao for um comentario
      if (linha[0] != ';') {
        // converte a linha pra sstream (facilitar a manipulacao de dados)
        istringstream instrucao(linha);
        // le o primeiro operador da linha
        instrucao >> op;
        // checa se e label
        if (op[op.length() - 1] == ':') {
          // pega a proxima instrucao
          instrucao >> op;
        }

        // atualiza o pc
        PC += countPC(op);

        // decodifica a instrucao
        if (op == "HALT") {
          programa << 0 << " ";
        } else if (op == "LOAD") {
          programa << 1 << " ";
          instrucao >> op;
          programa << decodeReg(op) << " ";
          instrucao >> labelLido;
          for (int i = 0; i < labelAtual; i++) {
            if (labelLido == tabelaLabel[i].name) {
              programa << (tabelaLabel[i].PC - PC) << " ";
              break;
            }
          }
        } else if (op == "STORE") {
          programa << 2 << " ";
          instrucao >> op;
          programa << decodeReg(op) << " ";
          instrucao >> labelLido;
          for (int i = 0; i < labelAtual; i++) {
            if (labelLido == tabelaLabel[i].name) {
              programa << (tabelaLabel[i].PC - PC) << " ";
              break;
            }
          }
        } else if (op == "READ") {
          programa << 3 << " ";
          instrucao >> op;
          programa << decodeReg(op) << " ";
        } else if (op == "WRITE") {
          programa << 4 << " ";
          instrucao >> op;
          programa << decodeReg(op) << " ";
        } else if (op == "COPY") {
          programa << 5 << " ";
          instrucao >> op;
          programa << decodeReg(op) << " ";
          instrucao >> op;
          programa << decodeReg(op) << " ";
        } else if (op == "PUSH") {
          programa << 6 << " ";
          instrucao >> op;
          programa << decodeReg(op) << " ";
        } else if (op == "POP") {
          programa << 7 << " ";
          instrucao >> op;
          programa << decodeReg(op) << " ";
        }
        // instrucoes com label
        else if ((op == "JUMP") || (op == "JZ") || (op == "JNZ") ||
                 (op == "JN") || (op == "JNN") || (op == "CALL")) {
          if (op == "JUMP") {
            programa << 8 << " ";
          } else if (op == "JZ") {
            programa << 9 << " ";
          } else if (op == "JNZ") {
            programa << 10 << " ";
          } else if (op == "JN") {
            programa << 11 << " ";
          } else if (op == "JNN") {
            programa << 12 << " ";
          } else if (op == "CALL") {
            programa << 13 << " ";
          }
          instrucao >> labelLido;
          for (int i = 0; i < labelAtual; i++) {
            if (labelLido == tabelaLabel[i].name) {
              programa << (tabelaLabel[i].PC - PC) << " ";
              break;
            }
          }
        } else if (op == "RET") {
          programa << 14 << " ";
        } else if (op == "AND") {
          programa << 15 << " ";
          instrucao >> op;
          programa << decodeReg(op) << " ";
          instrucao >> op;
          programa << decodeReg(op) << " ";
        } else if (op == "OR") {
          programa << 16 << " ";
          instrucao >> op;
          programa << decodeReg(op) << " ";
          instrucao >> op;
          programa << decodeReg(op) << " ";
        } else if (op == "NOT") {
          programa << 17 << " ";
          instrucao >> op;
          programa << decodeReg(op) << " ";
        } else if (op == "XOR") {
          programa << 18 << " ";
          instrucao >> op;
          programa << decodeReg(op) << " ";
          instrucao >> op;
          programa << decodeReg(op) << " ";
        } else if (op == "ADD") {
          programa << 19 << " ";
          instrucao >> op;
          programa << decodeReg(op) << " ";
          instrucao >> op;
          programa << decodeReg(op) << " ";
        } else if (op == "SUB") {
          programa << 20 << " ";
          instrucao >> op;
          programa << decodeReg(op) << " ";
          instrucao >> op;
          programa << decodeReg(op) << " ";
        } else if (op == "MUL") {
          programa << 21 << " ";
          instrucao >> op;
          programa << decodeReg(op) << " ";
          instrucao >> op;
          programa << decodeReg(op) << " ";
        } else if (op == "DIV") {
          programa << 22 << " ";
          instrucao >> op;
          programa << decodeReg(op) << " ";
          instrucao >> op;
          programa << decodeReg(op) << " ";
        } else if (op == "MOD") {
          programa << 23 << " ";
          instrucao >> op;
          programa << decodeReg(op) << " ";
          instrucao >> op;
          programa << decodeReg(op) << " ";
        } else if (op == "CMP") {
          programa << 24 << " ";
          instrucao >> op;
          programa << decodeReg(op) << " ";
          instrucao >> op;
          programa << decodeReg(op) << " ";
        } else if (op == "TST") {
          programa << 25 << " ";
          instrucao >> op;
          programa << decodeReg(op) << " ";
          instrucao >> op;
          programa << decodeReg(op) << " ";
        } else if (op == "WORD") {
          instrucao >> op;
          programa << op << " ";
        } else if (op == "END") {
          programa << " ";
          break;
        } else {
          cout << "erro! operador não encontrado" << endl;
          entrada.close();
          saida.close();
          return 0;
        }
      }
    }
  }
  // escreve os dados do programa na saida
  saida << PC << " " << 0 << " " << 999 << " " << 0 << endl;

  // escreve o programa na saida
  saida << programa.str() << endl;

  // fecha os arquivos de entrada e saida
  entrada.close();
  saida.close();
  // encerra o programa;
  return 0;
}
