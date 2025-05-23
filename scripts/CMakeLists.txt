set (scripts
    all_env.bat
    indent_b3d.py
    run_b3d_reader_for_env.bat
    run_b3d_reader_for_trucks.bat
    run_b3d_reader_for_common.bat
    run_b3d_reader_for_collecting_entries_list_patterns.bat
)


foreach (script ${scripts})
    configure_file(${script} ${APP_ROOT_DIRECTORY}/)
endforeach()
