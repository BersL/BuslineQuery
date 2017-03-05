#include "buslinedetailview.h"
#include "ui_buslinedetailview.h"
#include <QPainter>

BuslineDetailView::BuslineDetailView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BuslineDetailView)
{
    ui->setupUi(this);
    this->currentBusline = NULL;
}

BuslineDetailView::~BuslineDetailView()
{
    delete ui;
}

void BuslineDetailView::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    QPen pen(QColor(51, 133, 255)), whitePen(QColor(255, 255, 255)), blackPen(QColor(0, 0, 0));
    QBrush brush(QColor(51, 133, 255)), whiteBrush(QColor(255, 255, 255)), blackBrush(QColor(0, 0, 0));

    foreach(QRect rect, this->bkgRect){
        painter.setPen(pen);
        painter.setBrush(brush);
        painter.drawRoundedRect(rect, 7, 7);
        painter.setPen(whitePen);
        painter.setBrush(whiteBrush);
        painter.drawRect(rect.left() + 6, rect.top() + 18.5, rect.width() - 12, rect.height() - 37);
    }

    painter.setPen(whitePen);
    painter.setBrush(whiteBrush);

    if(this->currentBusline != NULL) {
        int count = this->currentBusline->stops.count();
        double semiWidth = this->size().width() / 2;
        double space = (this->contentHeight) / 20;
        double centerX = 30 + 4;
        double initY = this->bkgRect.first().width() / 2 + space / 2 - 1;
        for(int i = 0; i < count; ++i) {
            painter.drawEllipse(centerX + (i/20)*semiWidth, initY + (i%20)*space, 6, 6);
        }
        painter.setPen(blackPen);
        painter.setBrush(blackBrush);

        for(int i = 0; i < count; ++i) {
            QRect rect(4 + (i/20)*semiWidth, initY - 5 + (i%20)*space, 20, 16);
            painter.drawText(rect, Qt::AlignHCenter, QString::number(i+1, 10));
            QRect stopRect(centerX + 15 + (i/20)*semiWidth, rect.top() - 1, semiWidth - centerX - 20, 18);
            painter.drawText(stopRect, Qt::AlignLeft, this->currentBusline->stops[i]->name);
        }
    }

}

void BuslineDetailView::drawBusline(Busline *line) {
    this->currentBusline = line;
    this->bkgRect.clear();
    int count = line->stops.count();
    int comp = count / 20;
    int incomp = count % 20;
    double height = this->size().height() - 4, width = this->size().width() ,rectWidth = 14;
    this->contentHeight = height - rectWidth;
    int i = 0;
    for(; i < comp; ++i) {
        QRect rect(30 +  i * (width / 2), 2, rectWidth, height);
        this->bkgRect<<rect;
    }
    if(incomp != 0){
        QRect rect(30 + i*(width / 2), 2, rectWidth, incomp * (contentHeight / 20) + rectWidth);
        this->bkgRect<<rect;
    }
    this->update();
}
