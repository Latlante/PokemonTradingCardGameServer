QT -= gui

CONFIG += c++11 console
CONFIG -= app_bundle

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

Debug:OBJECTS_DIR = debug/obj/
Debug:MOC_DIR = debug/moc/

Release:OBJECTS_DIR = release/obj/
Release:MOC_DIR = release/moc/

SOURCES += \
        main.cpp \
    common/constantesqml.cpp \
    common/database.cpp \
    common/utils.cpp \
    src_Actions/attacks/actionattacklessdamageonhimself.cpp \
    src_Actions/attacks/actionattackonlyifenemyissleeping.cpp \
    src_Actions/attacks/actionblockoneenemyattackforoneturn.cpp \
    src_Actions/attacks/actionchangeenemystatus.cpp \
    src_Actions/attacks/actionchangeenemystatusrandom.cpp \
    src_Actions/attacks/actionchangeresistanceofhimself.cpp \
    src_Actions/attacks/actionchangeweaknessofenemy.cpp \
    src_Actions/attacks/actioncompleteprotectionbypayingoneenergy.cpp \
    src_Actions/attacks/actioncompleteprotectionrandom.cpp \
    src_Actions/attacks/actiondamagemultipliedbyheadortail.cpp \
    src_Actions/attacks/actiondamageofhalfenemylifeleft.cpp \
    src_Actions/attacks/actiondieandhurteverypokemononeachbench.cpp \
    src_Actions/attacks/actionenemycanattackonnextturn_random.cpp \
    src_Actions/attacks/actionenemydieifyoudieinnextturn.cpp \
    src_Actions/attacks/actionenemypoisoned.cpp \
    src_Actions/attacks/actionhealing.cpp \
    src_Actions/attacks/actionhurteverypokemononownbench.cpp \
    src_Actions/attacks/actionhurthimself.cpp \
    src_Actions/attacks/actionmoredamagebyenemydamage.cpp \
    src_Actions/attacks/actionmoredamagebyenergy.cpp \
    src_Actions/attacks/actionmoredamagebyenergyonenemy.cpp \
    src_Actions/attacks/actionmoredamagebyowndamage.cpp \
    src_Actions/attacks/actionmoredamageorhurthimself.cpp \
    src_Actions/attacks/actionnodamageonthreshold.cpp \
    src_Actions/attacks/actionprotectedagainstdamage.cpp \
    src_Actions/attacks/actionprotectedagainstdamagerandom.cpp \
    src_Actions/attacks/actionremoveenergyattached.cpp \
    src_Actions/attacks/actionremoveoneenergyonenemy.cpp \
    src_Actions/attacks/actionreplicatelastdamagetoenemy.cpp \
    src_Actions/attacks/actionreplicateoneattackfromenemy.cpp \
    src_Actions/attacks/actionswappokemonbetweenfigthandbench.cpp \
    src_Actions/attacks/actionuniqueattack_random.cpp \
    src_Actions/trainers/actiontrainer_fakeprofessorchen.cpp \
    src_Actions/trainers/actiontrainer_informaticsearch.cpp \
    src_Actions/trainers/actiontrainer_leo.cpp \
    src_Actions/trainers/actiontrainer_littlegirl.cpp \
    src_Actions/trainers/actiontrainer_pokemoncenter.cpp \
    src_Actions/trainers/actiontrainer_potion.cpp \
    src_Actions/trainers/actiontrainer_professorchen.cpp \
    src_Actions/trainers/actiontrainer_removeenergy.cpp \
    src_Actions/trainers/actiontrainer_restored.cpp \
    src_Actions/trainers/actiontrainer_saveenergies.cpp \
    src_Actions/trainers/actiontrainer_superpotion.cpp \
    src_Actions/trainers/actiontrainer_totalguerison.cpp \
    src_Actions/abstractaction.cpp \
    src_Actions/actioncreationfactory.cpp \
    src_Cards/abstractcard.cpp \
    src_Cards/cardaction.cpp \
    src_Cards/cardenergy.cpp \
    src_Cards/cardpokemon.cpp \
    src_Models/modellistenergies.cpp \
    src_Packets/abstractpacket.cpp \
    src_Packets/abstractpacketdynamic.cpp \
    src_Packets/abstractpacketstatic.cpp \
    src_Packets/bencharea.cpp \
    src_Packets/fightarea.cpp \
    src_Packets/packetdeck.cpp \
    src_Packets/packetdynamiccustom.cpp \
    src_Packets/packethand.cpp \
    src_Packets/packetrewards.cpp \
    src_Packets/packettrash.cpp \
    gamemanager.cpp \
    player.cpp \
    src_Communication/stdlistenerwritter.cpp \
    controller.cpp \
    src_Log/log.cpp \
    src_Log/historicalnotifications.cpp \
    src_Log/abstractnotification.cpp \
    src_Log/notificationplayerisready.cpp \
    src_Log/notificationendofturn.cpp \
    src_Log/notificationcardmoved.cpp \
    src_Log/notificationdatapokemonchanged.cpp \
    src_Log/notificationpokemonswitched.cpp \
    src_Log/notificationenergyadded.cpp \
    src_Log/notificationenergyremoved.cpp \
    ../Share/constantesshared.cpp \
    src_Log/notificationdisplaypackets.cpp

HEADERS += \
    common/constantesqml.h \
    common/database.h \
    common/utils.h \
    src_Actions/attacks/actionattacklessdamageonhimself.h \
    src_Actions/attacks/actionattackonlyifenemyissleeping.h \
    src_Actions/attacks/actionblockoneenemyattackforoneturn.h \
    src_Actions/attacks/actionchangeenemystatus.h \
    src_Actions/attacks/actionchangeenemystatusrandom.h \
    src_Actions/attacks/actionchangeresistanceofhimself.h \
    src_Actions/attacks/actionchangeweaknessofenemy.h \
    src_Actions/attacks/actioncompleteprotectionbypayingoneenergy.h \
    src_Actions/attacks/actioncompleteprotectionrandom.h \
    src_Actions/attacks/actiondamagemultipliedbyheadortail.h \
    src_Actions/attacks/actiondamageofhalfenemylifeleft.h \
    src_Actions/attacks/actiondieandhurteverypokemononeachbench.h \
    src_Actions/attacks/actionenemycanattackonnextturn_random.h \
    src_Actions/attacks/actionenemydieifyoudieinnextturn.h \
    src_Actions/attacks/actionenemypoisoned.h \
    src_Actions/attacks/actionhealing.h \
    src_Actions/attacks/actionhurteverypokemononownbench.h \
    src_Actions/attacks/actionhurthimself.h \
    src_Actions/attacks/actionmoredamagebyenemydamage.h \
    src_Actions/attacks/actionmoredamagebyenergy.h \
    src_Actions/attacks/actionmoredamagebyenergyonenemy.h \
    src_Actions/attacks/actionmoredamagebyowndamage.h \
    src_Actions/attacks/actionmoredamageorhurthimself.h \
    src_Actions/attacks/actionnodamageonthreshold.h \
    src_Actions/attacks/actionprotectedagainstdamage.h \
    src_Actions/attacks/actionprotectedagainstdamagerandom.h \
    src_Actions/attacks/actionremoveenergyattached.h \
    src_Actions/attacks/actionremoveoneenergyonenemy.h \
    src_Actions/attacks/actionreplicatelastdamagetoenemy.h \
    src_Actions/attacks/actionreplicateoneattackfromenemy.h \
    src_Actions/attacks/actionswappokemonbetweenfigthandbench.h \
    src_Actions/attacks/actionuniqueattack_random.h \
    src_Actions/trainers/actiontrainer_fakeprofessorchen.h \
    src_Actions/trainers/actiontrainer_informaticsearch.h \
    src_Actions/trainers/actiontrainer_leo.h \
    src_Actions/trainers/actiontrainer_littlegirl.h \
    src_Actions/trainers/actiontrainer_pokemoncenter.h \
    src_Actions/trainers/actiontrainer_potion.h \
    src_Actions/trainers/actiontrainer_professorchen.h \
    src_Actions/trainers/actiontrainer_removeenergy.h \
    src_Actions/trainers/actiontrainer_restored.h \
    src_Actions/trainers/actiontrainer_saveenergies.h \
    src_Actions/trainers/actiontrainer_superpotion.h \
    src_Actions/trainers/actiontrainer_totalguerison.h \
    src_Actions/abstractaction.h \
    src_Actions/actioncreationfactory.h \
    src_Cards/abstractcard.h \
    src_Cards/cardaction.h \
    src_Cards/cardenergy.h \
    src_Cards/cardpokemon.h \
    src_Models/modellistenergies.h \
    src_Packets/abstractpacket.h \
    src_Packets/abstractpacketdynamic.h \
    src_Packets/abstractpacketstatic.h \
    src_Packets/bencharea.h \
    src_Packets/fightarea.h \
    src_Packets/packetdeck.h \
    src_Packets/packetdynamiccustom.h \
    src_Packets/packethand.h \
    src_Packets/packetrewards.h \
    src_Packets/packettrash.h \
    gamemanager.h \
    player.h \
    src_Communication/stdlistenerwritter.h \
    controller.h \
    src_Log/log.h \
    src_Log/historicalnotifications.h \
    src_Log/abstractnotification.h \
    src_Log/notificationplayerisready.h \
    src_Log/notificationendofturn.h \
    src_Log/notificationcardmoved.h \
    src_Log/notificationdatapokemonchanged.h \
    src_Log/notificationpokemonswitched.h \
    src_Log/notificationenergyadded.h \
    src_Log/notificationenergyremoved.h \
    ../Share/constantesshared.h \
    src_Log/notificationdisplaypackets.h
