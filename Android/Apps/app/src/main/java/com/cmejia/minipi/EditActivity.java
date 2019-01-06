package com.cmejia.minipi;

import android.content.ContentValues;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

import clases.BookSQLite;

public class EditActivity extends AppCompatActivity {

    public EditText book;
    public EditText subject;
    public EditText details;
    public Button editButton;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_edit);

        book = findViewById(R.id.input_book_edit);
        subject = findViewById(R.id.input_subject_edit);
        details = findViewById(R.id.input_details_edit);
        editButton = findViewById(R.id.edit_button);



        editButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                // Crea el archivo de preferencias "UpdateBookDB" si no existe
                SharedPreferences preferences = getSharedPreferences("UpdateBookDB", Context.MODE_PRIVATE);

                SharedPreferences.Editor editor = preferences.edit();
                editor.putString("bookName", book.getText().toString());
                editor.putString("subject", subject.getText().toString());
                editor.putString("details", details.getText().toString());
                editor.apply();

                finish();
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
        editor.apply();
    }
}

