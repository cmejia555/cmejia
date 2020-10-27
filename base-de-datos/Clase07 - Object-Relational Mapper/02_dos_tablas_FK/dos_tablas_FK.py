#!/usr/bin/env python
# -*- coding: utf-8 -*-
import csv
import sqlobject as SO


class Artist(SO.SQLObject):
    name = SO.StringCol(length=120, varchar=True)


class Album(SO.SQLObject):
    title = SO.StringCol(length=160, varchar=True)
    artist = SO.ForeignKey('Artist', default=None)


if __name__ == '__main__':
    # declaro la conexion asi -> "mysql://user:password@host/database"
    connection = SO.connectionForURI("mysql://guest:guest@localhost/ormtest")
    SO.sqlhub.processConnection = connection
    # connection.debug = True

    # borro las tablas si ya existian
    Artist.dropTable(ifExists=True)
    Album.dropTable(ifExists=True)

    # creo las tablas
    Artist.createTable()
    Album.createTable()

    # lleno Artist con los artistas levantados de 'artist.txt'
    for n in open('artist.txt').readlines():
        Artist(name=n.rstrip('\n'))

    # lleno Album con los albums levantados de 'album.csv'
    reader = csv.DictReader(open('album.csv'), delimiter=',', quotechar='"')
    for row in reader:
        artist = Artist.selectBy(name=row['Name']).getOne()
        album = Album(title=row['Title'], artist=artist)

    input("miremos como quedaron las tablas")

    # algunas queries
    input("\nbusco los 10 primeros artista ordenados por nombre y los albums de cada uno")
    for artist in Artist.select(orderBy=Artist.q.name, limit=10):
        print(artist.name + ':')
        i = 1
        for album in Album.select(Album.q.artist == artist):
            print('\t{}) {}'.format(i, album.title))
            i += 1
        print()

    input("\nbusco la cantidad de albums de cada artista entrando por Artist")
    for artist in Artist.select(orderBy=Artist.q.name):
        print("{}: {}".format(artist.name, Album.select(Album.q.artist == artist).count()))

    init = input("\nbusco albums de los artistas que comiencen con: ")
    for album in Album.select(SO.AND(Album.q.artistID == Artist.q.id,
                                     Artist.q.name.startswith(init)
                                     )
                              ):
        print(album.artist.name, album.title)

    # lo mismo que antes pero usando SQL
    init = input("\nbusco albums de los artistas que comiencen con: ")
    query = "SELECT artist.name, album.title FROM artist, album WHERE artist.id=album.artist_id AND artist.name LIKE '{}%'".format(init)
    print(query)

    for row in connection.queryAll(query):
        print(row)
