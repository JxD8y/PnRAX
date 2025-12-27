add_subdirectory("ImGui")

add_library(PnRAXGUI STATIC 
		"MainWindow.cpp")

target_link_libraries(PnRAXGUI private ImEasy)