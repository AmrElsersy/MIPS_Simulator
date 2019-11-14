#include "instructiondecode.h"

InstructionDecode::InstructionDecode(QObject *parent) : Stage (parent)
{
    // Register File
    this->RegFile = new QGraphicsRectItem(-321,-100,155,220);
    this->RegFile->setPen(QPen(QBrush(Qt::red,Qt::SolidPattern),5));
    emit addnewItem(this->RegFile);
    // Hazard Unit
    painter.addRoundRect(-543,-436,195,97,75);
    this->Hazard_Unit = new QGraphicsPathItem(painter);
    this->Hazard_Unit->setPen(QPen(QBrush(Qt::red,Qt::SolidPattern),5));
    emit addnewItem(this->Hazard_Unit);
    // Control Unit
    this->Control = new QGraphicsEllipseItem(-413,-283,95,145);
    this->Control->setPen(QPen(QBrush(Qt::red,Qt::SolidPattern),6));
    emit addnewItem(this->Control);
    // Comparator
    this->Comparator = new QGraphicsEllipseItem(-130,-30,50,50);
    this->Comparator->setPen(QPen(QBrush(Qt::red,Qt::SolidPattern),6));
    emit addnewItem(this->Comparator);
    // Mux
    painter.addRoundRect(-154,-268,38,122,105);
    this->Mux_Control_IDEX = new QGraphicsPathItem(painter);
    this->Mux_Control_IDEX->setPen(QPen(QBrush(Qt::red,Qt::SolidPattern),5));
    emit addnewItem(Mux_Control_IDEX);

    // Paths
    this->paths["Instruction_Control"] = newPath( {"-594,-213" ,"-416,-212" });
    this->paths["ReadReg1"] = newPath( {"-592,-72" ,"-325,-72" });
    this->paths["ReadReg2"] = newPath( { "-593,-13" ,"-325,-13" });
    this->paths["ReadData1"] = newPath( { "-163,-74" ,"-41,-74"  });
    this->paths["ReadData2"] = newPath( { "-161,72" ,"-43,72"  });
    this->paths["Control_Mux"] = newPath( {"-315,-210" ,"-156,-210" });
    this->paths["Hazard_Mux"] = newPath( {"-347,-373" ,"-133,-373" ,"-133,-275"  });    this->paths["Hazard_Mux"]->setWidth(1);
    this->paths["rt_Hazard"] = newPath( {"-592,-380" ,"-546,-380"  });
    this->paths["rd_Hazard"] = newPath( {"-592,-357" ,"-542,-357"  });

    // ************** the same Path *******************
    this->paths["Mux_M"] = newPath( {"-113,-208" ,"-41,-208"  });
    this->paths["Mux_EX"] = newPath( {"-79,-207" ,"-79,-146" ,"-43,-146" });
    this->paths["Mux_WB"] = newPath( {"-79,-209" ,"-79,-266" ,"-44,-266" });
    // ************************************************

    this->paths["IFID_IDEX_rt"] = newPath({ "-594,325" ,"-41,325"  });
    this->paths["IFID_IDEX_rs"] = newPath({  "-41,296" ,"-592,296" });
    this->paths["IFID_IDEX_rd"] = newPath({  "-593,265" ,"-40,265" });

    // None Colored Wires (Not Important)
    this->paths["Instruction"] = newPath({  "-593,-380" ,"-593,326" });
    this->paths["IFID_Instruction"] = newPath({ "-593,93" ,"-622,93" });
    this->paths["zero_MUX"] = newPath({"-155,-180" ,"-187,-180"});

    // comparator
    this->paths["comparator_up"] = newPath({"-105,-30" ,"-105,-73"});
    this->paths["comparator_down"] = newPath({"-105,20" ,"-105,71"});
    this->paths["comparator_3alama1"] = newPath({"-112,-10" ,"-99,-10"});
    this->paths["comparator_3alama2"] = newPath({"-112,-2","-99,-2"});

    // path for MUX
    this->paths["Control_pcMUX"] = newPath( {"-324,-245" ,"-255,-245" ,"-255,-316" ,"-737,-316" ,"-737,-283" ,"-781,-283" });
     // ====================== Text  (Not Important) =============================
    this->Control_txt = new QGraphicsTextItem("Control");
    this->RegFile_txt = new QGraphicsTextItem("RegisterFile");
    this->HazardUnit_txt = new QGraphicsTextItem("Hazard Unit");
    this->Control_txt->setDefaultTextColor(QColor(TEXT_COLOR));
    this->RegFile_txt->setDefaultTextColor(QColor(TEXT_COLOR));
    this->HazardUnit_txt->setDefaultTextColor(QColor(TEXT_COLOR));

    this->RegFile_txt->setPos(-307,-15);
    this->Control_txt->setPos(-405,-225);
    this->HazardUnit_txt->setPos(-505,-405);
    this->RegFile_txt->setFont(QFont("Arial",FONT_SIZE,QFont::Bold));
    this->Control_txt->setFont(QFont("Arial",FONT_SIZE,QFont::Bold));
    this->HazardUnit_txt->setFont(QFont("Arial",FONT_SIZE,QFont::Bold));
    emit addnewItem(this->Control_txt);
    emit addnewItem(this->RegFile_txt);
    emit addnewItem(this->HazardUnit_txt);

    // ===== Text Instruction ============
    this->text_instruction->setPlainText("InstructionDecode");
    this->text_instruction->setFont(QFont("Arial",FONT_SIZE,QFont::Bold));
    this->text_instruction->setDefaultTextColor(TEXT_COLOR);
    this->text_instruction->setPos(-300,-350);
    emit addnewItem(this->text_instruction);

    // mlhomsh 3aza
    this->zero= new QGraphicsTextItem("0");
    this->zero->setFont(QFont("Arial",FONT_SIZE,QFont::Bold));
    this->zero->setDefaultTextColor(TEXT_COLOR);
    this->zero->setPos(-190,-180);
    emit addnewItem(this->zero);
}

void InstructionDecode::setStageColor(QColor clr,vector<string> muxs)
{
    // set Pen Color and Width
    this->color = clr;
    this->pen.setColor(this->color);
    this->pen.setWidth(PATH_WIDTH);
    // set Pen to all Components
    this->RegFile->setPen(this->pen);
    this->Hazard_Unit->setPen(this->pen);
    this->Control->setPen(this->pen);
    this->Mux_Control_IDEX->setPen(this->pen);
    this->Comparator->setPen(this->pen);

    // Paths
    for (auto i = this->paths.begin(); i != this->paths.end(); i++)
        i->second->setPen(this->pen);
    this->text_instruction->setDefaultTextColor(this->color);

    if (muxs.size() >0)
    {
        string mux = muxs[0]; // we have wire linked to the mux in the PC MUX in IF Stage
        if (mux == "0")
        {
            this->paths["Control_pcMUX"]->setColor(OFF_COLOR);
//            cout << "ID MUX:" << mux << endl;
        }
    }

}
