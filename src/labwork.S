  # labwork.S
  # Written 2015 by F Lundevall
  # Skeleton file for IS1200/IS1500 lab 1 2015-2016.
  # The macros PUSH and POP are in the public domain.

.macro	PUSH reg
	addi	$sp,$sp,-4
	sw	\reg,0($sp)
.endm

.macro	POP reg
	lw	\reg,0($sp)
	addi	$sp,$sp,4
.endm

  # You can add your own code here
  #

.global delay
.global time2string
.global enable_interrupt

.text

hexasc:
	andi 	$a0,0x0000000f	#change the 7 most significant nibbles (4 bits) to 0
	slti	$t0,$a0,10	#if a0 is larger or equal to 10, t0 = 0. if a0 is smaller than 10 t0 = 1.
	beq	$zero,$t0,case	#branch if t0 is equal to zero
	nop
	add	$v0,$a0,0x30	#else add a0 to 0x30 and put the value into $v0
	jr	$ra		#return to main
	nop
		case:		#Case when a0 is larger than 
	add	$v0,$a0,0x37	#we know that a0 is between 0 5 so we can add 41 to get the right ASCII code
	#jr	$ra		#return to main
	nop
	
#delay:
#	jr $ra
#	nop
	
# NEW DELAY FOR ASSIGNMENT 4
delay:	

	while:			
	beq $a0,$zero,quit
	nop
	addi $a0,$a0,-1
		
	addi $t7,$zero,17500
	for:
	beq  $t7, $zero, while
	addi $t7,$t7,-1
	j    for
	nop
		
	quit:
	jr $ra
	nop
	
time2string:
		# a0 pekar på var i minnet som ascii karaktärerna ska sparas
		# $a1 = tid info, varje 4 bitar av de 16 least significant bits representerar en digit
		# $t0 = sekunder,t1 = 10 tal sekunder,t2 = minuter,t3 = 10 tal minuter
		# Spara ascii karaktärerna i adressen $a0 med hjälp av sb instruktionen
		
		#i dessa 4 steg så lägger jag in en digit av tiden i varje 0-3 t register och shiftar så att de 4 least significant
		#bist är värdet av digiten som ska konverteras till ascii
		
	PUSH $ra
	PUSH $s0
	PUSH $s1
	PUSH $s2
	PUSH $s3
	PUSH $s4
	
	add $t1, $zero, $a0
	add $s3,$a1,$zero
	andi $s3,0x0000f000
	srl $s3,$s3,12
	
	add $s2,$a1,$zero
	andi $s2,0x00000f00
	srl $s2,$s2,8
	
	add $s1,$a1,$zero # QUESTION - DENNA LINJE HANTERAR 10 TALS DIGITEN! lägger till den i s1
	andi $s1,0x000000f0
	srl $s1,$s1,4
	
	add $s0,$a1,$zero
	andi $s0,0x0000000f
	
	# add $t4,$a1,$zero # sätt $t4 till $a1
	addi $s4,$zero,0x3A
	
	add $a0,$s3,$zero
	jal hexasc
	nop
	sb $v0,0($t1)
	
	add $a0,$s2,$zero
	jal hexasc
	nop
	sb $v0,1($t1)
	
	sb $s4,2($t1)
	
	add $a0,$s1,$zero # QUESTION - DENNA DEL HANTERAR 10 TALS DIGITEN! skickar den till hexasc och sparar i 3($a0)
	jal hexasc
	nop
	sb $v0,3($t1)
	
	add $a0,$s0,$zero
	jal hexasc
	nop
	sb $v0,4($t1)	
	
	addi $v0, $zero, 0x45
	andi $v1, $a1, 0x1
	sub $v0, $v0, $v1
	sb $v0,5($t1)
	
	sb $zero,6($t1)
	add $a0, $zero, $t1
	
	POP $s4
	POP $s3	
	POP $s2
	POP $s1
	POP $s0
	POP $ra
	jr $ra
	nop
	
enable_interrupt:
	ei
	jr $ra
	nop