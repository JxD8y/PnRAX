add_library(ImEasy STATIC
			ImGui_Impl_easy.cpp
			"ImGuiWindow.h")

find_package(imgui CONFIG REQUIRED)

target_link_libraries(ImEasy PRIVATE imgui::imgui)
