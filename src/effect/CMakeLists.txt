ADD_DEFINITIONS(-DFixPermedia=1)

INCLUDE_DIRECTORIES(../Inc ../zlib ../engine  ../soundengine)

SET(effect_SRC
    alephgeo.cpp
    combopane.cpp
    conegeo.cpp
    debrisgeo.cpp
    efapp.cpp
    efgeo.cpp
    efimage.cpp
    efpane.cpp
    efpopup.cpp
    efwindow.cpp
    explosion.cpp
    framedata.cpp
    gamestate.cpp
    help.cpp
    listpane.cpp
    mmlpopup.cpp
    navpane.cpp
    pagepane.cpp
    particlegeo.cpp
    starimage.cpp
    thinggeo.cpp
   )

ADD_LIBRARY(effect ${effect_SRC})
SET_PROPERTY(TARGET effect PROPERTY CXX_STANDARD 14)
