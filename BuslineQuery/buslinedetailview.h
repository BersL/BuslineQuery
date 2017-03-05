#ifndef BUSLINEDETAILVIEW_H
#define BUSLINEDETAILVIEW_H

#include <QWidget>
#include <QList>
#include "busline.h"
namespace Ui {
class BuslineDetailView;
}

class BuslineDetailView : public QWidget
{
    Q_OBJECT

public:
    explicit BuslineDetailView(QWidget *parent = 0);
    ~BuslineDetailView();
    void drawBusline(Busline *line);

private:
    Ui::BuslineDetailView *ui;
    Busline *currentBusline;
    QList<QRect> bkgRect;
    double contentHeight;

protected:
    void paintEvent(QPaintEvent *);
};

#endif // BUSLINEDETAILVIEW_H
