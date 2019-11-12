#include "piperegwidget.h"

PipeRegWidget::PipeRegWidget(QWidget *parent) : Tree_Widget(parent)
{
    this->setColumnCount(3);
    this->setHeaderLabels({"  PipelineRegister","Name","Value"});
    this->addItem({"IF_ID","Instruction","0"});
    this->addItem({"ID_EX","register write","0"});
    this->addItem({"ID_EX","memory to register","0"});
    this->addItem({"ID_EX","memory write","0"});
    this->addItem({"ID_EX","memory read","0"});
    this->addItem({"ID_EX","alu source","0"});
    this->addItem({"ID_EX","register destination","0"});
    this->addItem({"ID_EX","alu op","0"});
    this->addItem({"ID_EX","read data 1","0"});
    this->addItem({"ID_EX","read data 2","0"});
    this->addItem({"ID_EX","shift amount","0"});
    this->addItem({"ID_EX","sign extend","0"});
    this->addItem({"ID_EX","rs","0"});
    this->addItem({"ID_EX","rt","0"});
    this->addItem({"ID_EX","rd","0"});
    this->addItem({"ID_EX","function field","0"});
    this->addItem({"EX_MEM","register write","0"});
    this->addItem({"EX_MEM","memort to register","0"});
    this->addItem({"EX_MEM","memory write","0"});
    this->addItem({"EX_MEM","memory read","0"});
    this->addItem({"EX_MEM","alun result","0"});
    this->addItem({"EX_MEM","lower mux output","0"});
    this->addItem({"EX_MEM","register destination","0"});
    this->addItem({"MEM_WB","register write","0"});
    this->addItem({"MEM_WB","memory to register","0"});
    this->addItem({"MEM_WB","read data","0"});
    this->addItem({"MEM_WB","alu result","0"});
    this->addItem({"MEM_WB","register destination","0"});

}

void PipeRegWidget::updatePipeRegistersWidget(vector<string> clocks)
{
    for (uint i = 0; i < clocks.size(); ++i)
    {
        try {
            this->Items[i]->setText(VALUE_INDEX,QString::fromStdString( (clocks[i]) )  );
        } catch (...) {
            cout << "error in updatePipelineRegWidget" << endl;
        }
    }
}
