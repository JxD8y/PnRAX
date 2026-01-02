add_library(ImEasy STATIC
			ImGuiWindow.cpp
			ImGui_Impl_easy.cpp
			ImGuiWindowManager.cpp
			)

find_package(imgui CONFIG REQUIRED)

target_link_libraries(ImEasy PUBLIC imgui::imgui)
