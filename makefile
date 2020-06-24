COMPILER := c++
LIB := lib/
LINK := sqlite3
SRC := src/
BINARY := ha
FLAGS := -g3
RM := rm
BINFOLD := bin/

compile:
	$(COMPILER) $(FLAGS) -I$(LIB) -l$(LINK) $(SRC)*.cpp -o $(BINFOLD)$(BINARY)

clean:
	-$(RM) $(BINFOLD)$(BINARY)
