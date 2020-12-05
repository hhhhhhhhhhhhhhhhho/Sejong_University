.globl main
.data

select: .asciiz "Select Procedure : "
a_is: .asciiz "a is : "
b_is: .asciiz "b is : "
n_is: .asciiz "n is : "

notmulti: .asciiz " is not multiple of three! "
multi: .asciiz " is multiple of three! "

enter : .asciiz "\n"
space : .asciiz " "

.text
main:
	la $a0 select
	li $v0 4
	syscall
	# printf "Input number : Select Procedure"

	li $v0 5
	syscall
	#input number
	
	beq $v0,1,isthismulti
	#multi of three
	
	beq $v0,2,fivo	

isthismulti:
	
	la $a0,a_is
	li $v0,4
	syscall
	
	li $v0 5
	syscall
	#input number : a is ~
	
	li $t1,0
	add $t1,$v0,$zero
	#save a in t1
	
	la $a0,b_is
	li $v0,4
	syscall
	
	li $v0 5
	syscall
	#input number : a is ~
	
	li $t2,0
	add $t2,$v0,$zero
	#save a in t2
	
	sub $t1,$t1,$t2
	#save a-b into t1
	
	j multiofthree
	
multiofthree:
	
	li $a0,0
	add $a0,$a0,$t1
	li $v0,1
	syscall
	li $t3,3
	
	
	div $t1,$t3
	mfhi $s1
	
	beq $s1,1,multi_exit
	
	la $a0,multi
	li $v0 4
	syscall
	
	j exit

	
multi_exit:
	la $a0,notmulti
	li $v0,4
	syscall
	
	j exit

fivo:
	la $a0,n_is
	li $v0,4
	syscall

	li $v0,5
	syscall
	
	li $t1,0
	add $t1,$t1,$v0
	#save n into t1 register
	
	la $a0,a_is
	li $v0,4
	syscall

	li $v0,5
	syscall
	
	li $t2,0
	add $t2,$t2,$v0
	#save a into t2 register
	
	li $s0,1
	li $s1,1
	#if number is 0,1 -> return 1
	
	la $a0,enter
	li $v0,4
	syscall
	#enter
	
	li $s2,0
	li $t3,0
	
	li $a0,1
	li $v0,1
	syscall
	#print 0'th fivo num
	
	la $a0,space
	li $v0,4
	syscall
	# space
	
	li $a0,1
	li $v0,1
	syscall
	#print 1'th fivo num
	
	
	la $a0,space
	li $v0,4
	syscall
	# space
	
	li $t3,0
	sub $t1,$t1,1
	j fivo_plus
	
	
fivo_plus:
	# t1 = n , t2 = a
	#s0=1, s1=1 ,s2 = T(n)
	#t3=0 -> count 0~8
	# print 0,1 count  1, 1
	
	#Loop
	# s2 = s1+s0
	# print s2
	# s0 <- s1
	# s1 <- s2
	# t3 ++ if t3=n -> exit.
	
	beq $t3,$t1,exit
	
	add $s2,$s1,$s0 # T(n) = T(n-1)+T(n-2)
	add $s2,$s2,$t2 # T(n) = T(n) + a 
	la $a0,($s2)
	li $v0,1
	syscall
	#print n'th fivo num
	
	la $a0,space
	li $v0,4
	syscall
	# space
	
	li $t4,0
	add $t4,$zero,$s1
	# tmp = s1
	li $s1,0
	add $s1,$zero,$s2
	#s1 = s2
	
	li $s0,0
	add $s0,$zero,$t4
	#s0 = tmp
	
	addi $t3,$t3,1
	
	j fivo_plus
	
exit:
	li $v0,10
	syscall