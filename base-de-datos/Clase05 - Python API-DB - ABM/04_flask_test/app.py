from flask import Flask
import MySQLdb
import defs

app = Flask(__name__)


def listar_artistas():
    conn = MySQLdb.connect(host=defs.HOST,
                           user=defs.USER,
                           passwd=defs.PASSWORD,
                           db=defs.DBASE)
    cur = conn.cursor()
    cur.execute("SELECT * FROM Artist ORDER BY ArtistId ASC")
    res = cur.fetchall()
    cur.close()
    conn.close()
    return res


def listar_albums(artist):
    conn = MySQLdb.connect(host=defs.HOST,
                           user=defs.USER,
                           passwd=defs.PASSWORD,
                           db=defs.DBASE)
    cur = conn.cursor()
    cur.execute('''
                SELECT Artist.Name, Album.Title
                FROM Artist
                JOIN Album ON (Artist.ArtistId = Album.ArtistId)
                WHERE Artist.Name=%s
                ORDER BY Album.Title ASC;
                ''',
                (artist, ))
    res = cur.fetchall()
    cur.close()
    conn.close()
    return res


@app.route("/")
def hello():
    return "Hello World!"


@app.route("/goodbye")
def goodbye():
    return "Goodbye World!"


@app.route("/lista")
def lista():
    r = "<pre>"
    for aid, name in listar_artistas():
        r += "{}\t<b>{}</b>\n".format(aid, name)
    r += '</pre>'
    return r


@app.route("/albums/<artist>")
def albums(artist):
    r = "<pre>"
    for aname, title in listar_albums(artist):
        r += "{}\t<b>{}</b>\n".format(aname, title)
    r += '</pre>'
    return r


if __name__ == '__main__':
    app.debug = True
    app.run()
