//Declaração da Bibliotecas Necessárias
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//-------------------------------------
//Estrutura Data
typedef struct{
    int dia;
    int mes;
    int ano;
}Data;
//-------------------------------------
//Estrutura Cliente
typedef struct{
    int codigoCliente;
    char nomeCliente[50];
    char endereco[50];
    char telefoneCliente[15];
}Cliente;
//-------------------------------------
//Estrutura Funcionario;
typedef struct{
    int codigoFuncionario;
    char nomeFuncionario[50];
    char telefoneFuncionario[15];
    char cargo[50];
    float salario;
}Funcionario;
//--------------------------------------
//Estrutura Quarto
typedef struct{
    int numeroQuarto;
    int quantidadeHospedes;
    float valorDiaria;
    char status[50];
}Quarto;
//--------------------------------------
//Estrutura Estadia
typedef struct{
    int codigoEstadia;
    Data dataEntrega;
    Data dataSaida;
    int quantidadeDiarias;
    Cliente codigoCliente;
    Quarto numeroQuarto;
}Estadia;
//---------------------------------------
//Declaração de Funções e Parâmetros
void cadastrarClientes(Cliente clientes[], int *numClientes);
void cadastrarFuncionarios(Funcionario funcionarios[], int *numFuncionarios);
void cadastrarQuartos(Quarto quartos[], int *numQuartos);
void cadastrarEstadia(Estadia estadias[], int *numEstadias, Cliente *cliente[], int numClientes, Quarto *quartos[], int numQuartos);
void baixaEstadia(Estadia estadias[], int *numEstadias);
void pesquisarClientes(Cliente clientes[], int *numClientes);
void pesquisarFuncionarios(Funcionario funcionarios[], int numFuncionarios);
void totalEstadias(Estadia estadias[], int numEstadias);
//----------------------------------------
//Função Cadastrar Clientes


