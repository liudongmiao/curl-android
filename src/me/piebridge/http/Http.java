package me.piebridge.http;

import android.app.Activity;
import android.os.Bundle;
import android.widget.TextView;

public class Http extends Activity
{
    public native String getWebpage(String url);

    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);

	TextView textView = new TextView(this);
	String http = getWebpage("http://piebridge.me/i.html");
	String https = getWebpage("https://piebridge.me/i.html");
	textView.setText("http: " + http + "\r\nhttps: " + https);
	setContentView(textView);
    }

    static {
        System.loadLibrary("http");
    }
}
