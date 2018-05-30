#ifndef MYPIXMAPITEM_H
#define MYPIXMAPITEM_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QGraphicsSceneWheelEvent>
#include <QGraphicsSceneMouseEvent>

class MyPixmapItem:public QObject,public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    MyPixmapItem(QGraphicsPixmapItem *parent);

    void mousePressEvent(QGraphicsSceneMouseEvent* event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event);
    void wheelEvent(QGraphicsSceneWheelEvent* event);
    void reset();
    //void set

private:
    double RATE;

};

#endif // MYPIXMAPITEM_H
