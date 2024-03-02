set (scripts
    all_env.bat
    indent_b3d.py
    run_b3d_reader_for_env.bat
    run_b3d_reader_for_trucks.bat
    run_b3d_reader_for_types.bat
)


foreach (script ${scripts})
    configure_file(${script} ${APP_ROOT_DIRECTORY}/)
endforeach()
