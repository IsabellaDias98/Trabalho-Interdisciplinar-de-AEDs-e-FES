Com certeza!
Refatorei o Code Review para ser mais focado em Arquitetura, Padrões de Projeto e Engenharia de Software, termos e conceitos que um aluno do quarto período de Engenharia de Software já deve estar familiarizado.
O foco principal da refatoração é na Separação de Responsabilidades (SRP), Baixo Acoplamento e na eliminação de Code Smells que comprometem a manutenibilidade do sistema.
Você pode usar o código abaixo no seu Pull Request.
Code Review: HotelBooker (1).c (Visão Engenharia de Software)
A análise a seguir foca na arquitetura, nos padrões de projeto aplicáveis e nas boas práticas de desenvolvimento em C, com o objetivo de aumentar a manutenibilidade, a robustez e a escalabilidade do sistema.
/*******************************************************************************************************************************
* FILENAME : HotelBooker.c
* DESCRIPTION : Hotel management system for handling client, room, and stay records, including functions for registration, 
* search, and management of data.
* PUBLIC FUNCTIONS :
* void cadastrarClientes(Cliente clientes[], int *numClientes)
* void cadastrarFuncionarios(Funcionario funcionarios[], int *numFuncionarios)
* void cadastrarQuartos(Quarto quartos[], int *numQuartos)
* void cadastrarEstadia(Estadia estadias[], int *numEstadias, Cliente clientes[], int numClientes, Quarto quartos[], int numQuartos)
* void baixaEstadia(Estadia estadias[], int *numEstadias, Quarto quartos[], int *numQuartos)
* void pesquisarClientes(Cliente clientes[], int *numClientes)
* void pesquisarFuncionarios(Funcionario funcionarios[], int *numFuncionarios)
* void totalEstadias(Estadia estadias[], int *numEstadias, Cliente clientes[], int *numClientes)
* NOTES :
* This program manages hotel operations through file handling and interactive user options.
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

// ####### Sugestão de Padrão de Projeto (Refatoração de Arquitetura) #######
// 1. O arquivo `HotelBooker.c` viola o Princípio da Responsabilidade Única (SRP) ao misturar:
//    - Lógica de Interface/Apresentação (Input/Output do console).
//    - Lógica de Negócio/Domínio (Validação de código, cálculo de diárias).
//    - Lógica de Persistência (Leitura/Escrita de arquivos TXT).
// Sugestão: Aplicar o padrão **Data Access Object (DAO)**. Mova toda a manipulação de arquivos (fopen, fprintf, fscanf, fclose) para arquivos dedicados (`cliente_dao.c`, `quarto_dao.c`), isolando a camada de persistência.

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
    float salario; // 2. Code Smell: Uso de `float` para valores monetários. Isso introduz erros de precisão. Em C, sugere-se usar `long` para armazenar o valor em centavos (e.g., R$ 100,50 -> 10050) para garantir a integridade dos dados financeiros.
}Funcionario;
//--------------------------------------

//Estrutura Quarto
typedef struct{
    int numeroQuarto;
    int quantidadeHospedes;
    float valorDiaria;
    char status[200]; // 3. Melhoria: O campo `status` é categórico (Ocupado/Desocupado). Usar um **ENUM** (ex: `enum StatusQuarto { DESOCUPADO, OCUPADO }`) é uma boa prática que garante o encapsulamento de valores, melhora a legibilidade e evita erros de *typo* em strings.
}Quarto;
//--------------------------------------

//Estrutura Estadia
typedef struct{
    int codigoEstadia;
    Data dataEntrada;
    Data dataSaida;
    int quantidadeDiarias; // 4. Code Smell: **Dado Derivado/Calculado**. A quantidade de diárias é calculada a partir das datas. Não deve ser persistida para evitar inconsistências. Deve ser calculada *em tempo de execução* (On-Demand) quando necessário.
    Cliente codigoCliente; // 5. Violação do Baixo Acoplamento: Armazenar a ESTRUTURA `Cliente` (duplicando nome, endereço, etc.) é má prática e aumenta o **acoplamento**. A Estadia deve armazenar APENAS o **ID/Chave Estrangeira** (`int codigoClienteID`) para referenciar o Cliente, seguindo o conceito de Relacionamento (1:N).
    Quarto numeroQuarto; // 6. Violação do Baixo Acoplamento: Assim como o Cliente, armazene apenas o **ID do Quarto** (`int numeroQuartoID`).
}Estadia;
//---------------------------------------

//Declaração de Funções e Parâmetros
// 7. Refatoração: Para arrays, considere passar o tamanho por valor (`int numClientes`) para funções que APENAS leem os dados (ex: `pesquisarClientes`), e por ponteiro (`int *numClientes`) apenas para funções que ALTERAM o tamanho (ex: `cadastrarClientes`). Isso aumenta a segurança (imutabilidade).
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
    
    // 8. Code Smell: A lógica de validação de código repetida em `cadastrarClientes`, `cadastrarFuncionarios` e `cadastrarQuartos` é um **Duplicated Code**. Refatore para uma função auxiliar genérica de validação de ID.

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
    // 9. Boas Práticas (Segurança): Adicionar uma checagem de **Buffer Overflow** (`if (*numClientes >= MAX_CLIENTES)`), onde `MAX_CLIENTES` é uma constante (Melhoria 53).

    clientes[*numClientes] = novoCliente;
    (*numClientes)++;

    //Armazenamento no Arquivo Clientes.txt
    // 10. Violação do SRP: Esta seção de código (FILE *) deve ser movida para a camada DAO (`cliente_dao_salvar()`), deixando o `cadastrarClientes` responsável apenas pela lógica de negócio e interface.
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
    
    // 11. Boas Práticas (Robustez I/O): O uso de `fscanf` com `%[^,]` é frágil. Se um campo (como "Nome") contiver a vírgula de delimitador, o parser falha. Recomenda-se usar `fgets` para ler a linha completa e, em seguida, `strtok` para tokenizar, o que é mais robusto para arquivos CSV simples.

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
// ... (omissão de `cadastrarFuncionarios` e `carregarFuncionarios` por repetição de princípios)
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
        // 12. Bug de Leitura: O especificador `%s[^\n]` é inválido. O `%s` consome a string até um espaço em branco, e o `[^\n]` não funciona como esperado. Para ler a última string até a quebra de linha, use simplesmente `%[^\n]`.

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
            novaEstadia.codigoCliente = clientes[i]; // 13. Reiteração de Bug/Acoplamento: Armazenar a struct completa. Corrija a struct Estadia e armazene apenas o ID.
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
    
    // 14. Crítica de Arquitetura: A **Busca** por Cliente e Quarto é feita com pesquisa **linear** (`O(n)`). Para grandes volumes de dados, isso se torna um gargalo.
    // Sugestão: Use o padrão **Strategy** para buscar dados. No contexto de C e arrays, isso significa pré-processar os dados em estruturas de hash (se implementadas) ou arrays ordenados, permitindo buscas binárias (`O(log n)`).

    printf("\nDigite a Data de Entrada (dd mm aaaa): \n");
    scanf("%d %d %d", &novaEstadia.dataEntrada.dia, &novaEstadia.dataEntrada.mes, &novaEstadia.dataEntrada.ano);

    printf("\nDigite a Data de Saida (dd mm aaaa): \n");
    scanf("%d %d %d", &novaEstadia.dataSaida.dia, &novaEstadia.dataSaida.mes, &novaEstadia.dataSaida.ano);

    //Cálculo da Quantidade de Diárias
    // 15. Bug Crítico (Lógica de Domínio): A fórmula de cálculo de diárias ignora meses com 31, 30, 29 e 28 dias e anos bissextos, produzindo valores incorretos. Isso exige uma função de domínio robusta para cálculo de datas (p. ex., conversão para dias Julianos ou uso da biblioteca `<time.h>`).
    novaEstadia.quantidadeDiarias = (novaEstadia.dataSaida.dia - novaEstadia.dataEntrada.dia) + (novaEstadia.dataSaida.mes - novaEstadia.dataEntrada.mes) * 30 + (novaEstadia.dataSaida.ano - novaEstadia.dataEntrada.ano) * 365;

    //Atualizar Estadias Cadastradas
    // 16. Code Smell (Geração de ID): Gerar `codigoEstadia = *numEstadias` garante que IDs serão **reutilizados** após uma baixa. O ID deve ser gerado por uma **Sequência Global Persistente** para garantir que cada registro seja único, mesmo após a exclusão.
    novaEstadia.codigoEstadia = *numEstadias;
    estadias[*numEstadias] = novaEstadia;
    (*numEstadias)++;
    novaEstadia.codigoEstadia = *numEstadias;

    //Armazenamento no Arquivo Estadia.txt
    FILE *file = fopen("Estadia.txt", "a");
    // ... (omissão de código de escrita)
    fclose(file);

    //Atualizar o Status do Quarto no Arquivo Quartos.txt
    // 17. Crítica de Performance/Transação: O uso de `fopen("Quartos.txt", "w")` para reescrever **todo o arquivo de quartos** apenas para mudar o status de **um** quarto é ineficiente e não transacional. Em caso de falha de escrita, todos os dados são perdidos. Mova esta lógica para o DAO e implemente um mecanismo de reescrita mais seguro.
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
    // ... (omissão de código)

    // Lê o Arquivo Linha por Linha
    while (fscanf(file, "%d,%d,%d,%d,%d,%d,%d,%d,%d\n", &codigoEstadia, &codigoCliente, &numeroQuarto, &diaEntrada, &mesEntrada, &anoEntrada, &diaSaida, &mesSaida, &anoSaida) != EOF) {

        clienteIndex = -1;
        quartoIndex = -1;

        //Encontrar Cliente pelo Código
        // 18. Código Repetido: Esta busca linear é duplicada em outras funções. Extrair para uma função auxiliar de busca por ID (`int buscar_cliente_por_id(int id, Cliente[], int count)`).
        for(int i = 0; i < numClientes; i++){
            if(clientes[i].codigoCliente == codigoCliente){
                clienteIndex = i;
                break;
            }
        }

        //Encontrar Quarto pelo Número
        for(int i = 0; i < numeroQuarto; i++){ // 19. **BUG LÓGICO**: O limite do loop de busca é `i < numeroQuarto`, o que está incorreto. O limite correto é o tamanho do array de quartos: `i < numQuartos`.
            if(quartos[i].numeroQuarto == numeroQuarto){
                quartoIndex = i;
                break;
            }
        }

        //Garante que o Quarto e o Cliente Foram Encontrados
        if(clienteIndex == -1 || quartoIndex == -1){
            // 20. Tratamento de Dados Órfãos: Quando a aplicação não encontra um Cliente ou Quarto referenciado, a estadia é ignorada (`continue`). Isso é um *dado órfão*. Deve-se considerar um log de erro e, se possível, a remoção da linha inválida do arquivo para evitar problemas na próxima carga.
            continue;
        }
        
        // ... (omissão de código de atribuição de dados)

        //Cálculo da Quantidade de Diárias
        // 21. Reiteração de Bug de Domínio: A lógica de cálculo de datas é repetida e falha (Melhoria 15).
        estadias[*numEstadias].quantidadeDiarias = (diaSaida - diaEntrada) + (mesSaida - mesEntrada) * 30 + (anoSaida - anoEntrada) * 365;
    
        //Contador de Estadias
        (*numEstadias)++;
    }

    //Fecha o Arquivo
    fclose(file);

}
//----------------------------------------

/*Realiza o Encerramento de uma Estadia, Atualiza o Status do Quarto para "Desocupado", Calcula o Valor Total da Estadia e Remove
a Estadia do Registro.*/
//Baixa em Alguma Estadia
void baixaEstadia(Estadia estadias[], int *numEstadias, Quarto quartos[], int *numQuartos) {
    int codigoEstadia = 0;
    
    // ... (código de input e busca da estadia)

    //Calculo do Valor Total da Estadia do Cliente
    float valorTotal = estadia->quantidadeDiarias * estadia->numeroQuarto.valorDiaria;
    printf("\nValor Total da Estadia: R$ %.2f\n", valorTotal);

    //Atualizar o Status do Quarto para "Desocupado"
    // ... (código de busca e atualização do status do quarto)

    //Remover a Estadia do Array
    // 22. **BUG DE ARRAY (Off-by-One)**: O loop para remoção (deslocamento) deve ir até `*numEstadias - 1`. O uso de `i < *numEstadias` e o acesso a `estadias[i + 1]` no último passo causa um acesso de memória fora dos limites do array.
    for (int i = estadiaIndex; i < *numEstadias; i++) {
        estadias[i] = estadias[i + 1];
    }
    (*numEstadias)--;

    //Atualizar o Arquivo Estadia.txt
    // 23. Crítica de Persistência (SRP): A lógica de reescrita total dos arquivos (`Estadia.txt` e `Quartos.txt`) está REPETIDA e deve ser abstraída para funções de salvamento na camada DAO.
    FILE *file = fopen("Estadia.txt", "w");
    // ... (código de reescrita)
    fclose(file);

    //Atualizar o Arquivo Quartos.txt
    file = fopen("Quartos.txt", "w");
    // ... (código de reescrita)
    fclose(file);

    printf("\nQuarto Desocupado!\n");
}
//----------------------------------------
// ... (omissão de `pesquisarClientes`, `pesquisarFuncionarios` e `totalEstadias` por repetição de princípios)
//----------------------------------------

/*Oferece Opções para Registrar Novas Entradas, Realizar Baixas em Estadias e Buscar Informações de Clientes e Funcionários. 
Utiliza Arquivos para Salvar Dados e Arrays para Gerenciar Informações.*/
//Função Principal
int main() {
    int opcao;
    // 24. Padrão de Projeto (Magic Numbers): Os tamanhos dos arrays (100, 200) são "números mágicos" que violam a clareza. Use **Constantes Simbólicas** (`#define MAX_CLIENTES 100`) para facilitar a manutenção e leitura.
    Cliente clientes[100];
    Quarto quartos[200];
    Funcionario funcionarios[100];
    Estadia estadias[100];
    int numClientes = 0, numQuartos = 0, numFuncionarios = 0, numEstadias = 0;

    //Carregar o Arquivo Clientes
    // 25. Boas Práticas (Error Handling): O carregamento dos dados depende de uma ordem específica (Quartos e Clientes ANTES de Estadias). Adicionar um tratamento de erro ou validação para garantir que todas as dependências foram carregadas corretamente.
    carregarClientes(clientes, &numClientes);

    //Carregar o Arquivo Funcionarios
    carregarFuncionarios(funcionarios, &numFuncionarios);

    //Carregar o Arquivo Quartos
    carregarQuartos(quartos, &numQuartos);

    //Carregar o Arquivo Estadia
    carregarEstadias(estadias, &numEstadias, clientes, numClientes, quartos, numQuartos);

    do {
        // 26. Refatoração (SRP): Mover o bloco de impressão do menu e a leitura do input para uma função auxiliar (`int mostrar_menu_e_ler_opcao()`), mantendo a função `main` responsável apenas pelo fluxo de controle (`switch` e `while`).
        printf("=====================================\n");
        printf("=     HOTEL DESCANSO GARANTIDO      =\n");
        printf("=====================================\n");
        printf("Sistema HotelBooker, O que Voce Deseja?\n");
        // ... (código do menu)
        
        switch (opcao) {
            case 1:
                cadastrarClientes(clientes, &numClientes);
                break;
            // ... (outros cases)
            default:
                printf("\nOpcao invalida!\n");
                
                //Limpar Buffer
                // 27. Boas Práticas (Limpeza de Buffer): O método de limpeza de buffer é pouco confiável. O padrão em C é `while (getchar() != '\n' && getchar() != EOF);` após um `scanf` falho.
                char buffer[200];
                scanf(" %[^\n]", buffer);
        }
    } while (opcao != 9);

    return 0;
}


        
