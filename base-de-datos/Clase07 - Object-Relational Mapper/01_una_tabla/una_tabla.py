#!/usr/bin/env python
# -*- coding: utf-8 -*-
import sqlobject as SO

# declaro la conexion asi -> "mysql://user:password@host/database"
__connection__ = SO.connectionForURI("mysql://guest:guest@localhost/ormtest")
# __connection__.debug = True


class Artist(SO.SQLObject):
    name = SO.StringCol(length=120, varchar=True)


def pausa():
    input('\n...')


if __name__ == '__main__':
    # borro la tabla si ya existia
    Artist.dropTable(ifExists=True)

    # creo la tabla
    Artist.createTable()

    # la lleno con los artistas levantados de 'artist.txt'
    for n in open('artist.txt').readlines():
        Artist(name=n.rstrip('\n'))

    input("miremos como quedo la tabla")

    id = input("busco por id usando get(): ")
    a = Artist.get(int(id))

    # lo imprimo segun lo devuelve sqlobject
    print(a)

    # imprimo los atributos
    print("id={} => name={}".format(a.id, a.name))
    pausa()

    id = input("busco por id usando selectBy(): ")
    for a in Artist.selectBy(id=int(id)):
        print(a)
    pausa()

    id = input("busco por id usando select(): ")
    for a in Artist.select(Artist.q.id == int(id)):
        print(a)
    pausa()

    id = input("busco por id usando select() y getOne(): ")
    print(Artist.select(Artist.q.id == int(id)).getOne())
    pausa()

    n = input("busco por name usando selectBy(): ")
    for a in list(Artist.selectBy(name=n)):
        print(a)
    pausa()

    n = input("busco por name que comience con: ")
    for a in list(Artist.select(Artist.q.name.startswith(n))):
        print("id={}\t=> name={}".format(a.id, a.name))
