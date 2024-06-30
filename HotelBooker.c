/*******************************************************************************************************************************
* FILENAME : HotelBooker.c
* DESCRIPTION : Hotel management system for handling client, room, and stay records, including functions for registration, 
*               search, and management of data.
* PUBLIC FUNCTIONS :
*          void cadastrarClientes(Cliente clientes[], int *numClientes)
*          void cadastrarFuncionarios(Funcionario funcionarios[], int *numFuncionarios)
*          void cadastrarQuartos(Quarto quartos[], int *numQuartos)
*          void cadastrarEstadia(Estadia estadias[], int *numEstadias, Cliente clientes[], int numClientes, Quarto quartos[], int numQuartos)
*          void baixaEstadia(Estadia estadias[], int *numEstadias, Quarto quartos[], int *numQuartos)
*          void pesquisarClientes(Cliente clientes[], int *numClientes)
*          void pesquisarFuncionarios(Funcionario funcionarios[], int *numFuncionarios)
*          void totalEstadias(Estadia estadias[], int *numEstadias, Cliente clientes[], int *numClientes)
* NOTES :
*          This program manages hotel operations through file handling and interactive user options.
*
* AUTHOR : Isabella Dias
* AUTHOR : Gustavo Viana
* START DATE : 18 Jun 24
*******************************************************************************************************************************/

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
    char nomeCliente[200];
    char endereco[200];
    char telefoneCliente[100];
}Cliente;
//-------------------------------------

//Estrutura Funcionario;
typedef struct{
    int codigoFuncionario;
    char nomeFuncionario[200];
    char telefoneFuncionario[100];
    char cargo[200];
    float salario;
}Funcionario;
//--------------------------------------

//Estrutura Quarto
typedef struct{
    int numeroQuarto;
    int quantidadeHospedes;
    float valorDiaria;
    char status[200];
}Quarto;
//--------------------------------------

//Estrutura Estadia
typedef struct{
    int codigoEstadia;
    Data dataEntrada;
    Data dataSaida;
    int quantidadeDiarias;
    Cliente codigoCliente;
    Quarto numeroQuarto;
}Estadia;
//---------------------------------------

//Declaração de Funções e Parâmetros
void cadastrarClientes(Cliente clientes[], int *numClientes);
void carregarClientes(Cliente clientes[], int *numClientes);
void cadastrarFuncionarios(Funcionario funcionarios[], int *numFuncionarios);
void carregarFuncionarios(Funcionario funcionarios[], int *numFuncionarios);
void cadastrarQuartos(Quarto quartos[], int *numQuartos);
void carregarQuartos(Quarto quartos[], int *numQuartos);
void cadastrarEstadia(Estadia estadias[], int *numEstadias, Cliente cliente[], int numClientes, Quarto quartos[], int numQuartos);
void carregarEstadias(Estadia estadias[], int *numEstadias, Cliente cliente[], int numClientes, Quarto quartos[], int numQuartos);
void baixaEstadia(Estadia estadias[], int *numEstadias, Quarto quartos[], int *numQuartos);
void pesquisarClientes(Cliente clientes[], int *numClientes);
void pesquisarFuncionarios(Funcionario funcionarios[], int *numFuncionarios);
void totalEstadias(Estadia estadias[], int *numEstadias, Cliente clientes[], int *numClientes);
//----------------------------------------

/*Cadastra Novos Clientes, Armazenando os Dados em Memória e em um Arquivo "Clientes.txt"*/
//Função Cadastrar Clientes
void cadastrarClientes(Cliente clientes[], int *numClientes) {
    Cliente novoCliente;
    int codigoExistente = 0;

    //Entrada do Código do Novo Cliente
    printf("\nDigite o Codigo do Cliente:\n");
    scanf(" %d", &novoCliente.codigoCliente);

    if (novoCliente.codigoCliente < 0) {
        printf("Erro! Codigo Invalido!\n");
        return;
    }

    //Garantir que o Código Não Está em Uso
    for (int i = 0; i < *numClientes; i++) {
        if (clientes[i].codigoCliente == novoCliente.codigoCliente) {
            codigoExistente = 1;
            break;
        }
    }

    if (codigoExistente) {
        printf("Erro! Codigo em Uso!\n");
        return;
    }

    printf("Digite o Nome do Cliente: ");
    scanf(" %[^\n]", novoCliente.nomeCliente);
    printf("Digite o Endereco do Cliente: ");
    scanf(" %[^\n]", novoCliente.endereco);
    printf("Digite o Telefone do Cliente: ");
    scanf(" %[^\n]", novoCliente.telefoneCliente);

    //Atualizar Clientes Cadastrados
    clientes[*numClientes] = novoCliente;
    (*numClientes)++;

    //Armazenamento no Arquivo Clientes.txt
    FILE *file = fopen("Clientes.txt", "a");
    if (file == NULL) {
        printf("\nErro ao Abrir o Arquivo\n");
        return;
    }

    fprintf(file, "%d,", novoCliente.codigoCliente);
    fprintf(file, "%s,", novoCliente.nomeCliente);
    fprintf(file, "%s,", novoCliente.endereco);
    fprintf(file, "%s\n", novoCliente.telefoneCliente);
    fclose(file);

    printf("Cliente Cadastrado!\n");
}
//----------------------------------------

/*Carrega Dados dos Clientes a Partir do Arquivo "Clientes.txt", Incrementa um Array de Estruturas de Clientes
e Atualiza o Contador de Clientes.*/
//Carregar Arquivo Clientes
void carregarClientes(Cliente clientes[], int *numClientes) {

    FILE *file = fopen("Clientes.txt", "r");

    if (file == NULL) {
        return;
    }

    //Variáveis Temporárias
    char linha[800];
    int codigo;
    char nome[200];
    char endereco[200];
    char telefone[100];

    // Lê o Arquivo Linha por Linha
    while (fscanf(file, "%d,%[^,],%[^,],%[^\n]\n", &codigo, nome, endereco, telefone) != EOF) {

        clientes[*numClientes].codigoCliente = codigo;
        strcpy(clientes[*numClientes].nomeCliente, nome);
        strcpy(clientes[*numClientes].endereco, endereco);
        strcpy(clientes[*numClientes].telefoneCliente, telefone);

        //Contador de Clientes
        (*numClientes)++;

    }

    //Fecha o Arquivo
    fclose(file);
}
//----------------------------------------

/*Cadastra Novos Funcionários, Armazenando os Dados em Memória e em um Arquivo "Funcionarios.txt"*/
//Função Cadastrar Funcionários
void cadastrarFuncionarios(Funcionario funcionarios[], int *numFuncionarios) {
    Funcionario novoFuncionario;
    int codigoExistente = 0;

    //Entrada do Código do Novo Funcionário
    printf("\nDigite o Codigo do Funcionario:\n");
    scanf(" %d", &novoFuncionario.codigoFuncionario);

    if (novoFuncionario.codigoFuncionario < 0) {
        printf("Erro! Codigo Invalido!\n");
        return;
    }

    //Garantir que o Código Não Está em Uso
    for (int i = 0; i < *numFuncionarios; i++) {
        if (funcionarios[i].codigoFuncionario == novoFuncionario.codigoFuncionario) {
            codigoExistente = 1;
            break;
        }
    }

    if (codigoExistente) {
        printf("Erro! Codigo em Uso!\n");
        return;
    }

    printf("Digite o Nome do Funcionario: ");
    scanf(" %[^\n]", novoFuncionario.nomeFuncionario);
    printf("Digite o Telefone do Funcionario: ");
    scanf(" %[^\n]", novoFuncionario.telefoneFuncionario);
    printf("Digite o Cargo do Funcionario: ");
    scanf(" %[^\n]", novoFuncionario.cargo);
    printf("Digite o Salario do Funcionario: ");
    scanf(" %f", &novoFuncionario.salario);
    
    //Atualizar Funcionários Cadastrados
    funcionarios[*numFuncionarios] = novoFuncionario;
    (*numFuncionarios)++;

    //Armazenamento no Arquivo Funcionarios.txt
    FILE *file = fopen("Funcionarios.txt", "a");
    if (file == NULL) {
        printf("\nErro ao Abrir o Arquivo\n");
        return;
    }

    fprintf(file, "%d,", novoFuncionario.codigoFuncionario);
    fprintf(file, "%s,", novoFuncionario.nomeFuncionario);
    fprintf(file, "%s,", novoFuncionario.telefoneFuncionario);
    fprintf(file, "%s,", novoFuncionario.cargo);
    fprintf(file, "%.2f\n", novoFuncionario.salario);
    fclose(file);

    printf("Funcionario Cadastrado!\n");
}
//----------------------------------------

/*Carrega Dados dos Funcionários a Partir do Arquivo "Funcionarios.txt", Incrementa um Array de Estruturas de Funcionários
e Atualiza o Contador de Funcionários.*/
//Carregar Arquivo Funcionários
void carregarFuncionarios(Funcionario funcionarios[], int *numFuncionarios) {

    FILE *file = fopen("Funcionarios.txt", "r");

    if (file == NULL) {
        return;
    }

    //Variáveis Temporárias
    int codigo;
    char nome[200];
    char telefone[100];
    char cargo[100];
    float salario;

    // Lê o Arquivo Linha por Linha
    while (fscanf(file, "%d,%[^,],%[^,],%[^,],%f\n", &codigo, nome, telefone, cargo, &salario) != EOF) {

        funcionarios[*numFuncionarios].codigoFuncionario = codigo;
        strcpy(funcionarios[*numFuncionarios].nomeFuncionario, nome);
        strcpy(funcionarios[*numFuncionarios].telefoneFuncionario, telefone);
        strcpy(funcionarios[*numFuncionarios].cargo, cargo);
        funcionarios[*numFuncionarios].salario = salario;

        //Contador de Funcionários
        (*numFuncionarios)++;
    }

    //Fecha o Arquivo
    fclose(file);
}
//----------------------------------------

/*Cadastra Novos Quartos, Armazenando os Dados em Memória e em um Arquivo "Quartos.txt"*/
//Cadastrar Quartos
void cadastrarQuartos(Quarto quartos[], int *numQuartos){
    Quarto novoQuarto;

    //Entrada do Número do Novo Quarto
    printf("\nDigite o Numero do Quarto:\n");
    scanf(" %d", &novoQuarto.numeroQuarto);

    if (novoQuarto.numeroQuarto < 0) {
        printf("Erro! Numero Invalido!\n");
        return;
    }

    //Garantir que o Número do Quarto Não Está em Uso
    for(int i = 0; i < *numQuartos; i++){
        if(quartos[i].numeroQuarto == novoQuarto.numeroQuarto){
            printf("\nErro! Quarto em Uso! Cadastro Nao Realizado!\n");
            return;
        }
    }

    printf("\nDigite a Quantidade de Hospedes: ");
    scanf(" %d", &novoQuarto.quantidadeHospedes);
    printf("\nDigite oValor da Diaria: ");
    scanf(" %f", &novoQuarto.valorDiaria);
    printf("\nDigite o Status do Quarto: (Ocupado/Desocupado): ");
    scanf(" %[^\n]", novoQuarto.status);

    //Atualizar Quartos Cadastrados
    quartos[*numQuartos] = novoQuarto;
    (*numQuartos)++;

    //Armazenamento no Arquivo Quartos.txt
    FILE *file = fopen("Quartos.txt", "a");
    if (file == NULL) {
        printf("\nErro ao Abrir o Arquivo\n");
        return;
    }
    fprintf(file, "%d,", novoQuarto.numeroQuarto);
    fprintf(file, "%d,", novoQuarto.quantidadeHospedes);
    fprintf(file, "%.2f,", novoQuarto.valorDiaria);
    fprintf(file, "%s\n", novoQuarto.status);
    fclose(file);

    printf("Quarto Cadastrado!\n");
}
//----------------------------------------

/*Carrega Dados dos Quartos a Partir do Arquivo "Quartos.txt", Incrementa um Array de Estruturas de Quartos
e Atualiza o Contador de Quartos.*/
//Carregar Arquivo Quartos
void carregarQuartos(Quarto quartos[], int *numQuartos) {

    FILE *file = fopen("Quartos.txt", "r");

    if (file == NULL) {
        return;
    }

    //Variáveis Temporárias
    int numero;
    int hospedes;
    float diaria;
    char status[200];

    // Lê o Arquivo Linha por Linha
    while (fscanf(file, "%d,%d,%f,%s[^\n]\n", &numero, &hospedes, &diaria, status) != EOF) {

        quartos[*numQuartos].numeroQuarto = numero;
        quartos[*numQuartos].quantidadeHospedes = hospedes;
        quartos[*numQuartos].valorDiaria = diaria;
        strcpy(quartos[*numQuartos].status, status);

        //Contador de Quartos
        (*numQuartos)++;
    }

    //Fecha o Arquivo
    fclose(file);
}
//----------------------------------------

/*Cadastra uma Nova Estadia Associando um Cliente a um Quarto, Registrando Datas de Entrada e Saída, Calculando a Quantidade 
de Diárias e Atualizando Arquivos de Estadias e Quartos.*/
//Cadastrar Estadia
void cadastrarEstadia(Estadia estadias[], int *numEstadias, Cliente clientes[], int numClientes, Quarto quartos[], int numQuartos){
    Estadia novaEstadia;
    int codigoCliente, numeroQuarto;
    int clienteEncontrado = 0, quartoEncontrado = 0;

    //Entrada do Código do Cliente
    printf("\nDigite o Codigo do Cliente:\n");
    scanf("%d", &codigoCliente);

    //Garantir que o Cliente Existe
    for(int i = 0; i < numClientes; i++){
        if(clientes[i].codigoCliente == codigoCliente){
            novaEstadia.codigoCliente = clientes[i];
            clienteEncontrado = 1;
            break;
        }
    }

    if(!clienteEncontrado){
        printf("\nErro! Cliente Nao Encontrado!\n");
        return;
    }

    printf("\nDigite o Numero do Quarto: ");
    scanf(" %d", &numeroQuarto);
    
    //Garantir que o Quarto Existe e que seu Status Está Como Desocupado
    for(int i = 0; i < numQuartos; i++){
        if(quartos[i].numeroQuarto == numeroQuarto && strcmp(quartos[i].status, "Desocupado") == 0){
            novaEstadia.numeroQuarto = quartos[i];
            quartoEncontrado = 1;
            strcpy(quartos[i].status, "Ocupado");
            break;
        }
    }

    if(!quartoEncontrado){
        printf("\nErro! Quarto Nao Encontrado ou seu Status Esta como Ocupado!\n");
        return;
    }

    printf("\nDigite a Data de Entrada (dd mm aaaa): \n");
    scanf("%d %d %d", &novaEstadia.dataEntrada.dia, &novaEstadia.dataEntrada.mes, &novaEstadia.dataEntrada.ano);

    printf("\nDigite a Data de Saida (dd mm aaaa): \n");
    scanf("%d %d %d", &novaEstadia.dataSaida.dia, &novaEstadia.dataSaida.mes, &novaEstadia.dataSaida.ano);

    //Cálculo da Quantidade de Diárias
    novaEstadia.quantidadeDiarias = (novaEstadia.dataSaida.dia - novaEstadia.dataEntrada.dia) + (novaEstadia.dataSaida.mes - novaEstadia.dataEntrada.mes) * 30 + (novaEstadia.dataSaida.ano - novaEstadia.dataEntrada.ano) * 365;

    //Atualizar Estadias Cadastradas
    novaEstadia.codigoEstadia = *numEstadias;
    estadias[*numEstadias] = novaEstadia;
    (*numEstadias)++;
    novaEstadia.codigoEstadia = *numEstadias;

    //Armazenamento no Arquivo Estadia.txt
    FILE *file = fopen("Estadia.txt", "a");
    if (file == NULL) {
        printf("\nErro ao Abrir o Arquivo\n");
        return;
    }
    fprintf(file, " %d,", novaEstadia.codigoEstadia);
    fprintf(file, " %d,", novaEstadia.codigoCliente.codigoCliente);
    fprintf(file, " %d,", novaEstadia.numeroQuarto.numeroQuarto);
    fprintf(file, " %d,", novaEstadia.dataEntrada.dia);
    fprintf(file, " %d,", novaEstadia.dataEntrada.mes);
    fprintf(file, " %d,", novaEstadia.dataEntrada.ano);
    fprintf(file, " %d,", novaEstadia.dataSaida.dia);
    fprintf(file, " %d,", novaEstadia.dataSaida.mes);
    fprintf(file, " %d\n", novaEstadia.dataSaida.ano);
    fclose(file);

    //Atualizar o Status do Quarto no Arquivo Quartos.txt
    file = fopen("Quartos.txt", "w");
    if (file == NULL) {
        printf("\nErro ao Abrir o Arquivo\n");
        return;
    }
    for (int i = 0; i < numQuartos; i++) {
        fprintf(file, "%d,%d,%.2f,%s\n",
                quartos[i].numeroQuarto,
                quartos[i].quantidadeHospedes,
                quartos[i].valorDiaria,
                quartos[i].status);
    }
    fclose(file);

    printf("Estadia Cadastrada!\n");
}
//----------------------------------------

/*Carrega Informações de Estadias Registradas a Partir de um Arquivo "Estadia.txt", Associando Clientes e Quartos às Estadias, 
Calculando a Quantidade de Diárias com Base nas Datas de Entrada e Saída.*/
//Carregar Arquivo Estadia
void carregarEstadias(Estadia estadias[], int *numEstadias, Cliente clientes[], int numClientes, Quarto quartos[], int numQuartos) {

    FILE *file = fopen("Estadia.txt", "r");

    if (file == NULL) {
        return;
    }

    //Variáveis Temporárias
    int codigoEstadia, codigoCliente, numeroQuarto;
    int diaEntrada, mesEntrada, anoEntrada;
    int diaSaida, mesSaida, anoSaida;
    int clienteIndex, quartoIndex;

    // Lê o Arquivo Linha por Linha
    while (fscanf(file, "%d,%d,%d,%d,%d,%d,%d,%d,%d\n", &codigoEstadia, &codigoCliente, &numeroQuarto, &diaEntrada, &mesEntrada, &anoEntrada, &diaSaida, &mesSaida, &anoSaida) != EOF) {

        clienteIndex = -1;
        quartoIndex = -1;

        //Encontrar Cliente pelo Código
        for(int i = 0; i < numClientes; i++){
            if(clientes[i].codigoCliente == codigoCliente){
                clienteIndex = i;
                break;
            }
        }

        //Encontrar Quarto pelo Número
        for(int i = 0; i < numeroQuarto; i++){
            if(quartos[i].numeroQuarto == numeroQuarto){
                quartoIndex = i;
                break;
            }
        }

        //Garante que o Quarto e o Cliente Foram Encontrados
        if(clienteIndex == -1 || quartoIndex == -1){
            continue;
        }
        
        estadias[*numEstadias].codigoEstadia = codigoEstadia;
        estadias[*numEstadias].codigoCliente = clientes[clienteIndex];
        estadias[*numEstadias].numeroQuarto = quartos[quartoIndex];
        estadias[*numEstadias].dataEntrada.dia = diaEntrada;
        estadias[*numEstadias].dataEntrada.mes = mesEntrada;
        estadias[*numEstadias].dataEntrada.ano = anoEntrada;
        estadias[*numEstadias].dataSaida.dia = diaSaida;
        estadias[*numEstadias].dataSaida.mes = mesSaida;
        estadias[*numEstadias].dataSaida.ano = anoSaida;

        //Cálculo da Quantidade de Diárias
        estadias[*numEstadias].quantidadeDiarias = (diaSaida - diaEntrada) + (mesSaida - mesEntrada) * 30 + (anoSaida - anoEntrada) * 365;
    
        //Contador de Estadias
        (*numEstadias)++;
    }

    //Fecha o Arquivo
    fclose(file);

}
//----------------------------------------

//Teste: Printar Estadia
void printarEstadia(Estadia estadias[], int *numEstadias){
    for(int i = 0; i < *numEstadias; i++){
        printf("Estadia ID: %d\n", estadias[i].codigoEstadia);
    }
}
//----------------------------------------

/*Realiza o Encerramento de uma Estadia, Atualiza o Status do Quarto para "Desocupado", Calcula o Valor Total da Estadia e Remove
a Estadia do Registro.*/
//Baixa em Alguma Estadia
void baixaEstadia(Estadia estadias[], int *numEstadias, Quarto quartos[], int *numQuartos) {
    int codigoEstadia = 0;

    printf("\nDigite o Codigo da Estadia: \n");
    scanf(" %d", &codigoEstadia);

    //Procurando Estadia pelo Código
    int estadiaIndex = -1;
    for (int i = 0; i < *numEstadias; i++) {
        if (estadias[i].codigoEstadia == codigoEstadia) {
            estadiaIndex = i;
            break;
        }
    }
    if (estadiaIndex == -1) {
        printf("\nEstadia Nao Encontrada!\n");
        return;
    }

    //Imprimir as Informações da Estadia
    Estadia *estadia = &estadias[estadiaIndex];
    printf("\nEstadia Encontrada:\n");
    printf("Codigo Estadia: %d\n", estadia->codigoEstadia);
    printf("Codigo Cliente: %d\n", estadia->codigoCliente.codigoCliente);
    printf("Numero Quarto: %d\n", estadia->numeroQuarto.numeroQuarto);
    printf("Data Entrada: %d/%d/%d\n", estadia->dataEntrada.dia, estadia->dataEntrada.mes, estadia->dataEntrada.ano);
    printf("Data Saida: %d/%d/%d\n", estadia->dataSaida.dia, estadia->dataSaida.mes, estadia->dataSaida.ano);
    printf("Quantidade Diarias: %d\n", estadia->quantidadeDiarias);

    //Calculo do Valor Total da Estadia do Cliente
    float valorTotal = estadia->quantidadeDiarias * estadia->numeroQuarto.valorDiaria;
    printf("\nValor Total da Estadia: R$ %.2f\n", valorTotal);

    //Atualizar o Status do Quarto para "Desocupado"
    int quartoIndex = -1;
    for (int i = 0; i < *numQuartos; i++) {
        if (quartos[i].numeroQuarto == estadia->numeroQuarto.numeroQuarto) {
            quartoIndex = i;
            break;
        }
    }

    if (quartoIndex != -1) {
        strcpy(quartos[quartoIndex].status, "Desocupado");
    }

    //printarEstadia(estadias, numEstadias); (Teste Antes da Remoção)
    //Remover a Estadia do Array
    for (int i = estadiaIndex; i < *numEstadias; i++) {
        estadias[i] = estadias[i + 1];
    }
    (*numEstadias)--;
    //printarEstadia(estadias, numEstadias); (Teste Após a Remoção)


    //Atualizar o Arquivo Estadia.txt
    FILE *file = fopen("Estadia.txt", "w");
    if (file == NULL) {
        printf("\nErro ao Abrir o Arquivo\n");
        return;
    }

    for (int i = 0; i < *numEstadias; i++) {
        fprintf(file, "%d,%d,%d,%d,%d,%d,%d,%d,%d\n",
                estadias[i].codigoEstadia,
                estadias[i].codigoCliente.codigoCliente,
                estadias[i].numeroQuarto.numeroQuarto,
                estadias[i].dataEntrada.dia,
                estadias[i].dataEntrada.mes,
                estadias[i].dataEntrada.ano,
                estadias[i].dataSaida.dia,
                estadias[i].dataSaida.mes,
                estadias[i].dataSaida.ano);
    }
    fclose(file);

    //Atualizar o Arquivo Quartos.txt
    file = fopen("Quartos.txt", "w");
    if (file == NULL) {
        printf("\nErro ao Abrir o Arquivo\n");
        return;
    }

    for (int i = 0; i < *numQuartos; i++) {
        fprintf(file, "%d,%d,%.2f,%s\n",
                quartos[i].numeroQuarto,
                quartos[i].quantidadeHospedes,
                quartos[i].valorDiaria,
                quartos[i].status);
    }
    fclose(file);

    printf("\nQuarto Desocupado!\n");
}
//----------------------------------------

/*Pesquisa Clientes Cadastrados pelo Código ou Nome, Exibindo Suas Informações se Encontrados.*/
//Pesquisar Clientes
void pesquisarClientes(Cliente clientes[], int *numClientes) {
    int opcao;
    printf("Deseja Pesquisar Cliente:\n");
    printf("\n1 - Por Codigo");
    printf("\n2 - Por Nome\nOp: ");
    scanf("%d", &opcao);

    if (opcao == 1) {
        int codigo;
        printf("Digite o Codigo do Cliente que Deseja Pesquisar:\n");
        scanf(" %d", &codigo);

        for (int i = 0; i < *numClientes; i++) {
            if (clientes[i].codigoCliente == codigo) {
                printf("Cliente Encontrado:\n");
                printf("Codigo: %d\n", clientes[i].codigoCliente);
                printf("Nome: %s\n", clientes[i].nomeCliente);
                printf("Endereco: %s\n", clientes[i].endereco);
                printf("Telefone: %s\n", clientes[i].telefoneCliente);
                return;
            }
        }
        printf("Cliente Nao Encontrado!\n");
    } else if (opcao == 2) {
        char nome[200];
        printf("Digite o Nome do Cliente a ser Pesquisado: ");
        scanf(" %[^\n]", nome);

        for (int i = 0; i < *numClientes; i++) {
            if (strcmp(clientes[i].nomeCliente, nome) == 0) {
                printf("Cliente Encontrado:\n");
                printf("Codigo: %d\n", clientes[i].codigoCliente);
                printf("Nome: %s\n", clientes[i].nomeCliente);
                printf("Endereco: %s\n", clientes[i].endereco);
                printf("Telefone: %s\n", clientes[i].telefoneCliente);
                return;
            }
        }
        printf("Cliente Nao Encontrado.\n");
    } else {
        printf("Opcao Invalida!\n");
    }
}
//----------------------------------------

/*Pesquisa Funcionários Cadastrados pelo Código ou Nome, Exibindo Suas Informações se Encontrados.*/
//Pesquisar Funcionários
void pesquisarFuncionarios(Funcionario funcionarios[], int *numFuncionarios) {
    int opcao;
    printf("Deseja Pesquisar Funcionario:\n");
    printf("\n1 - Por Codigo");
    printf("\n2 - Por Nome\nOp: ");
    scanf(" %d", &opcao);

    if (opcao == 1) {
        int codigo;
        printf("Digite o Codigo do Funcionario que Deseja Pesquisar:\n");
        scanf(" %d", &codigo);

        for (int i = 0; i < *numFuncionarios; i++) {
            if (funcionarios[i].codigoFuncionario == codigo) {
                printf("Funcionario Encontrado:\n");
                printf("Codigo: %d\n", funcionarios[i].codigoFuncionario);
                printf("Nome: %s\n", funcionarios[i].nomeFuncionario);
                printf("Telefone: %s\n", funcionarios[i].telefoneFuncionario);
                printf("Cargo: %s\n", funcionarios[i].cargo);
                printf("Salario: %.2f\n", funcionarios[i].salario);
                return;
            }
        }
        printf("Funcionario Nao Encontrado!\n");
    } else if (opcao == 2) {
        char nome[200];
        printf("Digite o Nome do Funcionario a ser Pesquisado: ");
        scanf(" %[^\n]", nome);

        for (int i = 0; i < *numFuncionarios; i++) {
            if (strcmp(funcionarios[i].nomeFuncionario, nome) == 0) {
                printf("Funcionario Encontrado:\n");
                printf("Codigo: %d\n", funcionarios[i].codigoFuncionario);
                printf("Nome: %s\n", funcionarios[i].nomeFuncionario);
                printf("Telefone: %s\n", funcionarios[i].telefoneFuncionario);
                printf("Cargo: %s\n", funcionarios[i].cargo);
                printf("Salario: %.2f\n", funcionarios[i].salario);
                return;
            }
        }
        printf("Funcionario Nao Encontrado.\n");
    } else {
        printf("Opcao Invalida!\n");
    }
}
//----------------------------------------

/*Calcula e Exibi Todas as Estadias Registradas para um Cliente Específico, Identificado pelo seu Código.*/
//Total de Estadias de um Cliente
void totalEstadias(Estadia estadias[], int *numEstadias, Cliente clientes[], int *numClientes){
    int codigoCliente = 0;

    printf("\nDigite o Codigo do Cliente: \n");
    scanf(" %d", &codigoCliente);

    //Procurando Cliente pelo Código
    int clienteIndex = -1;
    for(int i = 0; i < *numClientes; i++){
        if(clientes[i].codigoCliente == codigoCliente){
            clienteIndex = i;
            break;
        }
    }
    if(clienteIndex == -1){
        printf("\nCliente Nao Encontrado!\n");
        return;
    }

    printf("\nEstadias do Cliente %s: \n", clientes[clienteIndex].nomeCliente);
    int estadiasEncontradas = 0;
    for(int i = 0; i < *numEstadias; i++){
        if(estadias[i].codigoCliente.codigoCliente == codigoCliente){
            printf("\nCodigo da Estadia: %d\n", estadias[i].codigoEstadia);
            printf("\nData da Entrada: %02d/%02d/%04d\n", estadias[i].dataEntrada.dia, estadias[i].dataEntrada.mes, estadias[i].dataEntrada.ano);
            printf("\nData da Saida: %02d/%02d/%04d\n", estadias[i].dataSaida.dia, estadias[i].dataSaida.mes, estadias[i].dataSaida.ano);

            printf("\nQuantidade de Diarias: %d\n", estadias[i].quantidadeDiarias);
            printf("\nNumero do Quarto: %d\n", estadias[i].numeroQuarto.numeroQuarto);
            printf("\n");

            estadiasEncontradas++;
        }
    }
    if(estadiasEncontradas == 0){
        printf("\nNenhuma Estadia Encontrada!\n");
    }
}
//----------------------------------------

/*Oferece Opções para Registrar Novas Entradas, Realizar Baixas em Estadias e Buscar Informações de Clientes e Funcionários. 
Utiliza Arquivos para Salvar Dados e Arrays para Gerenciar Informações.*/
//Função Principal
int main() {
    int opcao;
    Cliente clientes[100];
    Quarto quartos[200];
    Funcionario funcionarios[100];
    Estadia estadias[100];
    int numClientes = 0, numQuartos = 0, numFuncionarios = 0, numEstadias = 0;

    //Carregar o Arquivo Clientes
    carregarClientes(clientes, &numClientes);

    //Carregar o Arquivo Funcionarios
    carregarFuncionarios(funcionarios, &numFuncionarios);

    //Carregar o Arquivo Quartos
    carregarQuartos(quartos, &numQuartos);

    //Carregar o Arquivo Estadia
    carregarEstadias(estadias, &numEstadias, clientes, numClientes, quartos, numQuartos);

    //printarEstadia(estadias, &numEstadias);

    do {
        printf("\n--------------------Hotel Descanso Garantido--------------------\n");
        printf("\nSistema HotelBooker, O que Voce Deseja?\n");
        printf("1 - Cadastrar Cliente\n");
        printf("2 - Cadastrar Funcionario\n");
        printf("3 - Cadastrar Quarto\n");
        printf("4 - Cadastrar Estadia\n");
        printf("5 - Dar Baixa em Alguma Estadia\n");
        printf("6 - Pesquisar por Cliente\n");
        printf("7 - Pesquisar por Funcionario\n");
        printf("8 - Total de Estadias por Cliente\n");
        printf("9 - Sair\n\nOp: ");
        scanf(" %d", &opcao);

        switch (opcao) {
            case 1:
                cadastrarClientes(clientes, &numClientes);
                break;
            case 2:
                cadastrarFuncionarios(funcionarios, &numFuncionarios);
                break;
            case 3:
                cadastrarQuartos(quartos, &numQuartos);
                break;
            case 4:
                cadastrarEstadia(estadias, &numEstadias, clientes, numClientes, quartos, numQuartos);
                break;
            case 5:
                baixaEstadia(estadias, &numEstadias, quartos, &numQuartos);
                break;
            case 6: 
                pesquisarClientes(clientes, &numClientes);
                break;
            case 7:
                pesquisarFuncionarios(funcionarios, &numFuncionarios);
                break;
            case 8:
                totalEstadias(estadias, &numEstadias, clientes, &numClientes);
                break;
            case 9:
                printf("\nEncerrando o Programa\n");
                break;
            default:
                printf("\nOpcao invalida!\n");
                
                //Limpar Buffer
                char buffer[200];
                scanf(" %[^\n]", buffer);
        }
    } while (opcao != 9);

    return 0;
}