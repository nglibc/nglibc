# Copyright (C) 1995-2023 Free Software Foundation, Inc.
# This file is part of the GNU C Library.

# The GNU C Library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.

# The GNU C Library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.

# You should have received a copy of the GNU Lesser General Public
# License along with the GNU C Library; if not, see
# <https://www.gnu.org/licenses/>.

#
#	Specific makefile for libio.
#
subdir	:= stdio
vpath %.c     streambuf filebuf printf scanf tst
vpath %.sh    tst
vpath %.exp   tst
vpath %.input tst

include ../Makeconfig

headers	:= stdio.h \
	   bits/stdio.h bits/stdio2.h bits/stdio2-decl.h bits/stdio-ldbl.h \
	   bits/types/FILE.h bits/types/__FILE.h bits/types/struct_FILE.h \
	   bits/types/__fpos_t.h bits/types/__fpos64_t.h \
	   bits/types/cookie_io_functions_t.h

routines	:= iofdopen \
	iofopncook iopopen pclose memstream fileno iofopen freopen freopen64  \
	iofclose ioseekoff ioftell iosetvbuf filedoalloc wfiledoalloc	      \
	iowpadn wgenops wstrops wfileops iofwide wmemstream		      \
									      \
	clearerror feof ferror setvbuf fseek ftell fsetpos fclose fflush      \
	ofl ofl_add ext sbtoread sbtowrite sbmisc sbflush lockfile sbfwide    \
	fgetc ungetc fgets gets getdelim getw fread \
	uflow shgetc intscan floatscan mbfuncs fmodl scanf fscanf sscanf      \
	fgetwc ungetwc fgetws wscanf fwscanf swscanf fputc fputs puts putw    \
	padn fwrite overflow perror printf fprintf snprintf sprintf asprintf  \
	fputwc fputws wprintf fwprintf swprintf dprintf stdio_write	      \
	strfromd strfroml debug_compat libc_fatal fmemopen oldfmemopen vtables

tests = tst_swprintf tst_wprintf tst_swscanf tst_wscanf tst_getwc tst_putwc   \
	tst_wprintf2 tst-widetext test-fmemopen tst-ext tst-ext2 \
	tst-fgetws tst-ungetwc1 tst-ungetwc2 tst-swscanf tst-sscanf	      \
	tst-mmap-setvbuf bug-ungetwc1 bug-ungetwc2 tst-atime tst-eof          \
	tst-freopen bug-rewind bug-rewind2 bug-ungetc bug-fseek \
	tst-mmap-eofsync tst-mmap-fflushsync bug-mmap-fflush \
	tst-mmap2-eofsync tst-mmap-offend bug-fopena+ bug-wfflush \
	bug-ungetc2 bug-ftell bug-ungetc3 bug-ungetc4 tst-fopenloc2 \
	tst-memstream1 tst-memstream2 tst-memstream3 tst-memstream4 \
	tst-wmemstream1 tst-wmemstream2 tst-wmemstream3 tst-wmemstream4 \
	tst-wmemstream5 bug-memstream1 bug-wmemstream1 \
	tst-setvbuf1 tst-popen1 tst-fgetwc bug-wsetpos tst-fseek \
	tst-fwrite-error tst-ftell-partial-wide tst-ftell-active-handler \
	tst-ftell-append tst-fputws tst-bz22415 tst-fgetc-after-eof \
	tst-sprintf-ub tst-sprintf-chk-ub tst-bz24051 tst-bz24153 \
	tst-wfile-sync tst-bz28828 tst-getdelim

tests-internal = tst-vtables tst-vtables-interposed

ifeq (yes,$(build-shared))
# Add test-fopenloc only if shared library is enabled since it depends on
# shared localedata objects.
tests += tst-fopenloc
# Add tst-bz24228 only if shared library is enabled since it can never meet its
# objective with static linking because the relevant code just is not there.
tests += tst-bz24228
endif
test-srcs = test-freopen

ifeq ($(build-shared),yes)
routines += oldiofopen oldiofdopen oldiofclose oldiopopen oldpclose \
	    oldtmpfile oldiofgetpos oldiofgetpos64 oldiofsetpos	    \
	    oldiofsetpos64
endif

ifeq (yes,$(libc-reentrant))
routines += clearerr_u feof_u ferror_u \
	    getc_unlocked getchar_unlocked putc_unlocked putchar_unlocked
endif

CPPFLAGS += $(libio-mtsafe)

# Support for exception handling.
CFLAGS-fileops.c += -fexceptions
CFLAGS-fgetc.c += -fexceptions
CFLAGS-fputc.c += -fexceptions
CFLAGS-iofputc.c += -fexceptions
CFLAGS-fputwc.c += -fexceptions
CFLAGS-freopen64.c += -fexceptions
CFLAGS-freopen.c += -fexceptions
CFLAGS-fseek.c += -fexceptions
CFLAGS-iofseek.c += -fexceptions
CFLAGS-fseeko64.c += -fexceptions
CFLAGS-fseeko.c += -fexceptions
CFLAGS-ftello64.c += -fexceptions
CFLAGS-ftello.c += -fexceptions
CFLAGS-fwide.c += -fexceptions
CFLAGS-sbfwide.c += -fexceptions
CFLAGS-genops.c += -fexceptions
CFLAGS-getc.c += -fexceptions
CFLAGS-getchar.c += -fexceptions
CFLAGS-getwc.c += -fexceptions
CFLAGS-getwchar.c += -fexceptions
CFLAGS-fclose.c += -fexceptions
CFLAGS-iofclose.c += -fexceptions
CFLAGS-fflush.c += -fexceptions
CFLAGS-iofflush.c += -fexceptions $(config-cflags-wno-ignored-attributes)
CFLAGS-iofgetpos64.c += -fexceptions
CFLAGS-iofgetpos.c += -fexceptions
CFLAGS-fgets.c += -fexceptions
CFLAGS-iofgets.c += -fexceptions
CFLAGS-fgetws.c += -fexceptions
CFLAGS-iofgetws.c += -fexceptions
CFLAGS-fputs.c += -fexceptions
CFLAGS-iofputs.c += -fexceptions $(config-cflags-wno-ignored-attributes)
CFLAGS-fputws.c += -fexceptions
CFLAGS-iofputws.c += -fexceptions
CFLAGS-fread.c += -fexceptions
CFLAGS-iofread.c += -fexceptions
CFLAGS-iofsetpos64.c += -fexceptions
CFLAGS-fsetpos.c += -fexceptions
CFLAGS-iofsetpos.c += -fexceptions
CFLAGS-ftell.c += -fexceptions
CFLAGS-ioftell.c += -fexceptions
CFLAGS-fwrite.c += -fexceptions
CFLAGS-iofwrite.c += -fexceptions $(config-cflags-wno-ignored-attributes)
CFLAGS-getdelim.c += -fexceptions
CFLAGS-iogetdelim.c += -fexceptions
CFLAGS-iogetline.c += -fexceptions
CFLAGS-gets.c += -fexceptions
CFLAGS-iogets.c += -fexceptions
CFLAGS-iogetwline.c += -fexceptions
CFLAGS-puts.c += -fexceptions
CFLAGS-ioputs.c += -fexceptions
CFLAGS-ioseekoff.c += -fexceptions
CFLAGS-ioseekpos.c += -fexceptions
CFLAGS-iosetbuffer.c += -fexceptions
CFLAGS-setvbuf.c += -fexceptions
CFLAGS-iosetvbuf.c += -fexceptions
CFLAGS-ungetc.c += -fexceptions
CFLAGS-ioungetc.c += -fexceptions
CFLAGS-ungetwc.c += -fexceptions
CFLAGS-ioungetwc.c += -fexceptions
CFLAGS-oldfileops.c += -fexceptions
CFLAGS-oldiofclose.c += -fexceptions
CFLAGS-oldiofgetpos64.c += -fexceptions
CFLAGS-oldiofgetpos.c += -fexceptions
CFLAGS-oldiofsetpos64.c += -fexceptions
CFLAGS-oldiofsetpos.c += -fexceptions
CFLAGS-peekc.c += -fexceptions
CFLAGS-putc.c += -fexceptions
CFLAGS-putchar.c += -fexceptions
CFLAGS-putwc.c += -fexceptions
CFLAGS-putwchar.c += -fexceptions
CFLAGS-rewind.c += -fexceptions
CFLAGS-wfileops.c += -fexceptions
CFLAGS-wgenops.c += -fexceptions
CFLAGS-oldiofopen.c += -fexceptions
CFLAGS-iofopen.c += -fexceptions
CFLAGS-iofopen64.c += -fexceptions
CFLAGS-oldtmpfile.c += -fexceptions
CFLAGS-fileno.c += $(config-cflags-wno-ignored-attributes)
CFLAGS-feof_u.c += $(config-cflags-wno-ignored-attributes)
CFLAGS-ferror_u.c += $(config-cflags-wno-ignored-attributes)
CFLAGS-getc_u.c += $(config-cflags-wno-ignored-attributes)
CFLAGS-iofflush_u.c += $(config-cflags-wno-ignored-attributes)
CFLAGS-putc_u.c += $(config-cflags-wno-ignored-attributes)
CFLAGS-iofgets_u.c += $(config-cflags-wno-ignored-attributes)
CFLAGS-iofputs_u.c += $(config-cflags-wno-ignored-attributes)
CFLAGS-printf.c += -fexceptions -Wno-uninitialized
CFLAGS-fprintf.c += -fexceptions
CFLAGS-wprintf.c += -fexceptions -Wno-uninitialized
CFLAGS-fwprintf.c += -fexceptions
CFLAGS-scanf.c += -fexceptions -Wno-uninitialized
CFLAGS-fscanf.c += -fexceptions
CFLAGS-wscanf.c += -fexceptions -Wno-uninitialized
CFLAGS-fwscanf.c += -fexceptions
# XXX Do we need filedoalloc and wfiledoalloc?  Others?

CFLAGS-tst_putwc.c += -DOBJPFX=\"$(objpfx)\"

# These test cases intentionally use overlapping arguments
CFLAGS-tst-sprintf-ub.c += -Wno-restrict
CFLAGS-tst-sprintf-chk-ub.c += -Wno-restrict

LDFLAGS-tst-bz24228 = -Wl,--version-script=tst/tst-bz24228.map

tst_wprintf2-ARGS = "Some Text"

test-fmemopen-ENV = MALLOC_TRACE=$(objpfx)test-fmemopen.mtrace \
		    LD_PRELOAD=$(common-objpfx)/malloc/libc_malloc_debug.so
tst-fopenloc-ENV = MALLOC_TRACE=$(objpfx)tst-fopenloc.mtrace \
		   LD_PRELOAD=$(common-objpfx)/malloc/libc_malloc_debug.so
tst-bz22415-ENV = MALLOC_TRACE=$(objpfx)tst-bz22415.mtrace \
		  LD_PRELOAD=$(common-objpfx)/malloc/libc_malloc_debug.so
tst-bz24228-ENV = MALLOC_TRACE=$(objpfx)tst-bz24228.mtrace \
		  LD_PRELOAD=$(common-objpfx)/malloc/libc_malloc_debug.so

generated += test-fmemopen.mtrace test-fmemopen.check
generated += tst-fopenloc.mtrace tst-fopenloc.check
generated += tst-bz22415.mtrace tst-bz22415.check

aux	:= fileops genops stdfiles stdio strops

ifeq ($(build-shared),yes)
generated += tst-bz24228.mtrace tst-bz24228.check
aux	+= oldfileops oldstdfiles
endif

shared-only-routines = oldiofopen oldiofdopen oldiofclose oldfileops	\
		       oldstdfiles oldiopopen oldpclose oldtmpfile	\
		       oldiofgetpos oldiofgetpos64 oldiofsetpos		\
		       oldiofsetpos64

ifeq ($(run-built-tests),yes)
tests-special += $(objpfx)test-freopen.out $(objpfx)test-fmemopen-mem.out \
		 $(objpfx)tst-bz22415-mem.out
ifeq (yes,$(build-shared))
# Run tst-fopenloc-cmp.out and tst-openloc-mem.out only if shared
# library is enabled since they depend on tst-fopenloc.out.
tests-special += $(objpfx)tst-fopenloc-cmp.out $(objpfx)tst-fopenloc-mem.out \
		 $(objpfx)tst-bz24228-mem.out
endif

tests += tst-cleanup-default tst-cleanup-default-static
tests-static += tst-cleanup-default-static
tests-special += $(objpfx)tst-cleanup-default-cmp.out $(objpfx)tst-cleanup-default-static-cmp.out
LDFLAGS-tst-cleanup-default = -Wl,--gc-sections
LDFLAGS-tst-cleanup-default-static = -Wl,--gc-sections

ifeq ($(have-gnu-retain)$(have-z-start-stop-gc),yesyes)
tests += tst-cleanup-start-stop-gc tst-cleanup-start-stop-gc-static \
		tst-cleanup-nostart-stop-gc tst-cleanup-nostart-stop-gc-static
tests-static += tst-cleanup-start-stop-gc-static tst-cleanup-nostart-stop-gc-static
tests-special += $(objpfx)tst-cleanup-start-stop-gc-cmp.out \
		$(objpfx)tst-cleanup-start-stop-gc-static-cmp.out \
		$(objpfx)tst-cleanup-nostart-stop-gc-cmp.out \
		$(objpfx)tst-cleanup-nostart-stop-gc-static-cmp.out
LDFLAGS-tst-cleanup-start-stop-gc := -Wl,--gc-sections,-z,start-stop-gc
LDFLAGS-tst-cleanup-start-stop-gc-static := -Wl,--gc-sections,-z,start-stop-gc
LDFLAGS-tst-cleanup-nostart-stop-gc := -Wl,--gc-sections,-z,nostart-stop-gc
LDFLAGS-tst-cleanup-nostart-stop-gc-static := -Wl,--gc-sections,-z,nostart-stop-gc
endif
endif

include ../Rules

ifeq ($(run-built-tests),yes)
LOCALES := de_DE.ISO-8859-1 de_DE.UTF-8 en_US.ISO-8859-1 en_US.UTF-8 \
	   ja_JP.EUC-JP ja_JP.UTF-8
include ../gen-locales.mk

$(objpfx)bug-ftell.out: $(gen-locales)
$(objpfx)bug-ungetwc1.out: $(gen-locales)
$(objpfx)bug-ungetwc2.out: $(gen-locales)
$(objpfx)tst-fgetwc.out: $(gen-locales)
$(objpfx)tst-fgetws.out: $(gen-locales)
$(objpfx)tst-fopenloc.out: $(gen-locales)
$(objpfx)tst-fputws.out: $(gen-locales)
$(objpfx)tst-fseek.out: $(gen-locales)
$(objpfx)tst-ftell-active-handler.out: $(gen-locales)
$(objpfx)tst-ftell-append.out: $(gen-locales)
$(objpfx)tst-ftell-partial-wide.out: $(gen-locales)
$(objpfx)tst-setvbuf1.out: $(gen-locales)
$(objpfx)tst-swscanf.out: $(gen-locales)
$(objpfx)tst-ungetwc1.out: $(gen-locales)
$(objpfx)tst-ungetwc2.out: $(gen-locales)
$(objpfx)tst-widetext.out: $(gen-locales)
$(objpfx)tst_wprintf2.out: $(gen-locales)
$(objpfx)tst-wfile-sync.out: $(gen-locales)
endif

define gen-tst-cleanup
$(objpfx)tst-cleanup-$1-cmp.out: tst-cleanup.exp $(objpfx)tst-cleanup-$1.out
	cmp $$^ > $$@; $$(evaluate-test)
endef

$(foreach t,default default-static start-stop-gc start-stop-gc-static nostart-stop-gc nostart-stop-gc-static, \
  $(eval $(call gen-tst-cleanup,$(t))))

$(objpfx)test-freopen.out: test-freopen.sh $(objpfx)test-freopen
	$(SHELL) $< $(common-objpfx) '$(test-program-prefix)'	\
	$(common-objpfx)libio/; \
	$(evaluate-test)

$(objpfx)tst-fopenloc-cmp.out: ../iconvdata/testdata/ISO-8859-1..UTF8 \
			       $(objpfx)tst-fopenloc.out
	cmp $^ > $@; \
	$(evaluate-test)

$(objpfx)test-fmemopen-mem.out: $(objpfx)test-fmemopen.out
	$(common-objpfx)malloc/mtrace $(objpfx)test-fmemopen.mtrace > $@; \
	$(evaluate-test)

$(objpfx)tst-fopenloc-mem.out: $(objpfx)tst-fopenloc.out
	$(common-objpfx)malloc/mtrace $(objpfx)tst-fopenloc.mtrace > $@; \
	$(evaluate-test)

$(objpfx)tst-bz22415-mem.out: $(objpfx)tst-bz22415.out
	$(common-objpfx)malloc/mtrace $(objpfx)tst-bz22415.mtrace > $@; \
	$(evaluate-test)

$(objpfx)tst-bz24228-mem.out: $(objpfx)tst-bz24228.out
	$(common-objpfx)malloc/mtrace $(objpfx)tst-bz24228.mtrace > $@; \
	$(evaluate-test)
