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
 *
 */

#ifndef debug /* Don't redefine macro if already defined. */
   #ifdef DEBUG /* Define macro to print debug formatted text on stderr. */
      #define debug(_fmt, ...) fprintf(stderr, "Debug:   %s line:%d (%s) " _fmt "\n", __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)
   #else/* Define macro to do nothing. */
      #define debug(_fmt, ...)
   #endif
   #define warning(_fmt, ...) fprintf(stderr, "Warning: %s line:%d (%s) " _fmt "\n", __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)
#endif


