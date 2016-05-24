package com.example.estudiante.tdestructor;

import android.app.ActionBar;
import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.Context;
import android.content.Intent;
import android.os.Handler;
import android.os.Message;
import android.support.v4.app.FragmentActivity;
import android.support.v4.app.FragmentTransaction;
import android.support.v7.app.ActionBarActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ListView;
import android.widget.SeekBar;
import android.widget.TextView;
import android.widget.Toast;
import android.widget.ToggleButton;

import java.util.BitSet;


public class MainActivity extends ActionBarActivity {

    private static final String TAG = "BluetoothChatFragment";

    // Intent request codes
    private static final int REQUEST_CONNECT_DEVICE_SECURE = 1;
    private static final int REQUEST_CONNECT_DEVICE_INSECURE = 2;
    private static final int REQUEST_ENABLE_BT = 3;

    // Layout Views
    private ListView mConversationView;
    private EditText mOutEditText;
    private Button mSendButton;

    /**
     * Name of the connected device
     */
    private String mConnectedDeviceName = null;



    /**
     * Local Bluetooth adapter
     */
    private BluetoothAdapter mBluetoothAdapter = null;

    /**
     * Member object for the chat services
     */

    private BluetoothService mBluetoothService = null;

    private boolean conectado;
    private SeekBar seekBarMotor1;
    private SeekBar seekBarMotor2;
    private TextView textViewMotor1;
    private TextView textViewMotor2;
    private TextView textViewBateria;
    private TextView estadosMotores;

    private Menu menu;
    private ToggleButton buttonStop;
    private ToggleButton apagarMotores;
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        textViewBateria=(TextView) findViewById(R.id.textView3);
        estadosMotores=(TextView) findViewById(R.id.textView4);
        conectado=false;
        //MOTOR 1
        seekBarMotor1=(SeekBar) findViewById(R.id.seekBar1);
        textViewMotor1=(TextView) findViewById(R.id.textView);
        seekBarMotor1.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            int progress = 0;
            char tipo;
            char valor;

            @Override
            public void onProgressChanged(SeekBar seekBar, int progresValue, boolean fromUser) {
                progress = 140-progresValue;
                tipo=1;
                if(progress<0){
                    tipo=2;
                    progress=progress*-1;
                }
                tipo--;
                progress=(int)progress/3;
                valor=(char)((tipo<<6)|progress);
                textViewMotor1.setText("Motor 1: " + progress+"  -> "+(int)tipo+" - "+progress+" > "+(int)valor);
                buttonStop.setEnabled(true);
                buttonStop.setChecked(false);
                if(conectado) {

                    sendMessage(valor+"");
                }
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {

            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {
                //textViewMotor1.setText("Covered: " + progress + "/" + seekBar.getMax());
            }
        });
        //MOTOR 2
        seekBarMotor2=(SeekBar) findViewById(R.id.seekBar2);
        textViewMotor2=(TextView) findViewById(R.id.textView2);
        seekBarMotor2.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            int progress = 0;
            char tipo;
            char valor;

            @Override
            public void onProgressChanged(SeekBar seekBar, int progresValue, boolean fromUser) {
                progress = 140-progresValue;
                tipo=1;
                if(progress<0){
                    tipo=2;
                    progress=progress*-1;
                }
                tipo--;
                progress=(int)progress/3;
                valor=(char)((tipo<<6)|progress);
                textViewMotor2.setText("Motor 2: " + progress+"  -> "+(int)tipo+" - "+valor+" > "+(int)valor+" ");
                buttonStop.setEnabled(true);
                buttonStop.setChecked(false);
                if(conectado) {
                    byte[] send=new byte[1];
                    send[0]= (byte) valor;
                    send[0]= (byte) (send[0]|1<<7);

                    sendMessageByte(send);
                }
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {

            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {
                //textViewMotor1.setText("Covered: " + progress + "/" + seekBar.getMax());
            }
        });

        buttonStop= (ToggleButton) findViewById(R.id.toggleButton);
        buttonStop.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                // Send a message using content of the edit text widget
                char stop=(char)(50);
                seekBarMotor1.setProgress(140);
                seekBarMotor2.setProgress(140);
                buttonStop.setEnabled(false);
                if(conectado) {
                    sendMessage(stop+"");
                }

            }
        });

        apagarMotores= (ToggleButton) findViewById(R.id.toggleButton2);
        apagarMotores.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                // Send a message using content of the edit text widget
                int apagar=(51);
                /*seekBarMotor1.setProgress(140);
                seekBarMotor2.setProgress(140);
                buttonStop.setEnabled(false);
                */
                if(!apagarMotores.isChecked()){
                    apagar=(52);
                }
                estadosMotores.setText(""+apagar);
                if(conectado) {
                    sendMessage((char)apagar+"");
                }

            }
        });
        mBluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
        if (mBluetoothAdapter != null) {
            if (!mBluetoothAdapter.isEnabled()) {
                Intent enableIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
                startActivityForResult(enableIntent, REQUEST_ENABLE_BT);

            }
        }else{
            // Device does not support Bluetooth
            //FragmentActivity activity = getActivity();
            //Toast.makeText(getApplicationContext(), "Bluetooth is not available", Toast.LENGTH_LONG).show();
            //activity.finish();
        }

    }


    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_main, menu);
        this.menu=menu;
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        switch (item.getItemId()) {
            case R.id.secure_connect_scan: {
                // Launch the DeviceListActivity to see devices and do scan
                Intent serverIntent = new Intent(this, DeviceListActivity.class);
                //startActivity(serverIntent);
                startActivityForResult(serverIntent, REQUEST_CONNECT_DEVICE_SECURE);
                //onClickConnect_scan();
                return true;
            }

        }

        return super.onOptionsItemSelected(item);
    }

    public void onClickConnect_scan() {
        Context context = getApplicationContext();
        CharSequence text = "Hello toast!";
        int duration = Toast.LENGTH_SHORT;

        Toast toast = Toast.makeText(context, text, duration);
        toast.show();
    }

    public void showMsn(String msn) {
        Context context = getApplicationContext();
        CharSequence text = msn;
        int duration = Toast.LENGTH_SHORT;

        Toast toast = Toast.makeText(context, text, duration);
        toast.show();
    }

    public void onActivityResult(int requestCode, int resultCode, Intent data) {
        switch (requestCode) {
            case REQUEST_CONNECT_DEVICE_SECURE:
                // When DeviceListActivity returns with a device to connect
                if (resultCode == Activity.RESULT_OK) {
                     connectDevice(data, true);
                }
                break;
            /*case REQUEST_CONNECT_DEVICE_INSECURE:
                // When DeviceListActivity returns with a device to connect
                if (resultCode == Activity.RESULT_OK) {
                    connectDevice(data, false);
                }
                break;
            case REQUEST_ENABLE_BT:
                // When the request to enable Bluetooth returns
                if (resultCode == Activity.RESULT_OK) {
                    // Bluetooth is now enabled, so set up a chat session
                    setupChat();
                } else {
                    // User did not enable Bluetooth or an error occurred
                    Log.d(TAG, "BT not enabled");
                    Toast.makeText(getActivity(), R.string.bt_not_enabled_leaving,
                            Toast.LENGTH_SHORT).show();
                    getActivity().finish();
                }*/
        }
    }




    /**
     * The Handler that gets information back from the BluetoothService
     */
    private final Handler mHandler = new Handler() {
        @Override
        public void handleMessage(Message msg) {
            switch (msg.what) {
                case BluetoothService.MESSAGE_STATE_CHANGE:
                    switch (msg.arg1) {
                        case BluetoothService.STATE_CONNECTED:
                            //setStatus(getString(R.string.title_connected_to, mConnectedDeviceName));
                            updateMenuTitles("connected to -> " + mConnectedDeviceName);
                            showMsn("connected to -> " + mConnectedDeviceName);
                            //Log.d(TAG, "Entro ");
                            //mConversationArrayAdapter.clear();
                            //Log.d(TAG, "Salio ");
                            conectado = true;
                            break;
                        case BluetoothService.STATE_CONNECTING:
                            setStatus(R.string.title_connecting);
                            conectado = false;
                            break;
                        case BluetoothService.STATE_LISTEN:
                        case BluetoothService.STATE_NONE:
                            setStatus(R.string.title_not_connected);
                            conectado = false;
                            break;
                    }
                    break;
                case BluetoothService.MESSAGE_WRITE:
                    byte[] writeBuf = (byte[]) msg.obj;
                    // construct a string from the buffer
                    String writeMessage = new String(writeBuf);
                    //mConversationArrayAdapter.add("Me:  " + writeMessage);
                    break;
                case BluetoothService.MESSAGE_READ:
                    byte[] readBuf = (byte[]) msg.obj;
                    // construct a string from the valid bytes in the buffer
                    String readMessage = new String(readBuf, 0, msg.arg1);
                    //mConversationArrayAdapter.add(mConnectedDeviceName + ":  " + readMessage);
                    String tem="";
                    for(int i=0;i<readMessage.length();i++){
                        tem=(float)(readMessage.charAt(i)&255)/10+" - ";
                    }
                    textViewBateria.setText("Bateria:  "+tem/*+"  Tamaño: "+readMessage.length()*/);
                    //showMsn(readMessage);
                    break;
                case BluetoothService.MESSAGE_DEVICE_NAME:
                    // save the connected device's name
                    mConnectedDeviceName = msg.getData().getString(BluetoothService.DEVICE_NAME);
                    Toast.makeText(getApplicationContext(), "Connected to "
                            + mConnectedDeviceName, Toast.LENGTH_SHORT).show();
                    break;
                case BluetoothService.MESSAGE_TOAST:
                    Toast.makeText(getApplicationContext(), msg.getData().getString(BluetoothService.TOAST),
                            Toast.LENGTH_SHORT).show();
                    break;
            }
        }
    };

    /**
     * Updates the status on the action bar.
     *
     * @param resId a string resource ID
     */
    private void setStatus(int resId) {
        updateMenuTitles(resId);
    }


    /**
     * Establish connection with other divice
     *
     * @param data   An {@link Intent} with {@link DeviceListActivity#EXTRA_DEVICE_ADDRESS} extra.
     * @param secure Socket Security type - Secure (true) , Insecure (false)
     */
    private void connectDevice(Intent data, boolean secure) {
        // Get the device MAC address
        String address = data.getExtras().getString(DeviceListActivity.EXTRA_DEVICE_ADDRESS);
        showMsn(address);
        // Get the BluetoothDevice object
        BluetoothDevice device = mBluetoothAdapter.getRemoteDevice(address);

        // Attempt to connect to the device
        if(mBluetoothService==null){
            mBluetoothService= new BluetoothService(getApplicationContext(),mHandler);
        }
        mBluetoothService.connect(device, secure);
    }


    /**
     * Sends a message.
     *
     * @param message A string of text to send.
     */
    private void sendMessage(String message) {
        // Check that we're actually connected before trying anything
        if (mBluetoothService.getState() != BluetoothService.STATE_CONNECTED) {
            Toast.makeText(getApplicationContext(), R.string.not_connected, Toast.LENGTH_SHORT).show();
            return;
        }

        // Check that there's actually something to send
        if (message.length() > 0) {
            // Get the message bytes and tell the BluetoothService to write
            byte[] send = message.getBytes();
            mBluetoothService.write(send);


        }
    }
    private void sendMessageByte(byte[] send ) {
        // Check that we're actually connected before trying anything
        if (mBluetoothService.getState() != BluetoothService.STATE_CONNECTED) {
            Toast.makeText(getApplicationContext(), R.string.not_connected, Toast.LENGTH_SHORT).show();
            return;
        }

        // Check that there's actually something to send
        if (send.length > 0) {
            // Get the message bytes and tell the BluetoothService to write
            //byte[] send = message.getBytes();
            //textViewMotor2.setText("Motor 2: " +" "+send.length+" "+(int)send[0]+" "+(int)send[send.length-1]+" ");

            mBluetoothService.write(send);


        }
    }


    private void updateMenuTitles(int resId) {
        MenuItem bedMenuItem = menu.findItem(R.id.title_state);
        //MenuItem bedMenuItem = (MenuItem) findViewById(R.id.title_state);
        bedMenuItem.setTitle(resId);
    }
    private void updateMenuTitles(String message) {
        MenuItem bedMenuItem = menu.findItem(R.id.title_state);
        //MenuItem bedMenuItem = (MenuItem) findViewById(R.id.title_state);
        bedMenuItem.setTitle(message);
    }

}
