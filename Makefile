# Variáveis para facilitar a manutenção
CC = gcc
CFLAGS = -Wall -Wextra -std=c99
TARGET = validador_xadrez

# Regra padrão: compila o projeto
all: $(TARGET)

$(TARGET): validador_xadrez.c
	$(CC) $(CFLAGS) validador_xadrez.c -o $(TARGET)

# Regra para limpar arquivos binários
clean:
	rm -f $(TARGET)