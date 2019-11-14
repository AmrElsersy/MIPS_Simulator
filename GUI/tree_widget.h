#ifndef TREE_WIDGET_H
#define TREE_WIDGET_H

#define REGISTER "registers"
#define EXECUTE "Execute"
#define DATA_MEMORY "data_memory"
#define ADDRESS 0
#define CODE 1
#define INSTRUCTION 2
#define RegisterValue 2
#define RegisterName 0
#define COLOR 3,92,112

#include "register.h"
#include <bits/stdc++.h>
#include <QObject>
#include <QWidget>
#include <QTabWidget>
#include <QTextEdit>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QStringList>
#include <QIcon>
#include <QCoreApplication>
#define ICON_PATH "/home/amrelsersy/warning-icon.png"
using namespace std;

class Tree_Widget : public QTreeWidget
{
    Q_OBJECT
protected:
    QIcon warning_icon;
    bool centerAlign;
    bool warning;
    QColor color;
    bool flag_color;
    vector<QTreeWidgetItem*> Items ;

public:
    explicit Tree_Widget(QWidget *parent = nullptr);
    void addItem();
    void addItem(QStringList);
    void setColor(QTreeWidgetItem*item);
    void setTreeColor(QColor);
    void setAlignCenter(bool);
    void setWarning(bool);
    void SetHeaderLabels(QTreeWidgetItem*item); // not useds
    void Clear();


};

#endif // TREE_WIDGET_H
