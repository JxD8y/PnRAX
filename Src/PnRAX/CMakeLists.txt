add_executable(Pnrx "PnRAX.cpp")
add_subdirectory("PnLdr")
find_package(capstone CONFIG REQUIRED)
target_link_libraries(Pnrx PRIVATE capstone::capstone)