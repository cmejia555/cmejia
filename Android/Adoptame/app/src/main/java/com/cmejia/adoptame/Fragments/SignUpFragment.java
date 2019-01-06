package com.cmejia.adoptame.Fragments;


import android.app.ProgressDialog;
import android.content.Context;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.support.annotation.NonNull;
import android.support.annotation.Nullable;
import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentManager;
import android.support.v4.app.FragmentTransaction;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

import com.cmejia.adoptame.ChangeFragment;
import com.cmejia.adoptame.Clases.CompleteDialog;
import com.cmejia.adoptame.R;
import com.google.android.gms.tasks.OnCompleteListener;
import com.google.android.gms.tasks.Task;
import com.google.firebase.auth.AuthResult;
import com.google.firebase.auth.FirebaseAuth;
import com.google.firebase.auth.FirebaseAuthUserCollisionException;
import com.google.firebase.auth.FirebaseUser;

/**
 * A simple {@link Fragment} subclass.
 */
public class SignUpFragment extends Fragment {


    public EditText userName;
    public EditText userLastName;
    public EditText userPass;
    public EditText userEmail;
    public Button registerButton;

    public ProgressDialog progressDialog;

    public FirebaseAuth mAuth;

    public boolean status = false;

    public SignUpFragment() {
        // Required empty public constructor
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        // Inflate the layout for this fragment
        View v = inflater.inflate(R.layout.fragment_sign_up, container, false);

        userName = v.findViewById(R.id.user_name);
        userLastName = v.findViewById(R.id.user_last_name);
        userPass = v.findViewById(R.id.user_pass);
        userEmail = v.findViewById(R.id.user_email);
        registerButton = v.findViewById(R.id.register_button);
        progressDialog = new ProgressDialog(getActivity());
        return v;
    }

    @Override
    public void onActivityCreated(@Nullable Bundle savedInstanceState) {
        super.onActivityCreated(savedInstanceState);

        FirebaseUser currentUser = mAuth.getCurrentUser();
        //updateUI(currentUser);

        final SharedPreferences preferences = getActivity().getSharedPreferences("UserPreferences", Context.MODE_PRIVATE);

        registerButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                int userNameSize = userName.getText().length();
                int userLastNameSize = userLastName.getText().length();
                int userPasswordSize = userPass.getText().length();
                int userEmailSize = userEmail.getText().length();

                if( userNameSize != 0 && userPasswordSize != 0 && userEmailSize != 0 && userLastNameSize!=0 ) { // guardar en BBDD y checkear
                    String name = userName.getText().toString();
                    String lastName = userLastName.getText().toString();
                    String pass = userPass.getText().toString();
                    String email = userEmail.getText().toString();

                    SharedPreferences.Editor editor = preferences.edit();
                    editor.putString("email", userEmail.getText().toString());
                    editor.putString("pass",userPass.getText().toString());
                    editor.apply();

                    progressDialog.setMessage("Realizando registro en linea...");
                    progressDialog.show();

                    createAccount(email, pass);

                    if(status) {
                        ((ChangeFragment)getActivity()).changeFragment(new LoginFragment(), false);
                    }
                    //FragmentManager fragmentManager = getFragmentManager();
                    //fragmentManager.popBackStack();
                    //FragmentTransaction fragmentTransaction = getFragmentManager().beginTransaction();
                    //fragmentTransaction.commit();

                    /*String[] col = new String[] {"username"};
                    String[] args = new String[] {name};
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
                        register.put("username", name);
                        register.put("pass", pass);
                        register.put("email", email);
                        long id = db.insert("UserDataTable", null, register); // inserta un registro
                        if(id == -1) { // Error al insertar registro
                            // Mostrar dialog indicando el error
                        }
                        c.close();
                        db.close(); // Cierra la base de datos
                        finish();
                    }*/
                }
                else { // Falta completar campos (mostrar error)
                    // ************** Dialog **************************
                    FragmentManager fragmentManager = getFragmentManager();
                    CompleteDialog dialog = new CompleteDialog();
                    dialog.show(fragmentManager, "complete");
                }

            }
        });
    }

    public void createAccount(String email, String password) {
        mAuth.createUserWithEmailAndPassword(email, password).addOnCompleteListener(getActivity(), new OnCompleteListener<AuthResult>() {
            @Override
            public void onComplete(@NonNull Task<AuthResult> task) {
                if(task.isSuccessful()) {
                    FirebaseUser user = mAuth.getCurrentUser();
                    Toast.makeText(getActivity(), "Usuario registrado con exito.", Toast.LENGTH_SHORT).show();
                    status = true;
                    //updateUI(user);
                }
                else {
                    if( task.getException() instanceof FirebaseAuthUserCollisionException) {
                        Toast.makeText(getActivity(), "El usuario ya existe.", Toast.LENGTH_SHORT).show();
                    } else {
                        Toast.makeText(getActivity(), "No se pudo registrar el usuario.", Toast.LENGTH_SHORT).show();
                    }
                    status = false;
                    //updateUI(null);
                }
                progressDialog.dismiss();
            }
        });
    }

    @Override
    public void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        mAuth = FirebaseAuth.getInstance();
    }
}
