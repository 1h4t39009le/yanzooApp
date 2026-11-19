# Страницы роли подключают друг друга короткими путями ("Pages/...").
target_include_directories(${LIBRARY_NAME} PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/Keeper
	${CMAKE_CURRENT_SOURCE_DIR}/Vet
)

target_sources(${LIBRARY_NAME} PRIVATE
##Keeper
	#MainWidget
	Keeper/MainWidget.hpp
	Keeper/MainWidget.cpp
	Keeper/MainWidget.ui
	#KeeperData (UI-представление данных роли)
	Keeper/KeeperData.hpp
	#Pages/EnclosurePage
	Keeper/Pages/EnclosurePage.hpp
	Keeper/Pages/EnclosurePage.cpp
	Keeper/Pages/EnclosurePage.ui
	#Pages/AnimalsPage
	Keeper/Pages/AnimalsPage.hpp
	Keeper/Pages/AnimalsPage.cpp
	Keeper/Pages/AnimalsPage.ui
	#Pages/FeedingPage
	Keeper/Pages/FeedingPage.hpp
	Keeper/Pages/FeedingPage.cpp
	Keeper/Pages/FeedingPage.ui
##Vet
	#MainWidget
	Vet/MainWidget.hpp
	Vet/MainWidget.cpp
	Vet/VetMainWidget.ui
	#VetData (UI-представление данных роли)
	Vet/VetData.hpp
	#Pages/EventsPage
	Vet/Pages/EventsPage.hpp
	Vet/Pages/EventsPage.cpp
	Vet/Pages/EventsPage.ui
)
