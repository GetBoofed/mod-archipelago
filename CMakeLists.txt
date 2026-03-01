install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/data/" 
        DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/modules/archipelago_data"
        FILES_MATCHING PATTERN "*.json")