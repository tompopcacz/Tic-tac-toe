# CMakeList.txt : CMake project for Tic-tac-toe, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (Tic-tac-toe "Tic-tac-toe.cpp" "Tic-tac-toe.h" "TicTacToeBoard.h" "TicTacToeBoard.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Tic-tac-toe PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
