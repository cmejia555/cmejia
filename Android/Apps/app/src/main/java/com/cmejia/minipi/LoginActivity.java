package com.cmejia.minipi;

import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
import android.support.v4.app.FragmentManager;
import android.support.v4.view.ViewPager;
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

public class LoginActivity extends AppCompatActivity {

    public TextView tittle;
    public EditText userName;
    public EditText userPass;
    public Button loginButton;
    public TextView textSignUp;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_login);

        //textoBienvenida.findViewById(R.id.texto_bienvenida); NO UTILIZAR, crashea la app
        tittle = findViewById(R.id.tittle);
        userName = findViewById(R.id.user_name);
        userPass = findViewById(R.id.user_pass);
        loginButton = findViewById(R.id.login_btn);
        textSignUp = findViewById(R.id.sign_up);

        // Crea el archivo de preferencias "UserPreferences" si no existe
        final SharedPreferences preferences = getSharedPreferences("UserPreferences", Context.MODE_PRIVATE);
        userName.setText(preferences.getString("user","not user"));
        userPass.setText(preferences.getString("pass", "not pass"));

        final UserSQLite userdb = new UserSQLite(this, "DBUsers.db", null, 1);
        final SQLiteDatabase db = userdb.getWritableDatabase(); // Referencia a userdb para modificacion

        loginButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                //NOTA: si no hay nada en el getText se crashea el programa. Se debe hacer una VERIFICACION
                int userNameSize = userName.getText().length();
                int userPassSize = userPass.getText().length();

                if( userNameSize != 0 && userPassSize != 0 ) { // Todos los campos completos

                    String user = userName.getText().toString();
                    String pass = userPass.getText().toString();

                    String[] col = new String[] {"username", "pass"};
                    String[] args = new String[] {user, pass};
                    Cursor c = db.query("UserDataTable", null, "username=? AND pass=?",
                            args,null, null, null);

                    if(c.moveToFirst()) { // Si existe algun registro, user y pass correctos
                        // Instancia un intent para pasar a otra activity
                        Intent nextActInfoList = new Intent(LoginActivity.this, InfoListActivity.class);

                        SharedPreferences.Editor editor = preferences.edit();
                        editor.putString("user", userName.getText().toString());
                        editor.putString("pass",userPass.getText().toString());
                        editor.apply();

                        c.close();
                        db.close();
                        startActivity(nextActInfoList); // Inicio la otra activity
                        finish();
                    }
                    else { // Usuario o contraseña incorrectos
                        // Toast default
                        //Toast toast = Toast.makeText(getApplicationContext(), R.string.toast_error, Toast.LENGTH_SHORT);
                        //toast.setGravity(Gravity.CENTER,0,-50 );
                        //toast.show();

                        // Toast personalizado
                        Toast toast = new Toast(getApplicationContext());
                        LayoutInflater inflater = getLayoutInflater();
                        View view = inflater.inflate(R.layout.toast, (ViewGroup) findViewById(R.id.toast_layout));
                        toast.setGravity(Gravity.BOTTOM,0,50);
                        toast.setDuration(Toast.LENGTH_SHORT);
                        TextView text = view.findViewById(R.id.toast_text);
                        text.setText(R.string.toast_complete);
                        toast.setView(view);
                        toast.show();

                        userName.setText("");
                        userPass.setText("");
                    }
                }
                else { // Falta completar campos
                    // ************** Dialog **************************
                    FragmentManager fragmentManager = getSupportFragmentManager();
                    CompleteDialog dialog = new CompleteDialog();
                    dialog.show(fragmentManager, "complete");
                }
            }
        });

        // Inicia la activity SignUp
        textSignUp.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent nextActSignUp = new Intent(LoginActivity.this, SignUpActivity.class);
                // Clear a los campos EditText user y pass
                userName.setText("");
                userPass.setText("");
                startActivity(nextActSignUp);
            }
        });
    }
}
