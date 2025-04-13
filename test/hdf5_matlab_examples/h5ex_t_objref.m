function h5ex_t_objref
  %%**************************************************************************
  %%  This example is adapted from the original file generously provided
  %%  by a user to the HDFGroup examples.
  %%
  %% This example shows how to read and write object references
  %% to a dataset.  The program first creates objects in the
  %% file and writes references to those objects to a dataset
  %% with a dataspace of DIM0, then closes the file.  Next, it
  %% reopens the file, dereferences the references, and outputs
  %% the names of their targets to the screen.
  %%
  %%  This file is intended for use with HDF5 Library version 1.8
  %%  EDIT: Also tested with version 1.10.
  %%**************************************************************************

  fileName       = tempname ();
  DATASET        = 'DS1';

  %%
  %% Create a new file using the default properties.
  %%
  file = H5F.create (fileName, 'H5F_ACC_TRUNC', 'H5P_DEFAULT', 'H5P_DEFAULT');

  %%
%%%% Create a dataset with a scalar dataspace.
  %%
  space = H5S.create ('H5S_NULL');
  obj = H5D.create (file, 'DS2', 'H5T_STD_I32LE', space, 'H5P_DEFAULT');
  H5D.close (obj);
  H5S.close (space);

  %%
  %% Create a group.
  %%
  size_hint=H5ML.get_constant_value('H5P_DEFAULT');
  obj = H5G.create (file, 'G1', size_hint);
  H5G.close (obj);

  %%
  %% Create references to the previously created objects.  Passing -1
  %% as space_id causes this parameter to be ignored.  Other values
  %% besides valid dataspaces result in an error.
  %%
  wdata(1) = H5R.create (file, 'G1', 'H5R_OBJECT', -1);
  wdata(2) = H5R.create (file, 'DS2', 'H5R_OBJECT', -1);

  %%
  %% Create dataspace.  Setting maximum size to [] sets the maximum
  %% size to be the current size.
  %%
  space = H5S.create_simple (ndims (wdata),fliplr(size (wdata)), []);

  %%
  %% Create the dataset and write the object references to it.
  %%
  dset = H5D.create (file, DATASET, 'H5T_STD_REF_OBJ', space, 'H5P_DEFAULT');
  H5D.write (dset, 'H5T_STD_REF_OBJ', 'H5S_ALL', 'H5S_ALL', 'H5P_DEFAULT',wdata);

  %%
  %% Close and release resources.
  %%
  H5D.close (dset);
  H5S.close (space);
  H5F.close (file);

  %%
  %% Now we begin the read section of this example.  Here we assume
  %% the dataset has the same name and rank, but can have any size.
  %%

  %%
  %% Open file and dataset.
  %%
  file = H5F.open (fileName, 'H5F_ACC_RDONLY', 'H5P_DEFAULT');
  dset = H5D.open (file, DATASET);

  %%
  %% Get dataspace.
  %%
  space = H5D.get_space (dset);
  [~, dims] = H5S.get_simple_extent_dims (space);

  %%
  %% Read the data.
  %%
  rdata = H5D.read (dset, 'H5T_STD_REF_OBJ', 'H5S_ALL', 'H5S_ALL', 'H5P_DEFAULT')

  %%
  %% Output the data to the screen.
  %%
  links = {};
  for i=1:numel (rdata)
    obj = H5R.dereference (dset, 'H5R_OBJECT', rdata(i));
    objtype = H5R.get_obj_type (dset, 'H5R_OBJECT', rdata(i));
    
    %%
    %% Retrieve the name.
    %%
    name = H5I.get_name(obj);
    
    switch (objtype)
      case H5ML.get_constant_value('H5O_TYPE_GROUP')
        links = [links ["Group " name]];
      case H5ML.get_constant_value('H5O_TYPE_DATASET')
        links = [links ["Dataset " name]];
      case H5ML.get_constant_value('H5O_TYPE_TYPE')
        links = [links ["Type " name]];
    end
    H5O.close (obj);
  end

  %%
  %% Close and release resources.
  %%
  H5D.close (dset);
  H5S.close (space);
  H5F.close (file);
  delete (fileName);

  assert (rdata, wdata);
  
  assert (links, {"Group /G1", "Dataset /DS2"});
endfunction
