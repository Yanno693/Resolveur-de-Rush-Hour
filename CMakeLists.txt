cmake_minimum_required(VERSION 3.0)
set(CMAKE_SUPPRESS_REGENERATION true)

project(rushhour)

set(SRCS
	rushhour.cpp
	voiture.cpp
	grabator.cpp
	grille.cpp
	jeu.cpp
	mouvement.cpp
	case.cpp
	)
	
set(HEADERS
	voiture.hpp
	grabator.hpp
	grille.hpp
	jeu.hpp
	mouvement.hpp
	case.hpp
	enum.hpp)

set(SRCSG
	rushhour_graphique.cpp
	voiture.cpp
	grabator.cpp
	grille.cpp
	jeu.cpp
	mouvement.cpp
	case.cpp
	)
	
set(HEADERSG
	voiture.hpp
	grabator.hpp
	grille.hpp
	jeu.hpp
	mouvement.hpp
	enum.hpp
	case.hpp
	olcPixelGameEngine.h)

add_executable(rushhour ${SRCS} ${HEADERS})
add_executable(rushhour_graphique ${SRCSG} ${HEADERSG})
target_link_libraries(rushhour_graphique -lX11)
target_link_libraries(rushhour_graphique -lpng)
target_link_libraries(rushhour_graphique -lGL)
target_link_libraries(rushhour_graphique -lpthread)