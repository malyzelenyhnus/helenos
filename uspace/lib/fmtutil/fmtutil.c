/*
 * Copyright (c) 2011 Martin Sucha
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * - Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 * - Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the distribution.
 * - The name of the author may not be used to endorse or promote products
 *   derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <io/console.h>
#include <errno.h>
#include <malloc.h>
#include <fmtutil.h>

typedef struct {
	align_mode_t alignment;
	bool newline_always;
	size_t width;
} printmode_t;

int print_wrapped_console(const char *str, align_mode_t alignment)
{
	console_ctrl_t *console = console_init(stdin, stdout);
	if (console == NULL) {
		printf("%s", str);
		return EOK;
	}
	sysarg_t con_rows, con_cols, con_col, con_row;
	int rc = console_get_size(console, &con_cols, &con_rows);
	if (rc != EOK) {
		return rc;
	}
	rc = console_get_pos(console, &con_col, &con_row);
	if (rc != EOK) {
		return rc;
	}
	if (con_col != 0) {
		printf("\n");
	}
	return print_wrapped(str, con_cols, alignment);
}

static int print_line(wchar_t *wstr, size_t chars, void *data)
{
	//char *line = wstr_to_astr(wstr);
	printmode_t *pm = (printmode_t *) data;
	//if (line == NULL) {
	//	return ENOMEM;
	//}
	wstr[chars] = 0;
	return print_aligned_w(wstr, pm->width, pm->alignment);
	//printf("%s", line);
	//if (pm->newline_always || chars < pm->width)
	//	printf("\n");
	//free(line);
	//return EOK;
}

int print_wrapped(const char *str, size_t width, align_mode_t mode)
{
	printmode_t pm;
	pm.alignment = mode;
	pm.newline_always = false;
	pm.width = width;
	wchar_t *wstr = str_to_awstr(str);
	if (wstr == NULL) {
		return ENOMEM;
	}
	int rc = wrap(wstr, width, print_line, &pm);
	free(wstr);
	return rc;
}

int print_aligned_w(const wchar_t *wstr, size_t width, align_mode_t mode)
{
	size_t i;
	size_t len = wstr_length(wstr);
	if (mode == ALIGN_LEFT) {
		for (i = 0; i < width; i++) {
			if (i < len)
				putchar(wstr[i]);
			else
				putchar(' ');
		}
	}
	else if (mode == ALIGN_RIGHT) {
		for (i = 0; i < width; i++) {
			if (i < width - len)
				putchar(' ');
			else
				putchar(wstr[i- (width - len)]);
		}
	}
	else if (mode == ALIGN_CENTER) {
		size_t padding = (width - len) / 2;
		for (i = 0; i < width; i++) {
			if ((i < padding) || ((i - padding) >= len))
				putchar(' ');
			else
				putchar(wstr[i-padding]);
		}
	}
	else if (mode == ALIGN_JUSTIFY) {
		size_t words = 0;
		size_t word_chars = 0;
		bool space = true;
		for (i = 0; i < len; i++) {
			if (space && wstr[i] != ' ') {
				words++;
			}
			space = wstr[i] == ' ';
			if (!space) word_chars++;
		}
		if (words == 0) {
			return EOK;
		}
		size_t excess_spaces = width - word_chars - (words-1);
		space = true;
		i = 0;
		size_t done_words = 0;
		size_t done_chars = 0;
		size_t j;
		while (i < len) {
			/* Find a word */
			while (i < len && wstr[i] == ' ') i++;
			if (i == len) break;
			if (done_words) {
				// TODO: use better formula
				size_t spaces = 1 + (excess_spaces /
				    (words - 1));
				for (j = 0; j < spaces; j++) {
					putchar(' ');
				}
				done_chars += spaces;
			}
			while (i < len && wstr[i] != ' ') {
				putchar(wstr[i++]);
				done_chars++;
			}
			done_words++;
		}
		while (done_chars < width) {
			putchar(' ');
			done_chars++;
		}
	}
	else {
		return EINVAL;
	}
	
	return EOK;
}
int print_aligned(const char *str, size_t width, align_mode_t mode)
{
	wchar_t *wstr = str_to_awstr(str);
	if (wstr == NULL) {
		return ENOMEM;
	}
	int rc = print_aligned_w(wstr, width, mode);
	free(wstr);
	return rc;
}

/**
 */
int wrap(wchar_t *wstr, size_t width, line_consumer_fn consumer, void *data)
{
	size_t word_start = 0;
	size_t last_word_end = 0;
	size_t line_start = 0;
	size_t line_len = 0;
	size_t pos = 0;
	
	/*
	 * Invariants:
	 *  * line_len = last_word_end - line_start
	 *  * line_start <= last_word_end <= word_start <= pos
	 */
	
	while (wstr[pos] != 0) {
		/* Skip spaces and process newlines */
		while (wstr[pos] == ' ' || wstr[pos] == '\n') {
			if (wstr[pos] == '\n') {
				consumer(wstr + line_start, line_len, data);
				last_word_end = line_start = pos + 1;
				line_len = 0;
			}
			pos++;
		}
		word_start = pos;
		/* Find end of word */
		while (wstr[pos] != 0 && wstr[pos] != ' ' &&
		    wstr[pos] != '\n')
			pos++;
		/* Check if the line still fits width */
		if (pos - line_start > width) {
			if (line_len > 0)
				consumer(wstr + line_start, line_len, data);
			line_start = last_word_end = word_start;
			line_len = 0;
		}
		/* Check if we need to force wrap of long word*/
		if (pos - word_start > width) {
			consumer(wstr + word_start, width, data);
			pos = line_start = last_word_end = word_start + width;
			line_len = 0;
		}
		last_word_end = pos;
		line_len = last_word_end - line_start;
	}
	/* Here we have less than width chars starting from line_start.
	 * Moreover, the last portion does not contain spaces or newlines 
	 */
	if (pos - line_start > 0)
		consumer(wstr + line_start, pos - line_start, data);

	return EOK;
}

