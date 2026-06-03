Nome: Anna Carolina Rezende Garrido 104422984

Funcionamento do LBA:

1. Carregamento do Arquivo e Pré-processamento
Todo o processo inicia-se no carregamento do arquivo de entrada, no qual são inputadas as regras de transição que formam a gramática a ser validada pelo LBA. O número inicial de regras é lido e um laço de repetição é aplicado para a leitura de todas as regras.
Para otimizar o processamento e a performance da máquina, cada estado em formato de string (como q0, q1 ou qacc) é traduzido e mapeado para um índice inteiro único (onde q0 vira 0 e o estado final qacc vira -1). Essa conversão numérica elimina a necessidade de comparações custosas de strings durante a execução do autômato.

2. Estrutura de Dados e Matriz de Índices
Para armazenar as transições de forma estática, foi utilizada uma estrutura de vetor de structs.
O grande diferencial do projeto foi a criação de uma Matriz de Índices auxiliar. Após a carga, as transições são ordenadas pelo estado de origem via Bubble Sort, que é essencial caso as regras sejam configuradas fora da ordem de estado. 
A matriz auxiliar mapeia o índice de início e a quantidade de regras que cada estado possui. Isso transforma a busca linear por transições em um acesso focado de complexidade próxima a $O(1)$, onde a máquina salta direto para o bloco de regras do estado atual.

3. Derivação e Validação da Cadeia
A fita do LBA foi implementada utilizando um vetor estático isolado por barreiras físicas rígidas: o delimitador de início $ na posição 0 e o preenchimento do espaço restante da memória com -1, logo após o caractere de fim de dados \0 (que mapeia o _ do arquivo). 
O ciclo de controle segue os seguintes passos:
- Leitura e Sobrescrita Otimizada: O cabeçote lê o caractere atual da fita e realiza uma comparação ASCII antes de reescrever o novo símbolo, evitando escritas desnecessárias em memória se os valores já forem iguais.
- Verificação de Fronteiras: A movimentação do cabeçote (L ou R) possui condicionais explícitas de borda. Se a máquina tentar invadir a posição do $ ou pisar na área isolada por -1, o processamento é interrompido imediatamente por violação de limite.
- Critério de Parada: O critério de aceite utilizado é o estado final. No topo do looping, se o estado atual atingir o índice -1 (qacc), a cadeia é validada instantaneamente. Caso o par (estado, símbolo) não possua transição mapeada na matriz de índices, o laço quebra e a cadeia é declarada inválida.



