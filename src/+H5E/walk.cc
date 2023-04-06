/*

Copyright (C) 2021-2023 Pantxo Diribarne

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

*/

#include <octave/oct.h>
#include <octave/octave.h>
#include <octave/parse.h>
#include <octave/interpreter.h>

#include <hdf5/serial/hdf5.h>

#include "../util/h5_oct_util.h"


static octave_value current_err_cb;

herr_t
error_handler (unsigned n, const H5E_error2_t *err_desc, void* /*user_data*/)
{
  // Error structure
  octave_scalar_map m;
  m.assign ("cls_id", err_desc->cls_id);
  m.assign ("maj_num", err_desc->maj_num);
  m.assign ("min_num", err_desc->min_num);
  m.assign ("line", err_desc->line);
  m.assign ("func_name", std::string (err_desc->func_name));
  m.assign ("file_name", std::string (err_desc->file_name));
  if (err_desc->desc)
    m.assign ("desc", std::string (err_desc->desc));
  else
    m.assign ("desc", std::string (""));

  // Evaluate user supplied function
  herr_t retval = 0;

  try
    {
      retval = static_cast<herr_t> (octave::feval (current_err_cb, ovl (n, m),
                                                   1)(0).xint_value ("STATUS must be an integer"));
    }
  catch (const octave::execution_exception& ee)
    {
      octave_stdout << "error: " << ee.message () << std::endl;
      retval = -1;
    }

  return retval;
}

DEFUN_DLD(walk, args, nargout,
          "-*- texinfo -*-\n\
@deftypefn {} {} H5E.walk (@var{direction}, @var{fcn})\n\
@seealso{H5E.clear}\n\
@end deftypefn")
{
  octave_value_list retval;

  int nargin = args.length ();

  if (nargin != 2)
    print_usage ("H5E.walk");

  // Direction
  H5E_direction_t direction
    = static_cast<H5E_direction_t> (get_h5_id (args, 0, "DIRECTION",
                                               "H5E.walk"));

  // Error handler callback
  if (! args(1).is_function_handle ())
    error ("H5E.walk: FCN must be a function handle");
  else
    current_err_cb = args(1);

  hid_t estack_id = H5Eget_current_stack ();

  H5Ewalk (estack_id, direction, error_handler, nullptr);

  H5Eclose_stack (estack_id);

  return retval;
}
