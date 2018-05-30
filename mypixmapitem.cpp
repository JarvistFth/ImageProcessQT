#include "mypixmapitem.h"
#include <QDebug>
MyPixmapItem::MyPixmapItem(QGraphicsPixmapItem *parent):
    QGraphicsPixmapItem(parent),
    RATE(0.1)
{

}
void MyPixmapItem::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    setPos(pos()+
           mapToParent(event->pos())-
           mapToParent(event->lastPos()));
}
void MyPixmapItem::mousePressEvent(QGraphicsSceneMouseEvent* event)
{

}
void MyPixmapItem::wheelEvent(QGraphicsSceneWheelEvent* event)
{
    int delta = event->delta();
    double factor = scale();
    if(delta>0){
        factor *=(1+RATE);
    }
    if(delta<0){
        factor *=(1-RATE);
    }
    setTransformOriginPoint(boundingRect().width()/2,
                            boundingRect().height()/2);
    setScale(factor);
}
void MyPixmapItem::reset(){
    setPos(0,0);
    setScale(1.0);
}


