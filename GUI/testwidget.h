#ifndef TESTWIDGET_H
#define TESTWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QFileDialog>
#include "testcase.h"
#include "browsefile.h"
#include <QScrollArea>
#include <QCoreApplication>
#define TEST_CASES 5
#define TESTCASES_PATH "C:\\MIPS_Simulator\\TestCases\\TestCase"

class TestWidget : public QWidget
{
    Q_OBJECT
private:
    QVBoxLayout* centralLayout;
    QWidget* grid_widget ;
    QGridLayout* grid;
    QWidget* testcasesWidget;
    QScrollArea* scroll;
    QVBoxLayout* testCasesLayout ;
    BrowseFile* RegFileBrowse;
    BrowseFile* DataMemBrowse;
    BrowseFile* AssemblyBrowse;
    QPushButton* AddTestBtn;
    QPushButton* TestAllBtn;
    vector<TestCase*> TestCases;

public:
    TestWidget(QWidget *parent = nullptr);
    void Design();
    void ObserverPattern();
    void addOriginalTestCases();
signals:
    void start_simulation(string);
    void Start_simulation();
    void output_screen(vector<string>);
public slots:
    void addTestCase();
    void oneShootTest(string);
    void TestAll();
};

#endif // TESTWIDGET_H
