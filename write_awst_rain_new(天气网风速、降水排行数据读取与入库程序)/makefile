# target file
TARG_A = read_awst_rain
TARG_B = write_awst_rain
TARG_C = write_awst_wd


# source file
SOUR_A = read_awst_rain.c
SOUR_B = write_awst_rain.c
SOUR_C = write_awst_wd.c



# user lib
USERLIB = common.a

# complier options
CFLAGS = -I$(ORACLE_HOME)/rdbms/demo -I$(ORACLE_HOME)/rdbms/public -L$(ORACLE_HOME)/lib -L$(ORACLE_HOME)/rdbms/lib

COMLIBS = -lclntsh `cat $(ORACLE_HOME)/lib/sysliblist` -ldl -lm 

# all
all: $(TARG_A) $(TARG_B) $(TARG_C)

$(TARG_A):      $(SOUR_A) $(USERLIB)
	/usr/bin/gcc $(CFLAGS) -o $(TARG_A) $(SOUR_A) $(USERLIB) $(COMLIBS)
$(TARG_B):      $(SOUR_B) $(USERLIB)
	/usr/bin/gcc $(CFLAGS) -o $(TARG_B) $(SOUR_B) $(USERLIB) $(COMLIBS)
$(TARG_C):      $(SOUR_C) $(USERLIB)
	/usr/bin/gcc $(CFLAGS) -o $(TARG_C) $(SOUR_C) $(USERLIB) $(COMLIBS)


#clean
clean:
	rm -f $(TARG_A)
	rm -f $(TARG_B)
	rm -f $(TARG_C)
	

	
	