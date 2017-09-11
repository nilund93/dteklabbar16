


#fakultet för multiplikation
#a0 är input
factorialinit:
	addi $a0, $zero, 8	#input n
	addi $a1, $zero, 1	#räknare, börjar på 1
	addi $a2, $a0, -1	#n-1
	addi $v0, $zero, 1	#initialize v0 med specialfallen 0! och 1!
	beq  $a0, $zero, done	#gå till done om användaren matat in 0.
	beq  $a0, $a1, done	#gå till done om användaren matat in 1 - ÄNDRA DETTA OM VI ÄNDRAR A1
	
factorial:
	#gör nån branch
	beq $a2, $a1, done
	mul $v0, $a0, $a2	#multiply n with n-1, put in v0
	addi $a2, $a2, -1	#dra av 1 från a2
	addi $a0, $v0, 0	#sätt a0 till v0
	beq $zero, $zero, factorial
done:	
	
stop:	
	beq $zero, $zero, stop	#loop until end of time
	nop
	