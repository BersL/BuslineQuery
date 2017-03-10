#ifndef QUERYRESULTVIEW_H
#define QUERYRESULTVIEW_H

#include <QWidget>
#include "busline.h"

namespace Ui {
class QueryResultView;
}

class QueryResultView : public QWidget
{
    Q_OBJECT

public:
    explicit QueryResultView(QWidget *parent = 0);
    ~QueryResultView();
    void setQueryResult(Stop *startStop, QList<QueryResult*> result);

private:
    Ui::QueryResultView *ui;
    QList<QueryResult*> currentResult;
    Stop *startStop;
    void paintEvent(QPaintEvent *);
};

#endif // QUERYRESULTVIEW_H
