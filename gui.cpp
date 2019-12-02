#include "gui.h"

GUI::GUI(QWidget *parent) :   QWidget (parent)
{
    this->ApplicationPath = QCoreApplication::applicationDirPath() + "/../../MIPS_Simulator";
    // memory allocation
    this->simulator = new Simulator();
    //    this->simulator->Simulate(this->path);11
    this->grid    = new QGridLayout();

    this->stackedWidget = new QStackedWidget();
    this->stackedBar = new QToolBar(this);
    this->toolBar = new QToolBar(this);
    this->Execution       = new Execute_Widget();
    this->Execution->setStyleSheet("background: white;");
    this->Data_Memory     = new Data_Mem_Widget();
    this->Data_Memory->setStyleSheet("background: white;");
    this->testWidget      = new TestWidget();
    this->testWidget->setStyleSheet("background-color:#FFFAE3; color: white; font-weight: 400;");

    this->Right_TabWidget = new QTabWidget();
    this->Right_TabWidget->setStyleSheet("QTabWidget::tab-bar{padding: 2px 15px; margin-right:5px;}");
    this->Registers_Table = new regFile_Widget();
    this->Registers_Table->setStyleSheet("background: white;");
    this->Pipeline_Registers = new PipeRegWidget();
    this->Pipeline_Registers->setStyleSheet("background:white;");

    // set colors of highlight
    QColor Code_highlight_color; Code_highlight_color.setRgb(Code_color);
    this->Code_Editor     = new Editor(Code_highlight_color);
    this->Code_Editor->setStyleSheet("background: white;");
    this->IO_Screen       = new Editor();
    this->IO_Screen_Container = new QTabWidget();
    this->IO_Screen_Container->setStyleSheet("background:rgb(0,0,128);");
    this->IO_Screen->setReadOnly(true);
    this->IO_Screen->setStyleSheet("background: white;");

    this->init_toolBar();
    this->init_stackedBar();
    this->init_files_dialog();
    this->Design();
    this->Signals_Slots();
}
GUI::~GUI()
{
    delete this->grid;
    delete this->simulator;
    delete this->stackedWidget;
    delete this->Code_Editor;
    delete this->IO_Screen;
    delete this->Registers_Table;
    delete this->Execution;
}
void GUI::Design()
{
    this->stackedWidget->addWidget(this->Code_Editor);
    this->stackedWidget->addWidget(this->Execution);
    this->stackedWidget->addWidget(this->Data_Memory);
    this->stackedWidget->addWidget(this->testWidget);

    this->Right_TabWidget->addTab(this->Registers_Table,"Registers");
    this->Right_TabWidget->addTab(this->Pipeline_Registers,"Pipeline");


    this->IO_Screen_Container->addTab(this->IO_Screen,"Output");


    // Grid Design
    this->grid->addWidget(this->toolBar,0,0,1,-1);
    this->grid->addWidget(this->stackedBar,1,0,-1,1);
    this->grid->addWidget(this->stackedWidget,1,1,3,3);
    this->grid->addWidget(this->Right_TabWidget,1,4,-1,1);
    this->grid->addWidget(this->IO_Screen_Container,4,0,1,4);
    // for resizing
    this->grid->setColumnStretch(1,1); // TabWidget
    this->grid->setRowStretch(1,3); // TabWidget also
    this->grid->setRowStretch(2,3); // TabWidget also
    this->grid->setRowStretch(4,2); // IO_Screen

    // Set Min Size
    this->grid->setColumnMinimumWidth(1,Width_Editor); // TabWidget and IO_Screen
    this->grid->setRowMinimumHeight(1,Hight_Editor);   // TabWidget
    this->grid->setColumnMinimumWidth(4,Registers_Width); // Registers Table
    this->grid->setRowMinimumHeight(4,O_Screen_Hight);   // IO_Screen

    this->setLayout(this->grid);
}

void GUI::init_toolBar()
{
    // init Actions Icons and Tex
    this->OpenBtn = QIcon(this->ApplicationPath + "\\icons\\open.png");     this->OpenBtnText = "Open";

    this->SaveBtn = QIcon(this->ApplicationPath + "\\icons\\save.png"); this->SaveBtnText = "Save";
    this->RunBtn =  QIcon(this->ApplicationPath + "\\icons\\run.png"); this->RunBtnText  = "Run";
    this->PipelineBtn = QIcon(this->ApplicationPath + "\\icons\\pipe1.png");  this->PipelineBtnText="Pipeline";
    this->leftBtn =QIcon(this->ApplicationPath + "\\icons\\left.png");   this->leftBtnText  = "Backword";
    this->DebugBtn =QIcon(this->ApplicationPath + "\\icons\\debug.png"); this->DebugBtnText  = "Debug";
    this->rightBtn =QIcon(this->ApplicationPath + "\\icons\\right.png");   this->rightBtnText = "Forward";
    this->HelpBtn =QIcon(this->ApplicationPath + "\\icons\\help.png");   this->HelpBtnText = "Help";

    // init ToolBar
    this->toolBar->setMovable(false);
    this->toolBar->setIconSize(QSize(40,40));
    this->toolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    this->toolBar->setOrientation(Qt::Horizontal);
    this->toolBar->layout()->setAlignment(Qt::AlignRight);

    this->hover_color = "rgb(220,220,220)";
    this->press_color = "white";
    this->toolBar->setStyleSheet("QToolBar{background-color: #01333e; color: white;}"
                                 "QToolButton:hover{background-color:"+this->hover_color+"; color: black;}"
                                 "QToolButton:select{background-color:"+this->press_color+"; color: black;}"
                                 "QToolButton{color:white; font-weight: 400; border-right: 1px solid white; padding-right: 15px; padding-left: 15px; border-radius: 0;}");

    // add Buttons
    this->toolBar->addAction(this->OpenBtn,this->OpenBtnText);  this->toolBar->addSeparator();
    this->toolBar->addAction(this->SaveBtn,this->SaveBtnText);    this->toolBar->addSeparator();
    this->toolBar->addAction(this->RunBtn,this->RunBtnText);    this->toolBar->addSeparator();
    this->toolBar->addAction(this->PipelineBtn,this->PipelineBtnText);    this->toolBar->addSeparator();
    this->toolBar->addAction(this->leftBtn,this->leftBtnText);    this->toolBar->addSeparator();
    this->toolBar->addAction(this->DebugBtn,this->DebugBtnText);    this->toolBar->addSeparator();
    this->toolBar->addAction(this->rightBtn,this->rightBtnText);    this->toolBar->addSeparator();
    this->toolBar->addAction(this->HelpBtn,this->HelpBtnText);    this->toolBar->addSeparator();

}

void GUI::init_stackedBar()
{
    // init Buttons
    this->codeEditorToolBtn =  QIcon(this->ApplicationPath + "\\icons\\editor.png"); this->codeEditorToolBtnText  = "Editor";
    this->ExectutionToolBtn = QIcon(this->ApplicationPath + "\\icons\\execute.png"); this->ExectutionToolBtnText = "Execute";
    this->DataMemToolBtn =  QIcon(this->ApplicationPath + "\\icons\\memory.png"); this->DataMemToolBtnText  = "Memory";
    this->TestWidgetBtn = QIcon(this->ApplicationPath + "\\icons\\test.png"); this->TestWidgetBtnText = "Test";
    // init stacked toolBar
    this->stackedBar->setMovable(false);
    this->stackedBar->setIconSize(QSize(70,70));
    this->stackedBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    this->stackedBar->setOrientation(Qt::Vertical);
    this->stackedBar->setStyleSheet("QToolBar{background-color:#01333e;}"
                                 "QToolButton:hover{background-color:"+this->hover_color+"; color: black;}"
                                 "QToolButton:pressed{background-color:"+this->press_color+"; color: black;}"
                                 "QToolButton{color:white; font-weight: 400; border-bottom: 1px solid white; padding-bottom: 5px; padding-top: 5px; border-radius: 0;}");
    // add Actions Buttons
    this->stackedBar->addAction(this->codeEditorToolBtn,this->codeEditorToolBtnText); this->stackedBar->addSeparator();
    this->stackedBar->addAction(this->ExectutionToolBtn,this->ExectutionToolBtnText); this->stackedBar->addSeparator();
    this->stackedBar->addAction(this->DataMemToolBtn,this->DataMemToolBtnText);       this->stackedBar->addSeparator();
    this->stackedBar->addAction(this->TestWidgetBtn,this->TestWidgetBtnText);         this->stackedBar->addSeparator();

    this->stackedBar->setFixedWidth(70);

}

void GUI::handle_toolBar_Buttons(QAction* action)
{
    QString Button_pressed = action->text();
    if(Button_pressed== this->RunBtnText)
        this->Start_Simulation();
    else if(Button_pressed == this->OpenBtnText)
        this->Browse_file();
    else if(Button_pressed == this->SaveBtnText)
        this->Browse_Save_File();
    else if(Button_pressed == this->PipelineBtnText)
        this->Start_Pipeline_Simulation();
    else if(Button_pressed == this->DebugBtnText)
        this->Play_btn();
    else if(Button_pressed == this->rightBtnText)
        this->Right_btn();
    else if(Button_pressed == this->leftBtnText)
        this->Left_btn();
    else if(Button_pressed == this->HelpBtnText)
        QDesktopServices::openUrl(QUrl("http://bahlool.000webhostapp.com/help.html"));

}
void GUI::handle_stacked_Buttons(QAction* action)
{
    QString Button_pressed = action->text();
    if(Button_pressed== this->codeEditorToolBtnText)
        this->stackedWidget->setCurrentIndex(EDITOR_POS);
    else if(Button_pressed == this->ExectutionToolBtnText)
        this->stackedWidget->setCurrentIndex(EXECUTE_POS);
    else if(Button_pressed == this->DataMemToolBtnText)
        this->stackedWidget->setCurrentIndex(DATAMEM_POS);
    else if(Button_pressed == this->TestWidgetBtnText)
        this->stackedWidget->setCurrentIndex(TEST_POS);

}
void GUI::Signals_Slots()
{
    connect( this->toolBar,SIGNAL(actionTriggered(QAction*)),this,SLOT(handle_toolBar_Buttons(QAction*)));
    connect( this->stackedBar,SIGNAL(actionTriggered(QAction*)),this,SLOT(handle_stacked_Buttons(QAction*)));
    connect( this->include_file_dialog     , SIGNAL(filesSelected(QStringList) )     , this , SLOT( Start_Simulation_File(QStringList) ) );

    connect( this->Execution,SIGNAL(require_Instructions() ), this->simulator , SLOT ( get_instructions() ) );
    connect( this->Execution,SIGNAL(require_AssembledInstructions() ), this->simulator->assembler , SLOT ( get_assembled_strings() ) );
    connect( this->Data_Memory,SIGNAL(get_access_memory() ), this->simulator->data_memory, SLOT ( get_Memory_Access() ) );
    connect( this->Registers_Table,SIGNAL(get_registers() ), this->simulator->register_file , SLOT ( registers_reading() ) );
    connect( this->simulator->Alu ,SIGNAL( syscall(string) ) , this,SLOT( Output_Screen(string) ) );

    connect( this->simulator, SIGNAL(getInstruction_Editor()), this->Code_Editor,SLOT(Read_Code_Text_Editor() ));
    connect( this->simulator, SIGNAL(update_assembled_instruction() ) , this->Execution , SLOT( updateInstructions() ) );
    connect( this->simulator, SIGNAL(update_registers() )    , this->Registers_Table , SLOT( updateRegisters() ) );
    connect( this->simulator, SIGNAL(update_data_memory() ), this->Data_Memory , SLOT( update_memory() ) );
    connect( this->simulator, SIGNAL(clear_data_memory() ), this->Data_Memory , SLOT( clear() ) );
    connect( this->simulator, SIGNAL(clearTextEditor()),    this->Code_Editor,SLOT(Clear()) );
    connect( this->simulator, SIGNAL (update_Text_Editor(vector<string>)) , this->Code_Editor, SLOT(Write_Code_Text_Editor(vector<string>)) );
    connect( this->simulator, SIGNAL (show(vector<string>)) , this->IO_Screen, SLOT(Write_Code_Text_Editor(vector<string>)) );
    connect( this->simulator->Alu, SIGNAL(Info_Output(string)), this,SLOT(Output_Screen(string) ));
    connect( this->simulator->Alu , SIGNAL (update_memory_gui(uint)) , this->Data_Memory , SLOT(update_memory(uint)) );
    connect( this->simulator->data_memory , SIGNAL (update_dataMemory_GUI(uint)) , this->Data_Memory , SLOT(update_memory(uint)) );
    connect( this->simulator, SIGNAL (highlight(uint)) , this, SLOT(highlight(uint)) );
    connect( this->simulator, SIGNAL (Output_Print(string)) , this, SLOT(Output_Screen(string)) );

    connect(this->testWidget,SIGNAL(output_screen(vector<string>)),this->IO_Screen,SLOT(Write_Code_Text_Editor(vector<string>)));
    connect(this->testWidget,SIGNAL(start_simulation(string)), this,SLOT(Start_Simulation_File(string)));
    connect(this->testWidget,SIGNAL(Start_simulation()), this,SLOT(Start_Simulation()));

}
void GUI::init_files_dialog()
{
    this->include_file_dialog = new QFileDialog(this);
//    this->include_file_dialog->setDirectory("C:\\MIPS_Simulator\\TestCases"); // set the open directory
    this->include_file_dialog->setFileMode(QFileDialog::ExistingFile); // select existing file (one file )only
    this->include_file_dialog->setNameFilter("*.txt");                  // show only txt extentions
    this->include_file_dialog->setOption(QFileDialog::ReadOnly);        // readonly mode dosn't support deleting or writing
    this->include_file_dialog->setStyleSheet("background: white");

    this->save_file_dialog = new QFileDialog(this);
//    this->save_file_dialog ->setDirectory("C:\\MIPS_Simulator\\TestCases"); // set the open directory
    this->save_file_dialog->setAcceptMode(QFileDialog::AcceptSave);
    this->save_file_dialog ->setFileMode(QFileDialog::AnyFile); // for "save as" mode
}
void GUI::keyPressEvent(QKeyEvent *event)
{
    if(event->key()== Qt::Key_L){
        this->Execution->updateInstructions();
        cout << "L pressed" << endl;
    }
    else if (event->key() == Qt::Key_R) {
        this->Registers_Table->updateRegisters();
        cout << "R Pressed" << endl;
    }
}
void GUI::Start_Simulation()
{
    this->simulator->mode = "MIPS";
    this->simulator->Simulate();
}
void GUI::Start_Pipeline_Simulation()
{
    this->simulator->mode = "Pipeline";
    this->simulator->Simulate();
    emit pipeline_GUI();
}
void GUI::Start_Simulation_File(QStringList code_file_path)
{
    this->Code_Editor->clear();
//    this->IO_Screen->clear();
    string path = code_file_path[0].toStdString();
    this->simulator->Simulate(path);
    this->IO_Screen->Write_Code_Text_Editor({"Open is done"});

}
void GUI::Start_Simulation_File(string test_assembly_path)
{
    this->simulator->Simulate(test_assembly_path);

}
void GUI::Output_Screen(string error)
{
    vector<string> x ;
    x.push_back(error);
    this->IO_Screen->Write_Code_Text_Editor(x);
}
void GUI:: Browse_file()
{
    this->include_file_dialog->show();
}
void GUI::Browse_Save_File()
{
    // open the file dialog in save mode and return the selected path
    string path = this->save_file_dialog->getSaveFileName().toStdString();
    // fix the path to be suitable with c++ strings
    for (uint i = 0;i<path .length();i++)
        if (path[i] == '/')
            path.replace(i,1,"\\");

    // read the editor
    vector<string> code_editor = this->Code_Editor->Read_Code_Text_Editor();
    // one string = code_editor line + \n .... and iterate over all lines
    string total_code;
    for (uint i =0 ; i<code_editor.size(); i++)
        total_code += code_editor[i] + "\n";

    // write the one peace string in the file
    ofstream file ; file.open(path);
    file << total_code ;

}
void GUI::Right_btn()
{
    //    this->Code_Editor->highlight(i);
    this->simulator->updateState(1);
}
void GUI::Left_btn()
{
    this->simulator->updateState(-1);
}
void GUI::Play_btn()
{
    this->simulator->mode = "Debugger";
    this->simulator->Simulate();
}
void GUI::highlight(uint pos)
{
    this->Code_Editor->highlight(pos);
}





