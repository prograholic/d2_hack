d2_hack.resource.data.tool.b3d_reader.exe ENV %1 --skip_face_info --skip_mesh_info --skip_vector_data > %1.log 2>NUL
d2_hack.resource.data.tool.b3d_reader.exe ENV %1 --skip_optimization --skip_transformation --skip_face_info --skip_mesh_info --skip_vector_data --print_only_names > %1_names_noopt.log 2>NUL
d2_hack.resource.data.tool.b3d_reader.exe ENV %1 --skip_face_info --skip_mesh_info --skip_vector_data --print_only_names > %1_names.log 2>NUL
d2_hack.resource.data.tool.b3d_reader.exe ENV %1 --skip_optimization --skip_transformation  --skip_face_info --skip_mesh_info --skip_vector_data > %1_noopt.log
d2_hack.resource.data.tool.b3d_reader.exe ENV %1 > %1_full.log
d2_hack.resource.data.tool.b3d_reader.exe ENV %1 --skip_optimization --skip_transformation > %1_full_noopt.log
