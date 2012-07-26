/* vim: set sw=4 ts=4:
 * Author: Liu DongMiao <liudongmiao@gmail.com>
 * Created  : Thu 26 Jul 2012 02:13:55 PM CST
 * Modified : Thu 26 Jul 2012 08:50:05 PM CST
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

static buffer *download(const char *url)
{
	CURL *curl = curl_easy_init();
	buffer *content = buffer_init();

	if(curl) {
		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, content);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
		curl_easy_perform(curl);
		curl_easy_cleanup(curl);
	}

	return content;
}

#ifndef ANDROID
int main(int argc, const char **argv)
{
	buffer *content;
	if (argc == 1) {
		return -1;
	}

	content = download(argv[1]);
	if (content->used) {
		fwrite(content->ptr, content->used, 1, stdout);
	}

	free(content->ptr);
	free(content);

	return 0;
}
#else
jstring Java_me_piebridge_http_Http_getWebpage(JNIEnv* env, jobject thiz, jstring url)
{
	char *data = "";
	buffer *content;

	content = download((*env)->GetStringUTFChars(env, url, NULL));
	if (content->used) {
		// XXX: strdup cannot handle binary data
		data = strdup(content->ptr);
	}
	free(content->ptr);
	free(content);

	return (*env)->NewStringUTF(env, data);
}
#endif
