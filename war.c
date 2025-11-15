// ============================================================================
//         PROJETO WAR ESTRUTURADO - DESAFIO DE CÓDIGO
// ============================================================================
//        
// ============================================================================
//
// OBJETIVOS:
// - Modularizar completamente o código em funções especializadas.
// - Implementar um sistema de missões para um jogador.
// - Criar uma função para verificar se a missão foi cumprida.
// - Utilizar passagem por referência (ponteiros) para modificar dados e
//   passagem por valor/referência constante (const) para apenas ler.
// - Foco em: Design de software, modularização, const correctness, lógica de jogo.
//
// ============================================================================

// Inclusão das bibliotecas padrão necessárias para entrada/saída, alocação de memória, manipulação de strings e tempo.
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <locale.h>

// --- Constantes Globais ---
// Definem valores fixos para o número de territórios, missões e tamanho máximo de strings, facilitando a manutenção.
#define QTDE_TERRITORIOS 5
#define MAX_STR 50
#define MISAO_DESTRUIR_VERDE 1
#define MISAO_CONQUISTAR_3   2

// --- Estrutura de Dados ---
// Define a estrutura para um território, contendo seu nome, a cor do exército que o domina e o número de tropas.
typedef struct {
    char nome[MAX_STR];
    char cor[MAX_STR];
    int tropas;
} Territorio;
// --- Protótipos das Funções ---
// Declarações antecipadas de todas as funções que serão usadas no programa, organizadas por categoria.
// Funções de setup e gerenciamento de memória:
Territorio* alocarMapa();
void inicializarTerritorios(Territorio *mapa);
void liberarMemoria(Territorio *mapa);
// Funções de interface com o usuário:
void exibirMenuPrincipal();
void exibirMapa(const Territorio *mapa);
void exibirMissao(int missao);
// Funções de lógica principal do jogo:
void faseDeAtaque(Territorio *mapa, const char *corJogador);
void simularAtaque(Territorio *mapa, int atq, int def, const char *corJogador);

int sortearMissao();
int verificarVitoria(const Territorio *mapa, const char *corJogador, int missao);
// Função utilitária:
void limparBufferEntrada();
// --- Função Principal (main) ---
// Função principal que orquestra o fluxo do jogo, chamando as outras funções em ordem.
int main() {
    setlocale(LC_ALL, "portuguese");
    srand(time(NULL));

    Territorio *mapa = alocarMapa();
    if (!mapa) {
        printf("Erro ao alocar memória!\n");
        return 1;
    }

    inicializarTerritorios(mapa);

    char corJogador[MAX_STR] = "Azul";
    int missao = sortearMissao();

    int opcao;
    int venceu = 0;

    do {
        printf("\n====================================\n");
        printf("             MAPA ATUAL\n");
        printf("====================================\n");
        exibirMapa(mapa);

        printf("\n------------------------------------\n");
        printf("           SUA MISSÃO\n");
        printf("------------------------------------\n");
        exibirMissao(missao);

        exibirMenuPrincipal();
        printf("\nEscolha: ");
        scanf("%d", &opcao);
        limparBufferEntrada();

        switch (opcao) {
            case 1:
                faseDeAtaque(mapa, corJogador);
                break;
            case 2:
                venceu = verificarVitoria(mapa, corJogador, missao);
                if (venceu)
                    printf("\n🎉 MISSÃO CUMPRIDA! VOCÊ VENCEU!\n");
                else
                    printf("\n❌ Missão ainda não concluída.\n");
                break;
            case 0:
                printf("Encerrando o jogo...\n");
                break;
            default:
                printf("Opção inválida!\n");
        }

    } while (opcao != 0 && !venceu);

    liberarMemoria(mapa);
    return 0;
}
    // 1. Configuração Inicial (Setup):
    // - Define o locale para português.
    // - Inicializa a semente para geração de números aleatórios com base no tempo atual.
    // - Aloca a memória para o mapa do mundo e verifica se a alocação foi bem-sucedida.
    // - Preenche os territórios com seus dados iniciais (tropas, donos, etc.).
    // - Define a cor do jogador e sorteia sua missão secreta.
    // Aloca mapa dinamicamente
Territorio* alocarMapa() {
    return calloc(QTDE_TERRITORIOS, sizeof(Territorio));
}

// Inicializa territórios padrão
void inicializarTerritorios(Territorio *mapa) {
    strcpy(mapa[0].nome, "Alfa");
    strcpy(mapa[0].cor, "Vermelho");
    mapa[0].tropas = 3;

    strcpy(mapa[1].nome, "Bravo");
    strcpy(mapa[1].cor, "Verde");
    mapa[1].tropas = 4;

    strcpy(mapa[2].nome, "Charlie");
    strcpy(mapa[2].cor, "Amarelo");
    mapa[2].tropas = 5;

    strcpy(mapa[3].nome, "Delta");
    strcpy(mapa[3].cor, "Azul");
    mapa[3].tropas = 3;

    strcpy(mapa[4].nome, "Eco");
    strcpy(mapa[4].cor, "Verde");
    mapa[4].tropas = 2;
}

void liberarMemoria(Territorio *mapa) {
    free(mapa);
}

void exibirMenuPrincipal() {
    printf("\n====================================\n");
    printf("              MENU\n");
    printf("====================================\n");
    printf("1 - Atacar\n");
    printf("2 - Verificar Missão\n");
    printf("0 - Sair\n");
}

void exibirMapa(const Territorio *mapa) {
    printf("\n%-10s  %-12s  %-8s\n", "Nome", "Cor", "Tropas");
    printf("--------------------------------------\n");

    for (int i = 0; i < QTDE_TERRITORIOS; i++) {
        printf("%d) %-10s %-12s %-8d\n",
            i + 1, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
}

void exibirMissao(int missao) {
    if (missao == MISAO_DESTRUIR_VERDE) {
        printf("Destruir todos os territórios Verdes.\n");
    } else if (missao == MISAO_CONQUISTAR_3) {
        printf("Conquistar 3 territórios.\n");
    }
}
    // 2. Laço Principal do Jogo (Game Loop):
    // - Roda em um loop 'do-while' que continua até o jogador sair (opção 0) ou vencer.
    // - A cada iteração, exibe o mapa, a missão e o menu de ações.
    // - Lê a escolha do jogador e usa um 'switch' para chamar a função apropriada:
    //   - Opção 1: Inicia a fase de ataque.
    //   - Opção 2: Verifica se a condição de vitória foi alcançada e informa o jogador.
    //   - Opção 0: Encerra o jogo.
    // - Pausa a execução para que o jogador possa ler os resultados antes da próxima rodada.

    // 3. Limpeza:
    // - Ao final do jogo, libera a memória alocada para o mapa para evitar vazamentos de memória.

// --- Implementação das Funções ---

// alocarMapa():
// Aloca dinamicamente a memória para o vetor de territórios usando calloc.
// Retorna um ponteiro para a memória alocada ou NULL em caso de falha.

// inicializarTerritorios():
// Preenche os dados iniciais de cada território no mapa (nome, cor do exército, número de tropas).
// Esta função modifica o mapa passado por referência (ponteiro).

// liberarMemoria():
// Libera a memória previamente alocada para o mapa usando free.

// exibirMenuPrincipal():
// Imprime na tela o menu de ações disponíveis para o jogador.

// exibirMapa():
// Mostra o estado atual de todos os territórios no mapa, formatado como uma tabela.
// Usa 'const' para garantir que a função apenas leia os dados do mapa, sem modificá-los.

// exibirMissao():
// Exibe a descrição da missão atual do jogador com base no ID da missão sorteada.

// faseDeAtaque():
// Gerencia a interface para a ação de ataque, solicitando ao jogador os territórios de origem e destino.
// Chama a função simularAtaque() para executar a lógica da batalha.

// simularAtaque():
// Executa a lógica de uma batalha entre dois territórios.
// Realiza validações, rola os dados, compara os resultados e atualiza o número de tropas.
// Se um território for conquistado, atualiza seu dono e move uma tropa.

// sortearMissao():
// Sorteia e retorna um ID de missão aleatório para o jogador.

// verificarVitoria():
// Verifica se o jogador cumpriu os requisitos de sua missão atual.
// Implementa a lógica para cada tipo de missão (destruir um exército ou conquistar um número de territórios).
// Retorna 1 (verdadeiro) se a missão foi cumprida, e 0 (falso) caso contrário.

// limparBufferEntrada():
// Função utilitária para limpar o buffer de entrada do teclado (stdin), evitando problemas com leituras consecutivas de scanf e getchar.
    // Gerencia a fase de ataque
void faseDeAtaque(Territorio *mapa, const char *corJogador) {
    int origem, destino;

    printf("\nNúmero do território de origem (1-5): ");
    scanf("%d", &origem);
    printf("Número do território alvo (1-5): ");
    scanf("%d", &destino);
    limparBufferEntrada();

    simularAtaque(mapa, origem - 1, destino - 1, corJogador);
}

// Simula combate entre dois territórios
void simularAtaque(Territorio *mapa, int atq, int def, const char *corJogador) {
    if (atq < 0 || atq >= QTDE_TERRITORIOS || def < 0 || def >= QTDE_TERRITORIOS) {
        printf("Índices inválidos!\n");
        return;
    }

    if (strcmp(mapa[atq].cor, corJogador) != 0) {
        printf("Você só pode atacar usando seus próprios territórios!\n");
        return;
    }

    if (atq == def) {
        printf("Origem e destino não podem ser iguais!\n");
        return;
    }

    printf("\n🎲 Rolando dados...\n");
    int dadoAtaque = rand() % 6 + 1;
    int dadoDefesa = rand() % 6 + 1;

    printf("Ataque (%s): %d\n", mapa[atq].nome, dadoAtaque);
    printf("Defesa (%s): %d\n", mapa[def].nome, dadoDefesa);

    if (dadoAtaque >= dadoDefesa) {
        mapa[def].tropas--;

        if (mapa[def].tropas <= 0) {
            printf("\n🏆 Território conquistado!\n");
            strcpy(mapa[def].cor, corJogador);
            mapa[def].tropas = 1;
            mapa[atq].tropas--;
        } else {
            printf("✔ Defesa perdeu uma tropa!\n");
        }
    } else {
        mapa[atq].tropas--;
        printf("❌ Ataque falhou! Você perdeu uma tropa.\n");
    }
}

// Sorteia missão aleatória
int sortearMissao() {
    return (rand() % 2) + 1;
}

// Verifica se missão foi cumprida
int verificarVitoria(const Territorio *mapa, const char *corJogador, int missao) {
    if (missao == MISAO_DESTRUIR_VERDE) {
        for (int i = 0; i < QTDE_TERRITORIOS; i++) {
            if (strcmp(mapa[i].cor, "Verde") == 0)
                return 0;
        }
        return 1;
    }

    if (missao == MISAO_CONQUISTAR_3) {
        int cont = 0;
        for (int i = 0; i < QTDE_TERRITORIOS; i++) {
            if (strcmp(mapa[i].cor, corJogador) == 0)
                cont++;
        }
        return (cont >= 3);
    }

    return 0;
}

void limparBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}