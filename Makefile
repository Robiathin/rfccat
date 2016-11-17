# Copyright (c) 2016 Robert Tate <rob@rtate.se>
#
# Permission to use, copy, modify, and/or distribute this software for any
# purpose with or without fee is hereby granted, provided that the above
# copyright notice and this permission notice appear in all copies.
#
# THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
# WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
# MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
# ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
# WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
# ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
# OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

.PHONY: all clean install uninstall

RFCCAT = rfccat

CFLAGS = -std=c99 -O3 -Wall

LDFLAGS = -lcurl -lm

SRCS = \
	rfccat.c

# Use clang if found, otherwise use gcc.
CC ?= $(shell which clang)
ifeq ($(CC),)
	CC = gcc
endif

$(RFCCAT): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) $(LDFLAGS) -o $(RFCCAT)

all: $(RFCCAT)

clean:
	rm -f $(RFCCAT)

install:
	install -m 444 rfccat.1 /usr/share/man/man1/
	install -m 555 $(RFCCAT) /usr/local/bin/

uninstall:
	rm -f /usr/share/man/man1/rfccat.1
	rm -f /usr/local/bin/$(RFCCAT)
