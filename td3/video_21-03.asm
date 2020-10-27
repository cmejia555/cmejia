; Mostrar en pantalla 25 lineas de caracteres. La linea superior
; debe contener letras 'A', la segunda linea letras 'B' y asi sucesivamente
; hasta la letra 'Y'.
; Registros usados:
;    CL = Fila, CH = Columna, SI = Puntero, BL = Letra a mostrar
      ORG 0x8000
      MOV AX, 0xB800 
      MOV ES, AX
      MOV BL, "A" ; Caracter
      MOV CL, 0   ; Fila
      MOV SI, 0   ; Puntero
CICLO_EXTERNO:
      MOV CH, 0   ; Columna
CICLO_INTERNO:
      MOV [ES:SI], BL  ; Escribo en pantalla
      ADD SI, 2  ; Incremento el puntero
      INC CH     ; Incremento columna
      CMP CH, 80 
      JNE CICLO_INTERNO
      INC CL    ; Incremento la fila
      INC BL    ; Incremento el caracter
      CMP CL, 25
      JNE CICLO_EXTERNO
      JMP $
