

INCLUDE_DIRECTORIES(../Inc ../zlib ../engine  ../soundengine ../guids)

SET(Igc_SRC
    afterburnerIGC.cpp
    asteroidigc.cpp
    bucketigc.cpp
    buoyigc.cpp
    chaffigc.cpp
    chafftypeigc.cpp
    civilizationigc.cpp
    cloakigc.cpp
    clusterIGC.cpp
    common.cpp
    developmentigc.cpp
    dispenserigc.cpp
    dronetypeigc.cpp
    hullTypeIGC.cpp
    launchertypeigc.cpp
    magazineigc.cpp
    mapmakerigc.cpp
    mineigc.cpp
    minetypeigc.cpp
    missileigc.cpp
    missiletypeigc.cpp
    missionigc.cpp
    modelIGC.cpp
    packigc.cpp
    partTypeIGC.cpp
    probeigc.cpp
    probetypeigc.cpp
    projectileIGC.cpp
    projectileTypeIGC.cpp
    shieldIGC.cpp
    shipIGC.cpp
    sideigc.cpp
    stationIGC.cpp
    stationtypeigc.cpp
    treasureIGC.cpp
    treasuresetigc.cpp
    warpIGC.cpp
    weaponIGC.cpp
   )

ADD_LIBRARY(Igc ${Igc_SRC})
SET_PROPERTY(TARGET Igc PROPERTY CXX_STANDARD 14)
