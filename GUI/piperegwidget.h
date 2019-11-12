#ifndef PIPEREGWIDGET_H
#define PIPEREGWIDGET_H
#include "tree_widget.h"
#include <QObject>
#define VALUE_INDEX 2
class PipeRegWidget : public Tree_Widget
{
    Q_OBJECT
public:
    PipeRegWidget(QWidget* parent= nullptr);
    vector<string> split_string(string s,string splitter);
public slots:
    void updatePipeRegistersWidget(string s);
};

#endif // PIPEREGWIDGET_H
