#!/usr/bin/env python
# -*- coding: utf-8 -*-
import sys
import MySQLdb
from utils.parchivos import lee_datos
import defs


class ABM_rebasico:
    '''
        implementa un ABM sencillo para la tabla Artist de Chinook
    '''
    def __init__(self, host, user, passwd, dbase):
        self._host = host
        self._user = user
        self._passwd = passwd
        self._dbase = dbase
        self._conn = None

    def conectar(self):
        '''
            realiza la conexion a la base de datos
            retorna True si la conexion fue exitosa
        '''
        try:
            self._conn = MySQLdb.connect(host=self._host,
                                         user=self._user,
                                         passwd=self._passwd,
                                         db=self._dbase)
            self._conn.autocommit(True)
            return True
        except:
            print(sys.exc_info()[1])
            return False

    def _verifica_conexion(self):
        '''
            verifica que se haya realizado una conexion a la base de datos
            retorna True si ya se realizo la conexion
        '''
        if not self._conn:
            print("Error. Todavia no se ha conectado a la base de datos {}".format(self._dbase))
            return False
        return True

    def listar(self):
        '''
            lista la tabla Artist
            retorna True si pudo realizar la operacion exitosamente
        '''
        if not self._verifica_conexion():
            return False
        error = False
        try:
            cur = self._conn.cursor()
            cur.execute("SELECT * FROM Artist ORDER BY ArtistId ASC")
            for id, name in cur.fetchall():
                print("{}\t{}".format(id, name))
        except:
            print(sys.exc_info()[1])
            error = True
        finally:
            cur.close()
        return error

    def agregar(self, datos):
        '''
            agrega nuevos 'artists' en la tabla Artist
            el listado de los artists a insertar viene en 'datos'
            retorna True si la operacion fue exitosa
        '''
        if not self._verifica_conexion():
            return False
        error = False
        try:
            cur = self._conn.cursor()
            for artist, otro in datos:
                print("... insertando '{}' en la tabla Artist".format(artist))
                cur.execute("INSERT INTO Artist (Name) VALUES (%s)", (artist,))
        except:
            print(sys.exc_info()[1])
            error = True
        finally:
            cur.close()
        return error

    def borrar(self, datos):
        '''
            elimina 'artists' en la tabla Artist
            el listado de los artists a eliminar viene en 'datos'
            retorna True si la operacion fue exitosa
        '''
        if not self._verifica_conexion():
            return False
        error = False
        try:
            cur = self._conn.cursor()
            for artist, otro in datos:
                print("... borrando '{}' en la tabla Artist".format(artist))
                cur.execute("DELETE FROM Artist WHERE Name=%s", (artist,))
        except:
            print(sys.exc_info()[1])
            error = True
        finally:
            cur.close()
        return error

    def modificar(self, datos):
        '''
            modifica 'artists' en la tabla Artist
            el listado de los artists a modificar viene en 'datos'
            retorna True si la operacion fue exitosa
        '''
        if not self._verifica_conexion():
            return False
        error = False
        try:
            cur = self._conn.cursor()
            for artist, new_artist in datos:
                print("... modificando '{}' en la tabla Artist por '{}'" .format(artist, new_artist))
                cur.execute("UPDATE Artist SET Name=%s WHERE Name=%s", (new_artist, artist))
        except:
            print(sys.exc_info()[1])
            error = True
        finally:
            cur.close()
        return error

    def limpiar(self, datos):
        '''
            elimina 'artists' en la tabla Artist que hayan sido insertados o modificados
            el listado de los artists a eliminar viene en 'datos'
            retorna True si la operacion fue exitosa
        '''
        if not self._verifica_conexion():
            return False
        error = False
        try:
            cur = self._conn.cursor()
            for artist, otro in datos:
                print("... borrando '{}' y/o '{}' en la tabla Artist".format(artist, otro))
                cur.execute("DELETE FROM Artist WHERE Name=%s OR Name=%s", (artist, otro))
        except:
            print(sys.exc_info()[1])
            error = True
        finally:
            cur.close()
        return error


if __name__ == '__main__':
    # chequeo de los argumentos de la linea de comandos
    if len(sys.argv) != 3:
        print("Error en la linea de comandos. Cantidad de argumentos erroneos")
        sys.exit()

    if sys.argv[1] not in ('-a', '-b', '-m', '-l', '-c'):
        print("Error en la linea de comandos. Opcion no valida (-a, -b, -m, -l, -c)")
        sys.exit()

    # trato e leer el archivo de datos
    d = lee_datos(sys.argv[2])
    if d is None:
        sys.exit()

    # creo la clase que va a manejar el ABM
    abm = ABM_rebasico(defs.HOST, defs.USER, defs.PASSWORD, defs.DBASE)

    # hago la conexion
    if not abm.conectar():
        sys.exit()

    # ejecuto el metodo que corresponda segun la opcion de la linea de comandos
    if sys.argv[1] == '-l':
        abm.listar()
    elif sys.argv[1] == '-a':
        abm.agregar(d)
    elif sys.argv[1] == '-b':
        abm.borrar(d)
    elif sys.argv[1] == '-m':
        abm.modificar(d)
    elif sys.argv[1] == '-c':
        abm.limpiar(d)
    else:
        pass
