SRC_PATH = ./src

INC_PATH = -I ./src

FLAGS =

OBJ_PATH = ./bin

EJECUTABLE = test
CC = gcc

LIBS = 

OBJ_FILES=$(OBJ_PATH)/main.o

################################################################################
################################################################################

all:$(OBJ_PATH) | $(OBJ_FILES)
	$(CC) $(FLAGS) $(OBJ_FILES) -o $(EJECUTABLE) $(LIBS)

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.c
	$(CC) $(FLAGS) $(INC_PATH) $< -c -o $@

$(OBJ_PATH):
	mkdir $@

clean:
	rm $(OBJ_PATH)/*
	rm $(EJECUTABLE)

			
################################################################################
################################################################################
