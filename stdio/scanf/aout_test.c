/* Mini test suite for scanf subdir
   Compile:   cd scanf/ ; cc *.c ../streambuf/*.c 
   Run test:  ./a.out <aout_test.input  
   Exit code: 0 if success, 1 if fail.  */

#include <stdio.h>
#include <math.h>
#include <errno.h>
#include <ctype.h>
#include <limits.h>
#include <string.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdlib.h>
#include <inttypes.h>
#include <locale.h>

int __printf(const char *restrict fmt, ...);
int __scanf(const char *restrict fmt, ...);
int __sscanf(const char *p, const char *restrict fmt, ...);
int __fscanf(FILE *, const char *restrict fmt, ...);

#define scanf    __scanf
#define sscanf   __sscanf
#define fscanf   __fscanf
#define tputs(s,f) fprintf(stderr, "scanf/aout_test:%d:" s, __LINE__)

#define array_length(var) (sizeof (var) / sizeof ((var)[0])) 

/* tstscanf & tst-swscanf compiled #if 1 or scanfXX tests #if 0 */
#if 1

/* Copyright (C) 1991-2023 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <https://www.gnu.org/licenses/>.  */

int main (int argc, char **argv)
{
  char buf[BUFSIZ] = {};
  FILE *in = stdin, *out = stdout;
  int x;
  int result = 0;

  if (sscanf ("0", "%d", &x) != 1)
    {
      tputs ("test failed!\n", stdout);
      result = 1;
    }

  if (sscanf ("08905x", "%9[0-9]", buf) != 1
      || strcmp (buf, "08905") != 0)
    {
      tputs ("test failed!\n", stdout);
      result = 1;
    }

  if (sscanf ("", "%10[a-z]", buf) != EOF)
    {
      tputs ("test failed!\n", stdout);
      result = 1;
    }

  sscanf ("conversion] Zero flag Ze]ro#\n", "%*[^]] %[^#]\n", buf);
  if (strcmp (buf, "] Zero flag Ze]ro") != 0)
    {
      tputs ("test failed!\n", stdout);
      result = 1;
    }

  if (argc == 2 && !strcmp (argv[1], "-opipe"))
    {
      out = popen ("/bin/cat", "w");
      if (out == NULL)
	{
	  perror ("popen: /bin/cat");
	  result = 1;
	}
    }
  else if (argc == 3 && !strcmp (argv[1], "-ipipe"))
    {
      sprintf (buf, "/bin/cat %s", argv[2]);
      in = popen (buf, "r");
      if (in == NULL)
	{
	  perror ("popen: /bin/cat");
	  result = 1;
	}
    }

  {
    char name[50] = {};
    fprintf (out,
	     "sscanf (\"thompson\", \"%%s\", name) == %d, name == \"%s\"\n",
	     sscanf ("thompson", "%s", name),
	     name);
    if (strcmp (name, "thompson") != 0)
      {
	tputs ("test failed!\n", stdout);
	result = 1;
      }
  }

  fputs ("Testing scanf (vfscanf)\n", out);

  fputs ("Test 1:\n", out);
  {
    int n, i;
    float x;
    char name[50] = {};
    n = fscanf (in, "%d%f%s", &i, &x, name);
    fprintf (out, "n = %d, i = %d, x = %f, name = \"%.50s\"\n",
	     n, i, x, name);
    if (n != 3 || i != 25 || x != 5.432F || strcmp (name, "thompson"))
      {
	tputs ("test failed!\n", stdout);
	result = 1;
      }
  }
  fprintf (out, "Residual: \"%s\"\n", fgets (buf, sizeof (buf), in));
  if (strcmp (buf, "\n"))
    {
      tputs ("test failed!\n", stdout);
      result = 1;
    }
  fputs ("Test 2:\n", out);
  {
    int i;
    float x;
    char name[50] = {};
    (void) fscanf (in, "%2d%f%*d %[0123456789]", &i, &x, name);
    fprintf (out, "i = %d, x = %f, name = \"%.50s\"\n", i, x, name);
    if (i != 56 || x != 789.0F || strcmp (name, "56"))
      {
	tputs ("test failed!\n", stdout);
	result = 1;
      }
  }
  fprintf (out, "Residual: \"%s\"\n", fgets (buf, sizeof (buf), in));
  if (strcmp (buf, "a72\n"))
    {
      tputs ("test failed!\n", stdout);
      result = 1;
    }
  fputs ("Test 3:\n", out);
  {
    static struct {
      int count;
      float quant;
      const char *units;
      const char *item;
    } ok[] = {
      { 3, 2.0F, "quarts", "oil" },
      { 2, -12.8F, "degrees", "" },
      { 0, 0.0F, "", "" },
      { 3, 10.0F, "LBS", "fertilizer" },
      { 3, 100.0F, "rgs", "energy" },  // this fails in musl as 100e interpreted as invalid floating point
      { -1, 0.0F, "", "" }};
    size_t rounds = 0;
    float quant;
    char units[21], item[21];
    while (!feof (in) && !ferror (in))
      {
	int count;

	if (rounds++ >= array_length (ok))
	  {
	    tputs ("test failed!\n", stdout);
	    result = 1;
	  }

	quant = 0.0;
	units[0] = item[0] = '\0';
	count = fscanf (in, "%f%20s of %20s", &quant, units, item);
	(void) fscanf (in, "%*[^\n]");
	fprintf (out, "count = %d, quant = %f, item = %.21s, units = %.21s\n",
		 count, quant, item, units);
	if (count != ok[rounds-1].count || quant != ok[rounds-1].quant
	    || strcmp (item, ok[rounds-1].item)
	    || strcmp (units, ok[rounds-1].units))
	  {
	    tputs ("test failed!\n", stdout);
	    result = 1;
	  }
      }
  }
  buf[0] = '\0';
  fprintf (out, "Residual: \"%s\"\n", fgets (buf, sizeof (buf), in));
  if (strcmp (buf, ""))
    {
      tputs ("test failed!\n", stdout);
      result = 1;
    }

  if (out != stdout)
    pclose (out);

  fputs ("Test 4:\n", out);
  {
    int res, val, n;

    res = sscanf ("-242", "%3o%n", &val, &n);
    printf ("res = %d, val = %d, n = %d\n", res, val, n);
    if (res != 1 || val != -20 || n != 3)
      {
	tputs ("test failed!\n", stdout);
	result = 1;
      }
  }

  fputs ("Test 5:\n", out);
  {
    double a = 0, b = 0;
    int res, n;

    res = sscanf ("1234567", "%3lg%3lg%n", &a, &b, &n);
    printf ("res = %d, a = %g, b = %g, n = %d\n", res, a, b, n);

    if (res != 2 || a != 123 || b != 456 || n != 6)
      {
	tputs ("test failed!\n", stdout);
	result = 1;
      }

    res = sscanf ("0", "%lg", &a);
    printf ("res = %d, a = %g\n", res, a);

    if (res != 1 || a != 0)
      {
	tputs ("test failed!\n", stdout);
	result = 1;
      }

    res = sscanf ("1e3", "%lg%n", &a, &n);
    printf ("res = %d, a = %g, n = %d\n", res, a, n);

    if (res != 1 || a != 1000 || n != 3)
      {
	tputs ("test failed!\n", stdout);
	result = 1;
      }
  }

  fputs ("Test 6:\n", stdout);
  {
    char *p = (char *) -1;
    int res;

    sprintf (buf, "%p", NULL);
    res = sscanf (buf, "%p", &p);
    printf ("sscanf (\"%s\", \"%%p\", &p) = %d, p == %p\n", buf, res, p);

    if (res != 1 || p != NULL)
      {
	tputs ("test failed!\n", stdout);
	result = 1;
      }
  }

  fputs ("Test 7:\n", stdout);
  {
    short a[2] = { -1, -1 };
    int res;

    res = sscanf ("32767 1234", "%hd %hd", &a[0], &a[1]);
    printf ("res = %d, a[0] = %d, a[1] = %d\n", res, a[0], a[1]);

    if (res != 2 || a[0] != 32767 || a[1] != 1234)
      {
	tputs ("test failed!\n", stdout);
	result = 1;
      }
  }

  fputs ("Test 8:\n", stdout);
  {
    double d = 123456.789;
    int res;

    res = sscanf ("0x1234", "%lf", &d);
    printf ("res = %d, d = %f\n", res, d);

    if (res != 1 || d != 4660)
      {
	tputs ("test failed!\n", stdout);
	result = 1;
      }
  }

  fputs ("Test 9:\n", stdout);
  {
    /* From PR libc/1313 reported by Ben Caradoc-Davies <bmcd@physics.otago.ac.nz>.  */
    float value;
    int res;

    res = sscanf ("0123", "%2f", &value);
    if (res != 1 || value != 1.0)
      {
	tputs ("test failed!\n", stdout);
	result = 1;
      }
  }

  fputs ("Test 10:\n", stdout);
  {
    float value;
    int res;

    res = sscanf ("--", "%f", &value);
    if (res != 0)
      {
	tputs ("test failed!\n", stdout);
	result = 1;
      }
  }

  fputs ("Test 11:\n", stdout);
  {
    char uart[50];
    int res;

    res = sscanf ("uart:16550A tx:0", "uart:%31s tx:%*u", uart);
    if (res != 1)
      {
	tputs ("test failed!\n", stdout);
	result = 1;
      }
  }

  fputs ("Test 12:\n", stdout);
  {
    char uart[50];
    int res;

    res = sscanf ("uart:16550A", "uart:%31s tx:%*u", uart);
    if (res != 1)
      {
	tputs ("test failed!\n", stdout);
	result = 1;
      }
  }

  fputs ("Test 13:\n", stdout);
  {
    float value;
    int res;

    res = sscanf ("-InF", "%f", &value);
    if (res != 1 || isinf (value) != -1)
      {
	tputs ("test failed!\n", stdout);
	result = 1;
      }

    res = sscanf ("+InfiNiTY", "%f", &value);
    if (res != 1 || isinf (value) != 1)
      {
	tputs ("test failed!\n", stdout);
	result = 1;
      }
  }

  return result;
}

#else

/* Additional scanf tests in tst-misc directory */

#define exit return

#define main     main1
#include "../tst-misc/scanf1.c"
#undef  main

#define main     main2
#include "../tst-misc/scanf2.c"
#undef  main

#define main     main3
#include "../tst-misc/scanf3.c"
#undef  main

#define main     main5
#include "../tst-misc/scanf5.c"
#undef  main

#define main     main8
#include "../tst-misc/scanf8.c"
#undef  main

#define main     main9
#include "../tst-misc/scanf9.c"
#undef  main

#define main     main10
#include "../tst-misc/scanf10.c"
#undef  main

#define main     main11
#include "../tst-misc/scanf11.c"
#undef  main

#define main     main14
#include "../tst-misc/scanf14.c"
#undef  main

#define main     main15
#include "../tst-misc/scanf15.c"
#undef  main

#define main     main16
#include "../tst-misc/scanf16.c"
#undef  main

#define xscanf   yscanf
#define xsscanf  ysscanf
#define xfscanf  yfscanf
#define main     main17
#include "../tst-misc/scanf17.c"
#undef  main

int main(int argc, char *argv[]) {
  char rc[18] = {}, i = 0;
 
  printf("\n*** scanf %d ***\n", i+=1);
  rc[i] = main1(argc, argv);
  printf("\n*** scanf %d ***\n", i+=1);
  rc[i] = main2(argc, argv);
  printf("\n*** scanf %d ***\n", i+=1);
  rc[i] = main3(argc, argv);
  printf("\n*** scanf %d ***\n", i+=2);
  rc[i] = main5(argc, argv);
  printf("\n*** scanf %d ***\n", i+=3);
  rc[i] = main8(argc, argv);
  printf("\n*** scanf %d ***\n", i+=1);
  rc[i] = main9();
  printf("\n*** scanf %d ***\n", i+=1);
  rc[i] = main10(argc, argv);
  printf("\n*** scanf %d ***\n", i+=1);
  rc[i] = main11(argc, argv);
  printf("\n*** scanf %d ***\n", i+=3);
  rc[i] = main14();
  printf("\n*** scanf %d ***\n", i+=1);
  rc[i] = main15();
  printf("\n*** scanf %d ***\n", i+=1);
  rc[i] = main16();
  printf("\n*** scanf %d ***\n", i+=1);
  rc[i] = main17();
  printf("\n*** results ***\n");
  for (int j = 0; j < sizeof rc; j++) printf("rc.%d = %d\n", j, rc[j]);
}

#endif