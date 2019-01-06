package fragments;


import android.content.SharedPreferences;
import android.graphics.Color;
import android.os.Bundle;
import android.preference.PreferenceManager;
import android.support.annotation.Nullable;
import android.support.constraint.ConstraintLayout;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.FrameLayout;
import android.widget.Spinner;
import android.widget.TextView;

import com.cmejia.minipi.DetailsActivity;
import com.cmejia.minipi.R;

/**
 * A simple {@link Fragment} subclass.
 */
public class DetailsFragment extends Fragment {

    public TextView details;
    public Spinner options;
    public TextView debug;

    public DetailsFragment() {
        // Required empty public constructor
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {

        // Inflate the layout for this fragment
        View v = inflater.inflate(R.layout.fragment_details, container, false);
        options = v.findViewById(R.id.spinner_options);
        debug = v.findViewById(R.id.debug);
        details = v.findViewById(R.id.detail_frag);

        String buf = getArguments().getString("DETAILS");
        showDetails(buf);

        return v;
    }

    @Override
    public void onActivityCreated(@Nullable Bundle savedInstanceState) {
        super.onActivityCreated(savedInstanceState);

        // Creacion del adaptador con los strings desde el xml
        ArrayAdapter<CharSequence> adaptador =
                ArrayAdapter.createFromResource(getContext(), R.array.datos_array, android.R.layout.simple_spinner_item);

        adaptador.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item); // define como muestra los items

        options.setAdapter(adaptador); // setea el adaptador al spinner

        options.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {
            @Override
            public void onItemSelected(AdapterView<?> parent, View view, int position, long id) {
                debug.setText("Seleccionado " + parent.getItemAtPosition(position).toString());
            }

            @Override
            public void onNothingSelected(AdapterView<?> parent) {
                debug.setText("");
            }
        });
    }

    public void showDetails(String text) {
        details.setText(text);
    }

    @Override
    public void onResume() {
        super.onResume();
        SharedPreferences preferences = PreferenceManager.getDefaultSharedPreferences(getContext());
        String back = preferences.getString("pref_background", "none");
        String color = preferences.getString("pref_text_color","none");
        String size = preferences.getString("pref_text_size", "none");

        //FrameLayout layout = getView().findViewById(R.id.frame_layout);
        //layout.setBackgroundColor(Color.parseColor(color));
        details.setTextSize(Integer.valueOf(size));
        details.setTextColor(Color.parseColor(color));

    }
}
//private void copyDataBase() throws IOException {
  //  InputStream myInput = myContext.getAssets().open(DB_NAME);
    //String outFileName = DB_PATH + DB_NAME;
//}