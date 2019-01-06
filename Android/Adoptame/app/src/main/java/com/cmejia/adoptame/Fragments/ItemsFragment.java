package com.cmejia.adoptame.Fragments;


import android.os.Bundle;
import android.support.annotation.NonNull;
import android.support.v4.app.Fragment;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.GridLayoutManager;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.support.v7.widget.Toolbar;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.View;
import android.view.ViewGroup;

import com.cmejia.adoptame.ChangeFragment;
import com.cmejia.adoptame.Clases.Brand;
import com.cmejia.adoptame.R;
import com.cmejia.adoptame.TransferData;
import com.cmejia.adoptame.ViewHolders.BrandViewHolder;
import com.firebase.ui.database.FirebaseRecyclerAdapter;
import com.firebase.ui.database.FirebaseRecyclerOptions;
import com.google.firebase.database.FirebaseDatabase;
import com.google.firebase.database.Query;


/**
 * A simple {@link Fragment} subclass.
 */
public class ItemsFragment extends Fragment {

    private RecyclerView recView;
    private FirebaseRecyclerAdapter adapter;

    TransferData data;
    public ItemsFragment() {
        // Required empty public constructor
    }


    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        // Inflate the layout for this fragment
        View v = inflater.inflate(R.layout.fragment_items, container, false);
        setupToolbar(v);

        recView = v.findViewById(R.id.recView);
        recView.setLayoutManager(new GridLayoutManager(getContext(), 1, LinearLayoutManager.VERTICAL, false));

        data = (TransferData)getActivity();
        Query query = FirebaseDatabase.getInstance()
                .getReference()
                .child("brands")
                .limitToLast(50);

        FirebaseRecyclerOptions<Brand> options =
                new FirebaseRecyclerOptions.Builder<Brand>().setQuery(query, Brand.class).build();

        adapter = new FirebaseRecyclerAdapter<Brand, BrandViewHolder>(options) {
            @NonNull
            @Override
            public BrandViewHolder onCreateViewHolder(@NonNull ViewGroup parent, int viewType) {
                // Create a new instance of the ViewHolder, in this case we are using a custom
                // layout called R.layout.message for each item
                View view = LayoutInflater.from(parent.getContext()).
                        inflate(R.layout.brand_item, parent, false);

                BrandViewHolder viewHolder = new BrandViewHolder(view);

                viewHolder.setOnClickListener(new BrandViewHolder.ClickListener() {
                    @Override
                    public void onItemClick(View view, int position) {
                        int pos = position;
                        String nodo = "detaills";
                        data.send(pos, nodo);
                        //Brand item = (Brand) adapter.getItem(position);
                        //String mensaje = item.getDescripcion();
                        ((ChangeFragment) getActivity()).changeFragment(new DetailsFragment(), true); // Navigate to the next Fragment
                    }
                });
                return viewHolder;
            }

            @Override
            protected void onBindViewHolder(@NonNull BrandViewHolder holder, int position, @NonNull Brand model) {
                holder.bindBrand(model);
            }
        };

        recView.setAdapter(adapter);


        //int sidePadding = getResources().getDimensionPixelSize(R.dimen.brand_card_side_padding);
        //int itemPadding = getResources().getDimensionPixelSize(R.dimen.brand_card_item_padding);
        //recView.addItemDecoration(new BrandItemDecoration(sidePadding, itemPadding));

        return v;
    }

    private void setupToolbar(View view) {
        Toolbar toolbar = view.findViewById(R.id.toolbar);
        AppCompatActivity activity = (AppCompatActivity) getActivity();
        if (activity != null) {
            activity.setSupportActionBar(toolbar);
        }
        toolbar.setOnMenuItemClickListener(new Toolbar.OnMenuItemClickListener() {
            @Override
            public boolean onMenuItemClick(MenuItem menuItem) {
                switch (menuItem.getItemId()) {
                    case R.id.action_search:
                        //((NavigationHost) getActivity()).navigateTo(new LocationFragment(), true); // Navigate to the next Fragment
                        break;
                }
                return true;
            }
        });
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setHasOptionsMenu(true); // IMPORTANTE esta funcion para que aparescan los icones de la Toolbar
    }

    @Override
    public void onStart() {
        super.onStart();
        adapter.startListening();
    }

    @Override
    public void onStop() {
        super.onStop();
        adapter.stopListening();
    }

    // *********** Menu de la Toolbar **************************
    @Override
    public void onCreateOptionsMenu(Menu menu, MenuInflater menuInflater) {
        menuInflater.inflate(R.menu.menu_toolbar, menu);
        super.onCreateOptionsMenu(menu, menuInflater);
    }
}
