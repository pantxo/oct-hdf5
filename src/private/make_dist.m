## Copyright (C) 2023-2025 Pantxo Diribarne
##
## This program is free software: you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published by
## the Free Software Foundation, either version 3 of the License, or
## (at your option) any later version.
##
## This program is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
##
## You should have received a copy of the GNU General Public License
## along with this program.  If not, see <https://www.gnu.org/licenses/>.

## -*- texinfo -*-
## @deftypefn {} {} make_dist (@var{srcdir}, @{targetdir})
## Make package tarball.
## @seealso{}
## @end deftypefn

function make_dist (srcdir, targetdir)

  ## Ship test data files
  testdir = fullfile (srcdir, "..", "inst", "testdir");
  if (! exist (testdir, "dir"))
    mkdir (testdir);
  else
    warning ("off", "Octave:delete:no-such-file", "local")
    ## Delete previously present files
    delete (fullfile (testdir, "*"))
  endif

  copyfile (fullfile (srcdir, "..", "test", "data", "*.mat"), testdir);

  matio_test_file = fullfile (srcdir, "..", "test", "matio_test_datasets", ...
                              "matio_test_cases_uncompressed_hdf_le.mat");
  if (exist (matio_test_file, "file"))
    copyfile (matio_test_file, testdir);
  endif

  copyfile (fullfile (srcdir, "..", "test", "matio_test_cases.m"), testdir);
  copyfile (fullfile (srcdir, "..", "test", "hdf5_matlab_examples", ...
                      "*.m"), testdir);

  ## Archive all
  olddir = cd (fullfile (srcdir, "..",".."));

  files = {"DESCRIPTION", "INDEX", "COPYING", "Makefile", ...
           "src/Makefile", "src/*.m", ...
           "src/*.cc", ...
           "src/util/*.h", "src/util/*.cc", ...
           "doc/oct-hdf5*", ...
           "inst/testdir/*", };
  classes = dir (fullfile (srcdir, "..", "inst", "@*"));
  classfiles = cellfun (@(nm) fullfile ("inst", nm, [nm(2:end) ".m"]),
                        {classes.name}, "uni", false);
  files = [files, classfiles];
  
  files = cellfun (@(f) fullfile ("oct-hdf5", f), files, "uni", false);

  tarname = fullfile (targetdir, "oct-hdf5.tar.gz");
  tar (tarname, files);

  cd (olddir);
endfunction
