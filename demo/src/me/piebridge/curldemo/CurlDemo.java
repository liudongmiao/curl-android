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

package me.piebridge.curldemo;

import me.piebridge.curl.Curl;
import android.app.Activity;
import android.os.Bundle;
import android.os.Handler;
import android.util.Log;
import android.widget.TextView;

public class CurlDemo extends Activity
{

    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);

        TextView textView = new TextView(this);

        try {
            Data data = new Data();
            data.getURL("https://piebridge.me/demo.html");
            textView.setText(Curl.curl_version() + "\n==========\n" + data.getData());
        } catch (UnsatisfiedLinkError e) {
            textView.setText("UnsatisfiedLinkError");
        } catch (ExceptionInInitializerError e) {
            textView.setText("ExceptionInInitializerError");
        }
        setContentView(textView);
    }

    public class Data extends Curl {
        private String data = "";
        public String getData()
        {
            return data;
        }
        public String getURL(String url)
        {
            int curl = curl_init();

            if (0 == curl) {
                return "curl_init failed";
            }

            curl_setopt(curl, CURLOPT_URL, url);
            curl_setopt(curl, CURLOPT_VERBOSE, 1);

            // CURLOPT_WRITEFUNCTION will have header too if CURLOPT_HEADER is true
            // curl_setopt(curl, CURLOPT_HEADER, 1);

            // or set CURLOPT_WRITEHEADER to file path
            curl_setopt(curl, CURLOPT_HEADERFUNCTION, new Curl.Write() {
                public int callback(byte[] ptr) {
                    if (ptr == null) {
                        Log.d("CURL-J-HEADER", "write null");
                        return -1;
                    }
                    data += new String(ptr);
                    Log.d("CURL-J-HEADER", new String(ptr));
                    return ptr.length;
                }
            });

            // or set CURLOPT_WRITEDATA to file path
            curl_setopt(curl, CURLOPT_WRITEFUNCTION, new Curl.Write() {
                public int callback(byte[] ptr) {
                    if (ptr == null) {
                        Log.d("CURL-J-WRITE", "write null");
                        return -1;
                    }
                    Log.d("CURL-J-WRITE", new String(ptr));
                    data += new String(ptr);
                    return ptr.length;
                }
            });

            // or set CURLOPT_STDERR to file path
            curl_setopt(curl, CURLOPT_DEBUGFUNCTION, new Curl.Debug() {
                public int callback(int type, byte[] ptr) {
                    if (ptr == null) {
                        Log.d("CURL-J-DEBUG", CURLINFO[type] + ": write null");
                        return 0;
                    }
                    if (type == CURLINFO_TEXT) {
                        Log.d("CURL-J-DEBUG", CURLINFO[type] + ": " + new String(ptr));
                    }
                    return 0;
                }
            });

            // simple progress function
            curl_setopt(curl, CURLOPT_NOPROGRESS, 0);
            curl_setopt(curl, CURLOPT_PROGRESSFUNCTION, new Curl.Progress() {
                public int callback(double dltotal, double dlnow, double ultotal, double ulnow) {
                    return 0;
                }
            });

            curl_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);

            // support ipv4 only
            curl_setopt(curl, CURLOPT_IPRESOLVE, CURL_IPRESOLVE_V4);

            // set dns servers
            curl_setopt(curl, CURLOPT_DNS_SERVERS, "8.8.8.8");

            // disable ssl verify
            curl_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);

            // enable cookie engine
            curl_setopt(curl, CURLOPT_COOKIELIST, "");

            // enable certificate chain info gatherer
            curl_setopt(curl, CURLOPT_CERTINFO, 1);

            if (!curl_perform(curl)) {
                data = curl_error(curl);
            } else {
                data += "=====getinfo=====\n";
                data += "total_time: " + curl_getinfo(curl, CURLINFO_TOTAL_TIME) + "\n";
                data += "namelookup_time: " + curl_getinfo(curl, CURLINFO_NAMELOOKUP_TIME) + "\n";
                data += "connect_time: " + curl_getinfo(curl, CURLINFO_CONNECT_TIME) + "\n";
                data += "starttransfer_time: " + curl_getinfo(curl, CURLINFO_STARTTRANSFER_TIME) + "\n";
                data += "redirect_time: " + curl_getinfo(curl, CURLINFO_REDIRECT_TIME) + "\n";
            }

            String[] cookies = curl_getinfo_list(curl, CURLINFO_COOKIELIST);
            if (cookies != null) {
                for (String cookie: cookies) {
                    Log.d("CURL-J-COOKIE", cookie);
                }
            }

            Object[] certinfos = curl_getinfo_certinfo(curl, CURLINFO_CERTINFO);
            if (certinfos != null) {
                for (Object certinfo: certinfos) {
                    for (String cert: (String[])certinfo) {
                        Log.d("CURL-J-CERT", cert);
                    }
                }
            }

            curl_cleanup(curl);
            return data;
        }
    }
}
