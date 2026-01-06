add_library(ImEasy STATIC
			ImEasyWindow.cpp
			ImEasy.cpp
			ImEasyWindowManager.cpp
			ImEasyTiler.cpp
			ImEasyDialog.cpp)

find_package(imgui CONFIG REQUIRED)
find_package(capstone CONFIG REQUIRED)

target_link_libraries(ImEasy PUBLIC imgui::imgui)
target_link_libraries(ImEasy PUBLIC capstone::capstone)
