all: build run

# source file list
CPP_FILES := main.cpp
CPP_FILES += system.cpp
CPP_FILES += files.cpp
CPP_FILES += account.cpp
CPP_FILES += algorithms.cpp
CPP_FILES += UIElements.cpp

# lib list
LIB_LIST := -lsfml-graphics
LIB_LIST += -lsfml-window
LIB_LIST += -lsfml-system
#LIB_LIST := -lsfml-graphics -lsfml-window -lsfml-system -lopengl32 -lfreetype -lwinmm -lgdi32
#CXX flags
CXX_FLAGS := -c
#CXX_FLAGS += -w

build:
	@g++ $(CPP_FILES) -o main.run $(LIB_LIST)
run:
	@./main.run