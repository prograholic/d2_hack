Object naming scheme

b3d_file_name:     {D2_ROOT}/ENV/{b3d_id}.b3d
res_file_name:     {D2_ROOT}/ENV/{b3d_id}.res

materials:         list of material names from {res_file_name}
block_header_name: name of block from {b3d_file_name}
material_index:    index of material in corresponding block
material_name:     {b3d_id}_materials\[material_index\]

mesh_name:         {b3d_id}_{block_header_name}
entity_name:       {mesh_name}.entity
scene_node:        {mesh_name}.scene_node

==================================================================


Color file naming scheme:
1. Enumerate COLORS section from {b3d_id}.res file
2. Generage file name: {b3d_id}_COLORS{color_number}.d2colorinfo. color_number - is a number of color in COLORS section


Material parsing algorithm:
1. Enumerate MATERIALS section from .res file
2. Generate material name for each entry: {b3d_id}_{local_material_name}.material. local_material_name - is a name of material from MATERIALS section
3. Read 



  
  
Mines and Pins:
* dynamical -> AnmObj0002 -> PinKey - полицейские шипы (в AP) (ON/OFF)
* dynamical -> AnmObj0001 -> MineKey - мины (в AP) - несколько состояний


Foreign resources:

Terrain map:
  https://rnr-wiki.ru/index.php?title=%D0%A4%D0%B0%D0%B9%D0%BB:%D0%9A%D0%B0%D1%80%D1%82%D0%B0_%D0%BF%D1%83%D1%81%D1%82%D1%8B%D0%BD%D0%B8_5.jpg
  We used to rotate and concatenate images by hand (see TerrainSceneNode::DefineTerrains), may be there is a better approach???

Description of file formats:
  https://github.com/prograholic/HardTruck-RignRoll-file-formats
  https://github.com/aleko2144/Hard-Truck-1-2-Blender-plugins
