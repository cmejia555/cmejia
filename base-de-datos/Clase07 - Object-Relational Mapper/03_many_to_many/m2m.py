#!/usr/bin/env python
# -*- coding: utf-8 -*-
import sqlobject as SO


class Artist(SO.SQLObject):
    name = SO.StringCol(length=120, varchar=True)
    albums = SO.RelatedJoin('Album')


class Album(SO.SQLObject):
    title = SO.StringCol(length=160, varchar=True)
    artists = SO.RelatedJoin('Artist')


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
    import csv
    reader = csv.DictReader(open('album.csv'), delimiter=',', quotechar='"')
    for row in reader:
        artist = Artist.selectBy(name=row['Name']).getOne()
        album = Album(title=row['Title'])
        artist.addAlbum(album)

    input("miremos como quedaron las tablas")

    # algunos queries
    input("\nbusco los 10 primeros artista ordenados por nombre y los albums de cada uno")
    for artist in Artist.select(orderBy=Artist.q.name, limit=10):
        print(artist.name + ':')
        i = 1
        for album in artist.albums:
            print('\t{}) {}'.format(i, album.title))
            i += 1
        print()

    # creo un album con varios artistas
    ac_dc = Artist.selectBy(name='AC/DC').getOne()
    aerosmith = Artist.selectBy(name='Aerosmith').getOne()
    alice_in_chains = Artist.selectBy(name='Alice In Chains').getOne()
    led_zeppelin = Artist.selectBy(name='Led Zeppelin').getOne()
    album = Album(title='Compilado')
    album.addArtist(ac_dc)
    album.addArtist(aerosmith)
    album.addArtist(alice_in_chains)
    album.addArtist(led_zeppelin)

    # algunos queries
    input("\nbusco los 10 primeros artista ordenados por nombre y los albums de cada uno (deberia aparece el compilado)")
    for artist in Artist.select(orderBy=Artist.q.name, limit=10):
        print(artist.name + ':')
        i = 1
        for album in artist.albums:
            print('\t{}) {}'.format(i, album.title))
            i += 1
        print()

    init = input("busco por album con nombre que comience con: ")
    for album in Album.select(Album.q.title.startswith(init)):
        print(album.title + ':')
        i = 1
        for artist in album.artists:
            print('\t{}) {}'.format(i, artist.name))
            i += 1
        print()
