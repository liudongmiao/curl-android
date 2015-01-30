/*
 * vim: set sta sw=4 et:
 *
 * Copyright (C) 2012 - 2014 Liu DongMiao <thom@piebridge.me>
 *
 * This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://sam.zoy.org/wtfpl/COPYING for more details.
 *
 */

package me.piebridge.curl;

public class Curl {

    private static final int CURLINFO_STRING = 0x100000;
    private static final int CURLINFO_LONG = 0x200000;
    private static final int CURLINFO_DOUBLE = 0x300000;
    private static final int CURLINFO_SLIST = 0x400000;
    public static final int CURLINFO_EFFECTIVE_URL = CURLINFO_STRING + 1;
    public static final int CURLINFO_RESPONSE_CODE = CURLINFO_LONG + 2;
    public static final int CURLINFO_TOTAL_TIME = CURLINFO_DOUBLE + 3;
    public static final int CURLINFO_NAMELOOKUP_TIME = CURLINFO_DOUBLE + 4;
    public static final int CURLINFO_CONNECT_TIME = CURLINFO_DOUBLE + 5;
    public static final int CURLINFO_PRETRANSFER_TIME = CURLINFO_DOUBLE + 6;
    public static final int CURLINFO_SIZE_UPLOAD = CURLINFO_DOUBLE + 7;
    public static final int CURLINFO_SIZE_DOWNLOAD = CURLINFO_DOUBLE + 8;
    public static final int CURLINFO_SPEED_DOWNLOAD = CURLINFO_DOUBLE + 9;
    public static final int CURLINFO_SPEED_UPLOAD = CURLINFO_DOUBLE + 10;
    public static final int CURLINFO_HEADER_SIZE = CURLINFO_LONG + 11;
    public static final int CURLINFO_REQUEST_SIZE = CURLINFO_LONG + 12;
    public static final int CURLINFO_SSL_VERIFYRESULT = CURLINFO_LONG + 13;
    public static final int CURLINFO_FILETIME = CURLINFO_LONG + 14;
    public static final int CURLINFO_CONTENT_LENGTH_DOWNLOAD = CURLINFO_DOUBLE + 15;
    public static final int CURLINFO_CONTENT_LENGTH_UPLOAD = CURLINFO_DOUBLE + 16;
    public static final int CURLINFO_STARTTRANSFER_TIME = CURLINFO_DOUBLE + 17;
    public static final int CURLINFO_CONTENT_TYPE = CURLINFO_STRING + 18;
    public static final int CURLINFO_REDIRECT_TIME = CURLINFO_DOUBLE + 19;
    public static final int CURLINFO_REDIRECT_COUNT = CURLINFO_LONG + 20;
    public static final int CURLINFO_PRIVATE = CURLINFO_STRING + 21;
    public static final int CURLINFO_HTTP_CONNECTCODE = CURLINFO_LONG + 22;
    public static final int CURLINFO_HTTPAUTH_AVAIL = CURLINFO_LONG + 23;
    public static final int CURLINFO_PROXYAUTH_AVAIL = CURLINFO_LONG + 24;
    public static final int CURLINFO_OS_ERRNO = CURLINFO_LONG + 25;
    public static final int CURLINFO_NUM_CONNECTS = CURLINFO_LONG + 26;
    public static final int CURLINFO_SSL_ENGINES = CURLINFO_SLIST + 27;
    public static final int CURLINFO_COOKIELIST = CURLINFO_SLIST + 28;
    public static final int CURLINFO_LASTSOCKET = CURLINFO_LONG + 29;
    public static final int CURLINFO_FTP_ENTRY_PATH = CURLINFO_STRING + 30;
    public static final int CURLINFO_REDIRECT_URL = CURLINFO_STRING + 31;
    public static final int CURLINFO_PRIMARY_IP = CURLINFO_STRING + 32;
    public static final int CURLINFO_APPCONNECT_TIME = CURLINFO_DOUBLE + 33;
    public static final int CURLINFO_CERTINFO = CURLINFO_SLIST + 34;
    public static final int CURLINFO_CONDITION_UNMET = CURLINFO_LONG + 35;
    public static final int CURLINFO_RTSP_SESSION_ID = CURLINFO_STRING + 36;
    public static final int CURLINFO_RTSP_CLIENT_CSEQ = CURLINFO_LONG + 37;
    public static final int CURLINFO_RTSP_SERVER_CSEQ = CURLINFO_LONG + 38;
    public static final int CURLINFO_RTSP_CSEQ_RECV = CURLINFO_LONG + 39;
    public static final int CURLINFO_PRIMARY_PORT = CURLINFO_LONG + 40;
    public static final int CURLINFO_LOCAL_IP = CURLINFO_STRING + 41;
    public static final int CURLINFO_LOCAL_PORT = CURLINFO_LONG + 42;
    public static final int CURLINFO_TLS_SESSION = CURLINFO_SLIST + 43;

    private static final int CURLOPTTYPE_LONG = 0;
    private static final int CURLOPTTYPE_OBJECTPOINT = 10000;
    private static final int CURLOPTTYPE_FUNCTIONPOINT = 20000;
    private static final int CURLOPTTYPE_OFF_T = 30000;
    public static final int CURLOPT_WRITEDATA = CURLOPTTYPE_OBJECTPOINT + 1;
    public static final int CURLOPT_URL = CURLOPTTYPE_OBJECTPOINT + 2;
    public static final int CURLOPT_PORT = CURLOPTTYPE_LONG + 3;
    public static final int CURLOPT_PROXY = CURLOPTTYPE_OBJECTPOINT + 4;
    public static final int CURLOPT_USERPWD = CURLOPTTYPE_OBJECTPOINT + 5;
    public static final int CURLOPT_PROXYUSERPWD = CURLOPTTYPE_OBJECTPOINT + 6;
    public static final int CURLOPT_RANGE = CURLOPTTYPE_OBJECTPOINT + 7;
    public static final int CURLOPT_READDATA = CURLOPTTYPE_OBJECTPOINT + 9;
    public static final int CURLOPT_ERRORBUFFER = CURLOPTTYPE_OBJECTPOINT + 10;
    public static final int CURLOPT_WRITEFUNCTION = CURLOPTTYPE_FUNCTIONPOINT + 11;
    public static final int CURLOPT_READFUNCTION = CURLOPTTYPE_FUNCTIONPOINT + 12;
    public static final int CURLOPT_TIMEOUT = CURLOPTTYPE_LONG + 13;
    public static final int CURLOPT_INFILESIZE = CURLOPTTYPE_LONG + 14;
    public static final int CURLOPT_POSTFIELDS = CURLOPTTYPE_OBJECTPOINT + 15;
    public static final int CURLOPT_REFERER = CURLOPTTYPE_OBJECTPOINT + 16;
    public static final int CURLOPT_FTPPORT = CURLOPTTYPE_OBJECTPOINT + 17;
    public static final int CURLOPT_USERAGENT = CURLOPTTYPE_OBJECTPOINT + 18;
    public static final int CURLOPT_LOW_SPEED_LIMIT = CURLOPTTYPE_LONG + 19;
    public static final int CURLOPT_LOW_SPEED_TIME = CURLOPTTYPE_LONG + 20;
    public static final int CURLOPT_RESUME_FROM = CURLOPTTYPE_LONG + 21;
    public static final int CURLOPT_COOKIE = CURLOPTTYPE_OBJECTPOINT + 22;
    public static final int CURLOPT_HTTPHEADER = CURLOPTTYPE_OBJECTPOINT + 23;
    public static final int CURLOPT_HTTPPOST = CURLOPTTYPE_OBJECTPOINT + 24;
    public static final int CURLOPT_SSLCERT = CURLOPTTYPE_OBJECTPOINT + 25;
    public static final int CURLOPT_KEYPASSWD = CURLOPTTYPE_OBJECTPOINT + 26;
    public static final int CURLOPT_CRLF = CURLOPTTYPE_LONG + 27;
    public static final int CURLOPT_QUOTE = CURLOPTTYPE_OBJECTPOINT + 28;
    public static final int CURLOPT_HEADERDATA = CURLOPTTYPE_OBJECTPOINT + 29;
    public static final int CURLOPT_COOKIEFILE = CURLOPTTYPE_OBJECTPOINT + 31;
    public static final int CURLOPT_SSLVERSION = CURLOPTTYPE_LONG + 32;
    public static final int CURLOPT_TIMECONDITION = CURLOPTTYPE_LONG + 33;
    public static final int CURLOPT_TIMEVALUE = CURLOPTTYPE_LONG + 34;
    public static final int CURLOPT_CUSTOMREQUEST = CURLOPTTYPE_OBJECTPOINT + 36;
    public static final int CURLOPT_STDERR = CURLOPTTYPE_OBJECTPOINT + 37;
    public static final int CURLOPT_POSTQUOTE = CURLOPTTYPE_OBJECTPOINT + 39;
    @Deprecated
    public static final int CURLOPT_OBSOLETE40 = CURLOPTTYPE_OBJECTPOINT + 40;
    public static final int CURLOPT_VERBOSE = CURLOPTTYPE_LONG + 41;
    public static final int CURLOPT_HEADER = CURLOPTTYPE_LONG + 42;
    public static final int CURLOPT_NOPROGRESS = CURLOPTTYPE_LONG + 43;
    public static final int CURLOPT_NOBODY = CURLOPTTYPE_LONG + 44;
    public static final int CURLOPT_FAILONERROR = CURLOPTTYPE_LONG + 45;
    public static final int CURLOPT_UPLOAD = CURLOPTTYPE_LONG + 46;
    public static final int CURLOPT_POST = CURLOPTTYPE_LONG + 47;
    public static final int CURLOPT_DIRLISTONLY = CURLOPTTYPE_LONG + 48;
    public static final int CURLOPT_APPEND = CURLOPTTYPE_LONG + 50;
    public static final int CURLOPT_NETRC = CURLOPTTYPE_LONG + 51;
    public static final int CURLOPT_FOLLOWLOCATION = CURLOPTTYPE_LONG + 52;
    public static final int CURLOPT_TRANSFERTEXT = CURLOPTTYPE_LONG + 53;
    public static final int CURLOPT_PUT = CURLOPTTYPE_LONG + 54;
    /**
     * @deprecated by CURLOPT_XFERINFOFUNCTION
     */
    @Deprecated
    public static final int CURLOPT_PROGRESSFUNCTION = CURLOPTTYPE_FUNCTIONPOINT + 56;
    public static final int CURLOPT_PROGRESSDATA = CURLOPTTYPE_OBJECTPOINT + 57;
    public static final int CURLOPT_XFERINFODATA = CURLOPT_PROGRESSDATA;
    public static final int CURLOPT_AUTOREFERER = CURLOPTTYPE_LONG + 58;
    public static final int CURLOPT_PROXYPORT = CURLOPTTYPE_LONG + 59;
    public static final int CURLOPT_POSTFIELDSIZE = CURLOPTTYPE_LONG + 60;
    public static final int CURLOPT_HTTPPROXYTUNNEL = CURLOPTTYPE_LONG + 61;
    public static final int CURLOPT_INTERFACE = CURLOPTTYPE_OBJECTPOINT + 62;
    public static final int CURLOPT_KRBLEVEL = CURLOPTTYPE_OBJECTPOINT + 63;
    public static final int CURLOPT_SSL_VERIFYPEER = CURLOPTTYPE_LONG + 64;
    public static final int CURLOPT_CAINFO = CURLOPTTYPE_OBJECTPOINT + 65;
    public static final int CURLOPT_MAXREDIRS = CURLOPTTYPE_LONG + 68;
    public static final int CURLOPT_FILETIME = CURLOPTTYPE_LONG + 69;
    public static final int CURLOPT_TELNETOPTIONS = CURLOPTTYPE_OBJECTPOINT + 70;
    public static final int CURLOPT_MAXCONNECTS = CURLOPTTYPE_LONG + 71;
    @Deprecated
    public static final int CURLOPT_OBSOLETE72 = CURLOPTTYPE_LONG + 72;
    public static final int CURLOPT_FRESH_CONNECT = CURLOPTTYPE_LONG + 74;
    public static final int CURLOPT_FORBID_REUSE = CURLOPTTYPE_LONG + 75;
    public static final int CURLOPT_RANDOM_FILE = CURLOPTTYPE_OBJECTPOINT + 76;
    public static final int CURLOPT_EGDSOCKET = CURLOPTTYPE_OBJECTPOINT + 77;
    public static final int CURLOPT_CONNECTTIMEOUT = CURLOPTTYPE_LONG + 78;
    public static final int CURLOPT_HEADERFUNCTION = CURLOPTTYPE_FUNCTIONPOINT + 79;
    public static final int CURLOPT_HTTPGET = CURLOPTTYPE_LONG + 80;
    public static final int CURLOPT_SSL_VERIFYHOST = CURLOPTTYPE_LONG + 81;
    public static final int CURLOPT_COOKIEJAR = CURLOPTTYPE_OBJECTPOINT + 82;
    public static final int CURLOPT_SSL_CIPHER_LIST = CURLOPTTYPE_OBJECTPOINT + 83;
    public static final int CURLOPT_HTTP_VERSION = CURLOPTTYPE_LONG + 84;
    public static final int CURLOPT_FTP_USE_EPSV = CURLOPTTYPE_LONG + 85;
    public static final int CURLOPT_SSLCERTTYPE = CURLOPTTYPE_OBJECTPOINT + 86;
    public static final int CURLOPT_SSLKEY = CURLOPTTYPE_OBJECTPOINT + 87;
    public static final int CURLOPT_SSLKEYTYPE = CURLOPTTYPE_OBJECTPOINT + 88;
    public static final int CURLOPT_SSLENGINE = CURLOPTTYPE_OBJECTPOINT + 89;
    public static final int CURLOPT_SSLENGINE_DEFAULT = CURLOPTTYPE_LONG + 90;
    public static final int CURLOPT_DNS_USE_GLOBAL_CACHE = CURLOPTTYPE_LONG + 91;
    public static final int CURLOPT_DNS_CACHE_TIMEOUT = CURLOPTTYPE_LONG + 92;
    public static final int CURLOPT_PREQUOTE = CURLOPTTYPE_OBJECTPOINT + 93;
    public static final int CURLOPT_DEBUGFUNCTION = CURLOPTTYPE_FUNCTIONPOINT + 94;
    public static final int CURLOPT_DEBUGDATA = CURLOPTTYPE_OBJECTPOINT + 95;
    public static final int CURLOPT_COOKIESESSION = CURLOPTTYPE_LONG + 96;
    public static final int CURLOPT_CAPATH = CURLOPTTYPE_OBJECTPOINT + 97;
    public static final int CURLOPT_BUFFERSIZE = CURLOPTTYPE_LONG + 98;
    public static final int CURLOPT_NOSIGNAL = CURLOPTTYPE_LONG + 99;
    public static final int CURLOPT_SHARE = CURLOPTTYPE_OBJECTPOINT + 100;
    public static final int CURLOPT_PROXYTYPE = CURLOPTTYPE_LONG + 101;
    public static final int CURLOPT_ACCEPT_ENCODING = CURLOPTTYPE_OBJECTPOINT + 102;
    public static final int CURLOPT_PRIVATE = CURLOPTTYPE_OBJECTPOINT + 103;
    public static final int CURLOPT_HTTP200ALIASES = CURLOPTTYPE_OBJECTPOINT + 104;
    public static final int CURLOPT_UNRESTRICTED_AUTH = CURLOPTTYPE_LONG + 105;
    public static final int CURLOPT_FTP_USE_EPRT = CURLOPTTYPE_LONG + 106;
    public static final int CURLOPT_HTTPAUTH = CURLOPTTYPE_LONG + 107;
    public static final int CURLOPT_SSL_CTX_FUNCTION = CURLOPTTYPE_FUNCTIONPOINT + 108;
    public static final int CURLOPT_SSL_CTX_DATA = CURLOPTTYPE_OBJECTPOINT + 109;
    public static final int CURLOPT_FTP_CREATE_MISSING_DIRS = CURLOPTTYPE_LONG + 110;
    public static final int CURLOPT_PROXYAUTH = CURLOPTTYPE_LONG + 111;
    public static final int CURLOPT_FTP_RESPONSE_TIMEOUT = CURLOPTTYPE_LONG + 112;
    public static final int CURLOPT_SERVER_RESPONSE_TIMEOUT = CURLOPT_FTP_RESPONSE_TIMEOUT;
    public static final int CURLOPT_IPRESOLVE = CURLOPTTYPE_LONG + 113;
    public static final int CURLOPT_MAXFILESIZE = CURLOPTTYPE_LONG + 114;
    public static final int CURLOPT_INFILESIZE_LARGE = CURLOPTTYPE_OFF_T + 115;
    public static final int CURLOPT_RESUME_FROM_LARGE = CURLOPTTYPE_OFF_T + 116;
    public static final int CURLOPT_MAXFILESIZE_LARGE = CURLOPTTYPE_OFF_T + 117;
    public static final int CURLOPT_NETRC_FILE = CURLOPTTYPE_OBJECTPOINT + 118;
    public static final int CURLOPT_USE_SSL = CURLOPTTYPE_LONG + 119;
    public static final int CURLOPT_POSTFIELDSIZE_LARGE = CURLOPTTYPE_OFF_T + 120;
    public static final int CURLOPT_TCP_NODELAY = CURLOPTTYPE_LONG + 121;
    public static final int CURLOPT_FTPSSLAUTH = CURLOPTTYPE_LONG + 129;
    public static final int CURLOPT_IOCTLFUNCTION = CURLOPTTYPE_FUNCTIONPOINT + 130;
    public static final int CURLOPT_IOCTLDATA = CURLOPTTYPE_OBJECTPOINT + 131;
    public static final int CURLOPT_FTP_ACCOUNT = CURLOPTTYPE_OBJECTPOINT + 134;
    public static final int CURLOPT_COOKIELIST = CURLOPTTYPE_OBJECTPOINT + 135;
    public static final int CURLOPT_IGNORE_CONTENT_LENGTH = CURLOPTTYPE_LONG + 136;
    public static final int CURLOPT_FTP_SKIP_PASV_IP = CURLOPTTYPE_LONG + 137;
    public static final int CURLOPT_FTP_FILEMETHOD = CURLOPTTYPE_LONG + 138;
    public static final int CURLOPT_LOCALPORT = CURLOPTTYPE_LONG + 139;
    public static final int CURLOPT_LOCALPORTRANGE = CURLOPTTYPE_LONG + 140;
    public static final int CURLOPT_CONNECT_ONLY = CURLOPTTYPE_LONG + 141;
    public static final int CURLOPT_CONV_FROM_NETWORK_FUNCTION = CURLOPTTYPE_FUNCTIONPOINT + 142;
    public static final int CURLOPT_CONV_TO_NETWORK_FUNCTION = CURLOPTTYPE_FUNCTIONPOINT + 143;
    public static final int CURLOPT_CONV_FROM_UTF8_FUNCTION = CURLOPTTYPE_FUNCTIONPOINT + 144;
    public static final int CURLOPT_MAX_SEND_SPEED_LARGE = CURLOPTTYPE_OFF_T + 145;
    public static final int CURLOPT_MAX_RECV_SPEED_LARGE = CURLOPTTYPE_OFF_T + 146;
    public static final int CURLOPT_FTP_ALTERNATIVE_TO_USER = CURLOPTTYPE_OBJECTPOINT + 147;
    public static final int CURLOPT_SOCKOPTFUNCTION = CURLOPTTYPE_FUNCTIONPOINT + 148;
    public static final int CURLOPT_SOCKOPTDATA = CURLOPTTYPE_OBJECTPOINT + 149;
    public static final int CURLOPT_SSL_SESSIONID_CACHE = CURLOPTTYPE_LONG + 150;
    public static final int CURLOPT_SSH_AUTH_TYPES = CURLOPTTYPE_LONG + 151;
    public static final int CURLOPT_SSH_PUBLIC_KEYFILE = CURLOPTTYPE_OBJECTPOINT + 152;
    public static final int CURLOPT_SSH_PRIVATE_KEYFILE = CURLOPTTYPE_OBJECTPOINT + 153;
    public static final int CURLOPT_FTP_SSL_CCC = CURLOPTTYPE_LONG + 154;
    public static final int CURLOPT_TIMEOUT_MS = CURLOPTTYPE_LONG + 155;
    public static final int CURLOPT_CONNECTTIMEOUT_MS = CURLOPTTYPE_LONG + 156;
    public static final int CURLOPT_HTTP_TRANSFER_DECODING = CURLOPTTYPE_LONG + 157;
    public static final int CURLOPT_HTTP_CONTENT_DECODING = CURLOPTTYPE_LONG + 158;
    public static final int CURLOPT_NEW_FILE_PERMS = CURLOPTTYPE_LONG + 159;
    public static final int CURLOPT_NEW_DIRECTORY_PERMS = CURLOPTTYPE_LONG + 160;
    public static final int CURLOPT_POSTREDIR = CURLOPTTYPE_LONG + 161;
    public static final int CURLOPT_SSH_HOST_PUBLIC_KEY_MD5 = CURLOPTTYPE_OBJECTPOINT + 162;
    public static final int CURLOPT_OPENSOCKETFUNCTION = CURLOPTTYPE_FUNCTIONPOINT + 163;
    public static final int CURLOPT_OPENSOCKETDATA = CURLOPTTYPE_OBJECTPOINT + 164;
    public static final int CURLOPT_COPYPOSTFIELDS = CURLOPTTYPE_OBJECTPOINT + 165;
    public static final int CURLOPT_PROXY_TRANSFER_MODE = CURLOPTTYPE_LONG + 166;
    public static final int CURLOPT_SEEKFUNCTION = CURLOPTTYPE_FUNCTIONPOINT + 167;
    public static final int CURLOPT_SEEKDATA = CURLOPTTYPE_OBJECTPOINT + 168;
    public static final int CURLOPT_CRLFILE = CURLOPTTYPE_OBJECTPOINT + 169;
    public static final int CURLOPT_ISSUERCERT = CURLOPTTYPE_OBJECTPOINT + 170;
    public static final int CURLOPT_ADDRESS_SCOPE = CURLOPTTYPE_LONG + 171;
    public static final int CURLOPT_CERTINFO = CURLOPTTYPE_LONG + 172;
    public static final int CURLOPT_USERNAME = CURLOPTTYPE_OBJECTPOINT + 173;
    public static final int CURLOPT_PASSWORD = CURLOPTTYPE_OBJECTPOINT + 174;
    public static final int CURLOPT_PROXYUSERNAME = CURLOPTTYPE_OBJECTPOINT + 175;
    public static final int CURLOPT_PROXYPASSWORD = CURLOPTTYPE_OBJECTPOINT + 176;
    public static final int CURLOPT_NOPROXY = CURLOPTTYPE_OBJECTPOINT + 177;
    public static final int CURLOPT_TFTP_BLKSIZE = CURLOPTTYPE_LONG + 178;
    public static final int CURLOPT_SOCKS5_GSSAPI_SERVICE = CURLOPTTYPE_OBJECTPOINT + 179;
    public static final int CURLOPT_SOCKS5_GSSAPI_NEC = CURLOPTTYPE_LONG + 180;
    public static final int CURLOPT_PROTOCOLS = CURLOPTTYPE_LONG + 181;
    public static final int CURLOPT_REDIR_PROTOCOLS = CURLOPTTYPE_LONG + 182;
    public static final int CURLOPT_SSH_KNOWNHOSTS = CURLOPTTYPE_OBJECTPOINT + 183;
    public static final int CURLOPT_SSH_KEYFUNCTION = CURLOPTTYPE_FUNCTIONPOINT + 184;
    public static final int CURLOPT_SSH_KEYDATA = CURLOPTTYPE_OBJECTPOINT + 185;
    public static final int CURLOPT_MAIL_FROM = CURLOPTTYPE_OBJECTPOINT + 186;
    public static final int CURLOPT_MAIL_RCPT = CURLOPTTYPE_OBJECTPOINT + 187;
    public static final int CURLOPT_FTP_USE_PRET = CURLOPTTYPE_LONG + 188;
    public static final int CURLOPT_RTSP_REQUEST = CURLOPTTYPE_LONG + 189;
    public static final int CURLOPT_RTSP_SESSION_ID = CURLOPTTYPE_OBJECTPOINT + 190;
    public static final int CURLOPT_RTSP_STREAM_URI = CURLOPTTYPE_OBJECTPOINT + 191;
    public static final int CURLOPT_RTSP_TRANSPORT = CURLOPTTYPE_OBJECTPOINT + 192;
    public static final int CURLOPT_RTSP_CLIENT_CSEQ = CURLOPTTYPE_LONG + 193;
    public static final int CURLOPT_RTSP_SERVER_CSEQ = CURLOPTTYPE_LONG + 194;
    public static final int CURLOPT_INTERLEAVEDATA = CURLOPTTYPE_OBJECTPOINT + 195;
    public static final int CURLOPT_INTERLEAVEFUNCTION = CURLOPTTYPE_FUNCTIONPOINT + 196;
    public static final int CURLOPT_WILDCARDMATCH = CURLOPTTYPE_LONG + 197;
    public static final int CURLOPT_CHUNK_BGN_FUNCTION = CURLOPTTYPE_FUNCTIONPOINT + 198;
    public static final int CURLOPT_CHUNK_END_FUNCTION = CURLOPTTYPE_FUNCTIONPOINT + 199;
    public static final int CURLOPT_FNMATCH_FUNCTION = CURLOPTTYPE_FUNCTIONPOINT + 200;
    public static final int CURLOPT_CHUNK_DATA = CURLOPTTYPE_OBJECTPOINT + 201;
    public static final int CURLOPT_FNMATCH_DATA = CURLOPTTYPE_OBJECTPOINT + 202;
    public static final int CURLOPT_RESOLVE = CURLOPTTYPE_OBJECTPOINT + 203;
    public static final int CURLOPT_TLSAUTH_USERNAME = CURLOPTTYPE_OBJECTPOINT + 204;
    public static final int CURLOPT_TLSAUTH_PASSWORD = CURLOPTTYPE_OBJECTPOINT + 205;
    public static final int CURLOPT_TLSAUTH_TYPE = CURLOPTTYPE_OBJECTPOINT + 206;
    public static final int CURLOPT_TRANSFER_ENCODING = CURLOPTTYPE_LONG + 207;
    public static final int CURLOPT_CLOSESOCKETFUNCTION = CURLOPTTYPE_FUNCTIONPOINT + 208;
    public static final int CURLOPT_CLOSESOCKETDATA = CURLOPTTYPE_OBJECTPOINT + 209;
    public static final int CURLOPT_GSSAPI_DELEGATION = CURLOPTTYPE_LONG + 210;
    public static final int CURLOPT_DNS_SERVERS = CURLOPTTYPE_OBJECTPOINT + 211;
    public static final int CURLOPT_ACCEPTTIMEOUT_MS = CURLOPTTYPE_LONG + 212;
    public static final int CURLOPT_TCP_KEEPALIVE = CURLOPTTYPE_LONG + 213;
    public static final int CURLOPT_TCP_KEEPIDLE = CURLOPTTYPE_LONG + 214;
    public static final int CURLOPT_TCP_KEEPINTVL = CURLOPTTYPE_LONG + 215;
    public static final int CURLOPT_SSL_OPTIONS = CURLOPTTYPE_LONG + 216;
    public static final int CURLOPT_MAIL_AUTH = CURLOPTTYPE_OBJECTPOINT + 217;
    public static final int CURLOPT_SASL_IR = CURLOPTTYPE_LONG + 218;
    public static final int CURLOPT_XFERINFOFUNCTION = CURLOPTTYPE_FUNCTIONPOINT + 219;
    public static final int CURLOPT_XOAUTH2_BEARER = CURLOPTTYPE_OBJECTPOINT + 220;
    public static final int CURLOPT_DNS_INTERFACE = CURLOPTTYPE_OBJECTPOINT + 221;
    public static final int CURLOPT_DNS_LOCAL_IP4 = CURLOPTTYPE_OBJECTPOINT + 222;
    public static final int CURLOPT_DNS_LOCAL_IP6 = CURLOPTTYPE_OBJECTPOINT + 223;
    public static final int CURLOPT_LOGIN_OPTIONS = CURLOPTTYPE_OBJECTPOINT + 224;
    public static final int CURLOPT_SSL_ENABLE_NPN = CURLOPTTYPE_LONG + 225;
    public static final int CURLOPT_SSL_ENABLE_ALPN = CURLOPTTYPE_LONG + 226;
    public static final int CURLOPT_EXPECT_100_TIMEOUT_MS = CURLOPTTYPE_LONG + 227;
    public static final int CURLOPT_PROXYHEADER = CURLOPTTYPE_OBJECTPOINT + 228;
    public static final int CURLOPT_HEADEROPT = CURLOPTTYPE_LONG + 229;
    public static final int CURLOPT_PINNEDPUBLICKEY = CURLOPTTYPE_OBJECTPOINT + 230;
    @Deprecated
    public static final int CURLOPT_ENCODING = CURLOPT_ACCEPT_ENCODING;
    @Deprecated
    public static final int CURLOPT_FILE = CURLOPT_WRITEDATA;
    @Deprecated
    public static final int CURLOPT_INFILE = CURLOPT_READDATA;
    @Deprecated
    public static final int CURLOPT_WRITEHEADER = CURLOPT_HEADERDATA;
    @Deprecated
    public static final int CURLOPT_WRITEINFO = CURLOPT_OBSOLETE40;
    @Deprecated
    public static final int CURLOPT_CLOSEPOLICY = CURLOPT_OBSOLETE72;
    @Deprecated
    public static final int CURLOPT_POST301 = CURLOPT_POSTREDIR;
    @Deprecated
    public static final int CURLOPT_SSLKEYPASSWD = CURLOPT_KEYPASSWD;
    @Deprecated
    public static final int CURLOPT_FTPAPPEND = CURLOPT_APPEND;
    @Deprecated
    public static final int CURLOPT_FTPLISTONLY = CURLOPT_DIRLISTONLY;
    @Deprecated
    public static final int CURLOPT_FTP_SSL = CURLOPT_USE_SSL;
    @Deprecated
    public static final int CURLOPT_SSLCERTPASSWD = CURLOPT_KEYPASSWD;
    @Deprecated
    public static final int CURLOPT_KRB4LEVEL = CURLOPT_KRBLEVEL;
    public static final int CURLOPT_RTSPHEADER = CURLOPT_HTTPHEADER;

    public static final int CURL_IPRESOLVE_WHATEVER = 0;
    public static final int CURL_IPRESOLVE_V4 = 1;
    public static final int CURL_IPRESOLVE_V6 = 2;

    public static native int curl_init();

    public static native int curl_errno(int curl);

    public static native String curl_error(int ucrl);

    // curl_easy_setopt with long, unsigned long, curl_off_t
    public static native boolean curl_setopt(int curl, int option, long value);

    // curl_easy_setopt with char *
    public static native boolean curl_setopt(int curl, int option, String value);

    // curl_easy_setopt with curl_slit
    public static native boolean curl_setopt(int curl, int option, String[] value);

    // curl_easy_setopt with void *
    public static native boolean curl_setopt(int curl, int option, byte[] value);

    // curl_easy_setopt with callback, Object is a class implements callback
    public static native boolean curl_setopt(int curl, int option, Object value);

    public static native boolean curl_perform(int curl);

    // curl_easy_getinfo with long
    public static native long curl_getinfo_long(int curl, int info);

    // curl_easy_getinfo with double
    public static native double curl_getinfo_double(int curl, int info);

    // curl_easy_getinfo with curl_slist
    public static native String[] curl_getinfo_list(int curl, int info);

    // curl_easy_getinfo with certinfo, certinfo is set of curl_slist
    // Object[] is equal String[][]
    public static native Object[] curl_getinfo_certinfo(int curl, int info);

    // curl_easy_getinfo with string, including long and double
    public static native String curl_getinfo(int curl, int info);

    public static native void curl_cleanup(int curl);

    public static native String curl_version();

    /*
     * CURLOPT_WRITEFUNCTION
     * CURLOPT_HEADERFUNCTION
     * size_t function(void *ptr, size_t size, size_t nmemb, void *userdata);
     */
    public interface Write {
        public int callback(byte[] ptr);
    }

    /*
     * CURLOPT_READFUNCTION
     * size_t function(void *ptr, size_t size, size_t nmemb, void *userdata);
     */
    public interface Read {
        public int callback(byte[] ptr);
    }

    /*
     * CURLOPT_DEBUGFUNCTION
     * int curl_debug_callback(CURL *, curl_infotype, char*, size_t, void *);
     */
    public interface Debug {
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

    /*
     * @deprecated by Xferinfo
     * CURLOPT_PROGRESSFUNCTION
     * typedef int (*curl_progress_callback)(void*clientp, double dltotal, double dlnow, double ultotal, double ulnow);
     */
    @Deprecated
    public interface Progress {
        public int callback(double dltotal, double dlnow, double ultotal, double ulnow);
    }

    /*
     * CURLOPT_XFERINFOFUNCTION
     * typedef int (*curl_xferinfo_callback)(void *clientp, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow);
     */
    public interface Xferinfo {
        public int callback(long dltotal, long dlnow, long ultotal, long ulnow);
    }

    static {
        System.loadLibrary("curl");
    }

}
