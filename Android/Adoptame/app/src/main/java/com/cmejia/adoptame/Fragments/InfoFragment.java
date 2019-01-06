package com.cmejia.adoptame.Fragments;


import android.os.Bundle;
import android.support.annotation.NonNull;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageView;
import android.widget.TextView;

import com.cmejia.adoptame.R;
import com.google.firebase.database.DataSnapshot;
import com.google.firebase.database.DatabaseError;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;
import com.google.firebase.database.ValueEventListener;

/**
 * A simple {@link Fragment} subclass.
 */
public class InfoFragment extends Fragment {

    static int id_node;
    static String node_str;
    ImageView image;
    TextView name;
    TextView phone;
    TextView dir;

    public InfoFragment() {
        // Required empty public constructor
    }


    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        // Inflate the layout for this fragment
        View v = inflater.inflate(R.layout.fragment_info, container, false);

        image = v.findViewById(R.id.image);
        name = v.findViewById(R.id.name);
        phone = v.findViewById(R.id.phone);
        dir = v.findViewById(R.id.dir);

        DatabaseReference dbDescription = FirebaseDatabase.getInstance().getReference()
                .child(node_str)
                .child(String.valueOf(id_node));

        ValueEventListener eventListener = new ValueEventListener()
        {
            @Override
            public void onDataChange(@NonNull DataSnapshot dataSnapshot)
            {
                image.setImageResource(dataSnapshot.child("image").getValue(int.class));
                name.setText(dataSnapshot.child("name").getValue(String.class));
                phone.setText(dataSnapshot.child("phone").getValue(String.class));
                dir.setText(dataSnapshot.child("dir").getValue(String.class));

            }
            @Override
            public void onCancelled(@NonNull DatabaseError databaseError)
            {

            }
        };
        dbDescription.addListenerForSingleValueEvent(eventListener);


        return v;
    }

    public void setData(int id, String node) {
        id_node = id;
        node_str = node;
    }

}
