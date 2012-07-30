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

package me.piebridge.curl;

public interface Debug
{
    /*
     * CURLOPT_DEBUGFUNCTION
     * int curl_debug_callback(CURL *, curl_infotype, char *, size_t, void *);
     */
    public int callback(int type, byte[] ptr);

    public final int CURLINFO_TEXT = 0;
    public final int CURLINFO_HEADER_IN = 1;
    public final int CURLINFO_HEADER_OUT = 2;
    public final int CURLINFO_DATA_IN = 3;
    public final int CURLINFO_DATA_OUT = 4;
    public final int CURLINFO_SSL_DATA_IN = 5;
    public final int CURLINFO_SSL_DATA_OUT = 6;
    public final String[] CURLINFO = {
        "TEXT",
        "HEADER_IN",
        "HEADER_OUT",
        "DATA_IN",
        "DATA_OUT",
        "SSL_DATA_IN",
        "SSL_DATA_OUT",
        "END",
    };
}
