#include <stdio.h>
#include <stdlib.h>
#include <string.h> // Permitida apenas no pré-processamento/carregamento

#define MAX_REGRAS 500
#define MAX_ESTADOS 100
#define MAX_FITA 2048

// Matriz de transicao
typedef struct {
    int estado_inicial;
    char entrada;
    int estado_final;
    char sobrescrita;
    char mov;
} Transicao;

// Otimizacao de acesso na matriz, busca por índice
typedef struct {
    int inicio;
    int qtd;
} IndiceEstado;

// Declara variáveis
Transicao regras[MAX_REGRAS];
IndiceEstado indices[MAX_ESTADOS];
char fita[MAX_FITA];

// Mapear de estado para indice 
int mapear_estado_para_int(const char *nome_estado) {
    // Tratamento especial para o estado de aceitação qacc -> -1
    if (nome_estado[0] == 'q' && nome_estado[1] == 'a' && nome_estado[2] == 'c' && nome_estado[3] == 'c') {
        return -1;
    }
    
    // Converte estados no formato q0, q1, q2... extraindo o número via ASCII
    if (nome_estado[0] == 'q') {
        int num = 0;
        int i = 1;
        while (nome_estado[i] >= '0' && nome_estado[i] <= '9') {
            num = num * 10 + (nome_estado[i] - '0');
            i++;
        }
        return num;
    }
    
    return -2; 
}

// Ordenação (caso as regras sejam colocadas na entrada fora de ordem)
void carregar_e_ordenar_regras(const char *caminho_arquivo, int *total_regras) {
    FILE *file = fopen(caminho_arquivo, "r");
    if (!file) {
        printf("Erro ao abrir o arquivo de especificações.\n");
        exit(1);
    }

    // Lê o n para definir quantidade de regras
    if (fscanf(file, "%d\n", total_regras) != 1) {
        printf("Arquivo de entrada inválido: erro ao ler quantidade de regras.\n");
        fclose(file);
        exit(1);
    }

    if (*total_regras > MAX_REGRAS) {
        printf("Erro: Quantidade de regras excede o limite estático máximo.\n");
        fclose(file);
        exit(1);
    }

    char est_ini[20], est_fim[20];
    char entrada, sobrescrita, mov;
    int linhas_lidas = 0;

    // Lendo linha por linha no formato: estado_origem,entrada,estado_destino,sobrescrita,movimento
    while (linhas_lidas < *total_regras && 
           fscanf(file, "%[^,],%c,%[^,],%c,%c\n", est_ini, &entrada, est_fim, &sobrescrita, &mov) == 5) {
        
        regras[linhas_lidas].estado_inicial = mapear_estado_para_int(est_ini);
        regras[linhas_lidas].entrada = entrada;
        regras[linhas_lidas].estado_final = mapear_estado_para_int(est_fim);
        regras[linhas_lidas].sobrescrita = sobrescrita;
        regras[linhas_lidas].mov = mov;
        
        linhas_lidas++;
    }

    // Validacao de qtde de linhas com n 
    if (linhas_lidas != *total_regras) {
        printf("Arquivo de entrada inválido: quantidade de linhas divergente.\n");
        fclose(file);
        exit(1);
    }
    fclose(file);

    // Ordenação para manter estrutura ordenada
    for (int i = 0; i < *total_regras - 1; i++) {
        for (int j = 0; j < *total_regras - i - 1; j++) {
            if (regras[j].estado_inicial > regras[j + 1].estado_inicial) {
                Transicao temp = regras[j];
                regras[j] = regras[j + 1];
                regras[j + 1] = temp;
            }
        }
    }
}

// Constroi matriz de indices
void construir_matriz_indices(int total_regras) {
    for (int i = 0; i < MAX_ESTADOS; i++) {
        indices[i].inicio = -1;
        indices[i].qtd = 0;
    }

    for (int i = 0; i < total_regras; i++) {
        int est = regras[i].estado_inicial;
        
        // Ignora qacc se ele estiver incorretamente como origem
        if (est < 0 || est >= MAX_ESTADOS) continue; 

        if (indices[est].inicio == -1) {
            indices[est].inicio = i;
        }
        indices[est].qtd++;
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Uso: %s <arquivo_de_descricao> <mensagem>\n", argv[0]);
        return 1;
    }

    int total_regras = 0;

    // Carrega arquivo de entrada
    carregar_e_ordenar_regras(argv[1], &total_regras);
    construir_matriz_indices(total_regras);

    // Cria vetor como fita
    fita[0] = '$'; // Define limitador inicial
    int fita_idx = 1;
    int arg_idx = 0;
    
    // Copiando a string 
    while(argv[2][arg_idx] != '\0' && fita_idx < MAX_FITA - 2) {
        fita[fita_idx++] = argv[2][arg_idx++];
    }
    fita[fita_idx++] = '\0'; // Define limitador final
    
    // Barreira de segurança preenchendo o final com -1
    while(fita_idx < MAX_FITA) {
        fita[fita_idx++] = -1;
    }

    // Variáveis do LBA
    int estado_atual = 0; // q0
    int pos = 1;          // Começa a ler após o '$'
    int aceito = 0;

    while (1) {
        // Critério de Aceite: Chegou no estado de aceitação
        if (estado_atual == -1) { 
            aceito = 1;
            break;
        }

        char lido = fita[pos];

        // Converte O '\0' para '_'
        if (lido == '\0') lido = '_'; 

        // Busca otimizada na matriz de indices
        int start = indices[estado_atual].inicio;
        int count = indices[estado_atual].qtd;
        int transicao_encontrada = -1;

        // Procura apenas dentro do bloco do estado atual
        for (int i = start; i < start + count; i++) {
            if (regras[i].entrada == lido) {
                transicao_encontrada = i;
                break;
            }
        }

        // Combinação de estado e entrada fora da tabela
        if (transicao_encontrada == -1) {
            break; 
        }

        Transicao regra_atual = regras[transicao_encontrada];

        char escrever = regra_atual.sobrescrita;
        if (escrever == '_') escrever = '\0'; // Traduz de volta para o controle da fita
        
        if (fita[pos] != escrever) {
            fita[pos] = escrever;
        }

        estado_atual = regra_atual.estado_final;

        // Verificação de limites
        if (regra_atual.mov == 'L') {
            if (pos - 1 == 0) {
                break; 
            }
            pos--;
        } else if (regra_atual.mov == 'R') {
            if (fita[pos + 1] == -1) {
                break; 
            }
            pos++;
        }
    }

    // Resultado
    if (aceito) {
        printf("Cadeia Válida\n");
    } else {
        printf("Cadeia Inválida\n");
    }

    return 0;
}