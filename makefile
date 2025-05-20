BIN=process
SRC=$(wildcard *.c)
OBJ=$(SRC:.c=.o)
CC=gcc
RM=rm -f
  
$(BIN):$(OBJ)
	@$(CC) $^ -o $@
	@echo "链接 $^ 成 $@"
%.o:%.c
	$(CC) -c $< -g  
	@echo "编译... $< 成 $@"
  
.PHONY:clean
clean:
	@$(RM) $(OBJ) $(BIN)
                                                                             
.PHONY:test                                         
test:                       
	@echo $(BIN)     
	@echo $(SRC)                               
	@echo $(OBJ) 
