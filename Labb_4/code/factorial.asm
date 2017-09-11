.text

	add $a1,$0,$0
	add $a2,$0,$0
	add $a3,$0,$0
	addi $v0,$0,1
	addi $v1,$0,5
loop1:
	beq  $a2,$v1,done
	add $a1,$a2,$0
	add $a3,$v0,$0
loop2:	beq $a1,$0,outloop
	add $v0,$v0,$a3
	addi $a1,$a1,-1
	beq $0,$0,loop2
outloop:
	addi $a2,$a2,1
	beq $0,$0,loop1
done:
	beq $0,$0,done
	
		
