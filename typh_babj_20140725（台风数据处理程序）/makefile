# target file
TARG_A = proc_db_typh_babj_2014_new


# source file
SOUR_A = proc_db_typh_babj.c


# user lib
USERLIB = typh_babj.a

# complier options
CFLAGS = -I$(ORACLE_HOME)/rdbms/demo -I$(ORACLE_HOME)/rdbms/public -L$(ORACLE_HOME)/lib -L$(ORACLE_HOME)/rdbms/lib

COMLIBS = -lclntsh `cat $(ORACLE_HOME)/lib/sysliblist` -ldl -lm 

# all
all: $(TARG_A) $(TARG_B) $(TARG_C) $(TARG_D)

$(TARG_A):      $(SOUR_A) $(USERLIB)
	/usr/bin/gcc $(CFLAGS) -o $(TARG_A) $(SOUR_A) $(USERLIB) $(COMLIBS)


#clean
clean:
	rm -f $(TARG_A) $(TARG_B) $(TARG_C) $(TARG_D)
