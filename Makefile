# Variáveis
CC = gcc
CFLAGS = -Wall -g -Iinclude
EXEC = programa
SRC_DIR =  src



SRCS = $(wildcard $(SRC_DIR)/*.c)


#regra padrão
all: $(EXEC)
	
# Regra de compilação
$(EXEC): $(SRCS)
	$(CC) -o $@ $^

# Regra de compilação dos objetos
$(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:	
	rm -f $(SRC_DIR)/*.o 

