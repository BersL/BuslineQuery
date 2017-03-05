#include "firstTabDataManager.h"

FirstTabDataManager::FirstTabDataManager(BusMap *map): map(map) {
    this->allBuslines = this->currentBuslines = map->buslines;
    this->currentStops = map->getStopList();
}

QStringList FirstTabDataManager::currentBusLineList() {
    QStringList list;
    foreach(Busline *line, this->currentBuslines) {
        list<<line->name;
    }
    return list;
}

QStringList FirstTabDataManager::currentStopList() {
    QStringList list;
    foreach(Stop *stop, this->currentStops) {
        list<<stop->name;
    }
    return list;
}

void FirstTabDataManager::searchTextChangedTo(const QString text) {
    this->currentBuslines.clear();
    BuslineList list = this->allBuslines;
    foreach(Busline *line, list) {
        if (line->name.contains(text)) {
            this->currentBuslines<<line;
        }
    }
}

void FirstTabDataManager::stopSearchTextChangedTo(const QString text) {
    this->currentStops.clear();
    StopList list = this->map->getStopList();
    foreach(Stop *stop, list) {
        if (stop->name.contains(text)) {
            this->currentStops<<stop;
        }
    }
}

void FirstTabDataManager::stopSelected(Stop *stop) {
    this->currentBuslines.clear();
    this->allBuslines = this->currentBuslines = stop->buslines;
}

void FirstTabDataManager::resetAllBuslines() {
    this->allBuslines = this->currentBuslines = map->buslines;
}
