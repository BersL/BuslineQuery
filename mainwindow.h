#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "busline.h"
#include "firstTabDataManager.h"
#include "thirdTabDataManager.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void refreshBusLineList();
    void refreshAllStopList();
    void updateBusDetailInfo(Busline *line);

    void refreshStartStopList();
    void refreshEndStopList();

private slots:
    void on_busLineSearchLineEdit_textChanged(const QString &text);

    void on_allBusLinesList_doubleClicked(const QModelIndex &index);

    void on_addLineButton_clicked();

    void on_editLineButton_clicked();

    void on_removeLineButton_clicked();

    void receiveBuslineData(Busline *line);

    void on_tabWidget_currentChanged(int index);

    void on_startStopSearchEdit_textChanged(const QString &text);

    void on_stopSearchEdit_textChanged(const QString &text);

    void on_startStopList_doubleClicked(const QModelIndex &index);

    void on_endStopList_doubleClicked(const QModelIndex &index);

    void on_swapStopsButton_clicked();

    void on_confirmQueryButton_clicked();


    void on_allStopList_doubleClicked(const QModelIndex &index);

    void on_allStopSearchLineEdit_textChanged(const QString &text);

    void on_showAllButton_clicked();

private:
    Ui::MainWindow *ui;
    BusMap *map;
    FirstTabDataManager *firstManager;
    ThirdTabDataManager *thirdManager;
    bool thirdLoaded;
//    void paintEvent(QPaintEvent*);
};

#endif // MAINWINDOW_H
