add_library(ImEasy STATIC
			ImGuiWindow.cpp
			ImGui_Impl_easy.cpp
			ImGuiWindowManager.cpp
			ImGuiCustomDialog.cpp)

find_package(imgui CONFIG REQUIRED)
find_package(capstone CONFIG REQUIRED)

target_link_libraries(ImEasy PUBLIC imgui::imgui)
target_link_libraries(ImEasy PUBLIC capstone::capstone)
