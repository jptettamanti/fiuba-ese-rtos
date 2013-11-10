SRC_PATH = ./

INC_PATH  = -I ./
INC_PATH += -I ../delay_malloc

FLAGS =

OBJ_PATH = ../../bin

EJECUTABLE = test
CC = gcc

LIBS = -lasound

OBJ_FILES  = $(OBJ_PATH)/echo_test.o
OBJ_FILES += $(OBJ_PATH)/echo.o
OBJ_FILES += $(OBJ_PATH)/delay.o

################################################################################
################################################################################

all:$(OBJ_PATH) $(OBJ_FILES)
	$(CC) $(FLAGS) $(OBJ_FILES) -o $(EJECUTABLE) $(LIBS)

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.c
	$(CC) $(FLAGS) $(INC_PATH) $< -c -o $@

$(OBJ_PATH):
	mkdir $@

clean:
	rm -r $(OBJ_PATH)
	rm $(EJECUTABLE)

			
################################################################################
################################################################################
