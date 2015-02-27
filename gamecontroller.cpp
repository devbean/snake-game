#include <QEvent>
#include <QGraphicsScene>
#include <QKeyEvent>

#include "gamecontroller.h"
#include "food.h"
#include "snake.h"

GameController::GameController(QGraphicsScene &scene, QObject *parent) :
    QObject(parent),
    scene(scene),
    snake(new Snake(*this))
{
    timer.start( 1000/33 );

	Food *a1 = new Food(0, -50);        
    scene.addItem(a1);

    scene.addItem(snake);
    scene.installEventFilter(this);

    resume();
}

GameController::~GameController()
{
}

void GameController::snakeAteFood(Snake *snake, Food *food)
{
    scene.removeItem(food);

    addNewFood();
}

void GameController::snakeHitWall(Snake *snake, Wall *wall)
{
}

void GameController::snakeAteItself(Snake *snake)
{
    QTimer::singleShot(0, this, SLOT(gameOver()));
}

void GameController::handleKeyPressed(QKeyEvent *event)
{
    switch (event->key()) {
        case Qt::Key_Left:
			if (snake->currentDirection() == Snake::MoveRight){
				break;
			}
            snake->setMoveDirection(Snake::MoveLeft);
            break;
        case Qt::Key_Right:
			if (snake->currentDirection() == Snake::MoveLeft){
				break;
			}
            snake->setMoveDirection(Snake::MoveRight);
            break;
        case Qt::Key_Up:
			if (snake->currentDirection() == Snake::MoveDown){
				break;
			}
            snake->setMoveDirection(Snake::MoveUp);
            break;
        case Qt::Key_Down:
			if (snake->currentDirection() == Snake::MoveUp){
				break;
			}
            snake->setMoveDirection(Snake::MoveDown);
            break;
    }
}

void GameController::addNewFood()
{
    int x, y;

    do {
		x = (int)(qrand() % 200) / 10 - 10;
		y = (int)(qrand() % 200) / 10 - 10;

        x *= 10;
        y *= 10;
	} while (snake->shape().contains(snake->mapFromScene(QPointF(x + 5, y + 5))));

	Food *food = new Food(x, y);
    scene.addItem(food);
}

void GameController::gameOver()
{
    scene.clear();

    snake = new Snake(*this);
    scene.addItem(snake);
    addNewFood();
}

void GameController::pause()
{
    disconnect(&timer, SIGNAL(timeout()),
               &scene, SLOT(advance()));
}

void GameController::resume()
{
    connect(&timer, SIGNAL(timeout()),
            &scene, SLOT(advance()));
}

bool GameController::eventFilter(QObject *object, QEvent *event)
{
    if (event->type() == QEvent::KeyPress) {
        handleKeyPressed((QKeyEvent *)event);
        return true;
    } else {
        return QObject::eventFilter(object, event);
    }
}
