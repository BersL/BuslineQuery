#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "buslineInfoDialog.h"
#include <QPainter>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    thirdLoaded(false)
{
    ui->setupUi(this);
    this->map = new BusMap("/Users/Bers/Desktop/DataStructure/BuslineQuery/data.txt", "/Users/Bers/Desktop/DataStructure/BuslineQuery/busstops.txt");
    this->firstManager = new FirstTabDataManager(this->map);
    this->thirdManager = new ThirdTabDataManager(this->map);
    this->refreshBusLineList();
    this->refreshAllStopList();
}

void MainWindow::refreshBusLineList(){
    ui->allBusLinesList->clear();
    QStringList list = this->firstManager->currentBusLineList();
    foreach(QString name, list) {
        QListWidgetItem *item = new QListWidgetItem(name);
        ui->allBusLinesList->addItem(item);
    }
}

void MainWindow::refreshAllStopList() {
    ui->allStopList->clear();
    QStringList stops = this->firstManager->currentStopList();
    foreach(QString name, stops) {
        QListWidgetItem *item = new QListWidgetItem(name);
        ui->allStopList->addItem(item);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
    delete map;
}

void MainWindow::on_busLineSearchLineEdit_textChanged(const QString &text)
{
    this->firstManager->searchTextChangedTo(text);
    this->refreshBusLineList();
}

void MainWindow::on_allBusLinesList_doubleClicked(const QModelIndex &index)
{
    int row = index.row();
    Busline *line = this->firstManager->currentBuslines[row];
    this->updateBusDetailInfo(line);
    ui->buslineDetailView->drawBusline(line);
}

void MainWindow::updateBusDetailInfo(Busline *line) {
    QString timeInfo = line->firstTime.toString("hh:mm") + "-" + line->lastTime.toString("hh:mm");
    ui->currentLineInfo->setText(line->startStop + "->" + line->endStop);
    ui->currentLineNumber->setText(line->busNumber);
    ui->currentLineTime->setText(timeInfo);
    ui->currentLineTickets->setText(line->ticket.section(',' , 0, 0));
    ui->buslineDetailView->drawBusline(line);
}


void MainWindow::on_addLineButton_clicked()
{
    BuslineInfoDialog *dialog = new BuslineInfoDialog;
    dialog->setWindowTitle("新增线路");
    dialog->setAllStops(this->map->stops);
    connect(dialog, SIGNAL(sendBuslineData(Busline*)), this, SLOT(receiveBuslineData(Busline*)));
    dialog->show();
}

void MainWindow::on_editLineButton_clicked()
{
    int index = ui->allBusLinesList->currentRow();
    if(index<0) return;
    Busline *line = this->firstManager->currentBuslines[index];

    BuslineInfoDialog *dialog = new BuslineInfoDialog;
    dialog->setWindowTitle("编辑线路");
    dialog->setAllStops(this->map->stops);
    dialog->setBusline(line);
    connect(dialog, SIGNAL(sendBuslineData(Busline*)), this, SLOT(receiveBuslineData(Busline*)));
    dialog->show();
}

void MainWindow::on_removeLineButton_clicked()
{
    int index = ui->allBusLinesList->currentRow();
    if(index<0) return;
    delete ui->allBusLinesList->takeItem(index);
    Busline *line = this->firstManager->currentBuslines[index];
    this->firstManager->currentBuslines.removeAt(index);
    this->map->buslines.removeOne(line);

}

void MainWindow::receiveBuslineData(Busline *line) {
    if(this->map->buslines.contains(line)) {
        QListWidgetItem *item = ui->allBusLinesList->currentItem();
        item->setText(line->name);
    }else{
        // New
        this->map->buslines<<line;
        ui->busLineSearchLineEdit->setText("");
        this->firstManager->searchTextChangedTo("");
        QListWidgetItem *item = new QListWidgetItem(line->name);
        ui->allBusLinesList->addItem(item);
        ui->allBusLinesList->scrollToBottom();
        ui->allBusLinesList->setCurrentRow(this->map->buslines.count() - 1);
    }
    this->updateBusDetailInfo(line);
}


void MainWindow::on_allStopList_doubleClicked(const QModelIndex &index)
{
    int row = index.row();
    Stop *stop = this->firstManager->currentStops[row];
    this->firstManager->stopSelected(stop);
    this->refreshBusLineList();
}

void MainWindow::on_allStopSearchLineEdit_textChanged(const QString &text)
{
    this->firstManager->stopSearchTextChangedTo(text);
    this->refreshAllStopList();
}

void MainWindow::on_showAllButton_clicked()
{
    this->firstManager->resetAllBuslines();
    this->refreshBusLineList();
}


// Tab 2

void MainWindow::on_tabWidget_currentChanged(int index)
{
    if(index == 1 && !this->thirdLoaded){
        this->refreshStartStopList();
        this->refreshEndStopList();
        this->thirdLoaded = true;
    }
}

void MainWindow::refreshStartStopList() {
    QStringList list = this->thirdManager->startStopList();
    ui->startStopList->clear();
    foreach(QString str, list) {
        QListWidgetItem *item = new QListWidgetItem(str);
        ui->startStopList->addItem(item);
    }
}

void MainWindow::refreshEndStopList() {
    QStringList list = this->thirdManager->endStopList();
    ui->endStopList->clear();
    foreach(QString str, list) {
        QListWidgetItem *item = new QListWidgetItem(str);
        ui->endStopList->addItem(item);
    }
}


void MainWindow::on_startStopSearchEdit_textChanged(const QString &text)
{
    this->thirdManager->startStopSearchTextChangedTo(text);
    this->refreshStartStopList();
}

void MainWindow::on_stopSearchEdit_textChanged(const QString &text)
{
    this->thirdManager->endStopSearchTextChangedTo(text);
    this->refreshEndStopList();
}

void MainWindow::on_startStopList_doubleClicked(const QModelIndex &index)
{
    Stop *stop = this->thirdManager->getStartStop(index.row());
    this->thirdManager->currentStartStop = stop;
    ui->startStopLabel->setText(stop->name);

}

void MainWindow::on_endStopList_doubleClicked(const QModelIndex &index)
{
    Stop *stop = this->thirdManager->getEndStop(index.row());
    this->thirdManager->currentEndStop = stop;
    ui->endStopLabel->setText(stop->name);
}

void MainWindow::on_swapStopsButton_clicked()
{
    if(this->thirdManager->currentStartStop == NULL || this->thirdManager->currentEndStop == NULL) return;
    Stop *stop = this->thirdManager->currentStartStop;
    this->thirdManager->currentStartStop = this->thirdManager->currentEndStop;
    this->thirdManager->currentEndStop = stop;

    ui->startStopLabel->setText(this->thirdManager->currentStartStop->name);
    ui->endStopLabel->setText(this->thirdManager->currentEndStop->name);
}

void MainWindow::on_confirmQueryButton_clicked()
{
    ui->queryPromptLabel->setText("查询中...");
    QList<QueryResult*> results = this->thirdManager->queryBusline();
    if(ui->transferRadioButton->isChecked()) {
        int min = 10;
        QList<QueryResult*> minResult;
        foreach(QueryResult *result, results) {
            if(result->transLines.count() < min) {
                minResult.clear();
                minResult<<result;
                min = result->transLines.count();
            }else if(result->transLines.count() == min) {
                minResult<<result;
            }
        }
        results = minResult;
    }else if(ui->lengthRadioButton->isChecked()) {
        int min = 100;
        QList<QueryResult*> minResult;
        foreach(QueryResult *result, results) {
            if(result->totalStopCount < min) {
                minResult.clear();
                minResult<<result;
                min = result->totalStopCount;
            }else if(result->totalStopCount == min) {
                minResult<<result;
            }
        }
        results = minResult;
    }
    ui->queryPromptLabel->setText("查询结果");
    ui->queryResultWidget->setQueryResult(this->thirdManager->currentStartStop, results);
}
