TARGET = star.exe

SRC_DIR = resources\src
INCLUDE_DIR = include
LIB_DIR = lib

RES_FILE = resources.res
RC_FILE = config\meta\resources.rc
ICON_PATH = config\meta\icon.ico

CC = gcc
GFLAGS = -I$(INCLUDE_DIR) -L$(LIB_DIR)

LIBS = -lraylib -lopengl32 -lgdi32 -lwinmm -mwindows

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(SRCS:.c=.o)

$(TARGET): $(OBJS) $(RES_FILE)
	$(CC) -o $@ $^ $(GFLAGS) $(LIBS)

$(RES_FILE): $(RC_FILE) $(ICON_PATH)
	windres $(RC_FILE) -O coff -o $(RES_FILE)

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