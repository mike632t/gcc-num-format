/*
 *
 * gcc-num-debug.h - Debug macros.
 *
 * Copyright(C) 2018   MEJT
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
 * 17 Aug 13         - Initial version - MEJT
 * 18 Aug 13         - Added warning macro, changed format of messages to
 *                     indecate the type of message - MEJT
 * 14 Dec 18         - Added  a macro to allow optional blocks of code to be
 *                     included if DEBUG is defined - MEJT
 *                   - Added  operating system dependent definitions here as
 *                     this file is included in all modules - MEJT
 *                   - Added a macro to handle (fatal) errors - MEJT
 *                   - Updated message formats - MEJT
 * 15 Dec 18         - Tidied up debug code (removed duplication) - MEJT
 * 16 Dec 18         - Rewrote  debug macros to allow debugging to be turned
 *                     on and off as needed by refefining DEBUG - MEJT
 * 18 Dec 18         - Fixed bug in defination of debug macro - MEJT
 * 19 Dec 18         - Fixed spelling in comments - MEJT
 * 20 Dec 18         - Now compiles using VAXC - MEJT
 *
 */

/* Execute code if DEBUG is True */
#ifndef debug /* Don't redefine macro if already defined. */
#define debug(code) do {if(DEBUG){code;}} while(0)
#endif

#ifndef vms
/* Print a message to stderr if DEBUG is True */
   #ifndef print /* Don't redefine macro if already defined. */
      #define print(_fmt, ...) do { \
         if(DEBUG){ \
            fprintf(stderr,   "Debug\t: %s line : %d (%s) : " _fmt "\n", \
               __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__); \
         } \
      } while(0)
   #endif

   /* Print warning to stderr if DEBUG is True */
   #ifndef warning /* Don't redefine macro if already defined. */
      #define warning(_fmt, ...) do { \
         if(DEBUG){ \
            fprintf(stderr,   "Warning\t: %s line : %d (%s) : " _fmt "\n", \
               __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__); \
         } \
      } while(0)
   #endif

   /* Print an error message stderr and exit */
   #ifndef error /* Don't redefine macro if already defined. */
      #define error(_fmt, ...) do { \
         fprintf(stderr,   "Error\t: %s line : %d (%s) : " _fmt "\n", \
            __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__); \
         exit (1); \
      } while(0)
   #endif
#endif
