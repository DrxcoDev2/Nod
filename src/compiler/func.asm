section .text
global _start

; Definición de una "función" que suma dos números
sumar:
    ; Convención: los parámetros vienen en registros
    ; eax = primer número, ebx = segundo número
    add eax, ebx
    ret              ; vuelve al punto desde donde fue llamada

; Programa principal
_start:
    mov eax, 5       ; primer argumento
    mov ebx, 7       ; segundo argumento
    call sumar       ; llama a la "función"
                     ; (eax ahora contiene 12)
    
    ; salir del programa (Linux syscall)
    mov ebx, eax     ; código de salida = resultado
    mov eax, 1       ; syscall: exit
    int 0x80
