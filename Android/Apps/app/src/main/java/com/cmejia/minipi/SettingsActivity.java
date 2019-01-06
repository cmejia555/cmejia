package com.cmejia.minipi;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;

import fragments.SettingsFragment;

public class SettingsActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_settings);

        getFragmentManager().beginTransaction().replace(android.R.id.content, new SettingsFragment()).commit();

    }
}
