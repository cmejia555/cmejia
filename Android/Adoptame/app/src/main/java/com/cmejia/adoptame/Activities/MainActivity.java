package com.cmejia.adoptame.Activities;

import android.content.pm.ActivityInfo;
import android.icu.text.IDNA;
import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentManager;
import android.support.v4.app.FragmentTransaction;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.Window;

import com.cmejia.adoptame.ChangeFragment;
import com.cmejia.adoptame.Fragments.InfoFragment;
import com.cmejia.adoptame.Fragments.SplashFragment;
import com.cmejia.adoptame.R;
import com.cmejia.adoptame.TransferData;

public class MainActivity extends AppCompatActivity implements ChangeFragment, TransferData{

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        setContentView(R.layout.activity_main);


        if (savedInstanceState == null)
        {
            getSupportFragmentManager()
                    .beginTransaction()
                    .add(R.id.init_container, new SplashFragment())
                    .commit();
        }
    }

    @Override
    public void changeFragment(Fragment fragment, boolean addToBackStack) {
        FragmentTransaction transaction =
                getSupportFragmentManager()
                        .beginTransaction()
                        .setTransition(FragmentTransaction.TRANSIT_FRAGMENT_FADE)
                        .replace(R.id.init_container, fragment);

        if (addToBackStack) {
            transaction.addToBackStack(null);
        }

        transaction.commit();
    }

    @Override
    public void send(int id, String node) {
        FragmentManager manager = getSupportFragmentManager();
        InfoFragment fragment = new InfoFragment();
        //InfoFragment frag = (InfoFragment) manager.findFragmentById(R.id.frag_info);
        fragment.setData(id, node);
    }
}
