add_library(coreUserSessionLib STATIC
    UserSession.cpp
    UserSession.hpp
    BaseDbController.cpp
    BaseDbController.hpp
    UserKeeperView.cpp
    UserKeeperView.hpp
)

target_link_libraries(coreUserSessionLib PUBLIC
    Qt6::Core 
    Qt6::Sql
)