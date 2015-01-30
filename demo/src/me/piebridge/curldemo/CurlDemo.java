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

import static me.piebridge.curl.Curl.*;
import me.piebridge.curl.Curl;

import android.app.Activity;
import android.os.AsyncTask;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

public class CurlDemo extends Activity implements OnClickListener {

    private EditText dnsView;

    private EditText urlView;

    private RetriveAsyncTask task;

    private TextView contentView;

    private boolean curlAvailable = false;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
        final Button retrive = (Button) this.findViewById(R.id.retrive);
        final TextView version = (TextView) this.findViewById(R.id.version);

        dnsView = (EditText) this.findViewById(R.id.dns);
        urlView = (EditText) this.findViewById(R.id.url);
        contentView = (TextView) this.findViewById(R.id.content);
        try {
            curlAvailable = true;
            retrive.setOnClickListener(this);
            version.setText(Curl.curl_version());
        } catch (UnsatisfiedLinkError e) {
            retrive.setEnabled(false);
            version.setText("UnsatisfiedLinkError");
        } catch (ExceptionInInitializerError e) {
            retrive.setEnabled(false);
            version.setText("ExceptionInInitializerError");
        }

        doRetrive();
    }

    private static class RetriveAsyncTask extends AsyncTask<String, Integer, String> {

        private TextView view;

        private String dns;

        private String url;

        public RetriveAsyncTask(TextView view, String dns, String url) {
            super();
            this.view = view;
            this.dns = dns;
            this.url = url;
        }

        @Override
        protected String doInBackground(String... args) {
            return getURL(this.dns, this.url);
        }

        @Override
        protected void onPostExecute(String result) {
            if (!this.isCancelled()) {
                view.setText(result);
            }
        }

        @Override
        protected void onPreExecute() {
            view.setText("retrieving " + url + "...");
        }

    }

    public static String getURL(String dns, String url) {
        final StringBuilder data = new StringBuilder();
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
                String header = new String(ptr);
                data.append(header);
                Log.d("CURL-J-HEADER", header);
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
                String body = new String(ptr);
                Log.d("CURL-J-WRITE", body);
                data.append(body);
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
        curl_setopt(curl, CURLOPT_XFERINFOFUNCTION, new Curl.Xferinfo() {
            @Override
            public int callback(long dltotal, long dlnow, long ultotal, long ulnow) {
                return 0;
            }
        });

        curl_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);

        // support ipv4 only
        curl_setopt(curl, CURLOPT_IPRESOLVE, CURL_IPRESOLVE_V4);

        // set dns servers
        curl_setopt(curl, CURLOPT_DNS_SERVERS, dns);

        // disable ssl verify
        curl_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);

        // enable cookie engine
        curl_setopt(curl, CURLOPT_COOKIELIST, "");

        // enable certificate chain info gatherer
        curl_setopt(curl, CURLOPT_CERTINFO, 1);

        if (!curl_perform(curl)) {
            data.append(curl_error(curl));
        } else {
            data.append("=====getinfo=====\n");
            data.append("total_time: ");
            data.append(curl_getinfo(curl, CURLINFO_TOTAL_TIME));
            data.append("\n");
            data.append("namelookup_time: ");
            data.append(curl_getinfo(curl, CURLINFO_NAMELOOKUP_TIME));
            data.append("\n");
            data.append("connect_time: ");
            data.append(curl_getinfo(curl, CURLINFO_CONNECT_TIME));
            data.append("\n");
            data.append("starttransfer_time: ");
            data.append(curl_getinfo(curl, CURLINFO_STARTTRANSFER_TIME));
            data.append("\n");
            data.append("redirect_time: ");
            data.append(curl_getinfo(curl, CURLINFO_REDIRECT_TIME));
            data.append("\n");
        }

        String[] cookies = curl_getinfo_list(curl, CURLINFO_COOKIELIST);
        if (cookies != null) {
            for (String cookie : cookies) {
                Log.d("CURL-J-COOKIE", cookie);
            }
        }

        Object[] certinfos = curl_getinfo_certinfo(curl, CURLINFO_CERTINFO);
        if (certinfos != null) {
            for (Object certinfo : certinfos) {
                for (String cert : (String[]) certinfo) {
                    Log.d("CURL-J-CERT", cert);
                }
            }
        }

        curl_cleanup(curl);
        return data.toString();
    }

    @Override
    public void onClick(View view) {
        switch (view.getId()) {
        case R.id.retrive:
            doRetrive();
        }
    }

    private String getText(EditText view, String fallback) {
        String text = view.getText().toString();
        if (text == null || text.trim().length() == 0) {
            return fallback;
        } else {
            return text.trim();
        }
    }

    private void doRetrive() {
        if (!curlAvailable) {
            return;
        }
        if (task != null) {
            task.cancel(true);
        }
        String dns = getText(dnsView, "8.8.8.8");
        String url = getText(urlView, "https://piebridge.me/robots.txt");
        task = new RetriveAsyncTask(contentView, dns, url);
        task.execute();
    }

}
