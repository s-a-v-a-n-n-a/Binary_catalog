GREEN   = @echo "\033[0;32m";
DFLT    = @echo "\033[0;m";
 
ifeq ($(ASAN), 1)
ASAN =-fsanitize=address
else
ASAN =
endif

COMPILATOR  = g++ -std=c++17
INC     	= 
OPT     	= -O1
DEF     	= -DNDEBUG
CFLAGS  	= $(OPT) $(DEF) $(ASAN) -g $(INC) 
DLL			= -ldl -lm -lpthread -pthread
OBJ     	= obj/
EXE     	= main
 
FILES   	= $(OBJ)main.o \
			  $(OBJ)catalog_tree_read_from_file.o \
			  $(OBJ)catalog_tree_user_work.o \
			  $(OBJ)catalog_tree_work_with_text.o \
			  $(OBJ)catalog_tree.o \
			  $(OBJ)Stack.o \
 
 
#
# Rules
#
 
all: $(FILES)
	$(GREEN)
	@echo 'Linking $@ (changed $?)';
	$(DFLT)
	@$(COMPILATOR) $(CFLAGS) $(FILES) $(SFML) $(DLL) -o $(EXE)
	
obj/%.o: %.cpp
	$(GREEN)
	@echo 'Building $@ (changed $?)';
	$(DFLT)
	@$(COMPILATOR) $(CFLAGS) $^ -c
	@mkdir -p $(@D)
	@mv $(@F) $@
	
clean:
	$(GREEN)
	@echo 'Cleaning the project';
	$(DFLT)
	@rm $(FILES)

run: all
	./$(EXE)