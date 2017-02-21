# target file
TARG_A = write_CRO1_mdss
TARG_B = write_CRO2_mdss
TARG_C = write_CRO3_mdss
TARG_D = write_CRO4_mdss
TARG_E = write_CRO5_mdss
TARG_F = write_CRO6_mdss
TARG_G = write_CRO7_mdss
TARG_A1 = write_SOO1_mdss
TARG_B1 = write_SOO2_mdss
TARG_C1 = write_SOO3_mdss
TARG_D1 = write_SOO4_mdss
TARG_E1 = write_SOO5_mdss
TARG_A2 = write_PHO1_mdss
TARG_B2 = write_PHO2_mdss
TARG_C2 = write_PHO3_mdss
TARG_D2 = write_PHO4_mdss


# source file
SOUR_A = write_CRO1_mdss.c
SOUR_B = write_CRO2_mdss.c
SOUR_C = write_CRO3_mdss.c
SOUR_D = write_CRO4_mdss.c
SOUR_E = write_CRO5_mdss.c
SOUR_F = write_CRO6_mdss.c
SOUR_G = write_CRO7_mdss.c
SOUR_A1 = write_SO01_mdss.c
SOUR_B1 = write_SO02_mdss.c
SOUR_C1 = write_SO03_mdss.c
SOUR_D1 = write_SO04_mdss.c
SOUR_E1 = write_SO05_mdss.c
SOUR_A2 = write_PH01_mdss.c
SOUR_B2 = write_PH02_mdss.c
SOUR_C2 = write_PH03_mdss.c
SOUR_D2 = write_PH04_mdss.c


# user lib
USERLIB = common.a

# complier options
CFLAGS = -I$(ORACLE_HOME)/rdbms/demo -I$(ORACLE_HOME)/rdbms/public -L$(ORACLE_HOME)/lib -L$(ORACLE_HOME)/rdbms/lib

COMLIBS = -lclntsh `cat $(ORACLE_HOME)/lib/sysliblist` -ldl -lm 

# all
all: $(TARG_A) $(TARG_B) $(TARG_C) $(TARG_D) $(TARG_E) $(TARG_F) $(TARG_G) $(TARG_A1) $(TARG_B1) $(TARG_C1) $(TARG_D1) $(TARG_E1)  $(TARG_A2) $(TARG_B2) $(TARG_C2) $(TARG_D2)


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
$(TARG_G):      $(SOUR_G) $(USERLIB)
	/usr/bin/gcc $(CFLAGS) -o $(TARG_G) $(SOUR_G) $(USERLIB) $(COMLIBS)
$(TARG_A1):      $(SOUR_A1) $(USERLIB)
	/usr/bin/gcc $(CFLAGS) -o $(TARG_A1) $(SOUR_A1) $(USERLIB) $(COMLIBS)
$(TARG_B1):      $(SOUR_B1) $(USERLIB)
	/usr/bin/gcc $(CFLAGS) -o $(TARG_B1) $(SOUR_B1) $(USERLIB) $(COMLIBS)
$(TARG_C1):      $(SOUR_C1) $(USERLIB)
	/usr/bin/gcc $(CFLAGS) -o $(TARG_C1) $(SOUR_C1) $(USERLIB) $(COMLIBS)
$(TARG_D1):      $(SOUR_D1) $(USERLIB)
	/usr/bin/gcc $(CFLAGS) -o $(TARG_D1) $(SOUR_D1) $(USERLIB) $(COMLIBS)
$(TARG_E1):      $(SOUR_E) $(USERLIB)
	/usr/bin/gcc $(CFLAGS) -o $(TARG_E1) $(SOUR_E1) $(USERLIB) $(COMLIBS)
$(TARG_A2):      $(SOUR_A2) $(USERLIB)
	/usr/bin/gcc $(CFLAGS) -o $(TARG_A2) $(SOUR_A2) $(USERLIB) $(COMLIBS)
$(TARG_B2):      $(SOUR_B2) $(USERLIB)
	/usr/bin/gcc $(CFLAGS) -o $(TARG_B2) $(SOUR_B2) $(USERLIB) $(COMLIBS)
$(TARG_C2):      $(SOUR_C2) $(USERLIB)
	/usr/bin/gcc $(CFLAGS) -o $(TARG_C2) $(SOUR_C2) $(USERLIB) $(COMLIBS)
$(TARG_D2):      $(SOUR_D2) $(USERLIB)
	/usr/bin/gcc $(CFLAGS) -o $(TARG_D2) $(SOUR_D2) $(USERLIB) $(COMLIBS)





#clean
clean:
	rm -f $(TARG_A)
	rm -f $(TARG_B)
	rm -f $(TARG_C)
	rm -f $(TARG_D)
	rm -f $(TARG_E)
	rm -f $(TARG_F)
	rm -f $(TARG_G)
	rm -f $(TARG_A1)
	rm -f $(TARG_B1)
	rm -f $(TARG_C1)
	rm -f $(TARG_D1)
	rm -f $(TARG_E1)
	rm -f $(TARG_A2)
	rm -f $(TARG_B2)
	rm -f $(TARG_C2)
	rm -f $(TARG_D2)


	
	