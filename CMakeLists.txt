cmake_minimum_required(VERSION 2.8)
project(Opencv-focus)
find_package( OpenCV REQUIRED )

add_executable(test1 test.cpp)
target_link_libraries(test1 ${OpenCV_LIBS} )

add_executable(test2 test2.cpp)
target_link_libraries(test2 ${OpenCV_LIBS} )

add_executable(focus-contour focus-contour.cpp)
target_link_libraries(focus-contour ${OpenCV_LIBS} )

add_executable(canny-focus canny-focus.cpp)
target_link_libraries(canny-focus ${OpenCV_LIBS} )

add_executable(dilerosion dilerosion.cpp)
target_link_libraries(dilerosion ${OpenCV_LIBS} )

add_executable(ModifiedLap mlap.cpp)
target_link_libraries(ModifiedLap ${OpenCV_LIBS} )
