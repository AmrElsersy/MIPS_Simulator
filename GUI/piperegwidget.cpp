#include "piperegwidget.h"

PipeRegWidget::PipeRegWidget(QWidget *parent) : Tree_Widget(parent)
{
    this->setColumnCount(3);
    this->setHeaderLabels({"PipelineRegister","Name","Value"});
    this->addItem({"IF_ID","Instruction","0"});
    this->addItem({"ID_EX","Reg Write","0"});
    this->addItem({"ID_EX","MEM to Reg","0"});
    this->addItem({"ID_EX","MEM Write","0"});
    this->addItem({"ID_EX","MEM Read","0"});
    this->addItem({"ID_EX","ALU Src","0"});
    this->addItem({"ID_EX","Reg dest","0"});
    this->addItem({"ID_EX","ALU Op","0"});
    this->addItem({"ID_EX","Read Data1","0"});
    this->addItem({"ID_EX","Read Data2","0"});
    this->addItem({"ID_EX","Shift Amount","0"});
    this->addItem({"ID_EX","Sign Extend","0"});
    this->addItem({"ID_EX","Rs","0"});
    this->addItem({"ID_EX","Rt","0"});
    this->addItem({"ID_EX","Rd","0"});
    this->addItem({"ID_EX","Func Field","0"});
    this->addItem({"EX_MEM","Reg Write","0"});
    this->addItem({"EX_MEM","MEM to Reg","0"});
    this->addItem({"EX_MEM","MEM Write","0"});
    this->addItem({"EX_MEM","MEM Read","0"});
    this->addItem({"EX_MEM","ALU Result","0"});
    this->addItem({"EX_MEM","Lower mux","0"});
    this->addItem({"EX_MEM","Reg Dest","0"});
    this->addItem({"MEM_WB","Reg write","0"});
    this->addItem({"MEM_WB","MEM to Reg","0"});
    this->addItem({"MEM_WB","Read Data","0"});
    this->addItem({"MEM_WB","ALU Result","0"});
    this->addItem({"MEM_WB","Reg Dest","0"});
}

void PipeRegWidget::updatePipeRegistersWidget(string s)
{
    vector<string> clocks;
    if (s == "" || s == " ")
        return;
    vector<string> values = split_string(s,",");
    for(uint i=0; i< values.size();i++)
        clocks.push_back(values[i]);

    for (uint i = 0; i < clocks.size(); ++i)
    {
        try {
            this->Items[i]->setText(VALUE_INDEX,QString::fromStdString( (clocks[i]) )  );
        } catch (...) {
            cout << "error in updatePipelineRegWidget" << endl;
        }
    }
}
vector<string> PipeRegWidget:: split_string(string s,string splitter)
{
    vector<string> splitted;
    uint n = s.length();
    int pos = 0;
    int start = pos;
    while (pos != string::npos)
    {
        pos = s.find_first_of(splitter,pos+1);
        string splitted_string = s.substr(start,pos-start);
        splitted.push_back(splitted_string);
        start = pos+1;
    }

    return splitted;}
