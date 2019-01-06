package com.cmejia.adoptame.Fragments;


import android.os.Bundle;
import android.support.design.widget.TabLayout;
import android.support.v4.app.Fragment;
import android.support.v4.view.ViewPager;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import com.cmejia.adoptame.Adapters.ViewPagerAdapter;
import com.cmejia.adoptame.R;

/**
 * A simple {@link Fragment} subclass.
 */
public class DetailsFragment extends Fragment {
    private ViewPager viewPager;
    private TabLayout tabLayout;

    public DetailsFragment() {
        // Required empty public constructor
    }


    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        // Inflate the layout for this fragment
        View v = inflater.inflate(R.layout.fragment_details, container, false);

        viewPager = v.findViewById(R.id.viewpager);
        tabLayout = v.findViewById(R.id.tablayout);

        // AGREGADO DE FRAGMENTS Y TABS
        setupViewPager(viewPager);
        tabLayout.setupWithViewPager(viewPager); // Comunica los tabs con los fragments

        return v;
    }

    // Metodo para agregar fragments al ViewPager
    private void setupViewPager(ViewPager viewPager) {
        ViewPagerAdapter adapter = new ViewPagerAdapter(getChildFragmentManager());
        // getChildFragmentManager() usar esto cuando se tiene un fragment dentro de otro fragment
        // getSupportFragmentManager() usar esto cuando tengo un fragment dentro de una activity

        adapter.addFragment(new InfoFragment(), "DETALLES");
        adapter.addFragment(new MapFragment(), "UBICACION");

        viewPager.setAdapter(adapter);
    }

}
