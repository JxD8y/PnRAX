add_subdirectory("ImEasy")
add_subdirectory("Res")

add_library(PnRAXGUI STATIC 
	 "PEViewer.cpp"
	 "DecompilerWindow.cpp"
	 "GlobalContext.h"
	 "TopMenu.h"
	 "IETableWindow.cpp" "Res/ResUtil.h")

find_package(fmt REQUIRED)
find_package(imgui CONFIG REQUIRED)

target_link_libraries(PnRAXGUI PRIVATE fmt::fmt)
target_link_libraries(PnRAXGUI PRIVATE imgui::imgui)
target_link_libraries(PnRAXGUI PRIVATE ImEasy)

if(MSVC)
    target_include_directories(PnRAXGUI PRIVATE
	"${CMAKE_SOURCE_DIR}/vcpkg_installed/x64-windows/include"
	"${CMAKE_SOURCE_DIR}/Res/")
endif()