add_library(coreUserSessionLib STATIC
    UserSession.cpp
    UserSession.hpp
    DbAccessController.cpp
    DbAccessController.hpp
    RoleAccessView.hpp
    KeeperAccessView.cpp
    KeeperAccessView.hpp
    VetAccessView.cpp
    VetAccessView.hpp
)

target_link_libraries(coreUserSessionLib PUBLIC
    Qt6::Core 
    Qt6::Sql
)
