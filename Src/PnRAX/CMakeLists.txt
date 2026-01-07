set(CXX_STANDARD_REQUIRED ON)
set(CXX_STANDARD 20)

add_subdirectory("PnLdr")
add_subdirectory("Ui")
message(STATUS "DEBUG: My Resource File is: '${APP_RESOURCES}'")

add_executable(Pnrx WIN32 "PnRAX.cpp" 
                "Ui/Res/resource.rc"
                "Ui/Res/Fonts/calibri.ttf"
                "Ui/Res/app.ico")

if(MSVC)
    target_compile_options(ImEasy PRIVATE /std:c++20)
    target_compile_options(Pnrx PRIVATE /utf-8)
endif()

find_package(capstone CONFIG REQUIRED)
find_package(fmt REQUIRED)

target_link_libraries(Pnrx PRIVATE capstone::capstone)
target_link_libraries(Pnrx PRIVATE PnRAXGUI)
target_link_libraries(Pnrx PRIVATE PnLdr)