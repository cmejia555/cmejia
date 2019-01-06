package com.cmejia.minipi;

import android.support.design.widget.TabLayout;
import android.support.v4.view.ViewPager;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import adapters.ViewPagerAdapter;
import fragments.DetailsFragment;
import fragments.ImageFragment;

public class DetailsActivity extends AppCompatActivity {

    public ViewPager viewPager;
    public TabLayout tabs;

    public String detailsBuffer;
    public int imageBuffer;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_details);

        viewPager = findViewById(R.id.detail_pager);
        tabs = findViewById(R.id.tabs);

        // Obtengo la informacion pasada por la activity InfoList
        detailsBuffer = getIntent().getStringExtra("DETAILS");
        imageBuffer = getIntent().getIntExtra("IMAGE", 10);

        // AGREGADO DE FRAGMENTS Y TABS
        setupViewPager(viewPager);
        tabs.setupWithViewPager(viewPager); // Comunica los tabs con los fragments
        //tabs.setTabMode(TabLayout.MODE_SCROLLABLE);
        //tabs.setTabGravity(TabLayout.GRAVITY_FILL);

    }

    // Metodo para agregar fragments al ViewPager
    private void setupViewPager(ViewPager viewPager) {
        ViewPagerAdapter adapter = new ViewPagerAdapter(getSupportFragmentManager());
        // getChildFragmentManager() usar esto cuando se tiene un fragment dentro de otro fragment
        // getSupportFragmentManager() usar esto cuando tengo un fragment dentro de una activity

        adapter.addFragment(new DetailsFragment(), "DETALLES", detailsBuffer);
        adapter.addFragment(new ImageFragment(), "PORTADA", String.valueOf(imageBuffer));

        viewPager.setAdapter(adapter);
    }

    @Override
    protected void onResume() {
        super.onResume();

    }
}

