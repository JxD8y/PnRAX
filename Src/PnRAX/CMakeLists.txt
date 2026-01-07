set(CXX_STANDARD_REQUIRED ON)
set(CXX_STANDARD 20)


add_executable(Pnrx "PnRAX.cpp")
add_subdirectory("PnLdr")
add_subdirectory("Ui")

if(MSVC)
    target_compile_options(ImEasy PRIVATE /std:c++20)
    target_compile_options(Pnrx PRIVATE /utf-8)
endif()

find_package(capstone CONFIG REQUIRED)
find_package(fmt REQUIRED)

target_link_libraries(Pnrx PRIVATE capstone::capstone)
target_link_libraries(Pnrx PRIVATE PnRAXGUI)
target_link_libraries(Pnrx PRIVATE PnLdr)