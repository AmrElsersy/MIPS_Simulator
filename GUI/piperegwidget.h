#ifndef PIPEREGWIDGET_H
#define PIPEREGWIDGET_H
#include "tree_widget.h"
#define VALUE_INDEX 2
class PipeRegWidget : public Tree_Widget
{
public:
    PipeRegWidget(QWidget* parent= nullptr);
public slots:
    void updatePipeRegistersWidget(vector<string>);
};

#endif // PIPEREGWIDGET_H
