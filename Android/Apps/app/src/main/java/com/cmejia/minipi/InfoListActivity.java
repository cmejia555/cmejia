package com.cmejia.minipi;

import android.content.ContentValues;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.res.ColorStateList;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
import android.graphics.Camera;
import android.graphics.Color;
import android.preference.PreferenceManager;
import android.support.constraint.ConstraintLayout;
import android.support.design.widget.FloatingActionButton;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.ContextMenu;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ListView;
import android.widget.TextView;
import android.support.v7.widget.Toolbar;


import org.w3c.dom.Text;

import java.util.ArrayList;
import java.util.List;

import adapters.ListViewAdapter;
import clases.BookSQLite;
import clases.Library;

public class InfoListActivity extends AppCompatActivity {

    public ListView myListView;
    public TextView welcomeMsg;
    public Toolbar toolbar;
    public FloatingActionButton floatButton;

    public SQLiteDatabase db;
    public ListViewAdapter adapter;

    public List<Library> info;
    public int positionID;
    public boolean itemEditFlag = false;
    public String changedBookID;
    public boolean addItemFlag = false;

    public TextView text1;
    public TextView text2;
    public boolean settingFlag = false;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_info_list);

        text1 = findViewById(R.id.book_name);
        text2 = findViewById(R.id.subject);

        myListView = findViewById(R.id.info_list);
        toolbar = findViewById(R.id.toolbar);
        floatButton = findViewById(R.id.float_button);

        PreferenceManager.setDefaultValues(this, R.xml.preferences, false);

        setSupportActionBar(toolbar); // Indica que toolbar va a ser la ActionBar de la activity

        registerForContextMenu(myListView); // Enlaza el menu contextual al ListView

        // Levanta info de la base de datos y lo pasa a Library como parametros
        final BookSQLite bookdb = new BookSQLite(this, "DBBook.db", null, 1);
        db = bookdb.getWritableDatabase(); // Referencia a userdb para modificacion
        Cursor c = db.query("BookTable", null, null, // Pregunto por todos los registros
                null,null, null, null);

        // Instancio los objetos que se van a mostrar en el ListView a partir de la base de datos
        info = new ArrayList<>();
        if (c.moveToFirst()) { // Nos aseguramos de que existe al menos un registro
            do { //Recorremos el cursor hasta que no haya más registros
                int id = c.getInt(0);
                String book= c.getString(1);
                String subject = c.getString(2);
                String details = c.getString(3);
                int imageID = c.getInt(4);
                info.add(new Library(id, book, subject, details, imageID));
            } while(c.moveToNext());
            c.close();
        }
        setupListViewAdapter(info); // Creo el adapter a traves del metodo

        //myListView.setOnItemSelectedListener(); DIFERENCIA??!!!
        myListView.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
                String detail = ((Library)parent.getItemAtPosition(position)).getDetails();
                int image = ((Library)parent.getItemAtPosition(position)).getImageID();

                Intent nextActDetails = new Intent(InfoListActivity.this, DetailsActivity.class);
                nextActDetails.putExtra("DETAILS", detail); // envio datos a la otra activity con Intent
                nextActDetails.putExtra("IMAGE", image);
                startActivity(nextActDetails);
            }
        });

        floatButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                addItemFlag = true;
                Intent nextActAddItem = new Intent(InfoListActivity.this, AddItemActivity.class);
                startActivity(nextActAddItem);
            }
        });

        // TESTING SharedPreferences
        welcomeMsg = findViewById(R.id.welcome_msg);
        SharedPreferences preferences = getSharedPreferences("UserPreferences", Context.MODE_PRIVATE);
        String user = preferences.getString("user","nombre");
        if( !user.equals("nombre") ) {
            String buffer = "Bienvenido: " + user;
            welcomeMsg.setText(buffer);
        }
    }

    private void setupListViewAdapter(List<Library> list) {
        this.adapter = new ListViewAdapter(this, list);
        myListView.setAdapter(adapter); // Enlaza el ListView al adapter
    }

    @Override
    protected void onResume() {
        super.onResume();

//        if(settingFlag) {

            ConstraintLayout constraintLayout = findViewById(R.id.info_list_act);
            SharedPreferences preferences = PreferenceManager.getDefaultSharedPreferences(this);
            String color = preferences.getString("pref_text_color","none");
            String size = preferences.getString("pref_text_size", "none");
            String back = preferences.getString("pref_background", "none");

            adapter.setTextColor(Color.parseColor(color));
            adapter.setTextSize(Integer.valueOf(size));
            constraintLayout.setBackgroundColor(Color.parseColor(back));

            adapter.notifyDataSetChanged();
            settingFlag = false;
//        }

    }

    @Override
    protected void onRestart() {
        super.onRestart();

        if(itemEditFlag) {
            int cantChange = 0;

            ContentValues register = new ContentValues();
            SharedPreferences preferences = getSharedPreferences("UpdateBookDB", Context.MODE_PRIVATE);
            String book = preferences.getString("bookName","not_data");
            String subject = preferences.getString("subject", "not_data");
            String details = preferences.getString("details", "not_data");

            Library item = info.get(positionID); // Obteno el item Library que se modifico

            if( !book.isEmpty() ) {
                register.put("bookName", book);
                item.setBookName(book);
                cantChange++;
            }
            if( !subject.isEmpty() ) {
                register.put("subject", subject);
                item.setSubject(subject);
                cantChange++;
            }
            if( !details.isEmpty() ) {
                register.put("details", details);
                item.setDetails(details);
                cantChange++;
            }

            if( cantChange > 0 ) {
                info.set(positionID, item); // Actualiza el item modificado en la ListView
                adapter.notifyDataSetChanged(); // Notifica el cambio

                String[] args = new String[] {changedBookID}; // Actualiza la base de datos
                db.update("BookTable", register, "bookName=?", args);
            }

            changedBookID = "";
            itemEditFlag = false;
        }

        if(addItemFlag) {
            SharedPreferences preferences = getSharedPreferences("UpdateBookDB", Context.MODE_PRIVATE);
            String book = preferences.getString("bookName","not_data");
            String subject = preferences.getString("subject", "not_data");
            String details = preferences.getString("details", "not_data");
            int image = preferences.getInt("imageID", 0);
            if (image != 0) {
                ContentValues register = new ContentValues();
                register.put("bookName", book);
                register.put("subject", subject);
                register.put("details", details);
                register.put("imageID", image);

                Library item = new Library(info.size() + 1, book, subject, details, image);
                info.add(item);
                adapter.notifyDataSetChanged();

                db.insert("BookTable", null, register); // inserta un registro
            }
            addItemFlag = false;
        }
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        db.close();
    }

    // *********** Menu de la Toolbar **************************
    @Override
    public boolean onCreateOptionsMenu(Menu menu) { // Genera el menu que se ve en la Toolbar
        getMenuInflater().inflate(R.menu.menu_toolbar, menu);
        return super.onCreateOptionsMenu(menu);
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) { //Metodo para realizar una accion al tocar algun icono
        switch (item.getItemId()) {
            //case R.id.action_add:
                // agregar activity de Anadir item
              //  break;
            case R.id.action_settings:
                settingFlag = true;
                Intent nextActSettings = new Intent(InfoListActivity.this, SettingsActivity.class);
                startActivity(nextActSettings);
                break;
            case R.id.action_search:
                // implementar una busqueda
                break;
            case R.id.action_log_out:
                Intent nextActLogin = new Intent(InfoListActivity.this, LoginActivity.class);
                startActivity(nextActLogin);
                finish();
                break;
            default:
                break;
        }
        return super.onOptionsItemSelected(item);
    }

    // *********** Menu Contextual **************************
    @Override
    public void onCreateContextMenu(ContextMenu menu, View v, ContextMenu.ContextMenuInfo menuInfo) {
        super.onCreateContextMenu(menu, v, menuInfo);

        MenuInflater inflater = getMenuInflater();
        inflater.inflate(R.menu.context_menu, menu);
    }

    @Override
    public boolean onContextItemSelected(MenuItem item) {
        AdapterView.AdapterContextMenuInfo menu = (AdapterView.AdapterContextMenuInfo) item.getMenuInfo();
        positionID = menu.position;
        changedBookID = (adapter.getItem(positionID)).getBookName(); // Obtengo el "id" del registro que se va a borrar o editar
        switch (item.getItemId()) {
            case R.id.op_edit: // Abre una activity para editar datos del item de la ListView
                itemEditFlag = true;
                Intent nextActEdit = new Intent(InfoListActivity.this, EditActivity.class);
                startActivity(nextActEdit);
                return true;

            case R.id.op_delete:
                // Elimina un item de la ListView. NOTA: automaticamente se elimina un item de List info!!!
                adapter.remove(adapter.getItem(positionID)); // Elimina el item
                adapter.notifyDataSetChanged(); // Actuliza la ListView

                // Elimina un registro de la base de datos
                String[] args = new String[] {changedBookID};
                db.delete("BookTable", "bookName=?", args); // Elimina el registro
                return true;
        }
        return super.onContextItemSelected(item);
    }
}
