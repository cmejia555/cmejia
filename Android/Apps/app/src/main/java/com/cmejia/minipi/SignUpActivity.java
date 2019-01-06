package com.cmejia.minipi;

import android.content.ContentValues;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
import android.support.v4.app.FragmentManager;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

import clases.CompleteDialog;
import clases.UserSQLite;

public class SignUpActivity extends AppCompatActivity {

    public TextView title;
    public EditText userName;
    public EditText userPass;
    public EditText userEmail;
    public Button registerButton;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_sign_up);

        title = findViewById(R.id.tittle);
        userName = findViewById(R.id.user_name);
        userPass = findViewById(R.id.user_pass);
        userEmail = findViewById(R.id.user_email);
        registerButton = findViewById(R.id.register_btn);

        // Abre la base de datos, la crea si no existe. EL NOMBRE DE LA BASE DE DATOS DEBE TERMINAR EN .db !!!!
        final UserSQLite userdb = new UserSQLite(this, "DBUsers.db", null, 1);
        final SQLiteDatabase db = userdb.getWritableDatabase(); // Referencia a userdb para modificacion

        registerButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                int userNameSize = userName.getText().length();
                int userPasswordSize = userPass.getText().length();
                int userEmailSize = userEmail.getText().length();

                if( userNameSize != 0 && userPasswordSize != 0 && userEmailSize != 0 ) { // guardar en BBDD y checkear
                    String user = userName.getText().toString();
                    String pass = userPass.getText().toString();
                    String email = userEmail.getText().toString();

                    String[] col = new String[] {"username"};
                    String[] args = new String[] {user};
                    Cursor c = db.query("UserDataTable", null, "username=?",
                            args,null, null, null);

                    if(c.moveToFirst()) { // Si existe algun registro, existe el usuario! (Mostrar error)
                        // Toast personalizado
                        Toast toast = new Toast(getApplicationContext());
                        LayoutInflater inflater = getLayoutInflater();
                        View view = inflater.inflate(R.layout.toast, (ViewGroup) findViewById(R.id.toast_layout));
                        toast.setGravity(Gravity.BOTTOM,0,15);
                        toast.setDuration(Toast.LENGTH_SHORT);
                        TextView text = view.findViewById(R.id.toast_text);
                        text.setText(R.string.toast_user_repeat);
                        toast.setView(view);
                        toast.show();

                        userName.setText("");
                    }
                    else { // El usuario no existe, agregar a la base de datos
                        ContentValues register = new ContentValues();
                        register.put("username", user);
                        register.put("pass", pass);
                        register.put("email", email);
                        long id = db.insert("UserDataTable", null, register); // inserta un registro
                        if(id == -1) { // Error al insertar registro
                            // Mostrar dialog indicando el error
                        }
                        c.close();
                        db.close(); // Cierra la base de datos
                        finish();
                    }
                }
                else { // Falta completar campos (mostrar error)
                    // ************** Dialog **************************
                    FragmentManager fragmentManager = getSupportFragmentManager();
                    CompleteDialog dialog = new CompleteDialog();
                    dialog.show(fragmentManager, "complete");
                }

            }
        });
    }
}
