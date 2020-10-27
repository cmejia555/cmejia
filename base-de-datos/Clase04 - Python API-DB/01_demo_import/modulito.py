#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""\
Este archivo sirve para mostrar la diferencia entre
"importar" un modulo y usarlo como un script ejecutable
"""


def hola(nombre):
    '''
    imprime "hola, <nombre>"

    <nombre> debe ser un string
    '''
    print("hola, {}".format(nombre))    # aca esta el print

    # es lo mismo que esto
    # print("hola, %s" % nombre)    # usando el viejo formato tipo printf de C

    # o esto, usando concatenacion (no recomendable)
    # print("hola, " + nombre)

    # o esto otro para python >= 3.7, usando 'f-string'
    # print(f"hola, {nombre}")


print("la variable __name__ vale {}".format(__name__))


if __name__ == '__main__':
    hola('desde main')
