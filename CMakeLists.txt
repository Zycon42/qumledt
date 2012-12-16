project(ITU_Proj)
cmake_minimum_required(VERSION 2.6)
find_package(Qt4 REQUIRED)

include(${QT_USE_FILE})

include_directories(${CMAKE_CURRENT_BINARY_DIR})

# add compile definitions
add_definitions(-Wall -pedantic -std=c++98 -g)

# sources
set(SRCS
    src/diagramview.cpp
    src/mainwindow.cpp
    src/main.cpp
    src/diagramscene.cpp
    src/diagramitem.cpp
    src/entity.cpp
    src/classentity.cpp
    src/interfaceentity.cpp
    src/association.cpp
    src/biassociation.cpp
    src/roletextitem.cpp
    src/uniassociation.cpp
    src/generalization.cpp
    src/aggregation.cpp
    src/composition.cpp
    src/modes.cpp
    src/dialogs/basedialogs.cpp
    src/dialogs/classpropertiesdialog.cpp
    src/dialogs/interfacepropertiesdialog.cpp
    src/dialogs/biassociationdialog.cpp
    src/dialogs/uniassociationdialog.cpp
    src/dialogs/attributemodel.cpp
    src/dialogs/attributedialog.cpp
    src/dialogs/operationmodel.cpp
    src/dialogs/operationdialog.cpp
    src/dialogs/parametermodel.cpp
)

# headers for moc
set(HDRS
    src/diagramview.h
    src/mainwindow.h
    src/diagramscene.h
    src/diagramitem.h
    src/entity.h
    src/classentity.h
    src/interfaceentity.h
    src/association.h
    src/biassociation.h
    src/uniassociation.h
    src/generalization.h
    src/aggregation.h
    src/composition.h
    src/dialogs/basedialogs.h
    src/dialogs/classpropertiesdialog.h
    src/dialogs/interfacepropertiesdialog.h
    src/dialogs/biassociationdialog.h
    src/dialogs/uniassociationdialog.h
    src/dialogs/attributemodel.h
    src/dialogs/attributedialog.h
    src/dialogs/operationmodel.h
    src/dialogs/operationdialog.h
    src/dialogs/parametermodel.h
)

# ui files
set(UIS
    ui/mainwindow.ui
    ui/classdialog.ui
    ui/attributedialog.ui
    ui/operationdialog.ui
    ui/interfacedialog.ui
    ui/biassociationdialog.ui
    ui/uniassociationdialog.ui
)

set(RESOURCES res.qrc)

# run moc
qt4_wrap_cpp(MOC_HDRS ${HDRS})
# run uic
qt4_wrap_ui(UI_HDRS ${UIS})
# run rcc
qt4_add_resources(RESOURCES_RCC ${RESOURCES})

add_executable(ITU_Proj ${SRCS} ${MOC_HDRS} ${UI_HDRS} ${RESOURCES_RCC})

target_link_libraries(ITU_Proj ${QT_LIBRARIES})
