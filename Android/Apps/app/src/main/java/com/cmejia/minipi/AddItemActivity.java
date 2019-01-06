package com.cmejia.minipi;

import android.content.Context;
import android.content.SharedPreferences;
import android.support.v4.app.FragmentManager;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;

import clases.CompleteDialog;

public class AddItemActivity extends AppCompatActivity {

    public EditText book;
    public EditText subject;
    public EditText details;
    public Button addButton;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_add_item);

        book = findViewById(R.id.input_book_add);
        subject = findViewById(R.id.input_subject_add);
        details = findViewById(R.id.input_details_add);
        addButton = findViewById(R.id.add_button);

        addButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                // Crea el archivo de preferencias "UpdateBookDB" si no existe
                if(!book.getText().toString().isEmpty() && !subject.getText().toString().isEmpty()
                        && !details.getText().toString().isEmpty()) {
                    SharedPreferences preferences = getSharedPreferences("UpdateBookDB", Context.MODE_PRIVATE);
                    SharedPreferences.Editor editor = preferences.edit();

                    editor.putString("bookName", book.getText().toString());
                    editor.putString("subject", subject.getText().toString());
                    editor.putString("details", details.getText().toString());
                    editor.putInt("imageID", R.drawable.soliver);
                    editor.apply();

                    finish();

                }
                else {
                    // ************** Dialog **************************
                    FragmentManager fragmentManager = getSupportFragmentManager();
                    CompleteDialog dialog = new CompleteDialog();
                    dialog.show(fragmentManager, "complete");
                }
            }
        });
    }

    @Override
    protected void onResume() {
        super.onResume();
        SharedPreferences preferences = getSharedPreferences("UpdateBookDB", Context.MODE_PRIVATE);

        SharedPreferences.Editor editor = preferences.edit();
        editor.putString("bookName", "");
        editor.putString("subject", "");
        editor.putString("details", "");
        editor.putInt("imageID", 0);
        editor.apply();
    }

}
