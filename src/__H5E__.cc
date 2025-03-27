/*

Copyright (C) 2021-2025 Pantxo Diribarne

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

#include <octave/error.h>
#include <octave/oct.h>
#include <octave/octave.h>
#include <octave/parse.h>
#include <octave/interpreter.h>

#include <hdf5.h>

#include "./util/h5_oct_util.h"


static octave_value current_err_cb;

herr_t
error_handler (unsigned n, const H5E_error2_t *err_desc, void* exception_ptr)
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


  octave::execution_exception* exec_exception =
    static_cast<octave::execution_exception*> (exception_ptr);

  try
    {
      retval = static_cast<herr_t> (octave::feval (current_err_cb, ovl (n, m),
                                                   1)(0).xint_value ("STATUS must be an integer"));
    }
  catch (const octave::execution_exception& ee)
    {
      // Let the callback return an error status and store the exception
      // for further rethrow.
      retval = -1;
      *exec_exception = ee;
    }

  return retval;
}
// PKG_ADD: autoload ("__H5E_get_class_name__", "__H5E__.oct");
// PKG_DEL: autoload ("__H5E_get_class_name__", "__H5E__.oct", "remove");
DEFUN_DLD(__H5E_get_class_name__, args, nargout, 
"-*- texinfo -*-\n\
@deftypefn {} {@var{cname} =} H5E.get_class_name (@var{class_id})\n\
@seealso{H5E.walk}\n\
@end deftypefn")
{
  octave_value_list retval;

  int nargin = args.length ();

  if (nargin != 1)
    print_usage ("H5E.get_class_name");

  // Class id as returned in error structs
  hid_t class_id = get_h5_id (args, 0, "CLASS_ID", "H5E.get_class_name", false);

  size_t sz = 1024;
  char cname_char[sz];

  if (H5Eget_class_name (class_id, cname_char, sz) < 0)
    error ("H5E.get_class_name: unable to retrieve class name");

  return retval.append (std::string (cname_char));
}

// PKG_ADD: autoload ("__H5E_get_major__", "__H5E__.oct");
// PKG_DEL: autoload ("__H5E_get_major__", "__H5E__.oct", "remove");
DEFUN_DLD(__H5E_get_major__, args, nargout, 
"-*- texinfo -*-\n\
@deftypefn {} {@var{err_msg} =} H5E.get_major (@var{maj_num})\n\
@seealso{H5E.walk}\n\
@end deftypefn")
{
  octave_value_list retval;

  int nargin = args.length ();

  if (nargin != 1)
    print_usage ("H5E.get_major");

  H5E_major_t maj_num =
    static_cast<H5E_major_t> (args(0).xuint64_value ("H5E.get_major: MAJ_NUM  must an unsigned integer"));


  char* str = H5Eget_major (maj_num);

  if (str)
    {
      retval.append (std::string (str));
      std::free (str);
    }
  else
    retval.append (std::string ());

  return retval;
}

// PKG_ADD: autoload ("__H5E_get_msg__", "__H5E__.oct");
// PKG_DEL: autoload ("__H5E_get_msg__", "__H5E__.oct", "remove");
DEFUN_DLD(__H5E_get_msg__, args, nargout, 
"-*- texinfo -*-\n\
@deftypefn {} {@var{msg} =} H5E.get_msg (@var{msg_id})\n\
@seealso{H5E.walk}\n\
@end deftypefn")
{
  octave_value_list retval;

  int nargin = args.length ();

  if (nargin != 1)
    print_usage ("H5E.get_msg");

  // Message id as returned in error structs for major_num and minor_num
  hid_t msg_id = get_h5_id (args, 0, "MSG_ID", "H5E.get_msg", false);

  size_t sz = 1024;
  char msg[sz];
  H5E_type_t type;

  ssize_t nret = H5Eget_msg (msg_id, &type, msg, sz);

  printf ("ID is %ld, nret is %ld, msg is %s\n", msg_id, nret, msg);

  if (nret < 0)
    error ("H5E.get_msg: unable to retrieve message for id %ld", msg_id);

  return retval.append (std::string (msg));
}

// PKG_ADD: autoload ("__H5E_oclear__", "__H5E__.oct");
// PKG_DEL: autoload ("__H5E_oclear__", "__H5E__.oct", "remove");
DEFUN_DLD(__H5E_oclear__, args, nargout, 
"-*- texinfo -*-\n\
@deftypefn {} {} H5E.oclear ()\n\
@seealso{H5E.walk}\n\
@end deftypefn")
{
  octave_value_list retval;

  int nargin = args.length ();

  if (nargin != 0)
    print_usage ("H5E.oclear");


  hid_t estack_id = H5Eget_current_stack ();

  H5Eclear (estack_id);

  H5Eclose_stack (estack_id);

  return retval;
}

// PKG_ADD: autoload ("__H5E_set_auto__", "__H5E__.oct");
// PKG_DEL: autoload ("__H5E_set_auto__", "__H5E__.oct", "remove");
DEFUN_DLD(__H5E_set_auto__, args, nargout, 
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

// PKG_ADD: autoload ("__H5E_walk__", "__H5E__.oct");
// PKG_DEL: autoload ("__H5E_walk__", "__H5E__.oct", "remove");
DEFUN_DLD(__H5E_walk__, args, nargout, 
"-*- texinfo -*-\n\
@deftypefn {} {} H5E.walk (@var{direction}, @var{fcn})\n        \
@seealso{H5E.clear}\n                                           \
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

  octave::execution_exception ee;

  herr_t herr = H5Ewalk (estack_id, direction, error_handler, &ee);

  H5Eclose_stack (estack_id);

  // Rethrow previously caugh error if necessary
  if (! ee.message ().empty ())
    throw ee;

  return retval.append (herr);
}


