package com.cmejia.adoptame.Fragments;


import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.database.Cursor;
import android.os.Bundle;
import android.support.annotation.NonNull;
import android.support.annotation.Nullable;
import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentManager;
import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

import com.cmejia.adoptame.ChangeFragment;
import com.cmejia.adoptame.Clases.CompleteDialog;
import com.cmejia.adoptame.R;
import com.google.android.gms.tasks.OnCompleteListener;
import com.google.android.gms.tasks.Task;
import com.google.firebase.auth.AuthResult;
import com.google.firebase.auth.FirebaseAuth;
import com.google.firebase.auth.FirebaseUser;

/**
 * A simple {@link Fragment} subclass.
 */
public class LoginFragment extends Fragment {

    public EditText userEmail;
    public EditText userPass;
    public Button loginButton;
    public TextView textSignUp;

    public SharedPreferences preferences;

    public FirebaseAuth mAuth;

    public LoginFragment() {
        // Required empty public constructor
    }


    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        // Inflate the layout for this fragment
        View v = inflater.inflate(R.layout.fragment_login, container, false);

        userEmail = v.findViewById(R.id.user_email);
        userPass = v.findViewById(R.id.user_pass);
        loginButton = v.findViewById(R.id.login_button);
        textSignUp = v.findViewById(R.id.sign_up);

        return v;
    }

    @Override
    public void onActivityCreated(@Nullable Bundle savedInstanceState) {
        super.onActivityCreated(savedInstanceState);

        preferences = getActivity().getSharedPreferences("UserPreferences", Context.MODE_PRIVATE);
        String email = preferences.getString("email","not_value");
        String pass = preferences.getString("pass", "not_value");

        if( !email.equals("not_value") && !pass.equals("not_value") ) {
            userEmail.setText(email);
            userPass.setText(pass);
        }

        loginButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                //NOTA: si no hay nada en el getText se crashea el programa. Se debe hacer una VERIFICACION
                int userEmailSize = userEmail.getText().length();
                int userPassSize = userPass.getText().length();

                if( userEmailSize != 0 && userPassSize != 0 ) { // Todos los campos completos

                    String email = userEmail.getText().toString();
                    String pass = userPass.getText().toString();

                    signInAccount(email,pass);




                    /*if(c.moveToFirst()) { // Si existe algun registro, user y pass correctos
                        // Instancia un intent para pasar a otra activity
                        Intent nextActInfoList = new Intent(LoginActivity.this, InfoListActivity.class);

                        SharedPreferences.Editor editor = preferences.edit();
                        editor.putString("user", userEmail.getText().toString());
                        editor.putString("pass",userPass.getText().toString());
                        editor.apply();

                        c.close();
                        db.close();
                        startActivity(nextActInfoList); // Inicio la otra activity
                        finish();
                    }*/
                  //  else { // Usuario o contraseña incorrectos
                        // Toast default
                        //Toast toast = Toast.makeText(getApplicationContext(), R.string.toast_error, Toast.LENGTH_SHORT);
                        //toast.setGravity(Gravity.CENTER,0,-50 );
                        //toast.show();

                        // Toast personalizado
                        /*Toast toast = new Toast(getApplicationContext());
                        LayoutInflater inflater = getLayoutInflater();
                        View view = inflater.inflate(R.layout.toast, (ViewGroup) findViewById(R.id.toast_layout));
                        toast.setGravity(Gravity.BOTTOM,0,50);
                        toast.setDuration(Toast.LENGTH_SHORT);
                        TextView text = view.findViewById(R.id.toast_text);
                        text.setText(R.string.toast_complete);
                        toast.setView(view);
                        toast.show();
                        */

                    /*    userEmail.setText("");
                        userPass.setText("");
                    }*/
                }
                else { // Falta completar campos
                    // ************** Dialog **************************
                    FragmentManager fragmentManager = getFragmentManager();
                    CompleteDialog dialog = new CompleteDialog();
                    dialog.show(fragmentManager, "complete");
                }
            }
        });

        // Pasa al fragment SignUp
        textSignUp.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                // Clear a los campos EditText user y pass
                userEmail.setText("");
                userPass.setText("");
                ((ChangeFragment)getActivity()).changeFragment(new SignUpFragment(), true);
                //Intent nextActSignUp = new Intent(LoginActivity.this, SignUpActivity.class);
                //startActivity(nextActSignUp);
            }
        });
    }

    private void signInAccount(String email, String password) {
        mAuth.signInWithEmailAndPassword(email, password).addOnCompleteListener(getActivity(), new OnCompleteListener<AuthResult>() {

            @Override
            public void onComplete(@NonNull Task<AuthResult> task) {
                if (task.isSuccessful()) {
                    // Sign in success, update UI with the signed-in user's information
                    FirebaseUser user = mAuth.getCurrentUser();
                    //updateUI(user);
                    ((ChangeFragment)getActivity()).changeFragment(new ItemsFragment(), false);
                } else {
                    // If sign in fails, display a message to the user.
                    Toast.makeText(getActivity(), "Authentication failed.", Toast.LENGTH_SHORT).show();
                    //updateUI(null);
                }
            }
        });
    }

    @Override
    public void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        mAuth = FirebaseAuth.getInstance();
    }
}
