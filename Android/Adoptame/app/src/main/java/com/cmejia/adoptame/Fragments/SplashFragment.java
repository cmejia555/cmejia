package com.cmejia.adoptame.Fragments;


import android.os.AsyncTask;
import android.os.Bundle;
import android.support.annotation.Nullable;
import android.support.v4.app.Fragment;

import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ProgressBar;

import com.cmejia.adoptame.ChangeFragment;

import com.cmejia.adoptame.R;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;


/**
 * A simple {@link Fragment} subclass.
 */
public class SplashFragment extends Fragment {
    private final int SPLASH_SCREEN_DELAY = 2000;
    private ProgressBar pBar;
    private ProgressBarAsyncTask asyncTask;
    //private OnFragmentInteractionListener mListener;


    public SplashFragment() {
        // Required empty public constructor
    }


    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        // Inflate the layout for this fragment
        View v = inflater.inflate(R.layout.fragment_splash, container, false);
        pBar = v.findViewById(R.id.progress_bar);

        FirebaseDatabase database = FirebaseDatabase.getInstance();
        DatabaseReference myRef = database.getReference("brands");

        myRef.child("0").child("image").setValue(R.drawable.puppy);
        myRef.child("1").child("image").setValue(R.drawable.bulldog);
        myRef.child("2").child("image").setValue(R.drawable.rottweiler);
        myRef.child("3").child("image").setValue(R.drawable.weimaraner);

        myRef.child("0").child("descripcion").setValue("Soy mas bueno que lazy");
        myRef.child("1").child("descripcion").setValue("Te prometo que no rompo nada");
        myRef.child("2").child("descripcion").setValue("Soy muy cariñoso");
        myRef.child("3").child("descripcion").setValue("Vamos a ser los mejores amigos");


        myRef = database.getReference("detaills");
        myRef.child("0").child("image").setValue(R.drawable.puppy);
        myRef.child("1").child("image").setValue(R.drawable.bulldog);
        myRef.child("2").child("image").setValue(R.drawable.rottweiler);
        myRef.child("3").child("image").setValue(R.drawable.weimaraner);

        myRef.child("0").child("name").setValue("Bono");
        myRef.child("1").child("name").setValue("Rocky");
        myRef.child("2").child("name").setValue("Pancho");
        myRef.child("3").child("name").setValue("Tobias");

        myRef.child("0").child("phone").setValue("49234857");
        myRef.child("1").child("phone").setValue("44433333");
        myRef.child("2").child("phone").setValue("12345677");
        myRef.child("3").child("phone").setValue("49225588");

        myRef.child("0").child("dir").setValue("Av. Riestra 1750");
        myRef.child("1").child("dir").setValue("Av. Gaona 1600");
        myRef.child("2").child("dir").setValue("Av. Medrano 950");
        myRef.child("3").child("dir").setValue("Av Cordoba 100");


        return v;
    }

    @Override
    public void onActivityCreated(@Nullable Bundle savedInstanceState) {
        super.onActivityCreated(savedInstanceState);

        asyncTask = new ProgressBarAsyncTask();
        asyncTask.execute();

    }

    public class ProgressBarAsyncTask extends AsyncTask<Void, Integer, Boolean> {

        public ProgressBarAsyncTask() { }

        private void tareaLarga() {
            try {
                Thread.sleep(100);
            } catch (InterruptedException e) { }
        }

        @Override
        protected void onPreExecute() {
            super.onPreExecute();
            pBar.setProgress(0);
            pBar.setMax(100);
        }

        @Override
        protected Boolean doInBackground(Void... params) {
            for(int i = 1; i <= 10; i++) {
                tareaLarga();
                publishProgress(i*10);
            }
            return true;
        }

        @Override
        protected void onProgressUpdate(Integer... values) {
            super.onProgressUpdate(values);
            int progress = values[0];
            pBar.setProgress(progress);
        }

        @Override
        protected void onPostExecute(Boolean aBoolean) {
            super.onPostExecute(aBoolean);

            ((ChangeFragment)getActivity()).changeFragment(new LoginFragment(), false);
        }

        @Override
        protected void onCancelled(Boolean aBoolean) {
            super.onCancelled(aBoolean);
        }
    }
}
