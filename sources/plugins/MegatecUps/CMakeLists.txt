
# include common plugin definitions
include("../YadomsPluginsCommon.cmake")

#########################################
## Place here all the plugin source files
#########################################
set(MEGATECUPS_SOURCES
   stdafx.h
   stdafx.cpp
   IMegatecUpsConfiguration.h
   MegatecUps.cpp
   MegatecUps.h
   MegatecUpsConfiguration.cpp
   MegatecUpsConfiguration.h
   MegatecUpsFactory.cpp
   MegatecUpsFactory.h
   ProtocolException.hpp
   )

#########################################
## Place here all additional include directories
## (by default yadoms-shared and boost are included)
#########################################
set(MEGATECUPS_ADDITIONAL_INC_DIR
)

#########################################
## Place here all additional libraries to link with
## (by default yadoms-shared and boost are included)
#########################################
set(MEGATECUPS_ADDITIONAL_LIB
)

#########################################
## Always leave this three lines to configure plugin correctly
## even if MEGATECUPS_ADDITIONAL_INC_DIR or MEGATECUPS_ADDITIONAL_LIB are empty
#########################################
PLUGIN_SOURCES(megatecUps ${MEGATECUPS_SOURCES})
PLUGIN_INCLDIR(megatecUps ${MEGATECUPS_ADDITIONAL_INC_DIR})
PLUGIN_LINK(megatecUps ${MEGATECUPS_ADDITIONAL_LIB})

#########################################
## Post-build commands
## Copy files to build directory
## "package.json" and "icon.png" are needed, but you can add extra-files
#########################################
PLUGIN_POST_BUILD_COPY_FILE(megatecUps package.json)
PLUGIN_POST_BUILD_COPY_FILE(megatecUps icon.png)
PLUGIN_POST_BUILD_COPY_DIRECTORY(megatecUps locales)
