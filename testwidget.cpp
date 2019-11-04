#include "testwidget.h"

TestWidget::TestWidget(QWidget *parent) : QWidget(parent)
{
    this->centralLayout = new QVBoxLayout();
    this->grid = new QGridLayout();
    this->grid_widget = new QWidget();
    this->testcasesWidget = new QWidget();
    this->testCasesLayout = new QVBoxLayout();
    this->scroll = new QScrollArea();

    this->RegFileBrowse = new BrowseFile ("RegFile:");
    this->DataMemBrowse = new BrowseFile ("DataMemory:");
    this->AssemblyBrowse= new BrowseFile ("Asembly:");
    this->AddTestBtn = new QPushButton("Add TestCase");
    this->AddTestBtn->setStyleSheet("background-color:white;");
    this->TestAllBtn = new QPushButton("Test All");

    this->Design();
    this->ObserverPattern();
    this->addOriginalTestCases();
}

void TestWidget::Design()
{
    this->grid->addWidget(this->AssemblyBrowse,0,0,1,-1);
    this->grid->addWidget(this->RegFileBrowse ,1,0,1,-1);
    this->grid->addWidget(this->DataMemBrowse ,2,0,1,-1);
    this->grid->addWidget(this->AddTestBtn,3,0);
    this->grid->addWidget(this->TestAllBtn,3,1);

    this->testcasesWidget->setLayout(this->testCasesLayout);
    this->grid_widget->setLayout(this->grid);
    scroll->setWidget(this->testcasesWidget);
    scroll->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scroll->setWidgetResizable(true);

    this->centralLayout->addWidget(this->grid_widget);
    this->centralLayout->addWidget(this->scroll);

    this->setLayout(this->centralLayout);
    this->testcasesWidget->setStyleSheet("background-color:white;");
    this->grid_widget->setStyleSheet("background-color:blue");
}

void TestWidget::ObserverPattern()
{
    connect(this->AddTestBtn,SIGNAL(clicked()),this,SLOT(addTestCase()));
    connect(this->TestAllBtn,SIGNAL(clicked()),this,SLOT(TestAll()));
}

void TestWidget::addTestCase()
{
    if (this->RegFileBrowse->isSelectedPath() && this->DataMemBrowse->isSelectedPath() && this->AssemblyBrowse->isSelectedPath())
    {
        // add new TestCase
        ulong id = this->TestCases.size();
        TestCase* testcase = new TestCase("TestCase"+to_string(id+1));
        this->testCasesLayout->addWidget(testcase);
        this->TestCases.push_back(testcase);

        // set Paths
        testcase->setPaths(this->AssemblyBrowse->getText() , this-> RegFileBrowse->getText() , this->DataMemBrowse->getText());

        // clear the browsers
        this->RegFileBrowse->clear();
        this->DataMemBrowse->clear();
        this->AssemblyBrowse->clear();

        connect(testcase,SIGNAL(startSimulationTestCase(string)),this,SLOT(oneShootTest(string)));
    }
    else {
        emit output_screen({"Error in Adding NewTask ... Please Add All Required Files"});
    }
}

void TestWidget::oneShootTest(string Assembly)
{
    emit start_simulation(Assembly);
}

void TestWidget::TestAll()
{
    for (uint i = 0 ; i< TestCases.size() ; i++)
    {
        emit start_simulation(TestCases[i]->assembly_path);
        TestCases[i]->file_tester->StartTest();
    }
    emit output_screen({"Testing is Done"});

}


void TestWidget::addOriginalTestCases()
{
    string folder_path = TESTCASES_PATH;
    string assembly = "assembly.txt" , regFie = "regFile.txt" , datamem = "dataMemory.txt";
    for (int i =1 ; i <= TEST_CASES ; i++)
    {
        string path = folder_path + to_string(i) + "\\";
        // add new testCase in Design
        ulong id = this->TestCases.size();
        TestCase* testcase = new TestCase("TestCase"+to_string(id+1));
        this->testCasesLayout->addWidget(testcase);
        this->TestCases.push_back(testcase);
        // set paths
        testcase->setPaths(path+assembly  ,path+regFie  ,path+datamem);
        connect(testcase,SIGNAL(startSimulationTestCase(string)),this,SLOT(oneShootTest(string)));
    }

}

