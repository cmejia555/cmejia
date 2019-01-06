package clases;

import android.content.Context;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteOpenHelper;

public class UserSQLite extends SQLiteOpenHelper {

    private String createTableSQL = "CREATE TABLE UserDataTable (username TEXT, pass TEXT, email TEXT)";

    public UserSQLite(Context context, String name, SQLiteDatabase.CursorFactory factory, int version) {
        super(context, name, factory, version);
    }

    @Override
    public void onCreate(SQLiteDatabase db) {
        db.execSQL(createTableSQL); // Se crea la tabla
    }

    @Override
    public void onUpgrade(SQLiteDatabase db, int oldVersion, int newVersion) {

        db.execSQL("DROP TABLE IF EXISTS UserDataTable"); //Se elimina la versión anterior de la tabla

        db.execSQL(createTableSQL); //Se crea la NUEVA versión de la tabla (SE DEBERIA IMPORTAR LA TABLA ANTERIOR Y ACTUALIZARLA)
    }
}
