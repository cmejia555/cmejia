inicio_reset_vect:

      jmp word 0xf000:0  ;0xf0000
      times 16-($-inicio_reset_vect) db 0
