#ifndef STAGE_H
#define STAGE_H
#define PATH_WIDTH 4
#define TEXT_COLOR Qt::white
#define OFF_COLOR QColor(70,70,70)
#define FONT_SIZE 12
#include <QObject>
#include <QPen>
#include <QBrush>
#include <QFont>
#include <QPainter>
#include <QPaintEvent>
#include <QRect>
#include <QRectF>
#include <QLabel>
#include <QPointF>
#include <QPainterPath>
#include <bits/stdc++.h>
#include <QGraphicsItem>
#include <QGraphicsTextItem>
#include <QGraphicsRectItem>
#include <QGraphicsPolygonItem>
#include <iostream>
#include "path.h"
using namespace std;
class Stage : public QObject
{
    Q_OBJECT
protected:
    QColor color;
    QPen pen;
    QPainterPath painter;
    map<string,Path*> paths;
    QGraphicsTextItem* text_instruction;

public:
    Stage(QObject *parent); // we must have parent to connect the signals
    Path* newPath(vector<string> points);
    QColor getColor();
    void setTextInstruction(QString text);
    virtual void setStageColor(QColor clr, vector<string> muxs = vector<string>()) = 0;


signals:
    void addnewItem(QGraphicsItem*);
public slots:
};

#endif // STAGE_H
