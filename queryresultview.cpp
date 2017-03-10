#include "queryresultview.h"
#include "ui_queryresultview.h"
#include <QPainter>

int calcTextLength(QString text) {
    if(text.contains('(')) return (text.length()-2) * 13 + 8;
    if(text.contains('-')) return (text.length()-1) * 13;
    return text.length() * 13;
}

QueryResultView::QueryResultView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QueryResultView)
{
    ui->setupUi(this);
}

QueryResultView::~QueryResultView()
{
    delete ui;
}

void QueryResultView::paintEvent(QPaintEvent *) {
    if(this->currentResult.count() > 0) {

    int count = this->currentResult.count();

    double xOffset = 0, yOffset = 0;
//    double height = this->size().height() / this->currentResult.count();
    QPainter painter(this);
    QPen pen(QColor(255, 255, 255)), textPen(QColor(0, 0, 0));
    painter.setPen(pen);

    for(int i = 0; i < count ; ++i) {
        if(yOffset >= this->height() - 30) break;
        xOffset = 0;
        QueryResult *result = this->currentResult[i];
        int transCount = result->transStops.count() - 1;

        if(transCount == 0) {
            Stop *stop = result->transStops[0];
            Busline *line = result->transLines[0];

            QString text =  line->busNumber + "直达";
            int length = calcTextLength(text) + 4;
            QRect rect(xOffset, yOffset, length, 20);
            QBrush brush(QColor(253, 147, 150));
            painter.setPen(Qt::NoPen);
            painter.setBrush(brush);
            painter.drawRoundedRect(rect, 4, 4);

            xOffset += length;

            painter.setPen(pen);
            painter.drawText(rect, Qt::AlignHCenter, text);

            painter.setPen(textPen);
            text = QString(this->startStop->name + "->" + stop->name);
            length = calcTextLength(text);
            QRect lineRect(xOffset + 10, yOffset, length, 20);
            painter.drawText(lineRect, Qt::AlignLeft, text);
            xOffset += 10 + length;

        }else{
            QRect rect(xOffset, yOffset, 60, 20);
            QBrush brush(QColor(142, 221, 143));
            painter.setPen(Qt::NoPen);
            painter.setBrush(brush);
            painter.drawRoundedRect(rect, 4, 4);

            QString text("换乘");
            text += QString::number(transCount) + "站";
            painter.setPen(pen);
            painter.drawText(rect, Qt::AlignHCenter, text);
            xOffset += 70;

            QPen busPen(QColor(85, 170, 102));
            painter.setPen(textPen);
            int length = calcTextLength("起点");
            QRect lineRect(xOffset, yOffset, length, 20);
            xOffset += length;
            painter.drawText(lineRect, Qt::AlignLeft, "起点");

            for(int j = 0; j < result->transLines.count(); ++j){
                QRect arrowRect(xOffset, yOffset + 1, 15, 20);
//                if (j>0){
                    if(arrowRect.left() + arrowRect.width() > this->width()) {
                        xOffset = 0;
                        yOffset += 25;
                        arrowRect = QRect(xOffset, yOffset + 1, 15, 20);
                    }
//                }

                painter.drawText(arrowRect, Qt::AlignHCenter, "->");
                xOffset += 15;
                QString number = (j == 0 ? "乘坐":"换乘")+ result->transLines[j]->busNumber;
                int lineWidth = 40 + 26;
                lineRect = QRect(xOffset, yOffset, lineWidth, 20);
                painter.setPen(busPen);
                if(lineRect.left() + lineRect.width() > this->width()) {
                    xOffset = 0;
                    yOffset += 25;
                    lineRect = QRect(xOffset, yOffset, lineWidth, 20);
                }
                painter.drawText(lineRect, Qt::AlignHCenter, number);
                xOffset += lineWidth;
                
                painter.setPen(textPen);
                arrowRect = QRect(xOffset, yOffset + 1, 15, 20);
                if(arrowRect.left() + arrowRect.width() > this->width()) {
                    xOffset = 0;
                    yOffset += 25;
                    arrowRect = QRect(xOffset, yOffset + 1, 15, 20);
                }
                painter.drawText(arrowRect, Qt::AlignHCenter, "->");
                xOffset += 15;

                QString text = result->transStops[j]->name;
                lineWidth = calcTextLength(text);
                lineRect = QRect(xOffset, yOffset, lineWidth, 20);
                if(lineRect.left() + lineRect.width() > this->width()) {
                    xOffset = 0;
                    yOffset += 25;
                    lineRect = QRect(xOffset, yOffset, lineWidth, 20);
                }
                painter.drawText(lineRect, Qt::AlignLeft, text);
                xOffset += lineWidth;
            }

        }
        QRect distaceRect(xOffset + 5, yOffset, 50, 20);
        painter.setBrush(QBrush(QColor(225, 108, 93)));
        painter.setPen(Qt::NoPen);
        if(xOffset + 5 + 50 > this->width()) {
            xOffset = 0;
            yOffset += 25;
            distaceRect = QRect(xOffset + 70, yOffset, 50, 20);
        }
        painter.drawRoundedRect(distaceRect, 4, 4);

        painter.setPen(pen);
        painter.drawText(distaceRect, Qt::AlignHCenter, "共" + QString::number(result->totalStopCount) + "站");
        yOffset += 35;
    }

    }
}

bool compare(QueryResult *a, QueryResult *b) {
    return a->transLines.count() < b->transLines.count();
}

bool distaceCompare(QueryResult *a, QueryResult *b) {
    return a->totalStopCount < b->totalStopCount;
}

void QueryResultView::setQueryResult(Stop *startStop, QList<QueryResult*> result) {
    this->currentResult = result;
    std::sort(this->currentResult.begin(), this->currentResult.end(), distaceCompare);
    std::sort(this->currentResult.begin(), this->currentResult.end(), compare);
    this->startStop = startStop;
    this->update();

}
