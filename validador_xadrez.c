#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Declaração de variáveis
#define NUM_ESTADOS 64
#define ESTADO_ERRO 64 

// ESTADO_ERRO -> qerr

// Matriz de Transição Delta(Q, Sigma) 
int transicao[NUM_ESTADOS + 1][NUM_ESTADOS];
// 65 linhas (0-63 posicoes do tabuleiro + 64 estado de erro) -> estado inicial (q0)
// 64 colunas (0-63 posicoes do tabuleiro) -> estado final (q1)

// Converte coordenada (ex: "a1") para índice 0-63 
int coord_to_idx(char col, char lin) {

    // validador de posicao 
    if (col < 'a' || col > 'h' || lin < '1' || lin > '8') {
        return -1;
    }
    
    return (lin - '1') * 8 + (col - 'a');
}

// Converte índice de volta para string (para imprimir o caminho) 
void idx_to_coord(int idx, char *res) {
    // converte linha do tabuleiro (letra)
    res[0] = (idx % 8) + 'a';
    // converte coluna do tabuleiro (número)
    res[1] = (idx / 8) + '1';
    res[2] = '\0';
}

// Popula o autômato aplicando cálculo 

void popular_automato() {
    // Gerar estados iniciais (linhas da tabela de transicao)
    for (int i = 0; i < NUM_ESTADOS; i++) { 
        int x1 = i % 8; // Coluna (0-7)
        int y1 = i / 8; // Linha (0-7)
        
        // Gerar estados finais (colunas da tabela de transicao)
        for (int j = 0; j < NUM_ESTADOS; j++) {
            int x2 = j % 8;
            int y2 = j / 8;

            // Compara posicoes (linha e coluna) dos estados 
            // abs permite ir de cima ou abaixo, direita ou esquerda
            int dx = abs(x1 - x2);
            int dy = abs(y1 - y2);

            // Validação do movimento em L
            // dx == 1 && dy == 2: 1 deslocamento horizontal e 2 deslocamentos verticais
            // x == 2 && dy == 1: 2 deslocamentos horizontais e 1 deslocamentos vertical
            if ((dx == 1 && dy == 2) || (dx == 2 && dy == 1)) {
                transicao[i][j] = j; // Transição válida: vai para o estado da casa de destino [cite: 30]
            } else {
                transicao[i][j] = ESTADO_ERRO; // Transição inválida: vai para o sumidouro qerr [cite: 31]
            }
        }
    }
    
    // O Estado de Erro (64) é um estado poço: qualquer símbolo que derive para este estado se mantém
    // Preenche toda linha de estado origem Qerro com estados de erros
    for (int j = 0; j < NUM_ESTADOS; j++) {
        transicao[ESTADO_ERRO][j] = ESTADO_ERRO;
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Uso: %s <sequencia_de_movimentos>\n", argv[0]);
        return 1;
    }

    // Construir a matriz de transição 
    popular_automato(); 

    // Ler cadeia de entrada
    char *cadeia = argv[1];
    int tam = strlen(cadeia);

    // Validação de formato (tamanho deve ser par, ex: a1b3c5)
    if (tam < 2 || tam % 2 != 0) {
        printf("REJEITADA\n");
        printf("Formatação de Estado Inicial Inválida\n");
        return 0;
    }

    // Define estado inicial como primeira dupla de entrada 
    int estado_atual = coord_to_idx(cadeia[0], cadeia[1]);

    if (estado_atual == -1) {
        printf("REJEITADA\n");
        printf("Estado Inicial fora do tabuleiro\n");
        return 0;
    }

    // Buffer para armazenar o caminho percorrido 
    char caminho[2048] = "";

    // Armazena coordenada atráves do indice
    char temp[4];

    idx_to_coord(estado_atual, temp);

    strcat(caminho, temp);

    // Processamento dos símbolos subsequentes
    for (int i = 2; i < tam; i += 2) {
        int proximo_simbolo = coord_to_idx(cadeia[i], cadeia[i+1]);
        
        // Rejeitado por formato inválido
        if (proximo_simbolo == -1) {
            printf("REJEITADA\n");
            printf("Ponto de Quebra: %s\n", temp);
            return 0;
        }

        // Aplicação da Função de Transição Delta 
        estado_atual = transicao[estado_atual][proximo_simbolo];

        // Se transitar para o estado de erro, a cadeia é interrompida 
        if (estado_atual == ESTADO_ERRO) {
            printf("REJEITADA\n");
            printf("Ponto de Quebra: %s\n", temp);
            return 0;
        }

        // Adiciona ao caminho para o print final
        idx_to_coord(estado_atual, temp);
        strcat(caminho, " ");
        strcat(caminho, temp);
    }

    // Se terminou o processamento sem transitar para qerr, é ACEITA
    printf("ACEITA\n%s\n", caminho);

    return 0;
}
