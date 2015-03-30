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

import java.io.File;
import java.net.InetAddress;
import java.net.MalformedURLException;
import java.net.URL;
import java.net.UnknownHostException;
import java.util.Arrays;

import me.piebridge.curl.Curl;
import me.piebridge.curl.Curl.NameValuePair;
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

    private static File content;

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

        content = new File(getFilesDir(), "get.txt");
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

        public String getDns() {
            return this.dns;
        }

        public String getUrl() {
            return this.url;
        }

        @Override
        protected String doInBackground(String... args) {
            if (!this.isCancelled()) {
                return getURL(this.dns, this.url);
            } else {
                return null;
            }
        }

        @Override
        protected void onPostExecute(String result) {
            if (!this.isCancelled()) {
                view.setText(result);
            }
        }

        @Override
        protected void onPreExecute() {
            if (dns != null && dns.length() > 0) {
                view.setText("retrieving " + url + "... (" + dns + ")");
            } else {
                view.setText("retrieving " + url + "...");
            }
        }

    }

    static class Form implements NameValuePair {

        private static final String FILE_PREFIX = "@";
        private String name;
        private String value;

        public Form(String name, String value) {
            this.name = name;
            this.value = value;
        }

        @Override
        public String getName() {
            return this.name;
        }

        @Override
        public String getValue() {
            return this.value;
        }
    }

    static class FileForm extends Form implements FileNameValuePair {

        private String fileName;
        private String contentType;

        public FileForm(String name, File value, String fileName, String contentType) {
            super(name, value.getAbsolutePath());
            this.fileName = fileName;
            this.contentType = contentType;
        }

        @Override
        public String getFileName() {
            return this.fileName;
        }

        @Override
        public String getContentType() {
            return this.contentType;
        }
    }

    static class Callbacks extends Curl.Callbacks {

        private StringBuilder data;

        public Callbacks(StringBuilder data) {
            this.data = data;
        }

        @Override
        public int header(byte[] bytes) {
            if (bytes == null) {
                Log.d("CURL-J-HEADER", "write null");
                return -1;
            }
            String header = new String(bytes);
            data.append(header);
            Log.d("CURL-J-HEADER", header);
            return bytes.length;
        }

        @Override
        public int debug(int type, byte[] bytes) {
            if (bytes == null) {
                Log.d("CURL-J-DEBUG", CURLINFO[type] + ": write null");
                return 0;
            }
            if (type == CURLINFO_TEXT) {
                Log.d("CURL-J-DEBUG", CURLINFO[type] + ": " + new String(bytes));
            }
            return 0;
        }

        @Override
        public int xferinfo(long dltotal, long dlnow, long ultotal, long ulnow) {
            return 0;
        }
    }

    public static String getURL(String dns, String url) {
        final StringBuilder data = new StringBuilder();
        int curl = curl_init();

        if (0 == curl) {
            return "curl_init failed";
        }

        Callback callback = new Callbacks(data);

        curl_setopt(curl, CURLOPT_URL, url);
        curl_setopt(curl, CURLOPT_HTTPHEADER, Arrays.asList("User-Agent: Curl/Android"));
        curl_setopt(curl, CURLOPT_VERBOSE, 1);

        // CURLOPT_WRITEFUNCTION will have header too if CURLOPT_HEADER is true
        // curl_setopt(curl, CURLOPT_HEADER, 1);

        // or set CURLOPT_WRITEHEADER to file path
        curl_setopt(curl, CURLOPT_HEADERFUNCTION, callback);


        File output = content;
        try {
            URL uri = new URL(url);
            if (InetAddress.getByName(uri.getHost()).isSiteLocalAddress()) {
                curl_setopt(curl, CURLOPT_POST, 1);
                NameValuePair[] forms = Arrays.asList(
                        new Form("curl", "android"),
                        new Form("content", Form.FILE_PREFIX + content.getAbsolutePath()),
                        new FileForm("image", content, "image.png", "image/png")
                ).toArray(new NameValuePair[0]);
                curl_setopt(curl, CURLOPT_HTTPPOST, forms);
                output = new File(content.getParent(), "post.txt");
            }
        } catch (MalformedURLException e) {
            Log.w("", "invalid url: " + url, e);
        } catch (UnknownHostException e) {
            Log.w("", "invalid url: " + url, e);
        }

        // or set CURLOPT_WRITEFUNCTION to callback
        curl_setopt(curl, CURLOPT_FILE, output);

        // or set CURLOPT_STDERR to file path
        curl_setopt(curl, CURLOPT_DEBUGFUNCTION, callback);

        // simple progress function
        curl_setopt(curl, CURLOPT_NOPROGRESS, 0);
        curl_setopt(curl, CURLOPT_XFERINFOFUNCTION, callback);

        curl_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);

        // support ipv4 only
        curl_setopt(curl, CURLOPT_IPRESOLVE, CURL_IPRESOLVE_V4);

        // set dns servers
        if (dns != null && dns.length() > 0) {
            curl_setopt(curl, CURLOPT_DNS_SERVERS, dns);
        }

        // disable ssl verify
        curl_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);

        // enable cookie engine
        curl_setopt(curl, CURLOPT_COOKIELIST, "");

        // enable certificate chain info gatherer
        curl_setopt(curl, CURLOPT_CERTINFO, 1);

        if (!curl_perform(curl)) {
            data.append(curl_error(curl));
        } else {
            data.append("(content: see ");
            data.append(output);
            data.append(")");
            data.append("\n=====getinfo=====\n");
            data.append("code: ");
            data.append(curl_getinfo(curl, CURLINFO_RESPONSE_CODE));
            data.append("\n");
            data.append("content-type: ");
            data.append(curl_getinfo(curl, CURLINFO_CONTENT_TYPE));
            data.append("\n");
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

        byte[][] cookies = curl_getinfo_list(curl, CURLINFO_COOKIELIST);
        if (cookies != null) {
            for (byte[] cookie : cookies) {
                Log.d("CURL-J-COOKIE", new String(cookie));
            }
        }

        byte[][][] certinfos = curl_getinfo_certinfo(curl, CURLINFO_CERTINFO);
        if (certinfos != null) {
            for (byte[][] certinfo : certinfos) {
                for (byte[] cert : certinfo) {
                    Log.d("CURL-J-CERT", new String(cert));
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

    private boolean doRetrive() {
        if (!curlAvailable) {
            return false;
        }
        String dns = getText(dnsView, "");
        String url = getText(urlView, "https://piebridge.me/robots.txt");
        if (canIgnore(dns, url)) {
            return false;
        }
        if (task != null) {
            task.cancel(true);
        }
        task = new RetriveAsyncTask(contentView, dns, url);
        task.execute();
        return true;
    }

    private boolean canIgnore(String dns, String url) {
        if (task != null && task.getStatus() == AsyncTask.Status.RUNNING && task.getDns().equals(dns) && task.getUrl().equals(url)) {
            return true;
        }
        return false;
    }
}
