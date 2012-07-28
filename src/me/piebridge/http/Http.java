/*
 * vim: set sta sw=4 et:
 *
 * Copyright (C) 2012 Liu DongMiao <thom@piebridge.me>
 *
 * This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://sam.zoy.org/wtfpl/COPYING for more details.
 *
 */

package me.piebridge.http;

import android.app.Activity;
import android.os.Bundle;
import android.widget.TextView;
import android.util.Log;
import me.piebridge.curl.Curl;

public class Http extends Activity
{

    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);

        TextView textView = new TextView(this);

        Data data = new Data();
        String content = data.getURL("http://piebridge.me/i.html");
        textView.setText(content);
        setContentView(textView);
    }

    public class Data extends Curl {
        public String getURL(String url)
        {
            String data;
            int curl;

            curl = curl_init();
            if (0 == curl) {
                return "curl_init failed";
            }

            if (!curl_setopt(curl, CURLOPT_URL, url)) {
                return "curl_setopt curlopt_url failed";
            }

            if (!curl_setopt(curl, CURLOPT_HEADER, 1)) {
                return "curl_setopt curlopt_header failed";
            }

            if (!curl_setopt(curl, CURLOPT_FILETIME, 1)) {
                return "curl_setopt curlopt_filetime failed";
            }

            data = curl_perform(curl);
            if (curl_errno() != CURLE_OK) {
                data = curl_error();
            } else {
                data += "total_time: " + curl_getinfo(curl, CURLINFO_TOTAL_TIME) + "\n";
                data += "namelookup_time: " + curl_getinfo(curl, CURLINFO_NAMELOOKUP_TIME) + "\n";
                data += "connect_time: " + curl_getinfo(curl, CURLINFO_CONNECT_TIME) + "\n";
                data += "filetime: " + curl_getinfo(curl, CURLINFO_FILETIME) + "\n";
                data += "starttransfer_time: " + curl_getinfo(curl, CURLINFO_STARTTRANSFER_TIME) + "\n";
                data += "redirect_time: " + curl_getinfo(curl, CURLINFO_REDIRECT_TIME) + "\n";
            }

            curl_cleanup(curl);
            return data;
        }
    }
}
