set( SRC_FILES 
	 src/main.cpp )
	
#find_package(ImageMagick COMPONENTS Magick++ MagickWand MagickCore)

#set( ImageMagick_LIB_DIRS ${ImageMagick_EXECUTABLE_DIR}/lib )

include_directories(${COMMON_HEADERS})

#link_directories(${ImageMagick_LIB_DIRS})
	
add_executable(ClientPC ${SRC_FILES})
#target_link_libraries(ImageProcessing ${ImageMagick_Magick++_LIBRARY} 
#							${ImageMagick_MagickWand_LIBRARY} 
#							${ImageMagick_MagickCore_LIBRARY})


 
TARGET_LINK_LIBRARIES ( ClientPC
	${ZEROMQ_LIBRARY}
	${B64_LIBRARY}
)

install(TARGETS ClientPC DESTINATION ${INSTALL_DIR})