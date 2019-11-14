#include "testcase.h"

TestCase::TestCase(string Name,QWidget *parent) : QWidget (parent)
{
    // Memory Allocation
    this->file_tester = new filesTester();
    this->layout = new QHBoxLayout();
    this->setLayout(this->layout);
    this->label  = new QLabel();
    this->BtnDetails = new QPushButton("Details");
    this -> BtnDetails->setStyleSheet("background-color: #01333e;"
                                      "padding: 2px 0;"
                                      "color: white;"
                                     "border-radius: 5%;"
                                      "font-weight: bold;");
    this->BtnTest    = new QPushButton("Test");
    this->BtnTest->setStyleSheet("background-color: #01333e;"
                                 "padding: 2px 0;"
                                  "color: white;"
                                 "border-radius: 5%;"
                                 "font-weight: bold;");
    this->id = 0;
    // label init
    this->name = Name;
    this->label->setText(QString::fromStdString (this->name) ) ;
    this->label->setStyleSheet("background-color: #024e5f;"
                               "color: white;"
                               "border-radius: 5%;"
                               "font-weight: 400;");
    this->label->setAlignment(Qt::AlignCenter);
    // Design Layout
    this->layout->addWidget(this->label);
    this->layout->setStretch(0,50); // index 0 is label ,,, any big value is good for dominant stretching for label
    this->layout->addWidget(this->BtnDetails,Qt::AlignRight);
    this->layout->addWidget(this->BtnTest,Qt::AlignRight);
    this->setMinimumWidth(MIN_WIDTH);
    this->setMaximumHeight(TEST_HIGHT);

    this->init_details_widget();
    this->ObserverPattern();
    this->show();
}

void TestCase::ObserverPattern()
{
    connect(this->BtnDetails,SIGNAL(clicked()) ,this,SLOT(show_details()));
    connect(this->BtnTest   ,SIGNAL(clicked()) ,this,SLOT(start_simulating()));
    connect(this->file_tester,SIGNAL(set_status(bool)) ,this,SLOT( Test_Result(bool) ) );
    connect(this->file_tester,SIGNAL(set_reg_warnings (vector<string>)) ,this,SLOT( Set_Warnings_RegFile(vector<string>) ) );
    connect(this->file_tester,SIGNAL(set_data_warnings(vector<string>)) ,this,SLOT( Set_Warnings_DataMem(vector<string>) ) );

}

void TestCase::init_details_widget()
{
    // Memory Allocation
    this->DetailsWidget = new QTabWidget(); // don't put ("this" as parent) we want it hidden
    this->RegFile_Details = new Tree_Widget();
    this->DetailsWidget->setStyleSheet("background: rgb(255, 51,85);"
                                       "color: rgb(128,0,0);"
                                       "font-weight: 400;"
                                       "border-radius: 5%");
    this->DataMem_Details = new Tree_Widget();
    // Design of RegFile
    this->RegFile_Details->setColumnCount(2);
    this->RegFile_Details->setHeaderLabels ( {"    ID","Warnings"} );
    this->RegFile_Details->setStyleSheet("QTreeView::item {padding: 15px; color: black; font-weight: bold;}");
    this->RegFile_Details->setTreeColor(QColor(255,255,0));
    this->RegFile_Details->setWarning(true);
    this->RegFile_Details->headerItem()->setTextAlignment(0,Qt::AlignCenter);
    this->RegFile_Details->headerItem()->setTextAlignment(1,Qt::AlignCenter);
    // Design of DataMem
    this->DataMem_Details->setColumnCount(2);
    this->DataMem_Details->setHeaderLabels ( {"    ID","Warnings"} );
    this->DataMem_Details->setStyleSheet("QTreeView::item {padding: 15px; color: black; font-weight: bold;}");
    this->DataMem_Details->setTreeColor(QColor(255,255,0));
    this->DataMem_Details->setWarning(true);
    this->DataMem_Details->headerItem()->setTextAlignment(0,Qt::AlignCenter);
    this->DataMem_Details->headerItem()->setTextAlignment(1,Qt::AlignCenter);
    // Design
    this->DetailsWidget->addTab(this->RegFile_Details,"RegFile");
    this->DetailsWidget->addTab(this->DataMem_Details,"DataMemory");
    this->DetailsWidget->setMovable(true);
    this->DetailsWidget->setMinimumWidth(DETAILS_WIDTH);
    this->DetailsWidget->setMinimumHeight(DETAILS_HIGHT);
}
void TestCase::show_details()
{
    this->DetailsWidget->show();
}
void TestCase::Test_Result(bool result)
{
    this->testCaseState = result;
    if (this->testCaseState) // if sucess
        this->label->setStyleSheet("background-color: rgb(0,153,0);"
                                   "color: white;"
                                   "border-radius: 5%;"
                                   "font-weight: 400;");
    else
        this->label->setStyleSheet("background-color: rgb(153,0,0);"
                                   "color: white;"
                                   "border-radius: 5%;"
                                   "font-weight: 400;");
}

void TestCase::start_simulating()
{
    emit startSimulationTestCase(this->assembly_path);
    this->file_tester->StartTest();
}
void TestCase::setPaths(string assembly, string regfile, string datamem)
{
    this->file_tester->set_paths(regfile,datamem);
    this->assembly_path = assembly;
}
void TestCase::Set_Warnings_RegFile(vector<string> warnings)
{
    this->RegFile_Details->Clear();
    for (uint i =0 ; i< warnings.size() ; i++)
    {
        this->id ++ ;
        QStringList Warning ;
        // id , Warning ICon , Warning
        Warning.append( QString::fromStdString(to_string(this->id)) );
        Warning.append( QString::fromStdString("Warning : : " + warnings[i]) );
        // add the colomns item to RegFile
        this->RegFile_Details->addItem(Warning);
    }
}
void TestCase::Set_Warnings_DataMem(vector<string> warnings)
{
    this->DataMem_Details->Clear();
    for (uint i =0 ; i< warnings.size() ; i++)
    {
        this->id ++ ;
        QStringList Warning ;
        // id , Warning ICon , Warning
        Warning.append( QString::fromStdString(to_string(this->id)) );
        Warning.append( QString::fromStdString("Warning : : " + warnings[i]) );
        // add the colomns item to RegFile
        this->DataMem_Details->addItem(Warning);
    }
}

