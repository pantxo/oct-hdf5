function h5ex_t_vlstringatt
  %**************************************************************************
  %  This example is adapted from the original file generously provided
  %  by a user to the HDFGroup examples.
  %
  %  This example shows how to read and write variable-length
  %  string datatypes to an attribute.  The program first
  %  writes variable-length strings to an attribute with a
  %  dataspace of DIM0, then closes the file.  Next, it reopens
  %  the file, reads back the data, and outputs it to the
  %  screen.
  %
  %  This file is intended for use with HDF5 Library version 1.8
  %  EDIT: Also tested with version 1.10.
  %**************************************************************************

  fileName       = 'h5ex_t_vlstringatt.h5';
  DATASET        = 'DS1';
  ATTRIBUTE      = 'A1';
  DIM0           = 4;

  dims   = DIM0;
  wdata  = {'Parting'; 'is such'; 'sweet  '; 'sorrow.'};

                    %
  %% Create a new file using the default properties.
  %
  file = H5F.create (fileName, 'H5F_ACC_TRUNC', 'H5P_DEFAULT', 'H5P_DEFAULT');

    %
    % Create file and memory datatypes.  For this example we will save
    % the strings as FORTRAN strings.
    %
  filetype = H5T.copy ('H5T_FORTRAN_S1');
  H5T.set_size (filetype,'H5T_VARIABLE');
  memtype = H5T.copy ('H5T_C_S1');
  H5T.set_size (memtype, 'H5T_VARIABLE');
  
                             %
                             % Create dataset with a scalar dataspace.
                             %
  space = H5S.create ('H5S_SCALAR');
  dset = H5D.create (file, DATASET, 'H5T_STD_I32LE', space, 'H5P_DEFAULT');
  H5S.close (space);

      %
      % Create dataspace.  Setting maximum size to [] sets the maximum
      % size to be the current size.
      %
  space = H5S.create_simple (1,fliplr( dims), []);

      %
      % Create the attribute and write the variable-length string data
      % to it.
      %
  attr = H5A.create (dset, ATTRIBUTE, filetype, space, 'H5P_DEFAULT');
  H5A.write (attr, memtype, wdata');

                                %
                                % Close and release resources.
                                %
  H5A.close (attr);
  H5D.close (dset);
  H5S.close (space);
  H5T.close (filetype);
  H5T.close (memtype);
  H5F.close (file);

    %
  %% Now we begin the read section of this example.  Here we assume
  % the attribute has the same name and rank, but can have any size.
  %

                                %
                                % Open file, dataset, and attribute.
                                %
  file = H5F.open (fileName, 'H5F_ACC_RDONLY', 'H5P_DEFAULT');
  dset = H5D.open (file, DATASET);
  attr = H5A.open_name (dset, ATTRIBUTE);

                                %
                                % Get the datatype.
                                %
  filetype = H5A.get_type (attr);

                  %
                  % Get dataspace and allocate memory for read buffer.
                  %
  space = H5A.get_space (attr);
  [~, dims, ~] = H5S.get_simple_extent_dims (space);
  dims=fliplr(dims);

                                %
                                % Create the memory datatype.
                                %
  memtype = H5T.copy ('H5T_C_S1');
  H5T.set_size (memtype, 'H5T_VARIABLE');
     %Use this syntax for pre R2009b versions
     %H5T.set_size (memtype, H5ML.get_constant_value('H5T_VARIABLE'));

                                %
                                % Read the data.
                                %
  rdata = H5A.read (attr, memtype);

                                %
                                % Close and release resources.
                                %
  H5A.close (attr);
  H5D.close (dset);
  H5S.close (space);
  H5T.close (filetype);
  H5T.close (memtype);
  H5F.close (file);

  assert (rdata, wdata)
endfunction
