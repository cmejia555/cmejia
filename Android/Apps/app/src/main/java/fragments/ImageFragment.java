package fragments;


import android.os.Bundle;
import android.support.annotation.Nullable;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageView;

import com.cmejia.minipi.R;

/**
 * A simple {@link Fragment} subclass.
 */
public class ImageFragment extends Fragment {

    public ImageView image;

    public ImageFragment() {
        // Required empty public constructor
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        // Inflate the layout for this fragment
        View v = inflater.inflate(R.layout.fragment_image, container, false);

        image = v.findViewById(R.id.photo);

        return v;
    }

    @Override
    public void onActivityCreated(@Nullable Bundle savedInstanceState) {
        super.onActivityCreated(savedInstanceState);

        String buf = getArguments().getString("IMAGE");
        setImage(Integer.parseInt(buf));
    }

    public void setImage(int id) {
        image.setImageResource(id);
    }
}
