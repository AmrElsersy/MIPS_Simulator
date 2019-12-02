#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <bits/stdc++.h>
#include <QMainWindow>
#include "gui.h"
#include <QEvent>
#include <QKeyEvent>
#include "Pipeline/scene.h"
#include <QSpinBox>
using namespace  std;
class View :public QGraphicsView
{
    Q_OBJECT
public:
    myScene *      my_scene;
    View(QWidget *parent = nullptr);
protected:
    void keyPressEvent(QKeyEvent *event);
signals:
    void updatePipeline(int);

};

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    GUI* gui ;
    View* pipeline_gui;
    void ray2();
public:
    explicit MainWindow(QWidget *parent = nullptr);
public slots:
    void show_pipeline_gui();
    void slot_ray2(QAction*);
};

#endif // MAINWINDOW_H
