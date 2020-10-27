#!/usr/bin/env python
# -*- coding: utf-8 -*-
import sqlobject as SO

# declaro la conexion asi -> "mysql://user:password@host/database"
__connection__ = SO.connectionForURI("mysql://guest:guest@localhost/ormtest")
# __connection__.debug = True


class Artist(SO.SQLObject):
    name = SO.StringCol(length=120, varchar=True)

    def _set_name(self, value):
        self._SO_set_name(value.lower())

    def _get_name(self):
        return self._SO_get_name().upper()


if __name__ == '__main__':
    # borro la tabla si ya existia
    Artist.dropTable(ifExists=True)

    # creo la tabla
    Artist.createTable()

    # la lleno con los artistas levantados de 'artist.txt'
    for n in open('artist.txt').readlines():
        Artist(name=n.rstrip('\n'))

    n = input("busco por name usando selectBy(): ")
    for a in Artist.selectBy(name=n):
        print(a)
    input('\n...')

    n = input("busco por name que comience con...: ")
    for a in Artist.select(Artist.q.name.startswith(n)):
        print("id={}\t=> name={}".format(a.id, a.name))
