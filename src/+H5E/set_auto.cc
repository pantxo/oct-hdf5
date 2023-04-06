/*

Copyright (C) 2021 Pantxo Diribarne

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.

Author: Pantxo Diribarne <pantxo@dibona>
Created: 2021-04-25

*/

#include <octave/oct.h>
#include <octave/octave.h>
#include <octave/parse.h>
#include <octave/interpreter.h>

#include <hdf5/serial/hdf5.h>

#include "../util/h5_oct_util.h"


DEFUN_DLD(set_auto, args, nargout,
          "-*- texinfo -*-\n\
@deftypefn {} {} H5E.set_auto (@var{tf})\n\
@seealso{H5E.walk}\n\
@end deftypefn")
{
  octave_value_list retval;

  int nargin = args.length ();

  if (nargin != 1)
    print_usage ("H5E.set_auto");

  // Save default error logger
  static H5E_auto_t saved_func = nullptr;

  if (! saved_func)
    H5Eget_auto (H5E_DEFAULT, &saved_func, nullptr);

  // Use the default handler or none
  bool silence = ! args(0).xbool_value ("H5E.set_auto: TF must be logical.");

  if (silence)
    H5Eset_auto (H5E_DEFAULT, nullptr, nullptr);
  else
    H5Eset_auto (H5E_DEFAULT, saved_func, stderr);

  return retval;
}
