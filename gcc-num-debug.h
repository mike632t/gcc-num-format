/*
 *
 * gcc-num-debug.h - Debug macro.
 *
 * Copyright(C) 2013   MEJT
 *      
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *   
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.   See the
 * GNU General Public License for more details.
 *   
 * You should have received a copy of the GNU General Public License
 * along with this program.   If not, see <http://www.gnu.org/licenses/>.
 *
 * Contains the type definations and function prototypes used to create and
 * display a seven segment display element.
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
 *
 */
 
#ifndef linux
   #define linux /* Override operating system - if necessary */
#endif

#ifndef debug /* Don't redefine macro if already defined. */
   #ifdef DEBUG
      #define debug(_fmt, ...) fprintf(stderr,   "Debug   : %s line : %d (%s) : " _fmt "\n", __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)
   #else/* Define macro to do nothing. */
      #define debug(_fmt, ...)
   #endif
#endif

#ifndef optional /* Don't redefine macro if already defined. */
   #ifdef DEBUG /* Define macro to run optional code. */
      #define optional(code) do {code;} while(0)
   #else/* Define macro to do nothing. */
      #define optional(code) {}
   #endif
#endif

#ifndef trace /* Don't redefine macro if already defined. */
   #ifdef DEBUG /* Define macro to print formatted debug text on stderr. */
   #else/* Define macro to do nothing. */
      #define trace(_fmt, ...)
   #endif
#endif

#ifndef warning /* Don't redefine macro if already defined. */
   #ifndef RELEASE
      #define warning(_fmt, ...) fprintf(stderr, "Warning : %s line : %d (%s) : " _fmt "\n", __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)
   #else/* Define macro to do nothing. */
      #define warning(_fmt, ...)
   #endif
#endif

#ifndef error /* Don't redefine macro if already defined. */
   #define error(_fmt, ...) fprintf(stderr,   "Error   : %s line : %d (%s) : " _fmt "", __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__); exit(1)
#endif
