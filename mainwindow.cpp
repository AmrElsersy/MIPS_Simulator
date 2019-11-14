#include "mainwindow.h"
MainWindow::MainWindow(QWidget *parent) :QMainWindow(parent)
{
    // MIPS GUI
    this->gui = new GUI();
    this->pipeline_gui = new View();
    this->setCentralWidget(this->gui);
    this->setStyleSheet(" background-color: #01333e;");
    connect(this->gui->simulator,SIGNAL(updatePipelineAssemblyCode(vector<string>)),this->pipeline_gui->my_scene,SLOT(INIT_Scene(vector<string>)));
    // Scene Signals and Slots regFile DataMemory
    connect(this->pipeline_gui->my_scene,SIGNAL(updateRegFile(string)),this->gui->simulator->register_file,SLOT(updateRegFilePipeline(string)));
    connect(this->pipeline_gui->my_scene,SIGNAL(updateDataMem(string)),this->gui->simulator->data_memory,SLOT(updateDataMemPipeline(string)));
    connect( this->pipeline_gui->my_scene, SIGNAL(updateRegistersGUI() )    , this->gui->Registers_Table , SLOT( updateRegisters() ) );
    connect (this->pipeline_gui->my_scene, SIGNAL(updatepipeline_regs(string)), this->gui->Pipeline_Registers,SLOT(updatePipeRegistersWidget(string)));
    // to show the pipeline GUI
    connect(this->gui,SIGNAL(pipeline_GUI()),this,SLOT(show_pipeline_gui()));

}
void MainWindow ::show_pipeline_gui()
{
    this->pipeline_gui->show();
}

void MainWindow::slot_ray2(QAction * action)
{
    cout << action->text().toStdString() << " ispressed" << endl;
}

void MainWindow::spidy_slot()
{

}
void MainWindow::ray2()
{
    QIcon spidy("C:\\MIPS_Simulator\\spidy.png");
    QIcon warning_icon("C:\\MIPS_Simulator\\warning-icon.png");
    QIcon play_icon("C:\\MIPS_Simulator\\play_icon.png");
    QIcon bugy("C:\\MIPS_Simulator\\bugy.png");
    QIcon left_icon("C:\\MIPS_Simulator\\left_icon.png");
    QToolBar* toolBar = new QToolBar("Ray2");
    //    toolBar->setToolButtonStyle(Qt::ToolButtonIconOnly);
    //    toolBar->setToolButtonStyle(Qt::ToolButtonTextOnly);
    //    toolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    //    this->toolBar->setContentsMargins(100,100,300,200); // set the margin of the toolbar its self

    toolBar->setMovable(false);
    toolBar->setIconSize(QSize(100,100));
    toolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    toolBar->setOrientation(Qt::Vertical);
    //    toolBar->setStyleSheet("background-color:red");

    QAction* spidy_action = toolBar->addAction(spidy,"spidy");  toolBar->addSeparator();
    QAction* warning_action = toolBar->addAction(warning_icon,"warning_icon");  toolBar->addSeparator();
    QAction* play_action = toolBar->addAction(play_icon,"play_icon");  toolBar->addSeparator();
    QAction* bugy_action = toolBar->addAction(bugy,"bugy");  toolBar->addSeparator();
    QAction* left = toolBar->addAction(left_icon,"left");  toolBar->addSeparator();

    connect(toolBar,SIGNAL(actionTriggered(QAction*)),this,SLOT(slot_ray2(QAction*)));
    connect(spidy_action,SIGNAL(hovered()),this,SLOT(spidy_slot()));
    this->setCentralWidget(toolBar);
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
    else if (event->key() == Qt::Key_Space)
        cout << endl;
}

