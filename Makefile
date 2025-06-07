TARGET = star.exe

SRC_DIR = resources\src
OBJ_DIR = resources\src\obj
INCLUDE_DIR = include
LIB_DIR = lib

RES_FILE = config\meta\resources.res
RC_FILE = config\meta\resources.rc
ICON_PATH = config\meta\icon.ico

CC = gcc
GFLAGS = -I$(INCLUDE_DIR) -L$(LIB_DIR)

LIBS = -lraylib -lopengl32 -lgdi32 -lwinmm -mwindows

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

$(TARGET): $(OBJS) $(RES_FILE)
	$(CC) -o $@ $^ $(GFLAGS) $(LIBS)

$(RES_FILE): $(RC_FILE) $(ICON_PATH)
	windres $(RC_FILE) -O coff -o $(RES_FILE)

$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(GFLAGS) $(LIBS)


$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) -c $< -o $@ $(GFLAGS)

clean:
	@taskkill /F /IM $(TARGET) || echo "Game is not launched"
	del /Q $(OBJ_DIR)\*.o $(TARGET) \
    exit 0; \

run: $(TARGET)
	@start "" "$(TARGET)"