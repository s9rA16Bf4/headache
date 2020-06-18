COMPILER := c++
LIB := lib/
SRC := src/
BINARY := ha
FLAGS := -g3
RM := rm
BINFOLD := bin/

compile:
	$(COMPILER) $(FLAGS) -I$(LIB) $(SRC)*.cpp -o $(BINFOLD)$(BINARY)

clean:
	-$(RM) $(BINFOLD)$(BINARY)
