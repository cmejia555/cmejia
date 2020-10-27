#!/usr/bin/env python
# -*- coding: utf-8 -*-
import sqlobject as SO
from sqlobject.inheritance import InheritableSQLObject


class Persona(InheritableSQLObject):
    nombre = SO.UnicodeCol(length=50, varchar=True, notNone=True)
    apellido = SO.UnicodeCol(length=50, varchar=True, notNone=True)


class Profesor(Persona):
    titulo = SO.UnicodeCol(length=160, varchar=True, notNone=True)


class Alumno(Persona):
    carrera = SO.UnicodeCol(length=160, varchar=True, notNone=True)
    legajo = SO.UnicodeCol(length=20, varchar=True, notNone=True)


if __name__ == '__main__':
    # declaro la conexion asi -> "mysql://user:password@host/database"
    connection = SO.connectionForURI("mysql://guest:guest@localhost/ormtest")
    SO.sqlhub.processConnection = connection
    # connection.debug = True

    # borro las tablas si ya existian
    Persona.dropTable(ifExists=True)
    Profesor.dropTable(ifExists=True)
    Alumno.dropTable(ifExists=True)

    # creo las tablas
    Persona.createTable()
    Profesor.createTable()
    Alumno.createTable()

    # creo algunos profesores
    Profesor(nombre='Sergio', apellido='Kaszczyszyn', titulo='Ing. en Electrónica')
    Profesor(nombre='Roberto', apellido='Gómez', titulo='Ing. en Electrónica')

    # creo algunos alumnos
    Alumno(nombre='Marcelo', apellido='Acevedo', carrera='Electrónica', legajo='xxx.xxx-y')
    Alumno(nombre='Emiliano', apellido='Fernández', carrera='Electrónica', legajo='aaa.aaa-b')
    Alumno(nombre='Daniel', apellido='García', carrera='Electrónica', legajo='ccc.ccc-d')
    Alumno(nombre='Marcos', apellido='García', carrera='Electrónica', legajo='eee.fff-g')
    Alumno(nombre='Alejandro', apellido='Mouras', carrera='Electrónica', legajo='hhh.iii-j')

    # algunos queries
    input("alumnos ordenados por apellido")
    for alumno in list(Alumno.select().orderBy(Alumno.q.apellido)):
        print("{}, {}  legajo={}".format(alumno.apellido, alumno.nombre, alumno.legajo))
    print()

    input("profesores")
    for profe in list(Profesor.select()):
        print("{}, {}  titulo={}".format(profe.apellido, profe.nombre, profe.titulo))
    print()

    input("personas que son 'Profesor'")
    for persona in list(Persona.select(Persona.q.childName == 'Profesor')):
        print(persona)

    print('{}\n{}\n{}\n'.format('#' * 80,
                                '# mirar detalle!!! Busqué por Persona y me devolvió Profesores',
                                '#' * 80))

    input("personas que son 'Alumno'")
    for persona in list(Persona.select(Persona.q.childName == 'Alumno')):
        print(persona)

    print('{}\n{}\n{}\n'.format('#' * 80,
                                '# mirar detalle!!! Busqué por Persona y me devolvió Alumnos',
                                '#' * 80))

    input("personas con apellido G")
    for persona in list(Persona.select(Persona.q.apellido.startswith('G'))):
        print(persona)

    print('{}\n{}\n{}\n'.format('#' * 80,
                                '# mirar detalle!!! Busqué por Persona y me devolvió Profersores y Alumnos',
                                '#' * 80))
