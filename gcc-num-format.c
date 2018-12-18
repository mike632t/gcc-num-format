/*
 * gcc-num-format.c - Formatting for floating point numbers.
 *
 * Copyright(C) 2018   MEJT
 *
 * A  format routine that can format a floating pint number according to the
 * rules for FIX, SCI and ENG number formats.
 *
 * This  program is free software: you can redistribute it and/or modify  it
 * under  the  terms of the GNU General Public License as published  by  the
 * Free  Software  Foundation, either version 3 of the License, or (at  your
 * option) any later version.
 *
 * This  program  is  distributed in the hope that it will  be  useful,  but
 * WITHOUT   ANY   WARRANTY;   without  even   the   implied   warranty   of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General
 * Public License for more details.
 *
 * You  should have received a copy of the GNU General Public License  along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * 20 Jul 13   0.1   - Initial version - MEJT
 * 22 Jul 13         - Fixed  the truncation errors that were occouring when
 *                     formatting  some  fractions by  rounding  the  number
 *                     before truncating it - MEJT
 *                   - Defined  trunc and round locally (not in all versions
 *                     of math.h) - MEJT
 *                   - Defined  display  width  as a  constant  and  updated
 *                     formatting  routines to allow the display width to be
 *                     adjusted at compile time - MEJT
 * 24 Jul 13         - Modified code to use sprinf() instead of asprinf() to
 *                     allow routines to compile using VAX C - MEJT
 * 25 Jul 13         - Added mant () to return the mantessa - MEJT
 * 26 Jul 13         - Modified  eng()  and sci() to include spaces  between
 *                     the value and the exponent - MEJT
 *                   - Added sign function -MEJT
 *                   - Modified eng and sci to remove string constants  used
 *                     to store the sign character - MEJT
 * 27 Jul 13         - Fixed formatting errors in sci() and eng() - MEJT
 * 02 Aug 13         - Checks for overflow in sci() and eng() - MEJT
 * 17 Aug 13         - Added debug and warning macros - MEJT
 *                   - Combined sci() and eng() in format() - MEJT
 * 18 Aug 13         - Finally got format() working, fixed problems
 *                     with the mantessa when rounding up - MEJT
 *                   - Removed sci() and eng() - MEJT
 * 18 Aug 13         - Fixed fixed problem with mant() caused by rounding up
 *                     when truncating the number of digits - MEJT
 * 21 Aug 13   0.2   - Put test cases into an array - MEJT
 *                   - Added integer min() and max() functions - MEJT
 *                   - Now checks for numeric underflow - MEJT
 *                   - Display  width added as a parameter to  format()  and
 *                     mant() - MEJT
 *                   - Tidied  up warning messages (errors now display value
 *                     correctly) - MEJT
 *                   - Corrected  width  of some format strings  to  include
 *                     sign  and  decimal place, changed display of the  raw
 *                     test  values  to  always include a leading  sign  (to
 *                     workaround formatting issues with VAX C RTL) - MEJT
 * 25 Aug 13         - Test values now depend on display width - MEJT
 * 16 Dec 18         - Replaced local math functions with macros - MEJT
 *
 * TO DO :           - Format output using FIX.
 *                   - Test output for different display widths.
 *
 */

#define DEBUG 1       /* Can be DEBUG or RELEASE (or somthing else!). */

#define WIDTH          10
#define LIMIT          15

#define MIN(a,b)       (((a)<(b))?(a):(b))
#define MAX(a,b)       (((a)>(b))?(a):(b))

#include <stdio.h>     /* fprintf(), etc. */
#include <math.h>      /* log10(), etc. */

#include "gcc-num-format.h" /* TRUNC(), etc. */
#include "gcc-num-debug.h" /* debug(), warning(), etc. */

char* s_format(char*, double, int, int, int);
char* s_mant(char*, double, int);

int main(int argc, char *argv[]) {

   const double d_testcase[] = {
      0.00,
      1.0e+100, /* Overflow. */
      1.0e-100, /* Underflow. */
      100 * (1 - pow(10, - WIDTH)), /* Round up. */
      0.46 - pow(10, - WIDTH), /* Intermediate rounding. */
      2.0e-08, /* Fraction. */
      2.0 / -3.0, /* Recurring decimal value. */
      0.2, /* Recouring binary fraction. */
      (48 - 47.8) - 0.2,
      123456789.0,
      -1.2e-19,  // - Checked
      15,
      2.831068713e4,
      0.002};

   double d_test;
   char s_string[WIDTH + 3]; /* Allowing for the sign, decimal point and terminator. */
   int i_count, i_test;
   int i_start = 0;
   int i_limit = MIN(sizeof(d_testcase)/sizeof(d_testcase[0]), LIMIT);
   
   for (i_test = i_start; i_test < i_limit; i_test++) {
      
      d_test = d_testcase[i_test];
      
      /* FIX */
      for (i_count = 0; i_count <= 9; i_count++) {
         fprintf(stdout, "%-+17.9e\t= %s   \t(FIX %d)\n",  d_test, s_format(s_string, d_test, WIDTH, i_count, 0), i_count);
      }
      fprintf(stdout, "%-+17.9e\t= %s\n\n",  d_test, s_mant(s_string, d_test, WIDTH));   

      /* SCI */
      for (i_count = 0; i_count <= 9; i_count++) {
         fprintf(stdout, "%-+17.9e\t= %s    \t(SCI %d)\n",  d_test, s_format(s_string, d_test, WIDTH, i_count, 1), i_count);
      }

      fprintf(stdout, "%-+17.9e\t= %s\n\n",  d_test, s_mant(s_string, d_test, WIDTH));   
      
      /* ENG */
      for (i_count = 0; i_count <= 9; i_count++) {
         fprintf(stdout, "%-+17.9e\t= %s    \t(ENG %d)\n",  d_test, s_format(s_string, d_test, WIDTH, i_count, 2), i_count);
      }

      fprintf(stdout, "%-+17.9e\t= %s\n\n",  d_test, s_mant(s_string, d_test, WIDTH));   
   }
   return(0);
} 

char* s_format(char* s_string, double d_value, int i_width, int i_precision, int i_mode) {
   
   #undef DEBUG /* Disable debug code */
   #define DEBUG 1
   
   double d_number = d_value;
   int i_sign, i_exponent, i_decimals, i_digits;
   char c_sign = ' ';
   
   i_exponent = 0; i_digits = 1; i_decimals = i_precision;
   i_sign = SIGN(d_number);

   if (ABS(i_sign) > 0) { 

      d_number *= i_sign; /* Make number positive before formatting it and restore the sign at the end! */
      i_exponent = (int) ROUND(floor(log10(d_number))); /* Find exponent. */

      if (i_mode > 0 || (ABS(i_exponent) > (i_width)) || ((i_decimals + i_exponent) < 0)) {
         
         d_number /= pow(10.0, i_exponent); /*  Find mantessa. */

         /* Round up the the desired number of decimal places. */
         d_number = ROUND(d_number * pow(10.0, i_decimals)) / pow(10.0, i_decimals); 
         while (d_number >= 10.0) {d_number /= 10.0; i_exponent++;} /* Fix up value if necessary. */

         /* Check for numeric underflow. */
         if (i_exponent + (int) ROUND(floor(log10(d_number))) < -99) {
            warning("Warning: Underflow   (%- fe%+-2d)", d_number * i_sign, i_exponent);
            d_number = 0.0; i_exponent = 0; i_sign = 0;
         }

         /* Adjust exponent for enginering mode. */
         if (i_mode > 1) {
            i_digits += i_decimals;
            i_decimals -= i_exponent;
            if ((i_exponent / 3) * 3 - i_exponent != 0) {/* Is the exponent already a multiple of 3. */
               if(i_exponent >= 0) i_exponent = (i_exponent / 3) * 3; else i_exponent = ((-i_exponent + 3) / 3) * (-3); /* Round the exponent up/down as needed. */
            }
            i_decimals += i_exponent; /* Work out the change in the number of decimal places */
            i_digits -= i_decimals;
            if (i_decimals < 0) i_decimals = 0;
            d_number = ROUND(d_number * pow(10.0, i_digits - 1 + i_decimals))/ pow(10.0, i_decimals);
         }

         /* Check for numeric overflow. */
         if (i_exponent + (int) ROUND(floor(log10(d_number))) > 99) {
            while (d_number >= 10.0) {d_number /= 10.0; i_exponent++;} /* Fix up value if necessary. */
            warning("Warning: Overflow    (%- fe%+-2d)", d_number * i_sign, i_exponent);
            d_number = 9.999999999999999; i_exponent = 99;
            i_decimals = i_width - 1; i_digits = 1; /* Force all decimal places to be shown. */
         }
      }
      else {
         /* Round up the the desired number of decimal places. */
         if ((i_decimals + i_exponent) >= i_width) i_decimals = i_width - i_exponent -1;
         d_number = ROUND(d_number * pow(10.0, i_decimals)) / pow(10.0, i_decimals); 
      }
      d_number *= i_sign; /* Fix up the sign. */
   }
   
   if (i_mode > 0 || (ABS(i_exponent) > (i_width)) || ((i_decimals + i_exponent) < 0)) { /* SCI or ENG format */
      if (i_digits + i_decimals > i_width - 3){ /* Truncate mantessa if necessary. */
         d_number = TRUNC(d_number * pow(10.0, i_width - 3 - i_digits)) / pow(10.0, i_width - 3 - i_digits); /* Truncate. */
         i_decimals = i_width - 3 - i_digits; /* Adjust number of decimal places. */
      }
      if (i_exponent < 0) {c_sign = '-'; i_exponent = -i_exponent;} /* Is exponent negative? */
      sprintf(s_string, "% #*.*f%*c%02d+", i_digits + i_decimals + 2,  i_decimals, d_number, i_width - 2 - i_digits - i_decimals, c_sign, i_exponent); /* Allowing for the sign and decimal point. */
      return s_string;
   }
   else { /* FIX format */
      if (i_digits + i_decimals > i_width){ /* Truncate mantessa if necessary. */
         i_decimals = i_width - i_digits; /* Adjust number of decimal places. */
      }
      print("%d %d.%d", i_width, i_digits, i_decimals);
      //sprintf(s_string, "% #*.*f%*c+", i_digits + i_decimals + 2,  i_decimals, d_number, i_width - 2 - i_digits - i_decimals, ' '); /* Allowing for the sign and decimal point. */
      sprintf(s_string, "% #*.*f%*c", i_digits + i_decimals + 2,  i_decimals, d_number, i_width - i_digits - i_decimals, '+'); /* Allowing for the sign and decimal point. */
      return s_string;
   }
}

char* s_mant(char* s_string, const double d_value, int i_width) {

   double d_number = d_value;
   int i_exponent = 0;
   switch (SIGN(d_number)) {
   case -1:
      d_number = -d_number; /* Change sign of number and fall through. */
   case 1:
      i_exponent = (int) ROUND(floor(log10(d_number))); /* Find exponent. */
      d_number /= pow(10.0, i_exponent); /*  Find mantessa. */

      /* Round up the the desired number of decimal places. */
      d_number = ROUND(d_number * pow(10.0, i_width - 1)) / pow(10.0, i_width - 1); 
      if (d_number >= 10.0) {d_number /= 10.0; i_exponent++;}
      d_number = ROUND(d_number * pow(10.0, i_width - 1));
   }
   if ((i_exponent < -99)) d_number = 0.;  /* Check for numeric underflow. */
   if ((i_exponent >= 99) && (d_number >= 10)) d_number = 9999999999.; /* Check for numeric overflow. */
   sprintf(s_string, " %0*.0f", i_width, d_number);
   return s_string;
}


