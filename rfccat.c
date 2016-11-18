/*
 * Copyright (c) 2016 Robert Tate <rob@rtate.se>
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <curl/curl.h>

#include "rfccat.h"

CURL *curl;
CURLcode res;

int
main(int argc, char const *argv[])
{
	int err = 0;

	if (argc != 2) {
		fprintf(stderr, "An RFC number must be specified!\n");
		exit(1);
	}

	int rfc_num = atoi(argv[1]);

	if (rfc_num <= 0) {
		fprintf(stderr, "Invalid option for RFC number!\n");
		exit(1);
	}

	int url_len = strlen(RFC_URL_PREFIX) + GET_INT_SIZE(rfc_num) + strlen(RFC_URL_SUFFIX) + 1;
	char *url = malloc(url_len);
	
	if (url == NULL) {
		fprintf(stderr, "Error allocation memory!");
		exit(2);
	}

	snprintf(url, url_len, "%s%d%s", RFC_URL_PREFIX, rfc_num, RFC_URL_SUFFIX);

	curl_global_init(CURL_GLOBAL_ALL);
	curl = curl_easy_init();

	if (curl) {
		if (isatty(STDOUT_FILENO)) {
			pid_t pid;
			int p[2];

			if (pipe(p)) {
				fprintf(stderr, "Error opening pipe!\n");
				err = 1;
			} else {
				if ((pid = fork()) == -1) {
					fprintf(stderr, "Failed to fork!\n");
					err = 1;
				} else if (pid == 0) {
					close(p[0]);
					dup2(p[1], STDOUT_FILENO);
					close(p[1]);
				} else {
					close(p[1]);
					dup2(p[0], STDIN_FILENO);
					close(p[0]);

					char *pager_args[] = { getenv("PAGER") ? getenv("PAGER") : "less", NULL };
					execvp(pager_args[0], pager_args);

					fprintf(stderr, "Error executing pager!\n");
					err = 1;
				}
			}
		}

		if (!err) {
			curl_easy_setopt(curl, CURLOPT_URL, url);
			res = curl_easy_perform(curl);

			if (res != CURLE_OK) {
				fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
				err = 1;
			}
		}

		curl_easy_cleanup(curl);
	} else {
		fprintf(stderr, "Error initializing curl!\n");
		err = 1;
	}

	free(url);

	curl_global_cleanup();

	return err;
}
