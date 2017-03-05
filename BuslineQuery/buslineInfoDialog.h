#ifndef BUSLINEINFODIALOG_H
#define BUSLINEINFODIALOG_H

#include <QDialog>
#include "busline.h"

namespace Ui {
class BuslineInfoDialog;
}

class BuslineInfoDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BuslineInfoDialog(QWidget *parent = 0);
    ~BuslineInfoDialog();
    void setAllStops(const QMap<QString, Stop*> stops);
    void setBusline(Busline *line);

signals:
    void sendBuslineData(Busline *line);

private slots:
    void on_buttonBox_accepted();

    void on_addFromListButton_clicked();

    void on_removeFromListButton_clicked();

    void on_searchEdit_textChanged(const QString &text);

private:
    Ui::BuslineInfoDialog *ui;
    Busline *lineToEdit;
    QMap<QString, Stop*> stops;
    StopList filteredStops;
    StopList addedStops;
    void updateAllStopsList();
    void updateAddedStopsList(int updatedIndex, bool updateAll = true);
};

#endif // BUSLINEINFODIALOG_H
