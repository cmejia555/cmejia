# -*- coding: utf-8 -*-
import sys


def lee_datos(nombre_archivo):
    '''
        lee un archivo de texto separado por comas con dos campos
        devuelve una lista de tuplas conteniendo ambos campos, o
        None si hubo algun error al procesar el archivo, o
        una lista vacia ([]) si el archivo no contiene datos
    '''
    datos = []
    try:
        f = open(nombre_archivo)
        for line in f.readlines():
            campo1, campo2 = line.split(',')
            datos.append((campo1.rstrip().lstrip(), campo2.rstrip().lstrip()))
    except:
        print("Error al procesar {}".format(nombre_archivo))
        print(sys.exc_info()[1])
        datos = None
    finally:
        f.close()

    return datos
