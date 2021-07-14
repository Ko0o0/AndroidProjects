package com.example.dexclassloaderexample4;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Context;
import android.os.Bundle;
import android.widget.TextView;
import android.widget.Toast;

import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;

public class MainActivity extends AppCompatActivity {
    static {
        System.loadLibrary("koo");
    }
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        if(loadAssetManager(this)) {
            Toast.makeText(this, "load AssetManager from JNI", Toast.LENGTH_SHORT).show();
            loadDex();
        }

        TextView textView = (TextView) findViewById(R.id.text);
        textView.setText(getMessage());
    }

    private String getMsg() {
        try {
            Class clazz = Class.forName("com.example.dexclassloaderexample4.Flag");
            Object object = clazz.newInstance();
            Method method = clazz.getMethod("getMessage");
            String msg = (String) method.invoke(object);
            return msg;
        } catch (ClassNotFoundException e) {
            e.printStackTrace();
        } catch (IllegalAccessException e) {
            e.printStackTrace();
        } catch (InvocationTargetException e) {
            e.printStackTrace();
        } catch (InstantiationException e) {
            e.printStackTrace();
        } catch (NoSuchMethodException e) {
            e.printStackTrace();
        }
        return "Error occurred !";
    }

    public native String getMessage();
    public native boolean loadAssetManager(Context context);
    public native void loadDex();
}