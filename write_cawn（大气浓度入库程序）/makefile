# target file
TARG_A = write_pm
TARG_B = write_pm_record
TARG_C = write_o3
TARG_D = write_o3_record
TARG_E = write_mul
TARG_F = write_mul_record



# source file
SOUR_A = write_pm.c
SOUR_B = write_pm_record.c
SOUR_C = write_o3.c
SOUR_D = write_o3_record.c
SOUR_E = write_mul.c
SOUR_F = write_mul_record.c



# user lib
USERLIB = common.a

# complier options
CFLAGS = -I$(ORACLE_HOME)/rdbms/demo -I$(ORACLE_HOME)/rdbms/public -L$(ORACLE_HOME)/lib -L$(ORACLE_HOME)/rdbms/lib

COMLIBS = -lclntsh `cat $(ORACLE_HOME)/lib/sysliblist` -ldl -lm 

# all
all: $(TARG_A) $(TARG_B) $(TARG_C) $(TARG_D)  $(TARG_E) $(TARG_F)


$(TARG_A):      $(SOUR_A) $(USERLIB)
	/usr/bin/gcc $(CFLAGS) -o $(TARG_A) $(SOUR_A) $(USERLIB) $(COMLIBS)
$(TARG_B):      $(SOUR_B) $(USERLIB)
	/usr/bin/gcc $(CFLAGS) -o $(TARG_B) $(SOUR_B) $(USERLIB) $(COMLIBS)
$(TARG_C):      $(SOUR_C) $(USERLIB)
	/usr/bin/gcc $(CFLAGS) -o $(TARG_C) $(SOUR_C) $(USERLIB) $(COMLIBS)
$(TARG_D):      $(SOUR_D) $(USERLIB)
	/usr/bin/gcc $(CFLAGS) -o $(TARG_D) $(SOUR_D) $(USERLIB) $(COMLIBS)
$(TARG_E):      $(SOUR_E) $(USERLIB)
	/usr/bin/gcc $(CFLAGS) -o $(TARG_E) $(SOUR_E) $(USERLIB) $(COMLIBS)
$(TARG_F):      $(SOUR_F) $(USERLIB)
	/usr/bin/gcc $(CFLAGS) -o $(TARG_F) $(SOUR_F) $(USERLIB) $(COMLIBS)







#clean
clean:
	rm -f $(TARG_A)
	rm -f $(TARG_B)
	rm -f $(TARG_C)
	rm -f $(TARG_D)
	

	
	