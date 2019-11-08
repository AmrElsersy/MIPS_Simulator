#include "mainwindow.h"
MainWindow::MainWindow(QWidget *parent) :QMainWindow(parent)
{
    // MIPS GUI
    this->gui = new GUI();
    this->pipeline_gui = new View();
    this->setCentralWidget(this->gui);
    this->setStyleSheet("background-color:rgb(0,0,128);""color: rgb(0,0,102);""font-weight: 400;");

    connect(this->gui,SIGNAL(pipeline_GUI()),this,SLOT(show_pipeline_gui()));

}
void MainWindow ::show_pipeline_gui()
{
    this->pipeline_gui->show();
}


View :: View(QWidget* parent) : QGraphicsView(parent)
{
    // Pipeline GUI
    this->setFixedSize(    QSize(1920,1000)   );
    this->my_scene= new myScene(this);
    this->setScene(this->my_scene);

    connect(this,SIGNAL(updatePipeline(int)),this->my_scene,SLOT(UpdatePipeline(int)));
}

void View::keyPressEvent(QKeyEvent *event)
{
    if (event->key()== Qt::Key_D)
        emit updatePipeline(1);
    else if (event->key() == Qt::Key_A)
        emit updatePipeline(-1);
}

