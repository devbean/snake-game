#include <QPainter>

#include "constants.h"
#include "gamecontroller.h"
#include "snake.h"

static const qreal SNAKE_SIZE = TILE_SIZE;

Snake::Snake(GameController &controller) :
    head(0, 0),
    growing(7),
    speed(5),
    moveDirection(NoMove),
    controller(controller)
{
}

QRectF Snake::boundingRect() const
{
    qreal minX = head.x();
    qreal minY = head.y();
    qreal maxX = head.x();
    qreal maxY = head.y();

    foreach (QPointF p, tail) {
        maxX = p.x() > maxX ? p.x() : maxX;
        maxY = p.y() > maxY ? p.y() : maxY;
        minX = p.x() < minX ? p.x() : minX;
        minY = p.y() < minY ? p.y() : minY;
    }

    QPointF tl = mapFromScene(QPointF(minX, minY));
    QPointF br = mapFromScene(QPointF(maxX, maxY));

    QRectF bound = QRectF(tl.x(),  // x
                          tl.y(),  // y
                          br.x() - tl.x() + SNAKE_SIZE,      // width
                          br.y() - tl.y() + SNAKE_SIZE       //height
                          );
    return bound;
}

QPainterPath Snake::shape() const
{
    QPainterPath path;
    path.setFillRule(Qt::WindingFill);

    path.addRect(QRectF(0, 0, SNAKE_SIZE, SNAKE_SIZE));

    foreach (QPointF p, tail) {
        QPointF itemp = mapFromScene(p);
        path.addRect(QRectF(itemp.x(), itemp.y(), SNAKE_SIZE, SNAKE_SIZE));
    }

    return path;
}

void Snake::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->save();
    painter->fillPath(shape(), Qt::yellow);
    painter->restore();
}

void Snake::setMoveDirection(Direction direction)
{
    if (moveDirection == MoveLeft && direction == MoveRight)
        return;
    if (moveDirection == MoveRight && direction == MoveLeft)
        return;
    if (moveDirection == MoveUp && direction == MoveDown)
        return;
    if (moveDirection == MoveDown && direction == MoveUp)
        return;
    moveDirection = direction;
}

Snake::Direction Snake::currentDirection()
{
	return moveDirection;
}

void Snake::advance(int step)
{
    if (!step) {
        return;
    }
    if (tickCounter++ % speed != 0) {
        return;
    }
    if (moveDirection == NoMove) {
        return;
    }

    if (growing > 0) {
		QPointF tailPoint = head;
        tail << tailPoint;
        growing -= 1;
    } else {
        tail.removeFirst();
        tail << head;
    }

    switch (moveDirection) {
        case MoveLeft:
            moveLeft();
            break;
        case MoveRight:
            moveRight();
            break;
        case MoveUp:
            moveUp();
            break;
        case MoveDown:
            moveDown();
            break;
    }

    setPos(head);
    handleCollisions();
}

void Snake::moveLeft()
{
    head.rx() -= SNAKE_SIZE;
    if (head.rx() < -100) {
        head.rx() = 90;
    }
}

void Snake::moveRight()
{
    head.rx() += SNAKE_SIZE;
    if (head.rx() >= 100) {
        head.rx() = -100;
    }
}

void Snake::moveUp()
{
    head.ry() -= SNAKE_SIZE;
    if (head.ry() < -100) {
        head.ry() = 90;
    }
}

void Snake::moveDown()
{
    head.ry() += SNAKE_SIZE;
    if (head.ry() >= 100) {
        head.ry() = -100;
    }
}

void Snake::handleCollisions()
{
    QList<QGraphicsItem *> collisions = collidingItems();

    // Check collisions with other objects on screen
    foreach (QGraphicsItem *collidingItem, collisions) {
        if (collidingItem->data(GD_Type) == GO_Food) {
            // Let GameController handle the event by putting another apple
            controller.snakeAteFood((Food *)collidingItem);
            growing += 1;
        }
    }

    // Check snake eating itself
    if (tail.contains(head)) {
        controller.snakeAteItself();
    }
}
