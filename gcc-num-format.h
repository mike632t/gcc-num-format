/*
 *
 * gcc-num-format.h - Number macros.
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
 * 16 Dec 18         - Initial version - MEJT
 * 20 Dec 18         - Now compiles using VAXC - MEJT
 *
 * Note - Do NOT use operators live increment and decriment on values passed
 * to these macros the results will not be as intended!
 */

/* Truncate */
#ifndef TRUNC /* Don't redefine macro if already defined. */
#define TRUNC(x) ((x > 0) ? floor(x) : ceil(x))
#endif

/* Round - problems with floating point rounding errors.. */
#ifndef ROUND /* Don't redefine macro if already defined. */
#define ROUND(x) ((x >= 0) ? TRUNC(x + 0.5) : TRUNC(x - 0.5))
#endif

/* Sign */
#ifndef SIGN /* Don't redefine macro if already defined. */
#define SIGN(x) ((( x > 0 ) - ( x < 0 )))
#endif

/* Min */
#ifndef MIN /* Don't redefine macro if already defined. */
#define MIN(x, y) ((x < y) ? x : y)
#endif

/* Max */
#ifndef MAX /* Don't redefine macro if already defined. */
#define MAX(x, y) ((x >= y) ? x : y)
#endif

#ifndef ABS /* Don't redefine macro if already defined. */
#define ABS(x) (((x) < 0) ? -(x) : (x))
#endif
