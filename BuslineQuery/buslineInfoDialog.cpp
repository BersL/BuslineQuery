#include "buslineInfoDialog.h"
#include "ui_buslineInfoDialog.h"

BuslineInfoDialog::BuslineInfoDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BuslineInfoDialog)
{
    ui->setupUi(this);
    ui->buttonBox->buttons().at(0)->setText("确定");
    ui->buttonBox->buttons().at(1)->setText("取消");
    this->lineToEdit = NULL;
}

BuslineInfoDialog::~BuslineInfoDialog()
{
    delete ui;
}

void BuslineInfoDialog::on_buttonBox_accepted()
{
    QDialog::accept();
    Busline *line;
    if(this->lineToEdit != NULL) {
        line = this->lineToEdit;
        line->stops.clear();
    }else{
        line = new Busline;
    }
    line->busNumber = ui->lineNumber->text();
    line->firstTime = ui->firstTimeEidt->time();
    line->lastTime = ui->lastTimeEdit->time();
    line->ticket = ui->lineTicket->text();
    for(int i = 0; i<ui->addedStopsList->count(); ++i) {
        QListWidgetItem *item = ui->addedStopsList->item(i);
        Stop *stop = this->stops[item->text()];
        line->stops<<stop;
    }
    if (line->stops.count() > 0) {
        line->startStop = line->stops[0]->name;
        line->endStop = line->stops.last()->name;
    }
    line->name = line->busNumber + "-(" + line->startStop + "-" + line->endStop + ") ";
    emit this->sendBuslineData(line);
}

void BuslineInfoDialog::setAllStops(const QMap<QString, Stop*> stops) {
    this->stops = stops;
    StopList list = stops.values();
    std::reverse(list.begin(), list.end());
    this->filteredStops = list;

    foreach(Stop *stop, this->filteredStops) {
        QListWidgetItem *item = new QListWidgetItem(stop->name);
        ui->allStopsList->addItem(item);
    }
}

void BuslineInfoDialog::setBusline(Busline *line) {
    this->lineToEdit = line;
    this->addedStops = line->stops;
    foreach(Stop *stop, line->stops) {
        QListWidgetItem *item = new QListWidgetItem(stop->name);
        ui->addedStopsList->addItem(item);
    }

    ui->lineNumber->setText(line->busNumber);
    ui->firstTimeEidt->setTime(line->firstTime);
    ui->lastTimeEdit->setTime(line->lastTime);
    ui->lineTicket->setText(line->ticket);

}

void BuslineInfoDialog::on_addFromListButton_clicked()
{
    int index = ui->allStopsList->currentRow();
    int addedSelection = ui->addedStopsList->currentIndex().row();
    Stop *stop = this->filteredStops.at(index);
    if(addedSelection != -1) {
        if(!(this->addedStops.contains(stop))){
            this->addedStops.insert(addedSelection, stop);
            this->updateAddedStopsList(addedSelection, false);
        }
    }else{
        if(!(this->addedStops.contains(stop))){
            this->addedStops<<stop;
            this->updateAddedStopsList(this->addedStops.count() - 1, false);
        }
    }
}

void BuslineInfoDialog::on_removeFromListButton_clicked()
{
    int index = ui->addedStopsList->currentRow();
    this->addedStops.removeAt(index);
    delete ui->addedStopsList->takeItem(index);
}

void BuslineInfoDialog::updateAllStopsList() {
    ui->allStopsList->clear();
    foreach(Stop *stop, this->filteredStops) {
        QListWidgetItem *item = new QListWidgetItem(stop->name);
        ui->allStopsList->addItem(item);
    }
}

void BuslineInfoDialog::updateAddedStopsList(int updatedIndex, bool updateAll) {
    if(!updateAll) {
        Stop *stop = this->addedStops[updatedIndex];
        QListWidgetItem *item = new QListWidgetItem(stop->name);
        ui->addedStopsList->insertItem(updatedIndex + 1, item);
        ui->addedStopsList->scrollToItem(item);
        ui->addedStopsList->setCurrentRow(updatedIndex + 1);
        return;
    }
    ui->addedStopsList->clear();
    foreach(Stop *stop, this->addedStops) {
        QListWidgetItem *item = new QListWidgetItem(stop->name);
        ui->addedStopsList->addItem(item);
    }
}

void BuslineInfoDialog::on_searchEdit_textChanged(const QString &text)
{
    this->filteredStops.clear();
    foreach(Stop *stop, this->stops.values())
        if(stop->name.contains(text))
            this->filteredStops<<stop;
    this->updateAllStopsList();
}
