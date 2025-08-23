section .data 
   
msg db 'Sum = ', 10 
msgLen equ $-msg 
num1 db 35H ;first number
            
            ; plus
num2 db 46H ;second number

section .bss 

sum resb 1 
temp resb 1 

section .text 

global _start 
_start: 
mov rax,1 
mov rdi, 1 
mov rsi,msg 
mov rdx,msgLen 
syscall 

mov al,byte[num1] 
add al,byte[num2] 
mov byte[sum],al 

mov bp,2 
up:rol al,4 
mov bl,al 
and al,0Fh 
cmp al,09 
jbe down 
add al,07h 
down:add al,30h 
mov byte[temp],al 
mov rax,1 
mov rdi,1 
mov rsi,temp 
mov rdx,1 
syscall 
mov al,bl 
dec bp
 jnz up

mov rax,60
mov rdi,0
syscall