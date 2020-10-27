SECTIONS
{

/*Inicio ROM*/
  . = 0xFFFF0000;
  .rom_inicio :
  AT(0xF0000)
    { *(.rom_inicio) }



/*Vector de Reset*/
  . = 0xFFFFFFF0;
  .reset_vect :
  AT(0xFFFF0)
    { *(.reset_vect) }
}
