#ifndef GUI_H
#define GUI_H

#include "GUI/execute_widget.h"
#include "GUI/regfile_widget.h"
#include "GUI/data_mem_widget.h"
#include "GUI/tree_widget.h"
#include "GUI/editor.h"
#include "GUI/piperegwidget.h"

#include "simulator.h"
#include <bits/stdc++.h>
#include <QObject>
#include <QLineEdit>
#include <QDebug>
#include <QWidget>
#include <QPushButton>
#include <QKeyEvent>
#include <QBrush>
#include <QColor>
#include <QFileDialog>

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QSpacerItem>
#include <QVBoxLayout>
#include <QLabel>
#include <QTabWidget>
#include <QTextEdit>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QStringList>
#include "GUI/testwidget.h"
#include <QToolBar>
#include <QStackedWidget>


#define RUN "Run"
#define PIPELINE "Pipeline"
#define INCLUDE "Include"
#define EDIT "Edit"
#define EXECUTE "Execute"
#define TEST "Test"
#define REGISTERS_TYPE "Registers"
#define DATA_MEM "Memory"

#define EDITOR_POS 0
#define EXECUTE_POS 1
#define DATAMEM_POS 2
#define TEST_POS 3

#define Width_Editor 900
#define Hight_Editor 600
#define Registers_Width 350
#define O_Screen_Hight 150
#define Code_color 115, 197, 241 // grey

using namespace  std;


class GUI : public QWidget
{
    Q_OBJECT
    friend class MainWindow;
private:
    Simulator* simulator ;
    QString ApplicationPath;
    string path;
    int i ;

    QGridLayout* grid ;

    QStackedWidget* stackedWidget;
    QToolBar* stackedBar;

    Editor* Code_Editor;
    Execute_Widget* Execution;
    Data_Mem_Widget* Data_Memory;
    TestWidget* testWidget;

    QToolBar* toolBar;
    QString hover_color , press_color;

    QIcon OpenBtn; QString OpenBtnText;
    QIcon RunBtn; QString RunBtnText;
    QIcon SaveBtn; QString SaveBtnText;
    QIcon PipelineBtn; QString PipelineBtnText;
    QIcon rightBtn; QString rightBtnText;
    QIcon leftBtn; QString leftBtnText;
    QIcon DebugBtn; QString DebugBtnText;

    QIcon codeEditorToolBtn; QString codeEditorToolBtnText;
    QIcon ExectutionToolBtn; QString ExectutionToolBtnText;
    QIcon DataMemToolBtn; QString DataMemToolBtnText;
    QIcon TestWidgetBtn; QString TestWidgetBtnText;


    QFileDialog* save_file_dialog;
    QFileDialog* include_file_dialog;
    Editor* IO_Screen;
    QTabWidget* IO_Screen_Container;
    QTabWidget* Right_TabWidget;
    regFile_Widget* Registers_Table;
    PipeRegWidget* Pipeline_Registers;
public:
    explicit GUI(QWidget *parent = nullptr);
    ~GUI();

    void Design();
    void Signals_Slots();
    void init_files_dialog();
    void keyPressEvent(QKeyEvent * event);

    void init_toolBar();
    void init_stackedBar();
signals:
    void update(vector<int>);
    void pipeline_GUI();

public slots:
    void Start_Simulation();
    void Start_Pipeline_Simulation();
    void Start_Simulation_File(QStringList);
    void Output_Screen(string);
    void Start_Simulation_File(string);
    void Browse_file();
    void Browse_Save_File();

    void Right_btn();
    void Left_btn();
    void Play_btn();
    void highlight(uint);

    void handle_toolBar_Buttons(QAction*);
    void handle_stacked_Buttons(QAction*);
};

vector<string> split_string(string s,string splitter);

#endif // GUI_H
