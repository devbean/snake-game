#include <QGraphicsView>
#include <QTimer>
#include <qaction.h>
#include <qmenubar.h>
#include <qapplication.h>
#include <qmessagebox.h>

#include "constants.h"
#include "gamecontroller.h"
#include "mainwindow.h"
#include <QIcon>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      scene(new QGraphicsScene(this)),
      view(new QGraphicsView(scene, this)),
      game(new GameController(*scene, this))
{
    setCentralWidget(view);
//    resize(600, 600);
    setFixedSize(600, 600);
    setWindowIcon(QIcon(":/images/snake_ico"));

	createActions();
	createMenus();

    initScene();
    initSceneBackground();

    QTimer::singleShot(0, this, SLOT(adjustViewSize()));
}

MainWindow::~MainWindow()
{
    
}

void MainWindow::adjustViewSize()
{
    view->fitInView(scene->sceneRect(), Qt::KeepAspectRatioByExpanding);
}

void MainWindow::createActions()
{
	newGameAction = new QAction(tr("&New Game"), this);
	newGameAction->setShortcuts(QKeySequence::New);
	newGameAction->setStatusTip(tr("Start a new game"));
	connect(newGameAction, &QAction::triggered, this, &MainWindow::newGame);

	exitAction = new QAction(tr("&Exit"), this);
	exitAction->setShortcut(tr("Ctrl+Q"));
	exitAction->setStatusTip(tr("Exit the game"));
	connect(exitAction, &QAction::triggered, this, &MainWindow::close);

	pauseAction = new QAction(tr("&Pause"), this);
	pauseAction->setStatusTip(tr("Pause..."));
	connect(pauseAction, &QAction::triggered, game, &GameController::pause);

	resumeAction = new QAction(tr("&Resume"), this);
	resumeAction->setStatusTip(tr("Resume..."));
	connect(resumeAction, &QAction::triggered, game, &GameController::resume);

	gameHelpAction = new QAction(tr("Game &Help"), this);
	gameHelpAction->setShortcut(tr("Ctrl+H"));
	gameHelpAction->setStatusTip(tr("Help on this game"));
	connect(gameHelpAction, &QAction::triggered, this, &MainWindow::gameHelp);

	aboutAction = new QAction(tr("&About"), this);
	aboutAction->setStatusTip(tr("Show the application's about box"));
	connect(aboutAction, &QAction::triggered, this, &MainWindow::about);

	aboutQtAction = new QAction(tr("About &Qt"), this);
	aboutQtAction->setStatusTip(tr("Show the Qt library's About box"));
	connect(aboutQtAction, &QAction::triggered, qApp, QApplication::aboutQt);
}

void MainWindow::createMenus()
{
	QMenu *options = menuBar()->addMenu(tr("&Options"));
	options->addAction(newGameAction);
	options->addSeparator();
	options->addAction(pauseAction);
	options->addAction(resumeAction);
	options->addSeparator();
	options->addAction(exitAction);

	QMenu *help = menuBar()->addMenu(tr("&Help"));
	help->addAction(gameHelpAction);
	help->addAction(aboutAction);
	help->addAction(aboutQtAction);
}

void MainWindow::initScene()
{
    scene->setSceneRect(-100, -100, 200, 200);
}

void MainWindow::initSceneBackground()
{
    QPixmap bg(TILE_SIZE, TILE_SIZE);
    QPainter p(&bg);
    p.setBrush(QBrush(Qt::gray));
    p.drawRect(0, 0, TILE_SIZE, TILE_SIZE);

    view->setBackgroundBrush(QBrush(bg));
}

void MainWindow::newGame()
{
	QTimer::singleShot(0, game, SLOT(gameOver()));
}

void MainWindow::about()
{
	QMessageBox::about(this, tr("About this Game"), tr("<h2>Snake Game</h2>"
		"<p>Copyright &copy; XXX."
		"<p>This game is a small Qt application. It is based on the demo in the GitHub written by Devbean."));
}

void MainWindow::gameHelp()
{
	QMessageBox::about(this, tr("Game Help"), tr("Using direction keys to control the snake to eat the food"
		"<p>Space - pause & resume"));
}