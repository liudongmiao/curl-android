/* vim: set sw=4 ts=4:
 * Author: Liu DongMiao <liudongmiao@gmail.com>
 * Created  : Thu 26 Jul 2012 02:13:55 PM CST
 * Modified : Mon 30 Mar 2015 06:28:09 PM CST
 *
 * CopyRight (c) 2012, Liu DongMiao, <liudongmiao@gmail.com>.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "curl/curl.h"

#include <jni.h>
#ifdef __ANDROID__
#include <android/log.h>
#define TAG "CURL-C"
#define LOGE(...) (__android_log_print(ANDROID_LOG_ERROR, TAG, __VA_ARGS__))
#define LOGD(...) (__android_log_print(ANDROID_LOG_DEBUG, TAG, __VA_ARGS__))
#else
#define LOGE(...) ((void)0)
#define LOGD(...) ((void)0)
#endif

#define CLASSNAME "me/piebridge/curl/Curl"
#define ARRAY_SIZE(x) (sizeof(x) / sizeof(x[0]))

#define JCURL_JNI_VERSION JNI_VERSION_1_6

typedef struct {
	CURL *curl;
	CURLcode code;
	jobject *read;
	jobject *write;
	jobject *header;
	jobject *debug;
	jobject *progress;
	jobject *xferinfo;
	struct curl_slist *slists;
	struct curl_httppost *formpost;
} jcurl_t;

static JavaVM *jvm;

static inline JNIEnv *get_env() {
	JNIEnv *env;
	(*jvm)->GetEnv(jvm, (void **)&env, JCURL_JNI_VERSION);
	return env;
}

static inline jmethodID get_method_safely(JNIEnv *env, jclass class, const char *name, const char *sig)
{
	jmethodID method = (*env)->GetMethodID(env, class, name, sig);
	if (method == NULL) {
		(*env)->ExceptionClear(env);
	}
	return method;
}

#define METHOD_READ_NAME "read"
#define METHOD_WRITE_NAME "write"
#define METHOD_HEADER_NAME "header"
#define METHOD_DEBUG_NAME "debug"
#define METHOD_PROGRESS_NAME "progress"
#define METHOD_XFERINFO_NAME "xferinfo"

#define METHOD_READ_SIG "([B)I"
#define METHOD_WRITE_SIG "([B)I"
#define METHOD_HEADER_SIG "([B)I"
#define METHOD_DEBUG_SIG "(I[B)I"
#define METHOD_PROGRESS_SIG "(DDDD)I"
#define METHOD_XFERINFO_SIG "(JJJJ)I"

static jbyteArray convert_from_char(JNIEnv *env, char *data)
{
	int length = strlen(data);
	jbyteArray array = (*env)->NewByteArray(env, length);
	if (!array) {
		LOGE("cannot create byte[%d]", length);
		return NULL;
	}
	(*env)->SetByteArrayRegion(env, array, 0, length, (jbyte *)data);
	return array;
}

static jmethodID get_method(JNIEnv *env, jobject object, jint option)
{
	const char *sig;
	const char *name;
	jmethodID method;
	jclass class;

	switch (option) {
		case CURLOPT_READFUNCTION:
			name = METHOD_READ_NAME;
			sig = METHOD_READ_SIG;
			break;
		case CURLOPT_WRITEFUNCTION:
			name = METHOD_WRITE_NAME;
			sig = METHOD_WRITE_SIG;
			break;
		case CURLOPT_HEADERFUNCTION:
			name = METHOD_HEADER_NAME;
			sig = METHOD_HEADER_SIG;
			break;
		case CURLOPT_DEBUGFUNCTION:
			name = METHOD_DEBUG_NAME;
			sig = METHOD_DEBUG_SIG;
			break;
		case CURLOPT_PROGRESSFUNCTION:
			name = METHOD_PROGRESS_NAME;
			sig = METHOD_PROGRESS_SIG;
			break;
		case CURLOPT_SSL_CTX_FUNCTION:
		case CURLOPT_IOCTLFUNCTION:
		case CURLOPT_CONV_FROM_NETWORK_FUNCTION:
		case CURLOPT_CONV_TO_NETWORK_FUNCTION:
		case CURLOPT_CONV_FROM_UTF8_FUNCTION:
		case CURLOPT_SOCKOPTFUNCTION:
		case CURLOPT_OPENSOCKETFUNCTION:
		case CURLOPT_SEEKFUNCTION:
		case CURLOPT_SSH_KEYFUNCTION:
		case CURLOPT_INTERLEAVEFUNCTION:
		case CURLOPT_CHUNK_BGN_FUNCTION:
		case CURLOPT_CHUNK_END_FUNCTION:
		case CURLOPT_FNMATCH_FUNCTION:
		case CURLOPT_CLOSESOCKETFUNCTION:
			return NULL;
		case CURLOPT_XFERINFOFUNCTION:
			name = METHOD_XFERINFO_NAME;
			sig = METHOD_XFERINFO_SIG;
			break;
		default:
			return NULL;
	}
	class = (*env)->GetObjectClass(env, object);
	method = get_method_safely(env, class, name, sig);
	if (method == NULL) {
		method = get_method_safely(env, class, "callback", sig);
	}
	(*env)->DeleteLocalRef(env, class);
	return method;
}

static jint curl_init(JNIEnv *env, jobject clazz)
{
	jcurl_t *jcurl = NULL;
	CURL *curl = curl_easy_init();
	if (!curl) {
		return (int)jcurl;
	}
	jcurl = (jcurl_t *) malloc(sizeof(jcurl_t));
	if (jcurl) {
		memset((void *)jcurl, 0, sizeof(jcurl_t));
		jcurl->curl = curl;
	} else {
		curl_easy_cleanup(curl);
	}
	return (int)jcurl;
}

static void append_slists(struct curl_slist *slists, struct curl_slist *slist)
{
	struct curl_slist *head;
	if (NULL == slists) {
		slists = slist;
	} else {
		head = slists;
		while (head->next) {
			head = head->next;
		}
		head->next = slist;
	}
}

/**
 * curl_easy_setopt for long, curl_off_t
 */
static jboolean curl_setopt_long(JNIEnv *env, jobject clazz, jint handle, jint option, jlong value)
{
	jcurl_t *jcurl = (jcurl_t *)handle;
	if (NULL == jcurl) {
		return JNI_FALSE;
	}

	switch (option) {
		case CURLOPT_PORT:
		case CURLOPT_TIMEOUT:
		case CURLOPT_INFILESIZE:
		case CURLOPT_LOW_SPEED_LIMIT:
		case CURLOPT_LOW_SPEED_TIME:
		case CURLOPT_RESUME_FROM:
		case CURLOPT_CRLF:
		case CURLOPT_SSLVERSION:
		case CURLOPT_TIMECONDITION:
		case CURLOPT_TIMEVALUE:
		case CURLOPT_VERBOSE:
		case CURLOPT_HEADER:
		case CURLOPT_NOPROGRESS:
		case CURLOPT_NOBODY:
		case CURLOPT_FAILONERROR:
		case CURLOPT_UPLOAD:
		case CURLOPT_POST:
		case CURLOPT_DIRLISTONLY:
		case CURLOPT_APPEND:
		case CURLOPT_NETRC:
		case CURLOPT_FOLLOWLOCATION:
		case CURLOPT_TRANSFERTEXT:
		case CURLOPT_PUT:
		case CURLOPT_AUTOREFERER:
		case CURLOPT_PROXYPORT:
		case CURLOPT_POSTFIELDSIZE:
		case CURLOPT_HTTPPROXYTUNNEL:
		case CURLOPT_SSL_VERIFYPEER:
		case CURLOPT_MAXREDIRS:
		case CURLOPT_FILETIME:
		case CURLOPT_MAXCONNECTS:
		case CURLOPT_FRESH_CONNECT:
		case CURLOPT_FORBID_REUSE:
		case CURLOPT_CONNECTTIMEOUT:
		case CURLOPT_HTTPGET:
		case CURLOPT_SSL_VERIFYHOST:
		case CURLOPT_HTTP_VERSION:
		case CURLOPT_FTP_USE_EPSV:
		case CURLOPT_SSLENGINE_DEFAULT:
		case CURLOPT_DNS_USE_GLOBAL_CACHE:
		case CURLOPT_DNS_CACHE_TIMEOUT:
		case CURLOPT_COOKIESESSION:
		case CURLOPT_BUFFERSIZE:
		case CURLOPT_NOSIGNAL:
		case CURLOPT_PROXYTYPE:
		case CURLOPT_UNRESTRICTED_AUTH:
		case CURLOPT_FTP_USE_EPRT:
		case CURLOPT_HTTPAUTH:
		case CURLOPT_FTP_CREATE_MISSING_DIRS:
		case CURLOPT_PROXYAUTH:
		case CURLOPT_FTP_RESPONSE_TIMEOUT:
		case CURLOPT_IPRESOLVE:
		case CURLOPT_MAXFILESIZE:
		case CURLOPT_USE_SSL:
		case CURLOPT_TCP_NODELAY:
		case CURLOPT_FTPSSLAUTH:
		case CURLOPT_IGNORE_CONTENT_LENGTH:
		case CURLOPT_FTP_SKIP_PASV_IP:
		case CURLOPT_FTP_FILEMETHOD:
		case CURLOPT_LOCALPORT:
		case CURLOPT_LOCALPORTRANGE:
		case CURLOPT_CONNECT_ONLY:
		case CURLOPT_SSL_SESSIONID_CACHE:
		case CURLOPT_SSH_AUTH_TYPES:
		case CURLOPT_FTP_SSL_CCC:
		case CURLOPT_TIMEOUT_MS:
		case CURLOPT_CONNECTTIMEOUT_MS:
		case CURLOPT_HTTP_TRANSFER_DECODING:
		case CURLOPT_HTTP_CONTENT_DECODING:
		case CURLOPT_NEW_FILE_PERMS:
		case CURLOPT_NEW_DIRECTORY_PERMS:
		case CURLOPT_POSTREDIR:
		case CURLOPT_PROXY_TRANSFER_MODE:
		case CURLOPT_ADDRESS_SCOPE:
		case CURLOPT_CERTINFO:
		case CURLOPT_TFTP_BLKSIZE:
		case CURLOPT_SOCKS5_GSSAPI_NEC:
		case CURLOPT_PROTOCOLS:
		case CURLOPT_REDIR_PROTOCOLS:
		case CURLOPT_FTP_USE_PRET:
		case CURLOPT_RTSP_REQUEST:
		case CURLOPT_RTSP_CLIENT_CSEQ:
		case CURLOPT_RTSP_SERVER_CSEQ:
		case CURLOPT_WILDCARDMATCH:
		case CURLOPT_TRANSFER_ENCODING:
		case CURLOPT_TLSAUTH_TYPE:
		case CURLOPT_GSSAPI_DELEGATION:
		case CURLOPT_ACCEPTTIMEOUT_MS:
		case CURLOPT_TCP_KEEPALIVE:
		case CURLOPT_TCP_KEEPIDLE:
		case CURLOPT_TCP_KEEPINTVL:
		case CURLOPT_SSL_OPTIONS:
		case CURLOPT_SASL_IR:
		case CURLOPT_SSL_ENABLE_NPN:
		case CURLOPT_SSL_ENABLE_ALPN:
		case CURLOPT_EXPECT_100_TIMEOUT_MS:
		case CURLOPT_HEADEROPT:
		case CURLOPT_SSL_VERIFYSTATUS:
		/* curl_off_t is int64_t, jlong */
		case CURLOPT_INFILESIZE_LARGE:
		case CURLOPT_RESUME_FROM_LARGE:
		case CURLOPT_MAXFILESIZE_LARGE:
		case CURLOPT_POSTFIELDSIZE_LARGE:
		case CURLOPT_MAX_SEND_SPEED_LARGE:
		case CURLOPT_MAX_RECV_SPEED_LARGE:
			jcurl->code = curl_easy_setopt(jcurl->curl, option, value);
			if (CURLE_OK == jcurl->code) {
				return JNI_TRUE;
			}
		default:
			break;
	}
	return JNI_FALSE;
}

/**
 * curl_easy_setopt for char * or  FILE
 */
static jboolean curl_setopt_string(JNIEnv *env, jobject clazz, jint handle, jint option, jstring value)
{
	FILE *file;
	char *param;
	struct curl_slist *slist = NULL;
	jcurl_t *jcurl = (jcurl_t *)handle;
	if (NULL == jcurl) {
		return JNI_FALSE;
	}

	switch (option) {
		case CURLOPT_URL:
		case CURLOPT_PROXY:
		case CURLOPT_USERPWD:
		case CURLOPT_PROXYUSERPWD:
		case CURLOPT_RANGE:
		case CURLOPT_ERRORBUFFER:
		case CURLOPT_POSTFIELDS:
		case CURLOPT_REFERER:
		case CURLOPT_FTPPORT:
		case CURLOPT_USERAGENT:
		case CURLOPT_COOKIE:
		case CURLOPT_SSLCERT:
		case CURLOPT_KEYPASSWD:
		case CURLOPT_COOKIEFILE:
		case CURLOPT_CUSTOMREQUEST:
		case CURLOPT_INTERFACE:
		case CURLOPT_KRBLEVEL:
		case CURLOPT_CAINFO:
		case CURLOPT_RANDOM_FILE:
		case CURLOPT_EGDSOCKET:
		case CURLOPT_COOKIEJAR:
		case CURLOPT_SSL_CIPHER_LIST:
		case CURLOPT_SSLCERTTYPE:
		case CURLOPT_SSLKEY:
		case CURLOPT_SSLKEYTYPE:
		case CURLOPT_SSLENGINE:
		case CURLOPT_PREQUOTE:
		case CURLOPT_CAPATH:
		case CURLOPT_ACCEPT_ENCODING:
		case CURLOPT_NETRC_FILE:
		case CURLOPT_FTP_ACCOUNT:
		case CURLOPT_COOKIELIST:
		case CURLOPT_FTP_ALTERNATIVE_TO_USER:
		case CURLOPT_SSH_PUBLIC_KEYFILE:
		case CURLOPT_SSH_PRIVATE_KEYFILE:
		case CURLOPT_SSH_HOST_PUBLIC_KEY_MD5:
		case CURLOPT_COPYPOSTFIELDS:
		case CURLOPT_CRLFILE:
		case CURLOPT_ISSUERCERT:
		case CURLOPT_USERNAME:
		case CURLOPT_PASSWORD:
		case CURLOPT_PROXYUSERNAME:
		case CURLOPT_PROXYPASSWORD:
		case CURLOPT_NOPROXY:
		case CURLOPT_SOCKS5_GSSAPI_SERVICE:
		case CURLOPT_SSH_KNOWNHOSTS:
		case CURLOPT_MAIL_FROM:
		case CURLOPT_RTSP_SESSION_ID:
		case CURLOPT_RTSP_STREAM_URI:
		case CURLOPT_RTSP_TRANSPORT:
		case CURLOPT_TLSAUTH_USERNAME:
		case CURLOPT_TLSAUTH_PASSWORD:
		case CURLOPT_DNS_SERVERS:
		case CURLOPT_MAIL_AUTH:
		case CURLOPT_XOAUTH2_BEARER:
		case CURLOPT_DNS_INTERFACE:
		case CURLOPT_DNS_LOCAL_IP4:
		case CURLOPT_DNS_LOCAL_IP6:
		case CURLOPT_LOGIN_OPTIONS:
		case CURLOPT_UNIX_SOCKET_PATH:
			param = (char *)(*env)->GetStringUTFChars(env, value, NULL);
			slist = curl_slist_append(slist, (char *)param);
			(*env)->ReleaseStringUTFChars(env, value, param);
			if (NULL == slist) {
				LOGE("cannot append data to slist");
				return JNI_FALSE;
			}
			jcurl->code = curl_easy_setopt(jcurl->curl, option, slist->data);
			if (CURLE_OK == jcurl->code) {
				append_slists(jcurl->slists, slist);
				return JNI_TRUE;
			} else {
				curl_slist_free_all(slist);
				return JNI_FALSE;
			}
			break;
		/* FILE */
		case CURLOPT_WRITEDATA:
		case CURLOPT_READDATA:
		case CURLOPT_HEADERDATA:
		case CURLOPT_STDERR:
			param = (char *)(*env)->GetStringUTFChars(env, value, NULL);
			// XXX: how to close file?
			file = fopen(param, "wb");
			(*env)->ReleaseStringUTFChars(env, value, param);
			if (NULL == file) {
				LOGE("cannot open: %s", strerror(errno));
				return JNI_FALSE;
			}
			jcurl->code = curl_easy_setopt(jcurl->curl, option, file);
			if (CURLE_OK == jcurl->code) {
				return JNI_TRUE;
			} else {
				return JNI_FALSE;
			}
		default:
			return JNI_FALSE;
	}
}

/**
 * curl_easy_setopt for struct curl_slist *
 */
static jboolean curl_setopt_slist(JNIEnv *env, jobject clazz, jint handle, jint option, jobjectArray value)
{
	int i, count;
	struct curl_slist *slist = NULL;
	jcurl_t *jcurl = (jcurl_t *)handle;
	if (NULL == jcurl) {
		return JNI_FALSE;
	}

	switch (option) {
		case CURLOPT_HTTPHEADER:
		case CURLOPT_QUOTE:
		case CURLOPT_POSTQUOTE:
		case CURLOPT_TELNETOPTIONS:
		case CURLOPT_PREQUOTE:
		case CURLOPT_HTTP200ALIASES:
		case CURLOPT_MAIL_RCPT:
		case CURLOPT_RESOLVE:
			break;
		default:
			return JNI_FALSE;
	}

	count = (*env)->GetArrayLength(env, value);

	for (i = 0; i < count; ++i) {
		jbyteArray item = (*env)->GetObjectArrayElement(env, value, i);
		jbyte *data = (*env)->GetByteArrayElements(env, item, NULL);
		slist = curl_slist_append(slist, (char *)data);
		(*env)->ReleaseByteArrayElements(env, item, data, 0);
		(*env)->DeleteLocalRef(env, item);
		if (NULL == slist) {
			return JNI_FALSE;
		}
	}

	jcurl->code = curl_easy_setopt(jcurl->curl, option, slist);
	if (CURLE_OK != jcurl->code) {
		curl_slist_free_all(slist);
		return JNI_FALSE;
	}

	append_slists(jcurl->slists, slist);

	return JNI_TRUE;
}

/*
 * curl_easy_setopt for char *
 */
static jboolean curl_setopt_bytes(JNIEnv *env, jobject clazz, jint handle, jint option, jbyteArray value)
{
	jbyte *data;
	struct curl_slist *slist = NULL;
	jcurl_t *jcurl = (jcurl_t *)handle;
	if (NULL == jcurl) {
		return JNI_FALSE;
	}

	switch (option) {
		case CURLOPT_URL:
		case CURLOPT_PROXY:
		case CURLOPT_USERPWD:
		case CURLOPT_PROXYUSERPWD:
		case CURLOPT_RANGE:
		case CURLOPT_ERRORBUFFER:
		case CURLOPT_POSTFIELDS:
		case CURLOPT_REFERER:
		case CURLOPT_FTPPORT:
		case CURLOPT_USERAGENT:
		case CURLOPT_COOKIE:
		case CURLOPT_SSLCERT:
		case CURLOPT_KEYPASSWD:
		case CURLOPT_COOKIEFILE:
		case CURLOPT_CUSTOMREQUEST:
		case CURLOPT_INTERFACE:
		case CURLOPT_KRBLEVEL:
		case CURLOPT_CAINFO:
		case CURLOPT_RANDOM_FILE:
		case CURLOPT_EGDSOCKET:
		case CURLOPT_COOKIEJAR:
		case CURLOPT_SSL_CIPHER_LIST:
		case CURLOPT_SSLCERTTYPE:
		case CURLOPT_SSLKEY:
		case CURLOPT_SSLKEYTYPE:
		case CURLOPT_SSLENGINE:
		case CURLOPT_PREQUOTE:
		case CURLOPT_CAPATH:
		case CURLOPT_ACCEPT_ENCODING:
		case CURLOPT_NETRC_FILE:
		case CURLOPT_FTP_ACCOUNT:
		case CURLOPT_COOKIELIST:
		case CURLOPT_FTP_ALTERNATIVE_TO_USER:
		case CURLOPT_SSH_PUBLIC_KEYFILE:
		case CURLOPT_SSH_PRIVATE_KEYFILE:
		case CURLOPT_SSH_HOST_PUBLIC_KEY_MD5:
		case CURLOPT_COPYPOSTFIELDS:
		case CURLOPT_CRLFILE:
		case CURLOPT_ISSUERCERT:
		case CURLOPT_USERNAME:
		case CURLOPT_PASSWORD:
		case CURLOPT_PROXYUSERNAME:
		case CURLOPT_PROXYPASSWORD:
		case CURLOPT_NOPROXY:
		case CURLOPT_SOCKS5_GSSAPI_SERVICE:
		case CURLOPT_SSH_KNOWNHOSTS:
		case CURLOPT_MAIL_FROM:
		case CURLOPT_RTSP_SESSION_ID:
		case CURLOPT_RTSP_STREAM_URI:
		case CURLOPT_RTSP_TRANSPORT:
		case CURLOPT_TLSAUTH_USERNAME:
		case CURLOPT_TLSAUTH_PASSWORD:
		case CURLOPT_DNS_SERVERS:
		case CURLOPT_MAIL_AUTH:
		case CURLOPT_XOAUTH2_BEARER:
		case CURLOPT_DNS_INTERFACE:
		case CURLOPT_DNS_LOCAL_IP4:
		case CURLOPT_DNS_LOCAL_IP6:
		case CURLOPT_LOGIN_OPTIONS:
		case CURLOPT_UNIX_SOCKET_PATH:
			break;
		default:
			return JNI_FALSE;
	}

	data = (*env)->GetByteArrayElements(env, value, NULL);
	if (data == NULL) {
		LOGE("cannot get byte array elements");
		return JNI_FALSE;
	}

	slist = curl_slist_append(slist, (char *)data);
	(*env)->ReleaseByteArrayElements(env, value, data, 0);

	if (NULL == slist) {
		LOGE("cannot append data to slist");
		return JNI_FALSE;
	}
	jcurl->code = curl_easy_setopt(jcurl->curl, option, slist->data);

	if (CURLE_OK != jcurl->code) {
		curl_slist_free_all(slist);
		return JNI_FALSE;
	}

	append_slists(jcurl->slists, slist);
	return JNI_TRUE;
}

static size_t curl_read(char *ptr, size_t size, size_t nmemb, void *userdata)
{
	jbyte *data;
	jint result;
	jbyteArray array;
	jint length = size * nmemb;
	JNIEnv *env = get_env();
	jobject object = (jobject)userdata;
	jmethodID method;

	if (length == 0) {
		LOGE("%s length is 0", __FUNCTION__);
		return 0;
	}

	array = (*env)->NewByteArray(env, length);
	if (!array) {
		LOGE("%s could not create new byte[]", __FUNCTION__);
		return 0;
	}
	method = get_method(env, object, CURLOPT_READFUNCTION);
	result = (*env)->CallIntMethod(env, object, method, array);
	data = (*env)->GetByteArrayElements(env, array, NULL);
	memcpy(ptr, data, result);
	(*env)->ReleaseByteArrayElements(env, array, data, 0);
	(*env)->DeleteLocalRef(env, array);

	return result;
}

static size_t curl_write(char *ptr, size_t size, size_t nmemb, void *userdata)
{
	jint result;
	jbyteArray array;
	jint length = size * nmemb;
	JNIEnv *env = get_env();
	jobject object = (jobject)userdata;
	jmethodID method;

	if (length == 0) {
		LOGE("%s length is 0", __FUNCTION__);
		return 0;
	}

	array = (*env)->NewByteArray(env, length);
	if (!array) {
		LOGE("%s could not create new byte[%d]", __FUNCTION__, length);
		return 0;
	}
	(*env)->SetByteArrayRegion(env, array, 0, length, (jbyte *)ptr);
	method = get_method(env, object, CURLOPT_WRITEFUNCTION);
	result = (*env)->CallIntMethod(env, object, method, array);
	(*env)->DeleteLocalRef(env, array);

	return result;
}

static size_t curl_header(char *ptr, size_t size, size_t nmemb, void *userdata)
{
	jint result;
	jbyteArray array;
	jint length = size * nmemb;
	JNIEnv *env = get_env();
	jobject object = (jobject)userdata;
	jmethodID method;

	if (length == 0) {
		LOGE("%s length is 0", __FUNCTION__);
		return 0;
	}

	array = (*env)->NewByteArray(env, length);
	if (!array) {
		LOGE("%s could not create new byte[]", __FUNCTION__);
		return 0;
	}

	(*env)->SetByteArrayRegion(env, array, 0, length, (jbyte *)ptr);
	method = get_method(env, object, CURLOPT_HEADERFUNCTION);
	result = (*env)->CallIntMethod(env, object, method, array);
	(*env)->DeleteLocalRef(env, array);

	return result;
}

static int curl_debug(CURL *handle, curl_infotype type, char *data, size_t size, void *userdata)
{
	jint result;
	jbyteArray array;
	JNIEnv *env = get_env();
	jobject object = (jobject)userdata;
	jmethodID method;

	if (size == 0) {
		LOGE("curl_debug size is 0");
		return 0;
	}

	array = (*env)->NewByteArray(env, size);
	if (!array) {
		LOGE("curl_debug could not create new byte[]");
		return 0;
	}

	(*env)->SetByteArrayRegion(env, array, 0, size, (jbyte *)data);
	method = get_method(env, object, CURLOPT_DEBUGFUNCTION);
	result = (*env)->CallIntMethod(env, object, method, type, array);
	(*env)->DeleteLocalRef(env, array);

	return result;
}


static int curl_progress(void *userdata, double dltotal, double dlnow, double ultotal, double ulnow)
{
	jint result;
	JNIEnv *env = get_env();
	jobject object = (jobject)userdata;
	jmethodID method = get_method(env, object, CURLOPT_PROGRESSFUNCTION);

	result = (*env)->CallIntMethod(env, object, method, dltotal, dlnow, ultotal, ulnow);

	return result;
}

static int curl_xferinfo(void *userdata, long dltotal, long dlnow, long ultotal, long ulnow)
{
	jint result;
	JNIEnv *env = get_env();
	jobject object = (jobject)userdata;
	jmethodID method = get_method(env, object, CURLOPT_XFERINFOFUNCTION);

	result = (*env)->CallIntMethod(env, object, method, dltotal, dlnow, ultotal, ulnow);

	return result;
}

/*
 * curl_easy_setopt for callback
 char *
 */
static jboolean curl_setopt_callback(JNIEnv *env, jclass clazz, jint handle, jint option, jobject value)
{
	jcurl_t *jcurl = (jcurl_t *)handle;
	if (NULL == jcurl) {
		return JNI_FALSE;
	}

	if (NULL == get_method(env, value, option)) {
		return JNI_FALSE;
	}

	switch (option) {
		case CURLOPT_WRITEFUNCTION:
			LOGD("set writefunction");
			jcurl->write = (*env)->NewGlobalRef(env, value);
			curl_easy_setopt(jcurl->curl, CURLOPT_WRITEFUNCTION, curl_write);
			curl_easy_setopt(jcurl->curl, CURLOPT_WRITEDATA, jcurl->write);
			break;
		case CURLOPT_READFUNCTION:
			LOGD("set readfunction");
			jcurl->read = (*env)->NewGlobalRef(env, value);
			curl_easy_setopt(jcurl->curl, CURLOPT_READFUNCTION, curl_read);
			curl_easy_setopt(jcurl->curl, CURLOPT_READDATA, jcurl->read);
			break;
		case CURLOPT_PROGRESSFUNCTION:
			LOGD("set progressfunction");
			jcurl->progress = (*env)->NewGlobalRef(env, value);
			curl_easy_setopt(jcurl->curl, CURLOPT_PROGRESSFUNCTION, curl_progress);
			curl_easy_setopt(jcurl->curl, CURLOPT_PROGRESSDATA, jcurl->progress);
			break;
		case CURLOPT_HEADERFUNCTION:
			LOGD("set headerfunction");
			jcurl->header = (*env)->NewGlobalRef(env, value);
			curl_easy_setopt(jcurl->curl, CURLOPT_HEADERFUNCTION, curl_header);
			curl_easy_setopt(jcurl->curl, CURLOPT_HEADERDATA, jcurl->header);
			break;
		case CURLOPT_DEBUGFUNCTION:
			LOGD("set debugfunction");
			jcurl->debug = (*env)->NewGlobalRef(env, value);
			curl_easy_setopt(jcurl->curl, CURLOPT_DEBUGFUNCTION, curl_debug);
			curl_easy_setopt(jcurl->curl, CURLOPT_DEBUGDATA, jcurl->debug);
			break;
		case CURLOPT_XFERINFOFUNCTION:
			LOGD("set xferinfofunction");
			jcurl->xferinfo = (*env)->NewGlobalRef(env, value);
			curl_easy_setopt(jcurl->curl, CURLOPT_XFERINFOFUNCTION, curl_xferinfo);
			curl_easy_setopt(jcurl->curl, CURLOPT_XFERINFODATA, jcurl->xferinfo);
			break;
		default:
			return JNI_FALSE;
	}

	return JNI_TRUE;
}

/*
 * curl_easy_setopt for callback
 char *
 */
static jboolean curl_setopt_httppost(JNIEnv *env, jclass clazz, jint handle, jint option, jobjectArray value)
{
	int i, count;
	jclass class, class_extra;
	jmethodID get_name, get_value, get_file_name, get_content_type;
	struct curl_httppost *formpost = NULL;
	struct curl_httppost *lastptr = NULL;
	jcurl_t *jcurl = (jcurl_t *)handle;
	if (NULL == jcurl) {
		return JNI_FALSE;
	}

	switch (option) {
		case CURLOPT_HTTPPOST:
			break;
		default:
			return JNI_FALSE;
	}

	class = (*env)->FindClass(env, CLASSNAME "$NameValuePair");
	if (class == NULL) {
		LOGE("%s cannot find class: " CLASSNAME "$NameValuePair", __FUNCTION__);
		return JNI_FALSE;
	}
	class_extra = (*env)->FindClass(env, CLASSNAME "$FileNameValuePair");
	if (class_extra == NULL) {
		LOGE("%s cannot find class: " CLASSNAME "$FileNameValuePair", __FUNCTION__);
		return JNI_FALSE;
	}
	get_name = get_method_safely(env, class, "getName", "()Ljava/lang/String;");
	get_value = get_method_safely(env, class, "getValue", "()Ljava/lang/String;");
	get_file_name = get_method_safely(env, class_extra, "getFileName", "()Ljava/lang/String;");
	get_content_type = get_method_safely(env, class_extra, "getContentType", "()Ljava/lang/String;");

	count = (*env)->GetArrayLength(env, value);
	for (i = 0; i < count; ++i) {
		jobject item = (*env)->GetObjectArrayElement(env, value, i);
		jstring names = (*env)->CallObjectMethod(env, item, get_name);
		jstring values = (*env)->CallObjectMethod(env, item, get_value);
		const char *name = (*env)->GetStringUTFChars(env, names, NULL);
		const char *value = (*env)->GetStringUTFChars(env, values, NULL);
		if (name == NULL || value == NULL) {
		} else if ((*env)->IsInstanceOf(env, item, class_extra)) {
			const char *file;
			jstring filenames = (*env)->CallObjectMethod(env, item, get_file_name);
			jstring contenttypes = (*env)->CallObjectMethod(env, item, get_content_type);
			const char *filename = (*env)->GetStringUTFChars(env, filenames, NULL);;
			const char *contenttype = (*env)->GetStringUTFChars(env, contenttypes, NULL);;
			if (value[0] == '@') {
			    file = value + 1;
			} else {
			    file = value;
			}
			curl_formadd(&formpost, &lastptr, CURLFORM_COPYNAME, name, CURLFORM_FILENAME, filename, CURLFORM_CONTENTTYPE, contenttype, CURLFORM_FILE, file, CURLFORM_END);
			(*env)->ReleaseStringUTFChars(env, contenttypes, contenttype);
			(*env)->ReleaseStringUTFChars(env, filenames, filename);
			(*env)->DeleteLocalRef(env, contenttypes);
			(*env)->DeleteLocalRef(env, filenames);
		} else if (value[0] == '@') {
			const char *file = value + 1;
			curl_formadd(&formpost, &lastptr, CURLFORM_COPYNAME, name, CURLFORM_FILE, file, CURLFORM_END);
		} else {
			curl_formadd(&formpost, &lastptr, CURLFORM_COPYNAME, name, CURLFORM_COPYCONTENTS, value, CURLFORM_END);
		}
		(*env)->ReleaseStringUTFChars(env, values, value);
		(*env)->ReleaseStringUTFChars(env, names, name);
		(*env)->DeleteLocalRef(env, values);
		(*env)->DeleteLocalRef(env, names);
		(*env)->DeleteLocalRef(env, item);
	}

	(*env)->DeleteLocalRef(env, class);
	(*env)->DeleteLocalRef(env, class_extra);
	jcurl->code = curl_easy_setopt(jcurl->curl, option, formpost);
	if (CURLE_OK != jcurl->code) {
		curl_formfree(formpost);
		return JNI_FALSE;
	}

	jcurl->formpost = formpost;

	return JNI_TRUE;
}


static jboolean curl_perform(JNIEnv *env, jobject clazz, jint handle)
{
	jcurl_t *jcurl = (jcurl_t *)handle;
	if (NULL == jcurl) {
		return JNI_FALSE;
	}

	jcurl->code = curl_easy_perform(jcurl->curl);

	if (NULL != jcurl->slists) {
		curl_slist_free_all(jcurl->slists);
		jcurl->slists = NULL;
	}

	if (NULL != jcurl->formpost) {
		curl_formfree(jcurl->formpost);
		jcurl->formpost = NULL;
	}

	fflush(NULL);
	if (CURLE_OK == jcurl->code) {
		return JNI_TRUE;
	}

	return JNI_FALSE;
}

static jlong curl_getinfo_long(JNIEnv *env, jobject clazz, jint handle, jint info)
{
	long longp;
	jcurl_t *jcurl = (jcurl_t *)handle;
	if (NULL == jcurl) {
		return JNI_FALSE;
	}

	switch (info) {
		case CURLINFO_RESPONSE_CODE:
		case CURLINFO_HEADER_SIZE:
		case CURLINFO_REQUEST_SIZE:
		case CURLINFO_SSL_VERIFYRESULT:
		case CURLINFO_FILETIME:
		case CURLINFO_REDIRECT_COUNT:
		case CURLINFO_HTTP_CONNECTCODE:
		case CURLINFO_HTTPAUTH_AVAIL:
		case CURLINFO_PROXYAUTH_AVAIL:
		case CURLINFO_OS_ERRNO:
		case CURLINFO_NUM_CONNECTS:
		case CURLINFO_LASTSOCKET:
		case CURLINFO_CONDITION_UNMET:
		case CURLINFO_RTSP_CLIENT_CSEQ:
		case CURLINFO_RTSP_SERVER_CSEQ:
		case CURLINFO_RTSP_CSEQ_RECV:
		case CURLINFO_PRIMARY_PORT:
		case CURLINFO_LOCAL_PORT:
			jcurl->code = curl_easy_getinfo(jcurl->curl, info, &longp);
			if (CURLE_OK == jcurl->code) {
				return longp;
			}
			break;
	}

	return -1;
}

static jdouble curl_getinfo_double(JNIEnv *env, jobject clazz, jint handle, jint info)
{
	double doublep;
	jcurl_t *jcurl = (jcurl_t *)handle;
	if (NULL == jcurl) {
		return -1;
	}

	switch (info) {
		case CURLINFO_TOTAL_TIME:
		case CURLINFO_NAMELOOKUP_TIME:
		case CURLINFO_CONNECT_TIME:
		case CURLINFO_PRETRANSFER_TIME:
		case CURLINFO_SIZE_UPLOAD:
		case CURLINFO_SIZE_DOWNLOAD:
		case CURLINFO_SPEED_DOWNLOAD:
		case CURLINFO_SPEED_UPLOAD:
		case CURLINFO_CONTENT_LENGTH_DOWNLOAD:
		case CURLINFO_CONTENT_LENGTH_UPLOAD:
		case CURLINFO_STARTTRANSFER_TIME:
		case CURLINFO_REDIRECT_TIME:
		case CURLINFO_APPCONNECT_TIME:
			jcurl->code = curl_easy_getinfo(jcurl->curl, info, &doublep);
			if (CURLE_OK == jcurl->code) {
				return doublep;
			}
			break;
	}

	return -1;
}

static jobjectArray dump_slist(JNIEnv *env, struct curl_slist *slist)
{
	jclass class;
	jsize index, count = 0;
	jobjectArray result;
	struct curl_slist *head;

	if (slist == NULL) {
		return NULL;
	}

	head = slist;
	while (head) {
		++count;
		head = head->next;
	}

	class = (*env)->FindClass(env, "java/lang/Object");
	if (NULL == class) {
		LOGE("no such class: java/lang/Object");
		return NULL;
	}

	result = (*env)->NewObjectArray(env, count, class, NULL);
	(*env)->DeleteLocalRef(env, class);
	if (result == NULL) {
		LOGE("cannot create Object[]");
		return NULL;
	}

	for (index = 0, head = slist; index < count; ++index, head = head->next) {
		jbyteArray array = convert_from_char(env, head->data);
		if (!array) {
			LOGE("cannot create byte[] for %d in slist: %s", index, head->data);
			(*env)->DeleteLocalRef(env, result);
			return NULL;
		}
		(*env)->SetObjectArrayElement(env, result, index, array);
		(*env)->DeleteLocalRef(env, array);
	}

	return result;
}

static jobjectArray dump_certinfo(JNIEnv *env, struct curl_certinfo *certinfo)
{
	int length;
	jsize index;
	jclass class;
	jobjectArray result;

	length = certinfo->num_of_certs;
	if (length == 0) {
		return NULL;
	}

	class = (*env)->FindClass(env, "java/lang/Object");
	if (NULL == class) {
		LOGE("no such class: java/lang/Object");
		return NULL;
	}

	result = (*env)->NewObjectArray(env, length, class, NULL);
	(*env)->DeleteLocalRef(env, class);
	if (result == NULL) {
		LOGE("cannot create Object[]");
		return NULL;
	}

	for (index = 0; index < length; ++index) {
		jobjectArray object = dump_slist(env, certinfo->certinfo[index]);
		if (object == NULL) {
			LOGE("cannot create byte[][] for %d in certinfo %p", index, certinfo->certinfo[index]);
			(*env)->DeleteLocalRef(env, result);
			return NULL;
		}
		(*env)->SetObjectArrayElement(env, result, index, object);
	}

	return result;
}

static jobjectArray curl_getinfo_slist(JNIEnv *env, jobject clazz, jint handle, jint info)
{
	jobjectArray result = NULL;
	union {
		struct curl_slist *to_slist;
		struct curl_certinfo *to_certinfo;
	} ptr;
	jcurl_t *jcurl = (jcurl_t *)handle;
	if (NULL == jcurl) {
		return NULL;
	}

	switch (info) {
		case CURLINFO_COOKIELIST:
		case CURLINFO_SSL_ENGINES:
		case CURLINFO_CERTINFO:
			jcurl->code = curl_easy_getinfo(jcurl->curl, info, &(ptr.to_slist));
			if (CURLE_OK == jcurl->code) {
				break;
			}
		case CURLINFO_TLS_SESSION:
			break;
		default:
			return NULL;
	}

	switch (info) {
		case CURLINFO_COOKIELIST:
		case CURLINFO_SSL_ENGINES:
			result = dump_slist(env, ptr.to_slist);
			curl_slist_free_all(ptr.to_slist);
			break;
		case CURLINFO_CERTINFO:
			result = dump_certinfo(env, ptr.to_certinfo);
			break;
	}

	return result;
}

static jbyteArray curl_getinfo_bytes(JNIEnv *env, jobject clazz, jint handle, jint info)
{
	char *charp;
	jcurl_t *jcurl = (jcurl_t *)handle;
	if (jcurl == NULL) {
		return NULL;
	}

	switch (info) {
		case CURLINFO_EFFECTIVE_URL:
		case CURLINFO_CONTENT_TYPE:
		case CURLINFO_PRIVATE:
		case CURLINFO_FTP_ENTRY_PATH:
		case CURLINFO_REDIRECT_URL:
		case CURLINFO_PRIMARY_IP:
		case CURLINFO_RTSP_SESSION_ID:
		case CURLINFO_LOCAL_IP:
			jcurl->code = curl_easy_getinfo(jcurl->curl, info, &charp);
			if (CURLE_OK == jcurl->code) {
				return convert_from_char(env, charp);
			}
			break;
		default:
			break;
	}

	return NULL;
}

static jint curl_errno(JNIEnv *env, jobject clazz, jint handle)
{
	jcurl_t *jcurl = (jcurl_t *)handle;
	if (NULL == jcurl) {
		return 0;
	}
	return jcurl->code;
}

static jstring curl_error(JNIEnv *env, jobject clazz, jint handle)
{
	jcurl_t *jcurl = (jcurl_t *)handle;
	if (NULL == jcurl) {
		return (*env)->NewStringUTF(env, "");
	}
	return (*env)->NewStringUTF(env, curl_easy_strerror(jcurl->code));
}

#define deleteref(x) do {\
	if (x) {\
		(*env)->DeleteGlobalRef(env, x);\
	}\
} while (0)
static void curl_cleanup(JNIEnv *env, jobject clazz, jint handle)
{
	jcurl_t *jcurl = (jcurl_t *)handle;
	if (NULL == jcurl) {
		return;
	}
	curl_easy_cleanup(jcurl->curl);

	deleteref(jcurl->read);
	deleteref(jcurl->write);
	deleteref(jcurl->header);
	deleteref(jcurl->debug);
	deleteref(jcurl->progress);
	deleteref(jcurl->xferinfo);
}

static jstring libcurl_version(JNIEnv *env, jobject clazz)
{
	return (*env)->NewStringUTF(env, curl_version());
}

static JNINativeMethod methods[] = {
	/* name, signature, funcPtr */
	{"curl_init", "()I", (void *)curl_init},
	{"curl_setopt", "(IIJ)Z", (void *)curl_setopt_long},
	{"curl_setopt", "(IILjava/lang/String;)Z", (void *)curl_setopt_string},
	{"curl_setopt", "(II[[B)Z", (void *)curl_setopt_slist},
	{"curl_setopt", "(II[B)Z", (void *)curl_setopt_bytes},
	{"curl_setopt", "(IILme/piebridge/curl/Curl$Callback;)Z", (void *)curl_setopt_callback},
	{"curl_setopt", "(II[Lme/piebridge/curl/Curl$NameValuePair;)Z", (void *)curl_setopt_httppost},
	{"curl_perform", "(I)Z", (void *)curl_perform},
	{"curl_getinfo_bytes", "(II)[B", (void *)curl_getinfo_bytes},
	{"curl_getinfo_long", "(II)J", (void *)curl_getinfo_long},
	{"curl_getinfo_double", "(II)D", (void *)curl_getinfo_double},
	{"curl_getinfo_list", "(II)[[B", (void *)curl_getinfo_slist},
	{"curl_getinfo_certinfo", "(II)[[[B", (void *)curl_getinfo_slist},
	{"curl_cleanup", "(I)V", (void *)curl_cleanup},
	{"curl_errno", "(I)I", (void *)curl_errno},
	{"curl_error", "(I)Ljava/lang/String;", (void *)curl_error},
	{"curl_version", "()Ljava/lang/String;", (void *)libcurl_version},
};

jint JNI_OnLoad(JavaVM *vm, void *reserved)
{
	JNIEnv *env;
	jclass clazz;

	if ((*vm)->GetEnv(vm, (void **)&env, JCURL_JNI_VERSION) != JNI_OK) {
		return -1;
	}

	clazz = (*env)->FindClass(env, CLASSNAME);
	if (clazz == NULL) {
		LOGE("Can't find %s", CLASSNAME);
		return -1;
	}

	if ((*env)->RegisterNatives(env, clazz, methods, ARRAY_SIZE(methods)) < 0) {
		return -1;
	}

	(*env)->GetJavaVM(env, &jvm);

	return JCURL_JNI_VERSION;
}
