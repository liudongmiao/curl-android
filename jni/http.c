/* vim: set sw=4 ts=4:
 * Author: Liu DongMiao <liudongmiao@gmail.com>
 * Created  : Thu 26 Jul 2012 02:13:55 PM CST
 * Modified : Sat 28 Jul 2012 07:25:42 PM CST
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "curl/curl.h"
#ifdef ANDROID
#include <jni.h>
#endif

/* buffer function is stolen from lighttpd */

typedef struct {
	char *ptr;
	size_t used;
	size_t size;
} buffer;

static buffer* buffer_init(void)
{
	buffer *b;

	b = malloc(sizeof(*b));
	assert(b);

	b->ptr = NULL;
	b->size = 0;
	b->used = 0;

	return b;
}

#define BUFFER_PIECE_SIZE 64
static int buffer_prepare_append(buffer *b, size_t size) {
	if (!b) return -1;

	if (0 == b->size) {
		b->size = size;

		/* always allocate a multiply of BUFFER_PIECE_SIZE */
		b->size += BUFFER_PIECE_SIZE - (b->size % BUFFER_PIECE_SIZE);

		b->ptr = malloc(b->size);
		b->used = 0;
		assert(b->ptr);
	} else if (b->used + size > b->size) {
		b->size += size;

		/* always allocate a multiply of BUFFER_PIECE_SIZE */
		b->size += BUFFER_PIECE_SIZE - (b->size % BUFFER_PIECE_SIZE);

		b->ptr = realloc(b->ptr, b->size);
		assert(b->ptr);
	}
	return 0;
}

static int buffer_append_string_len(buffer *b, const char *s, size_t s_len)
{
	if (!s || !b) return -1;
	if (s_len == 0) return 0;

	buffer_prepare_append(b, s_len + 1);
	if (b->used == 0)
		b->used++;

	memcpy(b->ptr + b->used - 1, s, s_len);
	b->used += s_len;
	b->ptr[b->used - 1] = '\0';

	return 0;
}

static size_t write_data(void *buffer, size_t size, size_t nmemb, void *userp)
{
	buffer_append_string_len(userp, buffer, size * nmemb);
	return size * nmemb;
}

#ifdef ANDROID

#include <android/log.h>
#define TAG "XXXXXXXXXX-C"
#define LOGE(...) (__android_log_print(ANDROID_LOG_ERROR, TAG, __VA_ARGS__))
#define LOGD(...) (__android_log_print(ANDROID_LOG_DEBUG, TAG, __VA_ARGS__))

#define CLASSNAME "me/piebridge/curl/Curl"
#define ARRAY_SIZE(x) (sizeof(x) / sizeof(x[0]))

static CURLcode errno;

static jint curl_init(JNIEnv *env, jobject clazz)
{
	CURL *curl = curl_easy_init();
	return (int)curl;
}

static jboolean curl_setopt_long(JNIEnv *env, jobject clazz, jint handle, jint option, jlong value)
{
	CURL *curl = (CURL *)handle;
	if (NULL == curl) {
		return JNI_FALSE;
	}

	switch (option) {
		case CURLOPT_DNS_CACHE_TIMEOUT:
		case CURLOPT_DNS_USE_GLOBAL_CACHE:
		case CURLOPT_MAXCONNECTS:
		case CURLOPT_FORBID_REUSE:
		case CURLOPT_FRESH_CONNECT:
		case CURLOPT_VERBOSE:
		case CURLOPT_HEADER:
		case CURLOPT_NOPROGRESS:
		case CURLOPT_NOBODY:
		case CURLOPT_FAILONERROR:
		case CURLOPT_PUT:
		case CURLOPT_FILETIME:
		case CURLOPT_FTP_CREATE_MISSING_DIRS:
		case CURLOPT_SERVER_RESPONSE_TIMEOUT:
		case CURLOPT_TFTP_BLKSIZE:
		case CURLOPT_DIRLISTONLY:
		case CURLOPT_APPEND:
		case CURLOPT_FTP_FILEMETHOD:
		case CURLOPT_NETRC:
		case CURLOPT_TRANSFERTEXT:
		case CURLOPT_TIMECONDITION:
		case CURLOPT_TIMEVALUE:
		case CURLOPT_SSLVERSION:
		case CURLOPT_AUTOREFERER:
		case CURLOPT_TRANSFER_ENCODING:
		case CURLOPT_FOLLOWLOCATION:
		case CURLOPT_UNRESTRICTED_AUTH:
		case CURLOPT_MAXREDIRS:
		case CURLOPT_POSTREDIR:
		case CURLOPT_POST:
		case CURLOPT_POSTFIELDSIZE:
		case CURLOPT_COOKIESESSION:
		case CURLOPT_HTTPGET:
		case CURLOPT_HTTP_VERSION:
		case CURLOPT_HTTPPROXYTUNNEL:
		case CURLOPT_PROXYPORT:
		case CURLOPT_PROXYTYPE:
		case CURLOPT_PROXY_TRANSFER_MODE:
		case CURLOPT_SOCKS5_GSSAPI_NEC:
		case CURLOPT_FTP_USE_EPRT:
		case CURLOPT_FTP_USE_EPSV:
		case CURLOPT_FTP_USE_PRET:
		case CURLOPT_FTP_SSL_CCC:
		case CURLOPT_FTP_SKIP_PASV_IP:
		case CURLOPT_INFILESIZE:
		case CURLOPT_LOW_SPEED_LIMIT:
		case CURLOPT_LOW_SPEED_TIME:
		case CURLOPT_PORT:
		case CURLOPT_TIMEOUT:
		case CURLOPT_TIMEOUT_MS:
		case CURLOPT_CONNECTTIMEOUT:
		case CURLOPT_CONNECTTIMEOUT_MS:
		case CURLOPT_ACCEPTTIMEOUT_MS:
		case CURLOPT_RESUME_FROM:
		case CURLOPT_CRLF:
		case CURLOPT_LOCALPORT:
		case CURLOPT_LOCALPORTRANGE:
		case CURLOPT_GSSAPI_DELEGATION:
		case CURLOPT_SSL_VERIFYPEER:
		case CURLOPT_SSL_VERIFYHOST:
		case CURLOPT_CERTINFO:
		case CURLOPT_BUFFERSIZE:
		case CURLOPT_NOSIGNAL:
		case CURLOPT_MAXFILESIZE:
		case CURLOPT_USE_SSL:
		case CURLOPT_SSL_OPTIONS:
		case CURLOPT_FTPSSLAUTH:
		case CURLOPT_IPRESOLVE:
		case CURLOPT_TCP_NODELAY:
		case CURLOPT_IGNORE_CONTENT_LENGTH:
		case CURLOPT_CONNECT_ONLY:
		case CURLOPT_SSL_SESSIONID_CACHE:
		case CURLOPT_SSH_AUTH_TYPES:
		case CURLOPT_HTTP_TRANSFER_DECODING:
		case CURLOPT_HTTP_CONTENT_DECODING:
		case CURLOPT_NEW_FILE_PERMS:
		case CURLOPT_NEW_DIRECTORY_PERMS:
		case CURLOPT_ADDRESS_SCOPE:
		case CURLOPT_PROTOCOLS:
		case CURLOPT_REDIR_PROTOCOLS:
		case CURLOPT_RTSP_REQUEST:
		case CURLOPT_RTSP_CLIENT_CSEQ:
		case CURLOPT_RTSP_SERVER_CSEQ:
		case CURLOPT_WILDCARDMATCH:
		case CURLOPT_TCP_KEEPALIVE:
		case CURLOPT_TCP_KEEPIDLE:
		case CURLOPT_TCP_KEEPINTVL:
			errno = curl_easy_setopt(curl, option, value);
			if (CURLE_OK == errno) {
				return JNI_TRUE;
			}
			break;
	}
	return JNI_FALSE;
}


static jboolean curl_setopt_string(JNIEnv *env, jobject clazz, jint handle, jint option, jstring value)
{
	CURL *curl = (CURL *)handle;
	char *param = (char *)(*env)->GetStringUTFChars(env, value, NULL);
	if (NULL == curl) {
		return JNI_FALSE;
	}

	switch (option) {
		case CURLOPT_SSL_CIPHER_LIST:
		case CURLOPT_RANDOM_FILE:
		case CURLOPT_EGDSOCKET:
		case CURLOPT_NETRC_FILE:
		case CURLOPT_ACCEPT_ENCODING:
		case CURLOPT_COPYPOSTFIELDS:
		case CURLOPT_REFERER:
		case CURLOPT_USERAGENT:
		case CURLOPT_COOKIE:
		case CURLOPT_COOKIEJAR:
		case CURLOPT_COOKIELIST:
		case CURLOPT_CUSTOMREQUEST:
		case CURLOPT_PROXY:
		case CURLOPT_SOCKS5_GSSAPI_SERVICE:
		case CURLOPT_ERRORBUFFER:
		case CURLOPT_FTPPORT:
		case CURLOPT_URL:
		case CURLOPT_USERPWD:
		case CURLOPT_USERNAME:
		case CURLOPT_PASSWORD:
		case CURLOPT_PROXYUSERPWD:
		case CURLOPT_PROXYUSERNAME:
		case CURLOPT_PROXYPASSWORD:
		case CURLOPT_NOPROXY:
		case CURLOPT_RANGE:
		case CURLOPT_SSLCERT:
		case CURLOPT_SSLCERTTYPE:
		case CURLOPT_SSLKEY:
		case CURLOPT_SSLKEYTYPE:
		case CURLOPT_KEYPASSWD:
		case CURLOPT_SSLENGINE:
		case CURLOPT_INTERFACE:
		case CURLOPT_KRBLEVEL:
		case CURLOPT_CAINFO:
		case CURLOPT_CAPATH:
		case CURLOPT_CRLFILE:
		case CURLOPT_ISSUERCERT:
		case CURLOPT_FTP_ACCOUNT:
		case CURLOPT_FTP_ALTERNATIVE_TO_USER:
		case CURLOPT_SSH_PUBLIC_KEYFILE:
		case CURLOPT_SSH_PRIVATE_KEYFILE:
		case CURLOPT_SSH_HOST_PUBLIC_KEY_MD5:
		case CURLOPT_SSH_KNOWNHOSTS:
		case CURLOPT_MAIL_FROM:
		case CURLOPT_MAIL_AUTH:
		case CURLOPT_RTSP_SESSION_ID:
		case CURLOPT_RTSP_STREAM_URI:
		case CURLOPT_RTSP_TRANSPORT:
		case CURLOPT_TLSAUTH_USERNAME:
		case CURLOPT_TLSAUTH_PASSWORD:
		case CURLOPT_TLSAUTH_TYPE:
		case CURLOPT_DNS_SERVERS:
			errno = curl_easy_setopt(curl, option, param);
			if (CURLE_OK == errno) {
				return JNI_TRUE;
			}
			break;
	}
	return JNI_FALSE;
}

static jstring curl_perform(JNIEnv *env, jobject clazz, jint handle)
{
	char *data = "";
	CURL *curl = (CURL *)handle;
	buffer *buffer = buffer_init();
	if (NULL == curl) {
		return (*env)->NewStringUTF(env, data);
	}

	curl_easy_setopt(curl, CURLOPT_WRITEDATA, buffer);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
	errno = curl_easy_perform(curl);

	if (CURLE_OK == errno && buffer->used) {
		// TODO: handle binary data
		data = strdup(buffer->ptr);
	}

	free(buffer->ptr);
	free(buffer);

	return (*env)->NewStringUTF(env, data);
}

static jlong curl_getinfo_long(JNIEnv *env, jobject clazz, jint handle, jint info)
{
	long longp;
	CURL *curl = (CURL *)handle;
	if (NULL == curl) {
		return -1;
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
			errno = curl_easy_getinfo(curl, info, &longp);
			if (CURLE_OK == errno) {
				return longp;
			}
			break;
	}

	return -1;
}

static jdouble curl_getinfo_double(JNIEnv *env, jobject clazz, jint handle, jint info)
{
	double doublep;
	CURL *curl = (CURL *)handle;
	if (NULL == curl) {
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
			errno = curl_easy_getinfo(curl, info, &doublep);
			if (CURLE_OK == errno) {
				return doublep;
			}
			break;
	}

	return -1;
}

static jstring curl_getinfo(JNIEnv *env, jobject clazz, jint handle, jint info)
{
	long longp;
	double doublep;
	char value[64];
	char *charp;
	CURL *curl = (CURL *)handle;

	if (NULL == curl) {
		return (*env)->NewStringUTF(env, "");
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
			errno = curl_easy_getinfo(curl, info, &longp);
			if (CURLE_OK == errno) {
				sprintf(value, "%ld", longp);
				return (*env)->NewStringUTF(env, value);
			}
			break;
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
			errno = curl_easy_getinfo(curl, info, &doublep);
			if (CURLE_OK == errno) {
				sprintf(value, "%.3f", doublep);
				return (*env)->NewStringUTF(env, value);
			}
			break;
		case CURLINFO_EFFECTIVE_URL:
		case CURLINFO_CONTENT_TYPE:
		case CURLINFO_PRIVATE:
		case CURLINFO_FTP_ENTRY_PATH:
		case CURLINFO_REDIRECT_URL:
		case CURLINFO_PRIMARY_IP:
		case CURLINFO_RTSP_SESSION_ID:
		case CURLINFO_LOCAL_IP:
			errno = curl_easy_getinfo(curl, info, &charp);
			if (CURLE_OK == errno) {
				return (*env)->NewStringUTF(env, charp);
			}
			break;
	}

	return (*env)->NewStringUTF(env, "");
}

static jint curl_errno(JNIEnv *env, jobject clazz)
{
	return errno;
}

static jstring curl_error(JNIEnv *env, jobject clazz)
{
	return (*env)->NewStringUTF(env, curl_easy_strerror(errno));
}

static void curl_cleanup(JNIEnv *env, jobject clazz, jint handle)
{
	CURL *curl = (CURL *)handle;
	if (NULL != curl) {
		curl_easy_cleanup(curl);
	}
}

static JNINativeMethod methods[] = {
	/* name, signature, funcPtr */
	{"curl_init", "()I", (void*)curl_init},
	{"curl_setopt", "(IIJ)Z", (void*)curl_setopt_long},
	{"curl_setopt", "(IILjava/lang/String;)Z", (void*)curl_setopt_string},
	{"curl_perform", "(I)Ljava/lang/String;", (void*)curl_perform},
	{"curl_getinfo", "(II)Ljava/lang/String;", (void*)curl_getinfo},
	{"curl_getinfo_long", "(II)J", (void*)curl_getinfo_long},
	{"curl_getinfo_double", "(II)D", (void*)curl_getinfo_double},
	{"curl_cleanup", "(I)V", (void*)curl_cleanup},
	{"curl_errno", "()I", (void*)curl_errno},
	{"curl_error", "()Ljava/lang/String;", (void*)curl_error},
};

jint JNI_OnLoad(JavaVM *vm, void *reserved)
{
	JNIEnv *env;
	jclass clazz;

	if ((*vm)->GetEnv(vm, (void **)&env, JNI_VERSION_1_6) != JNI_OK) {
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

	return JNI_VERSION_1_6;
}
#endif
