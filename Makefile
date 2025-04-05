TARGET = Game.exe

SRC_DIR = resources\src
INCLUDE_DIR = include
LIB_DIR = lib

CC = gcc
GFLAGS = -I$(INCLUDE_DIR) -L$(LIB_DIR)

LIBS = -lraylib -lopengl32 -lgdi32 -lwinmm

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(SRCS:.c=.o)

$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(GFLAGS) $(LIBS)
	
	
%.o: %.c
	$(CC) -c $< -o $@ $(GFLAGS)
	
clean:
	@taskkill /F /IM $(TARGET) || echo "Game is not launched"
	del /Q $(SRC_DIR)\*.o $(TARGET) \
    exit 0; \

run: $(TARGET)
	@start "" "$(TARGET)"