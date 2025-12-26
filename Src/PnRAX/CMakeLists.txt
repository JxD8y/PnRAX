add_executable(Pnrx "PnRAX.cpp" "Ui/ImGui_Impl_easy.cpp" "Ui/ImGui_Impl_easy.h")
add_subdirectory("PnLdr")
add_subdirectory("Ui")
find_package(capstone CONFIG REQUIRED)

target_link_libraries(Pnrx PRIVATE capstone::capstone)
target_link_libraries(Pnrx PRIVATE ImEasy)