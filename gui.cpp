#include "gui.h"

GUI::GUI(QWidget *parent) :   QWidget (parent)
{
    this->path = "/home/amrelsersy/MIPS/dummy.txt";
    // memory allocation
    this->simulator = new Simulator();
//    this->simulator->Simulate(this->path);11
    this->grid    = new QGridLayout();

    this->tabWidget       = new QTabWidget();
    this->tabWidget->setStyleSheet("background-color:rgb(0,0,128);");
    this->Execution       = new Execute_Widget();
    this->Execution->setStyleSheet("background: white;");
    this->Data_Memory     = new Data_Mem_Widget();
    this->Data_Memory->setStyleSheet("background: white;");
    this->testWidget      = new TestWidget();
    this->testWidget->setStyleSheet("background: white;");

    this->Right_TabWidget = new QTabWidget();
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
    this->init_horizontal_layout();
    this->init_files_dialog();
    this->Design();
    this->Signals_Slots();
}

GUI::~GUI()
{
    delete this->grid;
    delete this->simulator;
    delete this->tabWidget;
    delete this->Code_Editor;
    delete this->IO_Screen;
    delete this->Registers_Table;
    delete this->Execution;
}

void GUI::Design()
{
    this->tabWidget->addTab(this->Code_Editor,EDIT);
    this->tabWidget->addTab(this->Execution,EXECUTE);
    this->tabWidget->addTab(this->Data_Memory,DATA_MEM);
    this->tabWidget->addTab(this->testWidget,TEST);
    this->tabWidget->setMovable(true);
    this->tabWidget->setUsesScrollButtons(true);


    this->Right_TabWidget->addTab(this->Registers_Table,"Registers");
    this->Right_TabWidget->addTab(this->Pipeline_Registers,"Pipeline");

    this->IO_Screen_Container->addTab(this->IO_Screen,"Output");


    // Grid Design
    this->grid->addLayout(this->horizontalLayout,0,0,1,-1);
    this->grid->addWidget(this->tabWidget,1,0,3,3);
    this->grid->addWidget(this->Right_TabWidget,1,3,-1,1);
    this->grid->addWidget(this->IO_Screen_Container,4,0,1,3);

    // for resizing
    this->grid->setColumnStretch(0,1); // TabWidget
    this->grid->setRowStretch(1,3); // TabWidget also
    this->grid->setRowStretch(2,3); // TabWidget also
    this->grid->setRowStretch(4,2); // IO_Screen

    // Set Min Size
    this->grid->setColumnMinimumWidth(0,Width_Editor); // TabWidget and IO_Screen
    this->grid->setRowMinimumHeight(1,Hight_Editor);   // TabWidget
    this->grid->setColumnMinimumWidth(3,Registers_Width); // Registers Table
    this->grid->setRowMinimumHeight(4,O_Screen_Hight);   // IO_Screen

    this->setLayout(this->grid);
}

void GUI::init_horizontal_layout()
{
    this->horizontalLayout = new QHBoxLayout();
    this->lineEdit         = new QLineEdit();
    this->lineEdit->setStyleSheet("background-color: white;");
    this->includeBtn       = new QPushButton(INCLUDE);
    this->includeBtn->setStyleSheet("background-color: white;"
                                    "color: rgb(0,0,128);"
                                   "border-radius: 5%;"
                                    "font-weight: 400;");
    this->RunBtn           = new QPushButton(RUN);
    this->RunBtn->setStyleSheet("background-color: white;"
                                "color: rgb(0,0,128);"
                               "border-radius: 5%;"
                                "font-weight: 400;");
    this->PipelineBtn         = new QPushButton(PIPELINE);
    this->PipelineBtn->setMinimumWidth(200);
    this->PipelineBtn->setStyleSheet("background-color: white;"
                                 "color: rgb(0,0,128);"
                                "border-radius: 5%;"
                                 "font-weight: 400;");
    this->right_btn = new QPushButton("");
    this->left_btn = new QPushButton("");
    this->play_btn = new QPushButton("");

    right_btn->setIcon(QIcon("C:\\MIPS_Simulator\\right_icon.png"));
    left_btn->setIcon(QIcon("C:\\MIPS_Simulator\\left_icon.png"));
    play_btn->setIcon(QIcon("C:\\MIPS_Simulator\\play_icon.png"));


    this->horizontalLayout->addWidget(this->right_btn);
    this->horizontalLayout->addWidget(this->play_btn);
    this->horizontalLayout->addWidget(this->left_btn);

    this->horizontalLayout->addWidget(this->lineEdit);
    this->horizontalLayout->addWidget(this->includeBtn);
    this->horizontalLayout->addWidget(this->RunBtn);
    this->horizontalLayout->addWidget(this->PipelineBtn);

}

void GUI::Signals_Slots()
{
    connect( this->Execution,SIGNAL(require_Instructions() ), this->simulator , SLOT ( get_instructions() ) );
    connect( this->Execution,SIGNAL(require_AssembledInstructions() ), this->simulator->assembler , SLOT ( get_assembled_strings() ) );

    connect( this->Data_Memory,SIGNAL(get_access_memory() ), this->simulator->data_memory, SLOT ( get_Memory_Access() ) );
    connect( this->Registers_Table,SIGNAL(get_registers() ), this->simulator->register_file , SLOT ( registers_reading() ) );
    connect( this->simulator->Alu ,SIGNAL( syscall(string) ) , this,SLOT( Output_Screen(string) ) );

    connect( this->RunBtn     , SIGNAL(clicked() )      , this , SLOT( Start_Simulation() ) );
    connect( this->PipelineBtn, SIGNAL(clicked())       , this,SLOT(Start_Pipeline_Simulation()) );
    connect( this->includeBtn , SIGNAL(clicked() )     , this , SLOT( Browse_file() ) );
    connect( this->include_file_dialog     , SIGNAL(filesSelected(QStringList) )     , this , SLOT( Start_Simulation_File(QStringList) ) );
    connect( this->file_dialog             , SIGNAL(filesSelected(QStringList) )     , this , SLOT( file_paths_selected_dialog(QStringList) ) );

    connect(this->left_btn,SIGNAL(clicked()),this,SLOT(Left_btn()));
    connect(this->right_btn,SIGNAL(clicked()),this,SLOT(Right_btn()));
    connect(this->play_btn,SIGNAL(clicked()),this,SLOT(Play_btn()));


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

    connect(this->testWidget,SIGNAL(output_screen(vector<string>)),this->IO_Screen,SLOT(Write_Code_Text_Editor(vector<string>)));
    connect(this->testWidget,SIGNAL(start_simulation(string)), this,SLOT(Start_Simulation_File(string)));

}

void GUI::init_files_dialog()
{
    this->file_dialog = new QFileDialog(this);
    this->file_dialog->setDirectory("C:\\MIPS"); // set the open directory
    this->file_dialog->setFileMode(QFileDialog::ExistingFiles); // select existing file only
    this->file_dialog->setNameFilter("*.txt");                  // show only txt extentions
    this->file_dialog->setOption(QFileDialog::ReadOnly);        // readonly mode dosn't support deleting or writing

    this->include_file_dialog = new QFileDialog(this);
    this->include_file_dialog->setDirectory("C:\\MIPS_Simulator\\TestCases"); // set the open directory
    this->include_file_dialog->setFileMode(QFileDialog::ExistingFile); // select existing file (one file )only
    this->include_file_dialog->setNameFilter("*.txt");                  // show only txt extentions
    this->include_file_dialog->setOption(QFileDialog::ReadOnly);        // readonly mode dosn't support deleting or writing
    this->include_file_dialog->setStyleSheet("background: white");
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
    this->IO_Screen->clear();
    this->simulator->mode = "MIPS";
    this->simulator->Simulate();
}

void GUI::Start_Pipeline_Simulation()
{
    this->IO_Screen->clear();
    this->simulator->mode = "Pipeline";
    this->simulator->Simulate();
    emit pipeline_GUI();
}

void GUI::Start_Simulation_File(QStringList code_file_path)
{

    this->Code_Editor->clear();
    for (int i =0 ; i< code_file_path.size(); i++)
        cout << code_file_path[i].toStdString() << endl;

    this->IO_Screen->clear();
    string path = code_file_path[0].toStdString();
    this->simulator->Simulate(path);
    this->lineEdit->setText(QString::fromStdString(path));
}

void GUI::Start_Simulation_File(string test_assembly_path)
{
    this->simulator->Simulate(test_assembly_path);
}

void GUI::Output_Screen(string syscall_msg)
{
    vector<string> x ;
    x.push_back(syscall_msg);
    this->IO_Screen->Write_Code_Text_Editor(x);
    x.clear();
}

void GUI::file_paths_selected_dialog(QStringList files_pahts)
{
    // when the files is selected (then accepted by "enter" or "open") the file dialog emits signal with paths to that files
    for (int i =0 ; i< files_pahts.size(); i++)
        cout << files_pahts[i].toStdString() << endl;
}

void GUI:: Browse_file()
{
    this->include_file_dialog->show();
}

void GUI::Right_btn()
{
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



