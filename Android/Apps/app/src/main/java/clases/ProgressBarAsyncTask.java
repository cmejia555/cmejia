package clases;

import android.os.AsyncTask;
import android.widget.ProgressBar;

public class ProgressBarAsyncTask extends AsyncTask<Integer, Integer, Boolean> {
    private ProgressBar pBar;

    public ProgressBarAsyncTask(ProgressBar progressBar) {
        this.pBar = progressBar;
    }
    @Override
    protected void onPreExecute() {
        super.onPreExecute();
        pBar.setProgress(0);
        pBar.setMax(100);
    }

    @Override
    protected Boolean doInBackground(Integer... integers) {
        for(int i = 1; i <= (integers[0]/1000); i++) {
            try {
                Thread.sleep(1000);
            }
            catch (InterruptedException e) {}

            publishProgress(i*33);
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
    }

    @Override
    protected void onCancelled(Boolean aBoolean) {
        super.onCancelled(aBoolean);
    }
}
