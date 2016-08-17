package com.innotechx.ndkaesdemo;

import android.content.pm.ApplicationInfo;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.TextView;
import android.widget.Toast;

import com.innotechx.jni.NativeUtils;


public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        boolean isDebuggable = (0 != (getApplicationInfo().flags & ApplicationInfo.FLAG_DEBUGGABLE));
        Log.i("test", "isDebuggable : " + isDebuggable);
    }

    public void getPrivateKey(View view) {
        String privateKey = NativeUtils.getPrivateKey(this);
        Toast.makeText(this, privateKey, Toast.LENGTH_SHORT).show();
    }

    public void checkSignature(View view) {
        boolean flag = NativeUtils.checkSignature(this);
        if (flag) {
            Toast.makeText(this, "签名没问题", Toast.LENGTH_SHORT).show();
        } else {
            Toast.makeText(this, "签名被篡改", Toast.LENGTH_SHORT).show();
        }
    }

    public void getSignature(View view) {
        TextView textView1 = (TextView) findViewById(R.id.text);
        textView1.setText("Signature的hashCode＝" + NativeUtils.getSignature(this));
    }
}
