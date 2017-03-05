#include "thirdTabDataManager.h"
#include <QQueue>

ThirdTabDataManager::ThirdTabDataManager(BusMap *map):
     currentStartStop(NULL), currentEndStop(NULL), map(map)
{
    this->filteredStartStopList = map->getStopList();
    this->filteredEndStopList = this->filteredStartStopList;
}

QStringList ThirdTabDataManager::startStopList() {
    QStringList list;
    StopList values = this->filteredStartStopList;
    foreach(Stop *stop, values) {
        list<<stop->name;
    }
    return list;
}

QStringList ThirdTabDataManager::endStopList() {
    QStringList list;
    StopList values = this->filteredEndStopList;
    foreach(Stop *stop, values) {
        list<<stop->name;
    }
    return list;
}

void ThirdTabDataManager::startStopSearchTextChangedTo(QString text) {
    this->filteredStartStopList.clear();
    StopList list = this->map->getStopList();
    foreach(Stop *stop, list) {
        if(stop->name.contains(text)) {
            this->filteredStartStopList<<stop;
        }
    }
}

void ThirdTabDataManager::endStopSearchTextChangedTo(QString text) {
    this->filteredEndStopList.clear();
    StopList list = this->map->getStopList();
    foreach(Stop *stop, list) {
        if(stop->name.contains(text)) {
            this->filteredEndStopList<<stop;
        }
    }
}

Stop* ThirdTabDataManager::getStartStop(int index) {
    return this->filteredStartStopList.at(index);
}

Stop* ThirdTabDataManager::getEndStop(int index) {
    return this->filteredEndStopList.at(index);
}

int distance(Stop *a, Stop *b, Busline *line) {
    int indexA = line->stops.indexOf(a);
    int indexB = line->stops.indexOf(b);
    return abs(indexA - indexB);
}

QList<QueryResult*> ThirdTabDataManager::queryBusline() {
    Stop *start = this->currentStartStop;
    Stop *end = this->currentEndStop;

    BuslineList startBuslines = start->buslines;
    int count = this->map->buslines.count();
    bool *lineVisited = new bool[count];
    memset(lineVisited, 0, sizeof(bool) * count);

    QList<QueryResult *> resultList;

    bool flagOfLevel1 = false;
    bool flagOfLevel2 = false;
    bool flagOfLevel3 = false;

    foreach(Busline *level_1_line, startBuslines) {
        // 经过起点站的每一条公交线路
        lineVisited[this->map->buslines.indexOf(level_1_line)] = true;
        foreach(Stop *level_1_stop, level_1_line->stops) {
            // 这条公交线路的每一站
            if(level_1_stop == end) {
                QueryResult *result = new QueryResult;
                result->transStops<<level_1_stop;
                result->transLines<<level_1_line;
                resultList<<result;
                flagOfLevel1 = true;
            }
        }

        foreach(Stop *level_1_stop, level_1_line->stops) {
            if(level_1_stop != end) {
                foreach(Busline *level_2_line, level_1_stop->buslines) {
                    // 已换乘一次
                    if(lineVisited[this->map->buslines.indexOf(level_2_line)])
                        continue;
                    lineVisited[this->map->buslines.indexOf(level_2_line)] = true;
                    foreach(Stop *level_2_stop, level_2_line->stops){
                        if (level_2_stop == end) {
                            QueryResult *result = new QueryResult;
                            result->transStops<<level_1_stop<<level_2_stop;
                            result->transLines<<level_1_line<<level_2_line;
                            resultList<<result;
                            flagOfLevel2 = true;
                        }
                    }

                    foreach(Stop *level_2_stop, level_2_line->stops){
                        if (level_2_stop == end && (!flagOfLevel2 || !flagOfLevel1)) {
                            foreach(Busline *level_3_line, level_2_stop->buslines){
                                // 换乘两次
                                if(lineVisited[this->map->buslines.indexOf(level_3_line)])
                                    continue;
                                lineVisited[this->map->buslines.indexOf(level_3_line)] = true;
                                foreach(Stop *level_3_stop, level_3_line->stops) {
                                    if(level_3_stop == end) {
                                        QueryResult *result = new QueryResult;
                                        result->transStops<<level_1_stop<<level_2_stop<<level_3_stop;
                                        result->transLines<<level_1_line<<level_2_line<<level_3_line;
                                        resultList<<result;
                                        flagOfLevel3 = true;
                                    }
                                }
                                foreach(Stop *level_3_stop, level_3_line->stops) {
                                  if(level_3_stop != end && (!flagOfLevel3 || !flagOfLevel2 || !flagOfLevel1)){
                                        foreach(Busline *level_4_line, level_3_stop->buslines){
                                            // 换乘两次
                                            if(lineVisited[this->map->buslines.indexOf(level_4_line)])
                                                // 该条路线已遍历过
                                                continue;
                                            lineVisited[this->map->buslines.indexOf(level_4_line)] = true;
                                            foreach(Stop *level_4_stop, level_4_line->stops) {
                                                if(level_4_stop == end){
                                                    QueryResult *result = new QueryResult;
                                                    result->transStops<<level_1_stop<<level_2_stop<<level_3_stop<<level_4_stop;
                                                    result->transLines<<level_1_line<<level_2_line<<level_3_line<<level_4_line;
                                                    resultList<<result;
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    // 过滤
    foreach(QueryResult *result, resultList) {
        if(flagOfLevel1) {
            if(result->transStops.count() > 2)
                resultList.removeOne(result);
        }else if(flagOfLevel2) {
            if(result->transStops.count() > 2)
                resultList.removeOne(result);
        }else if(flagOfLevel3) {
            if(result->transStops.count() > 3)
                resultList.removeOne(result);
        }
    }

    // 过滤
    QMap<QString, Busline*> status;

    foreach(QueryResult *result, resultList) {
        if(result->transLines.count() == 1){
            if (status.contains(result->transLines[0]->busNumber)) {
                resultList.removeOne(result);
            }else{
                status[result->transLines[0]->busNumber] = result->transLines[0];
            }
        }
    }

    foreach(QueryResult *result, resultList) {
        if(result->transLines.count() == 2) {
            if(result->transLines[0]->busNumber == result->transLines[1]->busNumber){
                if (status.contains(result->transLines[0]->busNumber)) {
                    resultList.removeOne(result);
                }else{
                    status[result->transLines[0]->busNumber] = result->transLines[0];
                }
            }
        }
    }

    foreach(QueryResult *result, resultList) {
        if(result->transLines.count() >= 2){
            status.clear();
            foreach(Busline *line, result->transLines){
                if(status.contains(line->busNumber)){
                    int index = result->transLines.indexOf(status[line->busNumber]);
                    result->transLines.removeOne(status[line->busNumber]);
                    result->transStops.removeAt(index);
                    status[line->busNumber] = line;
                }else{
                    status[line->busNumber] = line;
                }
            }
        }
    }

    QMap<QString, bool> lineStatus;
    foreach(QueryResult *result, resultList) {
        if(result->transLines.count() >= 2){
            QString key;
            foreach (Busline *line, result->transLines) {
               key += line->busNumber;
            }
            if(lineStatus.contains(key)){
                resultList.removeOne(result);
            }else{
                lineStatus[key] = true;
            }
        }
    }

    QMap<QString, Stop*> stopStatus;
    foreach(QueryResult *result, resultList) {
        if(result->transLines.count() >= 2){
            stopStatus.clear();
            foreach(Stop *stop, result->transStops){
                if(status.contains(stop->name)){
                    int index = result->transStops.indexOf(stopStatus[stop->name]);
                    result->transStops.removeOne(stopStatus[stop->name]);
                    result->transLines.removeAt(index);
                    stopStatus[stop->name] = stop;
                }else{
                    stopStatus[stop->name] = stop;
                }
            }
        }
    }

    // 计算站点数
    Stop *stop , *nextStop;
    foreach(QueryResult *result, resultList) {
        result->totalStopCount = 0;
        stop = start;
        for(int i = 0; i<result->transLines.count(); ++i) {
            nextStop = result->transStops[i];
            int dist = distance(stop, nextStop, result->transLines[i]);
            result->totalStopCount += dist;
            stop = nextStop;
        }
    }

    return resultList;

}

