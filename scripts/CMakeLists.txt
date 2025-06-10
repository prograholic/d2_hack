set (scripts
    indent_b3d.py
    run_b3d_reader_for_env.bat
    run_b3d_reader_for_trucks.bat
    run_b3d_reader_for_common.bat
    run_b3d_reader_for_collecting_entries_list_patterns.bat
    run_b3d_reader_for_matching_tree_generator_materials.bat
    generate_res_material_test.py
)


foreach (script ${scripts})
    configure_file(${script} ${APP_ROOT_DIRECTORY}/)
endforeach()
