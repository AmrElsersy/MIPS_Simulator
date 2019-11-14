#include "tree_widget.h"

Tree_Widget::Tree_Widget(QWidget *parent) : QTreeWidget(parent)
{
    this->color.setRgb(COLOR);
    this->flag_color = 1;
    this->centerAlign = false;
    this->warning = false;
}


void Tree_Widget::addItem()
{
    QTreeWidgetItem * item = new QTreeWidgetItem(QStringList());
    this->setColor(item);
    this->addTopLevelItem(item);

    this->Items.push_back(item);}

void Tree_Widget::addItem(QStringList s)
{
    QTreeWidgetItem * item = new QTreeWidgetItem(s);

    this->addTopLevelItem(item);
    this->Items.push_back(item);
    this->setColor(item);

    if (this->centerAlign)
        for(int i =0 ; i< item->columnCount(); i++)
            item->setTextAlignment(0,Qt::AlignCenter);

    if (this->warning)
    {
        item->setIcon(1,this->warning_icon);
        item->setTextAlignment(0,Qt::AlignCenter);
    }
}

void Tree_Widget::setColor(QTreeWidgetItem *item)
{
    if (this->flag_color)
    {
        for (int i =0;i <item->columnCount();i++) {
            item->setBackground(i,this->color);
            item->setTextColor(i,QColor(Qt::white));
        }
        this->flag_color = 0;
    }
    else {
        this->flag_color = 1;
    }
}

void Tree_Widget::setTreeColor(QColor clr)
{
    this->color = clr;
}

void Tree_Widget::setAlignCenter(bool center)
{
    this->centerAlign = center;
}

void Tree_Widget::setWarning(bool Warning)
{
    this->warning = Warning;
}

void Tree_Widget::SetHeaderLabels(QTreeWidgetItem *item)
{
    this->setHeaderItem(item);
    if (this->centerAlign)
        for(int i =0 ; i< item->columnCount(); i++)
            item->setTextAlignment(0,Qt::AlignCenter);
}

void Tree_Widget::Clear()
{
    this->clear();
    this->Items.clear();
}
