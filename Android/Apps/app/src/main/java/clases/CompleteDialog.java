package clases;

import android.support.v7.app.AlertDialog;
import android.app.Dialog;
import android.content.DialogInterface;
import android.os.Bundle;
import android.support.annotation.NonNull;
import android.support.annotation.Nullable;
import android.support.v4.app.DialogFragment;

import com.cmejia.minipi.R;

public class CompleteDialog extends DialogFragment {
    @NonNull
    @Override
    public Dialog onCreateDialog(@Nullable Bundle savedInstanceState) {
        // 1. Instanciar un AlertDialog.Builder con su constructor
        AlertDialog.Builder builder = new AlertDialog.Builder(getActivity());
        // 2. Definir las características del Dialog
        builder.setTitle(R.string.dialog_title);
        builder.setMessage(R.string.dialog_msg);
        builder.setPositiveButton(R.string.dialog_positive_ok, new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialog, int which) {

            }
        });
        // 3. Obtener el AlertDialog desde el método create()
        return builder.create();
    }
}
