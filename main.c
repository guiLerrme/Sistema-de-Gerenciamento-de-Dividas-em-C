#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INCREMENTO 5 // Tamanho do incremento ao expandir os vetores dinamicos
#define ARQ_CLIENTE "clientes.bin" // Nome do arquivo binario de clientes
#define ARQ_EMPRESA "empresas.bin" // Nome do arquivo binario de empresas
#define ARQ_DIVIDA "dividas.bin" // Nome do arquivo binario de dividas

typedef struct
{
    char cpf[12]; // CPF do cliente (11 digitos + '\0')
    char nome_cliente[50]; // Nome completo do cliente
    int idade_cliente; // Idade do cliente
} Cliente; // defini��o da struct Cliente

typedef struct
{
    char cnpj[15]; // CNPJ da empresa (14 digitos + '\0')
    char nome_empresa[50]; // Nome da empresa
    char produto[50]; // Produto ou servico oferecido pela empresa
} Empresa; // defini��o da struct Empresa

typedef struct
{
    char cpf_cliente[12]; // CPF do cliente devedor
    char cnpj_empresa[15]; // CNPJ da empresa credora
    float valor; // Valor da divida
} Divida; // defini��o da struct Divida


int BuscaCliente(Cliente *cliente, int quantidade_cliente, char cpf[]) //Fun��o que verifica se existe um cliente ativo no sistema
{
    for(int cont_c = 0; cont_c < quantidade_cliente; cont_c++)
    {
        if(strcmp(cliente[cont_c].cpf, cpf) == 0) // Percorre por todos os clientes cadastrados
            return cont_c; // Se existir, ele retornar� o indice que est� esse cliente
    }
    return -1; // Se n�o, retornar� -1
}

int BuscaEmpresa(Empresa *empresa, int quantidade_empresa, char cnpj[]) //Fun��o que verifica se existe um cliente ativo no sistema
{
    for(int cont_e = 0; cont_e < quantidade_empresa; cont_e++)
    {
        if(strcmp(empresa[cont_e].cnpj, cnpj) == 0) // Percorre por todos as empresas cadastradas
            return cont_e; // Se existir, ele retornar� o indice que est� essa empresa
    }
    return -1; // Se n�o, retornar� -1
}

int BuscaDivida(Divida *divida, int quantidade_divida, char cpf_cliente[], char cnpj_empresa[]) //Fun��o que verifica se existe uma Divida ativo no sistema
{
    for(int cont_d = 0; cont_d < quantidade_divida; cont_d++)
    {
        if(strcmp(divida[cont_d].cpf_cliente, cpf_cliente) == 0 && // Percorre por todos as divdas cadastradas
                strcmp(divida[cont_d].cnpj_empresa, cnpj_empresa) == 0) // Se existir, ele retornar� o indice que est� essa divida-
            return cont_d;
    }
    return -1; // Se n�o, retornar� -1
}

int BuscaClienteEmpresa(Cliente *cliente, Empresa *empresa,
                        char cpf[], char cnpj[],
                        int quantidade_cliente, int quantidade_empresa) // Fun��o que busca se existe o Cliente E a empresa cadastrada, se sim, estar� apta para a cria��o da divida
{
    if(BuscaCliente(cliente, quantidade_cliente, cpf) != -1 && // Buscanndo a fun��o BuscaCliente
            BuscaEmpresa(empresa, quantidade_empresa, cnpj) != -1) // e a BuscaEmpresa
    {
        printf("\nCLIENTE: %s",
               cliente[BuscaCliente(cliente, quantidade_cliente, cpf)].nome_cliente); // J� aproveita para printar para o usuario qual ser� o cliente e empresa

        printf("\nEMPRESA: %s\n",
               empresa[BuscaEmpresa(empresa, quantidade_empresa, cnpj)].nome_empresa);

        return 1; // Se o cliente e a empresa existir retorna 1
    }
    return -1; // Se n�o, retornar� -1
}

void Menu() // Cria��o do Menu para nao repetir codigo dentro da Main
{
    printf("\n _____________________________________________________\n");
    printf("\n ==            SGN ASSESSORIA FINANCEIRA            ==");
    printf("\n ==                  Cadastrar [1]                  ==");
    printf("\n ==                  Consultar [2]                  ==");
    printf("\n ==                  Atualizar [3]                  ==");
    printf("\n ==                   Excluir [4]                   ==");
    printf("\n ==                    Listar [5]                   ==");
    printf("\n ==               Gerar Relatorio [6]               ==");
    printf("\n ==               Sair do Programa[0]               ==");
    printf("\n _____________________________________________________\n");
    printf("\nDigite a opcao desejada: ");
}

void Qual_Struct() // Cria��o de um Sub Menu para nao repetir codigo dentro da Main
{
    printf("\n _____________________________________________________\n");
    printf("\n ==     CLIENTE [1] / EMPRESA [2] / DIVIDA [3]       ==");
    printf("\n _____________________________________________________\n");
    printf("\nDigite a opcao desejada: ");
}

void CarregarClientes(Cliente **cliente_lista, int *quantidade_cliente, int *capacidade_cliente) // Carrega os clientes salvos no arquivo binario para a memoria
{
    FILE *arquivo = fopen(ARQ_CLIENTE, "rb"); // Abre o arquivo de clientes em modo leitura binaria
    if(arquivo == NULL) // Se o arquivo nao existir, nao ha nada a carregar
        return;

    fread(quantidade_cliente, sizeof(int), 1, arquivo); // Le a quantidade de clientes armazenados

    while(*capacidade_cliente < *quantidade_cliente) // Expande a capacidade ate comportar todos os registros
        *capacidade_cliente += INCREMENTO;
    *cliente_lista = realloc(*cliente_lista, (*capacidade_cliente) * sizeof(Cliente)); // Redimensiona o vetor para a capacidade necessaria
    if(*cliente_lista == NULL) // Verifica se a realocacao falhou
    {
        printf("\n== ERRO: FALHA NA ALOCACAO DE MEMORIA ==\n");
        exit(1); // Encerra o programa pois nao e seguro continuar sem memoria
    }

    fread(*cliente_lista, sizeof(Cliente), *quantidade_cliente, arquivo); // Le todos os clientes do arquivo
    fclose(arquivo); // Fecha o arquivo apos a leitura
}

void SalvarClientes(Cliente *cliente_lista, int quantidade_cliente) // Salva todos os clientes da memoria no arquivo binario
{
    FILE *arquivo = fopen(ARQ_CLIENTE, "wb"); // Abre o arquivo de clientes em modo escrita binaria
    if(arquivo == NULL)
    {
        printf("\n== ERRO AO SALVAR CLIENTES ==\n"); // Exibe erro se o arquivo nao puder ser aberto
        return;
    }
    fwrite(&quantidade_cliente, sizeof(int), 1, arquivo); // Grava a quantidade de clientes primeiro
    fwrite(cliente_lista, sizeof(Cliente), quantidade_cliente, arquivo); // Grava todos os clientes no arquivo
    fclose(arquivo); // Fecha o arquivo apos a gravacao
}

void CarregarEmpresas(Empresa **empresa_lista, int *quantidade_empresa, int *capacidade_empresa) // Carrega as empresas salvas no arquivo binario para a memoria
{
    FILE *arquivo = fopen(ARQ_EMPRESA, "rb"); // Abre o arquivo de empresas em modo leitura binaria
    if(arquivo == NULL) // Se o arquivo nao existir, nao ha nada a carregar
        return;

    fread(quantidade_empresa, sizeof(int), 1, arquivo); // Le a quantidade de empresas armazenadas

    while(*capacidade_empresa < *quantidade_empresa) // Expande a capacidade ate comportar todos os registros
        *capacidade_empresa += INCREMENTO;
    *empresa_lista = realloc(*empresa_lista, (*capacidade_empresa) * sizeof(Empresa)); // Redimensiona o vetor para a capacidade necessaria
    if(*empresa_lista == NULL) // Verifica se a realocacao falhou
    {
        printf("\n== ERRO: FALHA NA ALOCACAO DE MEMORIA ==\n");
        exit(1); // Encerra o programa pois nao e seguro continuar sem memoria
    }

    fread(*empresa_lista, sizeof(Empresa), *quantidade_empresa, arquivo); // Le todas as empresas do arquivo
    fclose(arquivo); // Fecha o arquivo apos a leitura
}

void SalvarEmpresas(Empresa *empresa_lista, int quantidade_empresa) // Salva todas as empresas da memoria no arquivo binario
{
    FILE *arquivo = fopen(ARQ_EMPRESA, "wb"); // Abre o arquivo de empresas em modo escrita binaria
    if(arquivo == NULL)
    {
        printf("\n== ERRO AO SALVAR EMPRESAS ==\n"); // Exibe erro se o arquivo nao puder ser aberto
        return;
    }
    fwrite(&quantidade_empresa, sizeof(int), 1, arquivo); // Grava a quantidade de empresas primeiro
    fwrite(empresa_lista, sizeof(Empresa), quantidade_empresa, arquivo); // Grava todas as empresas no arquivo
    fclose(arquivo); // Fecha o arquivo apos a gravacao
}

void CarregarDividas(Divida **divida_lista, int *quantidade_divida, int *capacidade_divida) // Carrega as dividas salvas no arquivo binario para a memoria
{
    FILE *arquivo = fopen(ARQ_DIVIDA, "rb"); // Abre o arquivo de dividas em modo leitura binaria
    if(arquivo == NULL) // Se o arquivo nao existir, nao ha nada a carregar
        return;

    fread(quantidade_divida, sizeof(int), 1, arquivo); // Le a quantidade de dividas armazenadas

    while(*capacidade_divida < *quantidade_divida) // Expande a capacidade ate comportar todos os registros
        *capacidade_divida += INCREMENTO;
    *divida_lista = realloc(*divida_lista, (*capacidade_divida) * sizeof(Divida)); // Redimensiona o vetor para a capacidade necessaria
    if(*divida_lista == NULL) // Verifica se a realocacao falhou
    {
        printf("\n== ERRO: FALHA NA ALOCACAO DE MEMORIA ==\n");
        exit(1); // Encerra o programa pois nao e seguro continuar sem memoria
    }

    fread(*divida_lista, sizeof(Divida), *quantidade_divida, arquivo); // Le todas as dividas do arquivo
    fclose(arquivo); // Fecha o arquivo apos a leitura
}

void SalvarDividas(Divida *divida_lista, int quantidade_divida) // Salva todas as dividas da memoria no arquivo binario
{
    FILE *arquivo = fopen(ARQ_DIVIDA, "wb"); // Abre o arquivo de dividas em modo escrita binaria
    if(arquivo == NULL)
    {
        printf("\n== ERRO AO SALVAR DIVIDAS ==\n"); // Exibe erro se o arquivo nao puder ser aberto
        return;
    }
    fwrite(&quantidade_divida, sizeof(int), 1, arquivo); // Grava a quantidade de dividas primeiro
    fwrite(divida_lista, sizeof(Divida), quantidade_divida, arquivo); // Grava todas as dividas no arquivo
    fclose(arquivo); // Fecha o arquivo apos a gravacao
}

void CadastrarCliente(Cliente **cliente_lista, int *quantidade_cliente, int *capacidade_cliente) // Cadastra um novo cliente no sistema
{
    if(*capacidade_cliente == *quantidade_cliente) // Verifica se o vetor esta cheio
    {
        *capacidade_cliente += INCREMENTO; // Aumenta a capacidade
        *cliente_lista = realloc(*cliente_lista, (*capacidade_cliente) * sizeof(Cliente)); // Redimensiona o vetor
        if(*cliente_lista == NULL) // Verifica se a realocacao falhou
        {
            printf("\n== ERRO: FALHA NA ALOCACAO DE MEMORIA ==\n");
            exit(1); // Encerra o programa pois nao e seguro continuar sem memoria
        }
    }

    char cpf_temp[12]; // Variavel temporaria para armazenar o CPF digitado

    printf("\nDigite o CPF do Cliente (11 numeros): ");
    scanf("%11s", cpf_temp);

    if(BuscaCliente(*cliente_lista, *quantidade_cliente, cpf_temp) != -1) // Verifica se o CPF ja esta cadastrado
    {
        printf("\n== ESSE CPF JA CONSTA NO SISTEMA ==\n");
        return;
    }

    strcpy((*cliente_lista)[*quantidade_cliente].cpf, cpf_temp); // Copia o CPF para o novo registro

    while(getchar() != '\n'); // Limpa todo o buffer, descartando caracteres residuais ate o '\n'
    //teste de modificação
    printf("\nDigite o nome do Cliente: ");
    fgets((*cliente_lista)[*quantidade_cliente].nome_cliente, 50, stdin); // Le o nome com espacos
    (*cliente_lista)[*quantidade_cliente].nome_cliente[strcspn(
                (*cliente_lista)[*quantidade_cliente].nome_cliente, "\n")] = '\0'; // Remove o '\n' do final da string

    printf("\nDigite a idade do Cliente: ");
    scanf("%d", &(*cliente_lista)[*quantidade_cliente].idade_cliente); // Le a idade do cliente

    printf("\n== CLIENTE CADASTRADO COM SUCESSO! ==\n");

    (*quantidade_cliente)++; // Incrementa o contador de clientes
}

void CadastrarEmpresa(Empresa **empresa_lista, int *quantidade_empresa, int *capacidade_empresa) // Cadastra uma nova empresa no sistema
{
    if(*capacidade_empresa == *quantidade_empresa) // Verifica se o vetor esta cheio
    {
        *capacidade_empresa += INCREMENTO; // Aumenta a capacidade
        *empresa_lista = realloc(*empresa_lista, (*capacidade_empresa) * sizeof(Empresa)); // Redimensiona o vetor
        if(*empresa_lista == NULL) // Verifica se a realocacao falhou
        {
            printf("\n== ERRO: FALHA NA ALOCACAO DE MEMORIA ==\n");
            exit(1); // Encerra o programa pois nao e seguro continuar sem memoria
        }
    }

    char cnpj_temp[15]; // Variavel temporaria para armazenar o CNPJ digitado

    printf("\nDigite o CNPJ da empresa (14 numeros): ");
    scanf("%14s", cnpj_temp);

    if(BuscaEmpresa(*empresa_lista, *quantidade_empresa, cnpj_temp) != -1) // Verifica se o CNPJ ja esta cadastrado
    {
        printf("\n== ESSE CNPJ JA CONSTA NO SISTEMA ==\n");
        return;
    }

    strcpy((*empresa_lista)[*quantidade_empresa].cnpj, cnpj_temp); // Copia o CNPJ para o novo registro

    while(getchar() != '\n'); // Limpa todo o buffer, descartando caracteres residuais ate o '\n'

    printf("\nDigite o nome da empresa: ");
    fgets((*empresa_lista)[*quantidade_empresa].nome_empresa, 50, stdin); // Le o nome da empresa com espacos
    (*empresa_lista)[*quantidade_empresa].nome_empresa[strcspn(
                (*empresa_lista)[*quantidade_empresa].nome_empresa, "\n")] = '\0'; // Remove o '\n' do final da string

    printf("\nDigite o nome do produto: ");
    fgets((*empresa_lista)[*quantidade_empresa].produto, 50, stdin); // Le o nome do produto com espacos
    (*empresa_lista)[*quantidade_empresa].produto[strcspn(
                (*empresa_lista)[*quantidade_empresa].produto, "\n")] = '\0'; // Remove o '\n' do final da string

    printf("\n== EMPRESA CADASTRADA COM SUCESSO! ==\n");

    (*quantidade_empresa)++; // Incrementa o contador de empresas
}

void CadastrarDivida(Divida **divida_lista, int *quantidade_divida, int *capacidade_divida,
                     Cliente *cliente_lista, int quantidade_cliente,
                     Empresa *empresa_lista, int quantidade_empresa) // Cadastra uma nova divida vinculando cliente e empresa
{
    if(*capacidade_divida == *quantidade_divida) // Verifica se o vetor esta cheio
    {
        *capacidade_divida += INCREMENTO; // Aumenta a capacidade
        *divida_lista = realloc(*divida_lista, (*capacidade_divida) * sizeof(Divida)); // Redimensiona o vetor
        if(*divida_lista == NULL) // Verifica se a realocacao falhou
        {
            printf("\n== ERRO: FALHA NA ALOCACAO DE MEMORIA ==\n");
            exit(1); // Encerra o programa pois nao e seguro continuar sem memoria
        }
    }

    char cpf_temp[12]; // Variavel temporaria para o CPF do cliente
    char cnpj_temp[15]; // Variavel temporaria para o CNPJ da empresa

    printf("Digite o CPF do Cliente: ");
    scanf("%11s", cpf_temp);

    printf("Digite o CNPJ do credor: ");
    scanf("%14s", cnpj_temp);

    if(BuscaClienteEmpresa(cliente_lista, empresa_lista,
                           cpf_temp, cnpj_temp,
                           quantidade_cliente, quantidade_empresa) != -1) // Se o Cliente E a empresa estiverem cadastrados
    {
        if(BuscaDivida(*divida_lista, *quantidade_divida,
                       cpf_temp, cnpj_temp) != -1) // Busca se a divida j� foi cadastrada
        {
            printf("\n== DIVIDA JA CADASTRADA ==\n");
            return;
        }

        strcpy((*divida_lista)[*quantidade_divida].cpf_cliente, cpf_temp); // Copia o CPF para o novo registro de divida
        strcpy((*divida_lista)[*quantidade_divida].cnpj_empresa, cnpj_temp); // Copia o CNPJ para o novo registro de divida

        printf("Digite o valor da Divida: ");
        scanf("%f", &(*divida_lista)[*quantidade_divida].valor); // Le o valor da divida

        printf("\n== DIVIDA CADASTRADA COM SUCESSO! ==\n");

        (*quantidade_divida)++; // Incrementa o contador de dividas
    }
    else
        printf("\n== CLIENTE OU EMPRESA NAO CADASTRADOS ==\n");
}

void ConsultarCliente(Cliente *cliente_lista, int quantidade_cliente) // Consulta e exibe os dados de um cliente pelo CPF
{
    if(quantidade_cliente == 0) // Verifica se ha clientes cadastrados
    {
        printf("\n== NENHUM CLIENTE CADASTRADO! ==\n");
        return;
    }

    char cpf_temp[12]; // Variavel temporaria para o CPF de busca

    printf("\nDigite o CPF do cliente: ");
    scanf("%11s", cpf_temp);

    if(BuscaCliente(cliente_lista, quantidade_cliente, cpf_temp) == -1) // Verifica se o cliente existe
    {
        printf("\n== CLIENTE NAO ENCONTRADO ==\n");
        return;
    }

    // Exibe os dados do cliente encontrado
    printf("\nCPF: %s", cliente_lista[BuscaCliente(cliente_lista, quantidade_cliente, cpf_temp)].cpf);
    printf("\nNOME: %s", cliente_lista[BuscaCliente(cliente_lista, quantidade_cliente, cpf_temp)].nome_cliente);
    printf("\nIDADE: %d\n", cliente_lista[BuscaCliente(cliente_lista, quantidade_cliente, cpf_temp)].idade_cliente);
}

void ConsultarEmpresa(Empresa *empresa_lista, int quantidade_empresa) // Consulta e exibe os dados de uma empresa pelo CNPJ
{
    if(quantidade_empresa == 0) // Verifica se ha empresas cadastradas
    {
        printf("\n== NENHUMA EMPRESA CADASTRADA! ==\n");
        return;
    }

    char cnpj_temp[15]; // Variavel temporaria para o CNPJ de busca

    printf("\nDigite o CNPJ da empresa: ");
    scanf("%14s", cnpj_temp);

    if(BuscaEmpresa(empresa_lista, quantidade_empresa, cnpj_temp) == -1) // Verifica se a empresa existe
    {
        printf("\n== EMPRESA NAO ENCONTRADA ==\n");
        return;
    }

    // Exibe os dados da empresa encontrada
    printf("\nCNPJ: %s", empresa_lista[BuscaEmpresa(empresa_lista, quantidade_empresa, cnpj_temp)].cnpj);
    printf("\nNOME: %s", empresa_lista[BuscaEmpresa(empresa_lista, quantidade_empresa, cnpj_temp)].nome_empresa);
    printf("\nPRODUTO: %s\n", empresa_lista[BuscaEmpresa(empresa_lista, quantidade_empresa, cnpj_temp)].produto);
}

void ConsultarDivida(Divida *divida_lista, int quantidade_divida,
                     Cliente *cliente_lista, int quantidade_cliente,
                     Empresa *empresa_lista, int quantidade_empresa) // Consulta dividas com filtros por CPF, CNPJ ou valor minimo
{
    if(quantidade_divida == 0) // Verifica se ha dividas cadastradas
    {
        printf("\n== NENHUMA DIVIDA CADASTRADA! ==\n");
        return;
    }

    int opcao_filtro; // Armazena a opcao de filtro escolhida pelo usuario
    printf("\n _____________________________________________________\n");
    printf("\n ==   Filtrar por:                                   ==");
    printf("\n ==   CPF do Cliente    [1]                          ==");
    printf("\n ==   CNPJ da Empresa   [2]                          ==");
    printf("\n ==   Valor Minimo      [3]                          ==");
    printf("\n _____________________________________________________\n");
    printf("\nDigite a opcao desejada: ");
    scanf("%d", &opcao_filtro);

    char cpf_temp[12]; // Variavel temporaria para filtro por CPF
    char cnpj_temp[15]; // Variavel temporaria para filtro por CNPJ
    float valor_minimo; // Variavel para filtro por valor minimo
    int achou = 0; // Flag que indica se alguma divida foi encontrada

    if(opcao_filtro == 1) // Filtra dividas pelo CPF do cliente
    {
        printf("\nDigite o CPF do cliente: ");
        scanf("%11s", cpf_temp);

        for(int cont_d = 0; cont_d < quantidade_divida; cont_d++)
        {
            if(strcmp(divida_lista[cont_d].cpf_cliente, cpf_temp) == 0) // Compara o CPF de cada divida com o digitado
            {
                printf("\n---------------------------------------");
                printf("\nVALOR DA DIVIDA: R$%.2f", divida_lista[cont_d].valor);
                printf("\nCLIENTE: %s", cliente_lista[BuscaCliente(cliente_lista, quantidade_cliente, divida_lista[cont_d].cpf_cliente)].nome_cliente);
                printf("\nCPF: %s", divida_lista[cont_d].cpf_cliente);
                printf("\nEMPRESA: %s", empresa_lista[BuscaEmpresa(empresa_lista, quantidade_empresa, divida_lista[cont_d].cnpj_empresa)].nome_empresa);
                printf("\nCNPJ: %s\n", divida_lista[cont_d].cnpj_empresa);
                achou = 1; // Marca que ao menos uma divida foi encontrada
            }
        }
    }
    else if(opcao_filtro == 2) // Filtra dividas pelo CNPJ da empresa
    {
        printf("\nDigite o CNPJ da empresa: ");
        scanf("%14s", cnpj_temp);

        for(int cont_d = 0; cont_d < quantidade_divida; cont_d++)
        {
            if(strcmp(divida_lista[cont_d].cnpj_empresa, cnpj_temp) == 0) // Compara o CNPJ de cada divida com o digitado
            {
                printf("\n---------------------------------------");
                printf("\nVALOR DA DIVIDA: R$%.2f", divida_lista[cont_d].valor);
                printf("\nCLIENTE: %s", cliente_lista[BuscaCliente(cliente_lista, quantidade_cliente, divida_lista[cont_d].cpf_cliente)].nome_cliente);
                printf("\nCPF: %s", divida_lista[cont_d].cpf_cliente);
                printf("\nEMPRESA: %s", empresa_lista[BuscaEmpresa(empresa_lista, quantidade_empresa, divida_lista[cont_d].cnpj_empresa)].nome_empresa);
                printf("\nCNPJ: %s\n", divida_lista[cont_d].cnpj_empresa);
                achou = 1; // Marca que ao menos uma divida foi encontrada
            }
        }
    }
    else if(opcao_filtro == 3) // Filtra dividas pelo valor minimo
    {
        printf("\nDigite o valor minimo: ");
        scanf("%f", &valor_minimo);

        for(int cont_d = 0; cont_d < quantidade_divida; cont_d++)
        {
            if(divida_lista[cont_d].valor >= valor_minimo) // Verifica se o valor da divida atende ao filtro
            {
                printf("\n---------------------------------------");
                printf("\nVALOR DA DIVIDA: R$%.2f", divida_lista[cont_d].valor);
                printf("\nCLIENTE: %s", cliente_lista[BuscaCliente(cliente_lista, quantidade_cliente, divida_lista[cont_d].cpf_cliente)].nome_cliente);
                printf("\nCPF: %s", divida_lista[cont_d].cpf_cliente);
                printf("\nEMPRESA: %s", empresa_lista[BuscaEmpresa(empresa_lista, quantidade_empresa, divida_lista[cont_d].cnpj_empresa)].nome_empresa);
                printf("\nCNPJ: %s\n", divida_lista[cont_d].cnpj_empresa);
                achou = 1; // Marca que ao menos uma divida foi encontrada
            }
        }
    }
    else
        printf("\n== OPCAO INVALIDA ==\n");

    if(achou == 0 && opcao_filtro >= 1 && opcao_filtro <= 3) // Se nenhuma divida foi encontrada com o filtro aplicado
        printf("\n== NENHUMA DIVIDA ENCONTRADA ==\n");
}

void ListarClientes(Cliente *cliente_lista, int quantidade_cliente) // Lista todos os clientes cadastrados no sistema
{
    if(quantidade_cliente == 0) // Verifica se ha clientes cadastrados
    {
        printf("\n== NENHUM CLIENTE CADASTRADO! ==\n");
        return;
    }

    for(int cont_c = 0; cont_c < quantidade_cliente; cont_c++) // Percorre e exibe todos os clientes
    {
        printf("\nCPF: %s", cliente_lista[cont_c].cpf);
        printf("\nNOME: %s", cliente_lista[cont_c].nome_cliente);
        printf("\nIDADE: %d\n", cliente_lista[cont_c].idade_cliente);
    }
}

void ListarEmpresas(Empresa *empresa_lista, int quantidade_empresa) // Lista todas as empresas cadastradas no sistema
{
    if(quantidade_empresa == 0) // Verifica se ha empresas cadastradas
    {
        printf("\n== NENHUMA EMPRESA CADASTRADA! ==\n");
        return;
    }

    for(int cont_e = 0; cont_e < quantidade_empresa; cont_e++) // Percorre e exibe todas as empresas
    {
        printf("\nCNPJ: %s", empresa_lista[cont_e].cnpj);
        printf("\nNOME: %s", empresa_lista[cont_e].nome_empresa);
        printf("\nPRODUTO: %s\n", empresa_lista[cont_e].produto);
    }
}

void ListarDividas(Divida *divida_lista, int quantidade_divida) // Lista todas as dividas cadastradas no sistema
{
    if(quantidade_divida == 0) // Verifica se ha dividas cadastradas
    {
        printf("\n== NENHUMA DIVIDA CADASTRADA! ==\n");
        return;
    }

    for(int cont_d = 0; cont_d < quantidade_divida; cont_d++) // Percorre e exibe todas as dividas
    {
        printf("\nCPF DO CLIENTE: %s", divida_lista[cont_d].cpf_cliente);
        printf("\nCNPJ DO CREDOR: %s", divida_lista[cont_d].cnpj_empresa);
        printf("\nVALOR DA DIVIDA: R$%.2f\n", divida_lista[cont_d].valor);
    }
}

void AtualizarCliente(Cliente *cliente_lista, int quantidade_cliente) // Atualiza nome e idade de um cliente existente
{
    if(quantidade_cliente == 0) // Verifica se ha clientes cadastrados
    {
        printf("\n== NENHUM CLIENTE CADASTRADO! ==\n");
        return;
    }

    char cpf_temp[12]; // Variavel temporaria para o CPF de busca

    printf("\nDigite o CPF do cliente a atualizar: ");
    scanf("%11s", cpf_temp);

    if(BuscaCliente(cliente_lista, quantidade_cliente, cpf_temp) == -1) // Verifica se o cliente existe
    {
        printf("\n== CLIENTE NAO ENCONTRADO ==\n");
        return;
    }

    int pos_c = BuscaCliente(cliente_lista, quantidade_cliente, cpf_temp); // Armazena o indice do cliente encontrado

    while(getchar() != '\n'); // Limpa todo o buffer, descartando caracteres residuais ate o '\n'

    printf("\nDigite o novo nome: ");
    fgets(cliente_lista[pos_c].nome_cliente, 50, stdin); // Le o novo nome com espacos
    cliente_lista[pos_c].nome_cliente[strcspn(cliente_lista[pos_c].nome_cliente, "\n")] = '\0'; // Remove o '\n' do final da string

    printf("\nDigite a nova idade: ");
    scanf("%d", &cliente_lista[pos_c].idade_cliente); // Le a nova idade

    printf("\n== CLIENTE ATUALIZADO COM SUCESSO! ==\n");
}

void AtualizarEmpresa(Empresa *empresa_lista, int quantidade_empresa) // Atualiza nome e produto de uma empresa existente
{
    if(quantidade_empresa == 0) // Verifica se ha empresas cadastradas
    {
        printf("\n== NENHUMA EMPRESA CADASTRADA! ==\n");
        return;
    }

    char cnpj_temp[15]; // Variavel temporaria para o CNPJ de busca

    printf("\nDigite o CNPJ da empresa a atualizar: ");
    scanf("%14s", cnpj_temp);

    if(BuscaEmpresa(empresa_lista, quantidade_empresa, cnpj_temp) == -1) // Verifica se a empresa existe
    {
        printf("\n== EMPRESA NAO ENCONTRADA ==\n");
        return;
    }

    int pos_e = BuscaEmpresa(empresa_lista, quantidade_empresa, cnpj_temp); // Armazena o indice da empresa encontrada

    while(getchar() != '\n'); // Limpa todo o buffer, descartando caracteres residuais ate o '\n'

    printf("\nDigite o novo nome: ");
    fgets(empresa_lista[pos_e].nome_empresa, 50, stdin); // Le o novo nome da empresa com espacos
    empresa_lista[pos_e].nome_empresa[strcspn(empresa_lista[pos_e].nome_empresa, "\n")] = '\0'; // Remove o '\n' do final da string

    printf("\nDigite o novo produto: ");
    fgets(empresa_lista[pos_e].produto, 50, stdin); // Le o novo produto com espacos
    empresa_lista[pos_e].produto[strcspn(empresa_lista[pos_e].produto, "\n")] = '\0'; // Remove o '\n' do final da string

    printf("\n== EMPRESA ATUALIZADA COM SUCESSO! ==\n");
}

void AtualizarDivida(Divida *divida_lista, int quantidade_divida) // Atualiza o valor de uma divida existente
{
    if(quantidade_divida == 0) // Verifica se ha dividas cadastradas
    {
        printf("\n== NENHUMA DIVIDA CADASTRADA! ==\n");
        return;
    }

    char cpf_temp[12]; // Variavel temporaria para o CPF de busca
    char cnpj_temp[15]; // Variavel temporaria para o CNPJ de busca

    printf("\nDigite o CPF do cliente: ");
    scanf("%11s", cpf_temp);
    printf("\nDigite o CNPJ do credor: ");
    scanf("%14s", cnpj_temp);

    if(BuscaDivida(divida_lista, quantidade_divida, cpf_temp, cnpj_temp) == -1) // Verifica se a divida existe
    {
        printf("\n== DIVIDA NAO ENCONTRADA ==\n");
        return;
    }

    int pos_d = BuscaDivida(divida_lista, quantidade_divida, cpf_temp, cnpj_temp); // Armazena o indice da divida encontrada

    printf("\nDigite o novo valor: ");
    scanf("%f", &divida_lista[pos_d].valor); // Le o novo valor da divida

    printf("\n== DIVIDA ATUALIZADA COM SUCESSO! ==\n");
}

void ExcluirCliente(Cliente **cliente_lista, int *quantidade_cliente,
                    Divida *divida_lista, int quantidade_divida) // Exclui um cliente, bloqueando se houver dividas vinculadas
{
    if(*quantidade_cliente == 0) // Verifica se ha clientes cadastrados
    {
        printf("\n== NENHUM CLIENTE CADASTRADO! ==\n");
        return;
    }

    char cpf_temp[12]; // Variavel temporaria para o CPF de busca

    printf("\nDigite o CPF do cliente a excluir: ");
    scanf("%11s", cpf_temp);

    if(BuscaCliente(*cliente_lista, *quantidade_cliente, cpf_temp) == -1) // Verifica se o cliente existe
    {
        printf("\n== CLIENTE NAO ENCONTRADO ==\n");
        return;
    }

    for(int cont_d = 0; cont_d < quantidade_divida; cont_d++) // Verifica se o cliente possui dividas antes de excluir
    {
        if(strcmp(divida_lista[cont_d].cpf_cliente, cpf_temp) == 0)
        {
            printf("\n== CLIENTE POSSUI DIVIDAS E NAO PODE SER EXCLUIDO ==\n");
            return;
        }
    }

    int pos_c = BuscaCliente(*cliente_lista, *quantidade_cliente, cpf_temp); // Armazena o indice do cliente a excluir

    for(int cont_c = pos_c; cont_c < *quantidade_cliente - 1; cont_c++) // Desloca os registros para preencher o espaco do excluido
        (*cliente_lista)[cont_c] = (*cliente_lista)[cont_c + 1];

    (*quantidade_cliente)--; // Decrementa o contador de clientes
    printf("\n== CLIENTE EXCLUIDO COM SUCESSO! ==\n");
}

void ExcluirEmpresa(Empresa **empresa_lista, int *quantidade_empresa,
                    Divida *divida_lista, int quantidade_divida) // Exclui uma empresa, bloqueando se houver dividas vinculadas
{
    if(*quantidade_empresa == 0) // Verifica se ha empresas cadastradas
    {
        printf("\n== NENHUMA EMPRESA CADASTRADA! ==\n");
        return;
    }

    char cnpj_temp[15]; // Variavel temporaria para o CNPJ de busca

    printf("\nDigite o CNPJ da empresa a excluir: ");
    scanf("%14s", cnpj_temp);

    if(BuscaEmpresa(*empresa_lista, *quantidade_empresa, cnpj_temp) == -1) // Verifica se a empresa existe
    {
        printf("\n== EMPRESA NAO ENCONTRADA ==\n");
        return;
    }

    for(int cont_d = 0; cont_d < quantidade_divida; cont_d++) // Verifica se a empresa possui dividas antes de excluir
    {
        if(strcmp(divida_lista[cont_d].cnpj_empresa, cnpj_temp) == 0)
        {
            printf("\n== EMPRESA POSSUI DIVIDAS E NAO PODE SER EXCLUIDA ==\n");
            return;
        }
    }

    int pos_e = BuscaEmpresa(*empresa_lista, *quantidade_empresa, cnpj_temp); // Armazena o indice da empresa a excluir

    for(int cont_e = pos_e; cont_e < *quantidade_empresa - 1; cont_e++) // Desloca os registros para preencher o espaco da excluida
        (*empresa_lista)[cont_e] = (*empresa_lista)[cont_e + 1];

    (*quantidade_empresa)--; // Decrementa o contador de empresas
    printf("\n== EMPRESA EXCLUIDA COM SUCESSO! ==\n");
}

void ExcluirDivida(Divida **divida_lista, int *quantidade_divida) // Exclui uma divida identificada pelo CPF e CNPJ
{
    if(*quantidade_divida == 0) // Verifica se ha dividas cadastradas
    {
        printf("\n== NENHUMA DIVIDA CADASTRADA! ==\n");
        return;
    }

    char cpf_temp[12]; // Variavel temporaria para o CPF de busca
    char cnpj_temp[15]; // Variavel temporaria para o CNPJ de busca

    printf("\nDigite o CPF do cliente: ");
    scanf("%11s", cpf_temp);
    printf("\nDigite o CNPJ do credor: ");
    scanf("%14s", cnpj_temp);

    if(BuscaDivida(*divida_lista, *quantidade_divida, cpf_temp, cnpj_temp) == -1) // Verifica se a divida existe
    {
        printf("\n== DIVIDA NAO ENCONTRADA ==\n");
        return;
    }

    int pos_d = BuscaDivida(*divida_lista, *quantidade_divida, cpf_temp, cnpj_temp); // Armazena o indice da divida a excluir

    for(int cont_d = pos_d; cont_d < *quantidade_divida - 1; cont_d++) // Desloca os registros para preencher o espaco da excluida
        (*divida_lista)[cont_d] = (*divida_lista)[cont_d + 1];

    (*quantidade_divida)--; // Decrementa o contador de dividas
    printf("\n== DIVIDA EXCLUIDA COM SUCESSO! ==\n");
}

void GerarRelatorio(Divida *divida_lista, int quantidade_divida,
                    Cliente *cliente_lista, int quantidade_cliente,
                    Empresa *empresa_lista, int quantidade_empresa) // Gera um relatorio em arquivo .txt com filtros por CPF, CNPJ, valor ou todas as dividas
{
    if(quantidade_divida == 0) // Verifica se ha dividas cadastradas
    {
        printf("\n== NENHUMA DIVIDA CADASTRADA! ==\n");
        return;
    }

    int opcao_relatorio; // Armazena a opcao de relatorio escolhida pelo usuario
    printf("\n _____________________________________________________\n");
    printf("\n ==   Gerar relatorio por:                           ==");
    printf("\n ==   CPF do Cliente    [1]                          ==");
    printf("\n ==   CNPJ da Empresa   [2]                          ==");
    printf("\n ==   Valor Minimo      [3]                          ==");
    printf("\n ==   Todas as dividas  [4]                          ==");
    printf("\n _____________________________________________________\n");
    printf("\nDigite a opcao desejada: ");
    scanf("%d", &opcao_relatorio);

    char cpf_temp[12]; // Variavel temporaria para filtro por CPF
    char cnpj_temp[15]; // Variavel temporaria para filtro por CNPJ
    float valor_minimo; // Variavel para filtro por valor minimo

    if(opcao_relatorio == 1) // Le o CPF para filtrar o relatorio
    {
        printf("\nDigite o CPF do cliente: ");
        scanf("%11s", cpf_temp);
    }
    else if(opcao_relatorio == 2) // Le o CNPJ para filtrar o relatorio
    {
        printf("\nDigite o CNPJ da empresa: ");
        scanf("%14s", cnpj_temp);
    }
    else if(opcao_relatorio == 3) // Le o valor minimo para filtrar o relatorio
    {
        printf("\nDigite o valor minimo: ");
        scanf("%f", &valor_minimo);
    }
    else if(opcao_relatorio != 4) // Opcao 4 lista tudo, qualquer outro valor e invalido
    {
        printf("\n== OPCAO INVALIDA ==\n");
        return;
    }

    FILE *arquivo = fopen("relatorio_dividas.txt", "w"); // Cria ou sobrescreve o arquivo de relatorio
    if(arquivo == NULL)
    {
        printf("\n== ERRO AO CRIAR ARQUIVO ==\n"); // Exibe erro se o arquivo nao puder ser criado
        return;
    }

    // Escreve o cabecalho do relatorio no arquivo
    fprintf(arquivo, "=========================================================\n");
    fprintf(arquivo, "         SGN ASSESSORIA FINANCEIRA - RELATORIO           \n");
    fprintf(arquivo, "=========================================================\n\n");

    int num_registro = 1; // Contador de registros incluidos no relatorio

    for(int cont_d = 0; cont_d < quantidade_divida; cont_d++) // Percorre todas as dividas para aplicar o filtro
    {
        int imprime = 0; // Flag que indica se o registro atual deve ser incluido no relatorio

        if(opcao_relatorio == 1 && strcmp(divida_lista[cont_d].cpf_cliente, cpf_temp) == 0) // Verifica filtro por CPF
            imprime = 1;
        else if(opcao_relatorio == 2 && strcmp(divida_lista[cont_d].cnpj_empresa, cnpj_temp) == 0) // Verifica filtro por CNPJ
            imprime = 1;
        else if(opcao_relatorio == 3 && divida_lista[cont_d].valor >= valor_minimo) // Verifica filtro por valor minimo
            imprime = 1;
        else if(opcao_relatorio == 4) // Opcao 4 inclui todas as dividas
            imprime = 1;

        if(imprime == 1) // Grava o registro no arquivo de relatorio
        {
            fprintf(arquivo, "---------------------------------------------------------\n");
            fprintf(arquivo, " REGISTRO %d\n", num_registro);
            fprintf(arquivo, "---------------------------------------------------------\n");
            fprintf(arquivo, " DIVIDA\n");
            fprintf(arquivo, "   Valor: R$%.2f\n\n", divida_lista[cont_d].valor);
            fprintf(arquivo, " CLIENTE\n");
            fprintf(arquivo, "   CPF: %s\n", divida_lista[cont_d].cpf_cliente);
            fprintf(arquivo, "   Nome: %s\n", cliente_lista[BuscaCliente(cliente_lista, quantidade_cliente, divida_lista[cont_d].cpf_cliente)].nome_cliente);
            fprintf(arquivo, "   Idade: %d anos\n\n", cliente_lista[BuscaCliente(cliente_lista, quantidade_cliente, divida_lista[cont_d].cpf_cliente)].idade_cliente);
            fprintf(arquivo, " EMPRESA CREDORA\n");
            fprintf(arquivo, "   CNPJ: %s\n", divida_lista[cont_d].cnpj_empresa);
            fprintf(arquivo, "   Nome: %s\n", empresa_lista[BuscaEmpresa(empresa_lista, quantidade_empresa, divida_lista[cont_d].cnpj_empresa)].nome_empresa);
            fprintf(arquivo, "   Produto: %s\n\n", empresa_lista[BuscaEmpresa(empresa_lista, quantidade_empresa, divida_lista[cont_d].cnpj_empresa)].produto);

            num_registro++; // Incrementa o contador de registros gravados
        }
    }

    // Escreve o rodape com o total de registros
    fprintf(arquivo, "=========================================================\n");
    fprintf(arquivo, " Total de registros: %d\n", num_registro - 1);
    fprintf(arquivo, "=========================================================\n");

    fclose(arquivo); // Fecha o arquivo de relatorio apos a gravacao
    printf("\n== RELATORIO GERADO: relatorio_dividas.txt ==\n");
}


// Inicio
int main()
{
    int flag, flag_struct; // flag: opcao do menu principal / flag_struct: opcao do sub menu de entidade

    int capacidade_cliente = INCREMENTO, quantidade_cliente = 0; // Capacidade inicial e contador de clientes
    int capacidade_empresa = INCREMENTO, quantidade_empresa = 0; // Capacidade inicial e contador de empresas
    int capacidade_divida = INCREMENTO, quantidade_divida = 0; // Capacidade inicial e contador de dividas

    Cliente *cliente_lista = malloc(capacidade_cliente * sizeof(Cliente)); // Aloca o vetor dinamico de clientes
    if(cliente_lista == NULL) // Verifica se a alocacao inicial falhou
    {
        printf("\n== ERRO: FALHA NA ALOCACAO DE MEMORIA ==\n");
        exit(1); // Encerra o programa pois nao e seguro continuar sem memoria
    }

    Empresa *empresa_lista = malloc(capacidade_empresa * sizeof(Empresa)); // Aloca o vetor dinamico de empresas
    if(empresa_lista == NULL) // Verifica se a alocacao inicial falhou
    {
        printf("\n== ERRO: FALHA NA ALOCACAO DE MEMORIA ==\n");
        exit(1); // Encerra o programa pois nao e seguro continuar sem memoria
    }

    Divida *divida_lista = malloc(capacidade_divida * sizeof(Divida)); // Aloca o vetor dinamico de dividas
    if(divida_lista == NULL) // Verifica se a alocacao inicial falhou
    {
        printf("\n== ERRO: FALHA NA ALOCACAO DE MEMORIA ==\n");
        exit(1); // Encerra o programa pois nao e seguro continuar sem memoria
    }

    CarregarClientes(&cliente_lista, &quantidade_cliente, &capacidade_cliente); // Carrega os clientes salvos em disco
    CarregarEmpresas(&empresa_lista, &quantidade_empresa, &capacidade_empresa); // Carrega as empresas salvas em disco
    CarregarDividas(&divida_lista, &quantidade_divida, &capacidade_divida); // Carrega as dividas salvas em disco

    do
    {
        Menu(); // Exibe o menu principal
        scanf("%d", &flag);

        if(flag == 1) // Opcao Cadastrar
        {
            Qual_Struct(); // Exibe o sub menu de entidade
            scanf("%d", &flag_struct);

            if(flag_struct == 1)
                CadastrarCliente(&cliente_lista, &quantidade_cliente, &capacidade_cliente);
            else if(flag_struct == 2)
                CadastrarEmpresa(&empresa_lista, &quantidade_empresa, &capacidade_empresa);
            else if(flag_struct == 3)
                CadastrarDivida(&divida_lista, &quantidade_divida, &capacidade_divida,
                                cliente_lista, quantidade_cliente,
                                empresa_lista, quantidade_empresa);
            else
                printf("\n== OPCAO INVALIDA ==\n");
        }
        else if(flag == 2) // Opcao Consultar
        {
            Qual_Struct(); // Exibe o sub menu de entidade
            scanf("%d", &flag_struct);

            if(flag_struct == 1)
                ConsultarCliente(cliente_lista, quantidade_cliente);
            else if(flag_struct == 2)
                ConsultarEmpresa(empresa_lista, quantidade_empresa);
            else if(flag_struct == 3)
                ConsultarDivida(divida_lista, quantidade_divida,
                                cliente_lista, quantidade_cliente,
                                empresa_lista, quantidade_empresa);
            else
                printf("\n== OPCAO INVALIDA ==\n");
        }
        else if(flag == 3) // Opcao Atualizar
        {
            Qual_Struct(); // Exibe o sub menu de entidade
            scanf("%d", &flag_struct);

            if(flag_struct == 1)
                AtualizarCliente(cliente_lista, quantidade_cliente);
            else if(flag_struct == 2)
                AtualizarEmpresa(empresa_lista, quantidade_empresa);
            else if(flag_struct == 3)
                AtualizarDivida(divida_lista, quantidade_divida);
            else
                printf("\n== OPCAO INVALIDA ==\n");
        }
        else if(flag == 4) // Opcao Excluir
        {
            Qual_Struct(); // Exibe o sub menu de entidade
            scanf("%d", &flag_struct);

            if(flag_struct == 1)
                ExcluirCliente(&cliente_lista, &quantidade_cliente,
                               divida_lista, quantidade_divida);
            else if(flag_struct == 2)
                ExcluirEmpresa(&empresa_lista, &quantidade_empresa,
                               divida_lista, quantidade_divida);
            else if(flag_struct == 3)
                ExcluirDivida(&divida_lista, &quantidade_divida);
            else
                printf("\n== OPCAO INVALIDA ==\n");
        }
        else if(flag == 5) // Opcao Listar
        {
            Qual_Struct(); // Exibe o sub menu de entidade
            scanf("%d", &flag_struct);

            if(flag_struct == 1)
                ListarClientes(cliente_lista, quantidade_cliente);
            else if(flag_struct == 2)
                ListarEmpresas(empresa_lista, quantidade_empresa);
            else if(flag_struct == 3)
                ListarDividas(divida_lista, quantidade_divida);
            else
                printf("\n== OPCAO INVALIDA ==\n");
        }
        else if(flag == 6) // Opcao Gerar Relatorio
        {
            GerarRelatorio(divida_lista, quantidade_divida,
                           cliente_lista, quantidade_cliente,
                           empresa_lista, quantidade_empresa);
        }
        else if(flag == 0) // Sair do Programa
        {
            printf("\n== PROGRMAMA FINALIZADO ==\n");
        }
        else
                printf("\n== OPCAO INVALIDA ==\n");

    }
    while(flag != 0); // Continua ate o usuario escolher sair (opcao 0)

    // Salva todos os dados em disco antes de encerrar o programa
    SalvarClientes(cliente_lista, quantidade_cliente);
    SalvarEmpresas(empresa_lista, quantidade_empresa);
    SalvarDividas(divida_lista, quantidade_divida);

    // Libera toda a memoria alocada dinamicamente
    free(cliente_lista);
    free(empresa_lista);
    free(divida_lista);

    return 0;
}
