#include "scene.h"

myScene::myScene(QWidget *parent) : QGraphicsScene(MAX_TOP_LEFT_CORNER,1900,1000,parent)
{
    // cursor for printing points coordinates
    this->cursor = new QGraphicsTextItem();
    this->cursor->setPos(600,-350);
    this->addItem(this->cursor);
    // MIPS Pipeline Image

    // Draw Paths on the Image
    this->IF = new InstructionFetch(this);
    this->ID = new InstructionDecode(this);
    this->EX = new Execution(this);
    this->MEM = new MemoryStage(this);
    this->WB = new WriteBackStage(this);

    // Pipeline Registers
    this->RegistersUnit();
    this->initText();
    this->initColors();

    // Color Array
    this->colors = {QColor(150,0,0) , QColor(Qt::darkCyan), QColor(Qt::blue), QColor(255,100,0), QColor(0,255,255) };
    this->index_color = 0;
    this->PC = 0;
    this->flush  =0 ;
    this->stall= 0;

    // code reading
    this->verilogPath = "C:\\Pipeline\\pc.txt";
    this->regfilePath = "C:\\Pipeline\\regFile.txt";
    this->pipeline_regsPath = "C:\\Pipeline\\regs.txt";
    this->dataMemPath = "C:\\Pipeline\\dataMemory.txt";
    // directions
    this->index = 0;

    this->timer = new QTimer();
    this->timer->setInterval(700);
    connect(this->timer,SIGNAL(timeout()),this,SLOT(continuous_play()));

    this->setBackgroundBrush(QBrush(QColor(Qt::black)));
    this->initButtons();

    //    this->image = QImage("C:\\MIPS_Simulator\\pipeline.jpg");
}

void myScene::updateStagesColors(int direction)
{

    if(direction == 1) // right
    {
        if (this->index == this->max_clocks ) // if index is the last index in clocks vector
        {
            cout << "End of Execution" << endl;
            return;
        }
        this->index++;
    }
    else if (direction == -1) // backward
    {
        if(this->index == 0) // if it is the first state
        {
            cout << "First State cannot go backword" << endl;
            return;
        }
        this->index --;
    }
    this->IF->setStageColor(this->states[index][fetch].color,this->states[index][fetch].MUXs);
    this->ID->setStageColor(this->states[index][decode].color,this->states[index][decode].MUXs);
    this->EX->setStageColor(this->states[index][ex].color,this->states[index][ex].MUXs);
    this->MEM->setStageColor(this->states[index][memory].color,this->states[index][memory].MUXs); // give it the fetch mux as it is connected to it too
    this->WB->setStageColor(this->states[index][wb].color,this->states[index][wb].MUXs);

    this->IF->setTextInstruction(this->states[index][fetch].text_instruction);
    this->ID->setTextInstruction(this->states[index][decode].text_instruction);
    this->EX->setTextInstruction(this->states[index][ex].text_instruction);
    this->MEM->setTextInstruction(this->states[index][memory].text_instruction);
    this->WB->setTextInstruction(this->states[index][wb].text_instruction);

    if (this->index < this->regfile_clocks.size()) // for crashing on last index , you can check also on datamemory_clocks it is the same
    {
        emit updateDataMem(this->datamemory_clocks[this->index]); // updates both data memory and data_mem table
        emit updateRegFile(this->regfile_clocks[this->index]);   // updates only register file
        if(this->index == 0)
            emit updatepipeline_regs(this->pipeline_regs_clocks[(this->index)]);
        else
            emit updatepipeline_regs(this->pipeline_regs_clocks[(this->index)-1]);
        emit updateRegistersGUI();                              // updates only regfile table
    }
//    cout << "progress value= " << this->progressBar->value() << " min=" << this->progressBar->minimum() << " max=" << this->progressBar->maximum() << endl;
    try {

        this->progressBar->setValue(this->index);
//        QCoreApplication::processEvents();
    } catch (...) {
        cout << "ProgressBar is not Ray2..." << this->index << "/" << this->progressBar->value() << endl;
    }
}

void myScene::INIT_Scene(vector<string> Code)
{
    // clear all
    this->index = 0;
    this->states.clear();
    this->initColors();    // initialize black color
    this->clocks_verilog.clear();
    this->regfile_clocks.clear();
    this->datamemory_clocks.clear();
    this->pipeline_regs_clocks.clear();
    this->code.clear();

    // get instruction code
    for (uint i =0; i<Code.size();i++)
        this->code.push_back( Code[i] );
    this->code.push_back("xxxx");

    // read clocks description
    this->ReadClocks(); // read pc.txt
    this->ReadRegFile(); // read regFile.txt
    this->ReadDataMem(); // read dataMemory.txt
    this->Readpipeline_regs();// fill the states
    this->initStates();

    this->index = 0;
    this->progressBar->setRange(0,this->max_clocks-2);
}


void myScene::initStates()
{
    for (uint i = 0 ; i < this->clocks_verilog.size() ; i++)
    {
        vector<state> clock_states(5);  // for 5 stages
        vector<string> pc_stall_flush__muxs = split_string(this->clocks_verilog[i]," ");
        vector<string> one_clock = split_string(pc_stall_flush__muxs[0],","); // PC Stall Flush
        vector<string> muxs = split_string(pc_stall_flush__muxs[1],","); // 4 Selectors MUXs    (PC_MUX , ALU1_MUX,ALU2_MUX ,WB_MUX)

        // PC MUX
        clock_states[fetch].MUXs.push_back(muxs[0]);
        // PC MUX for helping Fetch Stage
        clock_states[decode].MUXs.push_back(muxs[0]);
        // MUX1 , MUX2 ALU
        clock_states[ex].MUXs.push_back(muxs[1]);
        clock_states[ex].MUXs.push_back(muxs[2]);
        // MUX1 , MUX2 ALU for just helping the execute stage
        clock_states[memory].MUXs.push_back(muxs[1]);
        clock_states[memory].MUXs.push_back(muxs[2]);

        clock_states[wb].MUXs.push_back(muxs[3]); // for wb mux
        clock_states[wb].MUXs.push_back(muxs[1]); // for execute help
        clock_states[wb].MUXs.push_back(muxs[2]); // for execute help

        this->PC = stoi(one_clock[PC_POS]);
        this->stall = stoi(one_clock[STALL]);
        this->flush = stoi(one_clock[FLUSH]);

        clock_states[wb].color = this->states[i][memory].color;
        clock_states[memory].color = this->states[i][ex].color;
        clock_states[ex].color = this->states[i][decode].color;
        clock_states[decode].color = this->states[i][fetch].color;

        clock_states[wb].text_instruction = this->states[i][memory].text_instruction;
        clock_states[memory].text_instruction= this->states[i][ex].text_instruction;
        clock_states[ex].text_instruction = this->states[i][decode].text_instruction;
        clock_states[decode].text_instruction = this->states[i][fetch].text_instruction;

        if (this->stall)
        {
            clock_states[ex].color = OFF_COLOR;
            clock_states[ex].text_instruction = QString("**Stall**");

            // remain un changed
            clock_states[fetch].color = this->states[i][fetch].color;
            clock_states[fetch].text_instruction = this->states[i][fetch].text_instruction;
            // remain un changed
            clock_states[decode].color = this->states[i][decode].color;
            clock_states[decode].text_instruction = this->states[i][decode].text_instruction;
        }
        else if (this->flush)
        {
            clock_states[decode].color = FLUSH_COLOR;
            clock_states[decode].text_instruction = QString("**Flush**");
            if(PC < this->code.size())
            {
                clock_states[fetch].color = this->colors[this->index_color];
                clock_states[fetch].text_instruction = QString::fromStdString(this->code[this->PC]);
                index_color ++;
            }
            else
            {
                clock_states[fetch].color = OFF_COLOR;
                clock_states[fetch].text_instruction = QString("xxxx");
            }
        }
        else
        {
            if (this->PC < this->code.size())
            {
                clock_states[fetch].color = this->colors[this->index_color];
                clock_states[fetch].text_instruction = QString::fromStdString(this->code[this->PC]);
                index_color ++;
            }
            else
            {
                clock_states[fetch].color = OFF_COLOR;
                clock_states[fetch].text_instruction = QString("xxxx");
            }
        }
        this->states.push_back(clock_states);

        // if you reach the end of the array return to the first color
        if (this->index_color == 5)
            this->index_color = 0;

    }
}
void myScene::rightButton()
{
    if(this->timer->isActive())
        this->timer->stop();
    this->UpdatePipeline(1);
}
void myScene::leftButton()
{
    if(this->timer->isActive())
        this->timer->stop();
    this->UpdatePipeline(-1);
}
void myScene::playButton()
{
    if(!this->timer->isActive())
        this->timer->start();
    else
        this->timer->stop();
}

void myScene::continuous_play()
{
    if(this->index == this->max_clocks)
    {
        this->timer->stop();
        return;
    }
    this->UpdatePipeline(1);
}


void myScene::initButtons()
{
    QWidget* buttons_widget = new QWidget();
    QGridLayout* layout = new QGridLayout();
    buttons_widget->setStyleSheet("background-color:#035c70; border-radius:5px;");

    this->progressBar = new QProgressBar();
    this->progressBar->setRange(0,100);
    this->progressBar->setStyleSheet("QProgressBar{border: 2px solid grey;border-radius: 5px;text-align: center;background-color:rgb(15,15,15); margin-bottom:5px;}"
                                     "QProgressBar::chunk{background-color:#fff;};");//#55cc00
    this->progressBar->setValue(0);
    QPushButton* right_btn = new QPushButton("");
    QPushButton* left_btn = new QPushButton("");
    QPushButton* play_btn = new QPushButton("");

    right_btn->setIcon(QIcon("C:\\MIPS_Simulator\\icons\\right.png"));
    left_btn->setIcon(QIcon("C:\\MIPS_Simulator\\icons\\left.png"));
    play_btn->setIcon(QIcon("C:\\MIPS_Simulator\\icons\\play.png"));

    right_btn->setStyleSheet("border:0; background:#01333e; padding: 5px;");
    left_btn->setStyleSheet("border:0; background:#01333e; padding: 5px;");
    play_btn->setStyleSheet("border:0; background:#01333e; padding: 5px;");

    right_btn->setIconSize(QSize(30,30));
    left_btn->setIconSize(QSize(30,30));
    play_btn->setIconSize(QSize(30,30));

    connect(right_btn,SIGNAL(pressed()),this,SLOT(rightButton()));
    connect(left_btn,SIGNAL(pressed()),this,SLOT(leftButton()));
    connect(play_btn,SIGNAL(pressed()),this,SLOT(playButton()));

    layout->addWidget(this->progressBar,0,0,1,-1);
    layout->addWidget(left_btn,1,0);
    layout->addWidget(play_btn,1,1);
    layout->addWidget(right_btn,1,2);

    buttons_widget->setLayout(layout);
    this->addWidget(buttons_widget)->setPos(-920,420);
}
void myScene::ReadClocks()
{
    this->verilog_file.open(this->verilogPath);
    string s;
    while (getline(this->verilog_file,s))
    {
        if (s == "" || s == " ")
            continue;
        this->clocks_verilog.push_back(s);
    }

    for (uint i =0 ; i< this->clocks_verilog.size() ; i++ )
        cout << this->clocks_verilog[i] << "   " ;
    cout << endl;
    this->max_clocks = this->clocks_verilog.size();
    cout << "clocks = " << this->max_clocks << endl;
    this->verilog_file.close();
}
void myScene::ReadRegFile()
{
    this->regfile_file.open(this->regfilePath);if (!this->regfile_file.is_open()) { cout << "cannot open regfile.txt Pipeline" << endl;}
    string s;
    while (getline(this->regfile_file,s))
        this->regfile_clocks.push_back(s);
    this->regfile_file.close();
}
void myScene::Readpipeline_regs()
{
    this->pipeline_regs_file.open(this->pipeline_regsPath);if (!this->pipeline_regs_file.is_open()) { cout << "cannot open regfile.txt Pipeline" << endl;}
    string s;
    while (getline(this->pipeline_regs_file,s))
        this->pipeline_regs_clocks.push_back(s);

}

void myScene::ReadDataMem()
{
    this->dataMem_file.open(this->dataMemPath); if (!this->dataMem_file.is_open()) { cout << "cannot open dataMemory.txt Pipeline" << endl; }
    string s;
    while (getline(this->dataMem_file,s))
        this->datamemory_clocks.push_back(s);

    for (uint i =0 ; i< this->datamemory_clocks.size() ; i++ )
        cout << this->datamemory_clocks[i] << "   " ;
    cout << endl;
    cout << "data memory size=" << datamemory_clocks.size() << endl;

    this->dataMem_file.close();
}
void myScene::UpdatePipeline(int direction)
{
    this->updateStagesColors(direction);
}
void myScene::RegistersUnit()
{
    // Registers_Unit
    this->IF_ID = new QGraphicsRectItem(-673,-128,50,500);
    this->IF_ID->setPen(QPen(QBrush(REGISTERS_COLOR ,Qt::SolidPattern),5));
    this->addItem(this->IF_ID);

    this->ID_EX= new QGraphicsRectItem(-39,-283,50,655);
    this->ID_EX->setPen(QPen(QBrush(REGISTERS_COLOR ,Qt::SolidPattern),5));
    this->addItem(this->ID_EX);

    this->EX_MEM= new QGraphicsRectItem(414,-230,48,603);
    this->EX_MEM->setPen(QPen(QBrush(REGISTERS_COLOR ,Qt::SolidPattern),5));
    this->addItem(this->EX_MEM);

    this->MEM_WB= new QGraphicsRectItem(711,-179,48,555);
    this->MEM_WB->setPen(QPen(QBrush(REGISTERS_COLOR ,Qt::SolidPattern),5));
    this->addItem(this->MEM_WB);
}
void myScene::initColors()
{
    this->IF->setStageColor(QColor(INITAIL_COLOR));
    this->ID->setStageColor(QColor(INITAIL_COLOR));
    this->EX->setStageColor(QColor(INITAIL_COLOR));
    this->MEM->setStageColor(QColor(INITAIL_COLOR));
    this->WB->setStageColor(QColor(INITAIL_COLOR));

    this->states.push_back({state(QColor(INITAIL_COLOR)) , state(QColor(INITAIL_COLOR)) , state(QColor(INITAIL_COLOR)) ,state(QColor(INITAIL_COLOR)) ,state(QColor(INITAIL_COLOR))});

}
void myScene::initText()
{
    // Memory Allocation
    this->IFID_txt = new QGraphicsTextItem("IF/ID");
    this->IDEX_txt = new QGraphicsTextItem("ID/EX");
    this->EXMEM_txt = new QGraphicsTextItem("EX/MEM");
    this->MEMWB_txt = new QGraphicsTextItem("MEM/WB");
    // Set Position
    this->IFID_txt->setPos(-675,-160);
    this->IDEX_txt->setPos(-40,-315);
    this->EXMEM_txt->setPos(395,-260);
    this->MEMWB_txt->setPos(685,-210);
    // Set Font
    this->IFID_txt->setFont(QFont("Arial",FONT_SIZE,QFont::Bold));
    this->IDEX_txt->setFont(QFont("Arial",FONT_SIZE,QFont::Bold));
    this->EXMEM_txt->setFont(QFont("Arial",FONT_SIZE,QFont::Bold));
    this->MEMWB_txt->setFont(QFont("Arial",FONT_SIZE,QFont::Bold));
    //Set Color
    this->IFID_txt->setDefaultTextColor(TEXT_COLOR);
    this->IDEX_txt->setDefaultTextColor(TEXT_COLOR);
    this->EXMEM_txt->setDefaultTextColor(TEXT_COLOR);
    this->MEMWB_txt->setDefaultTextColor(TEXT_COLOR);
    // Add to the Scene
    this->addItem(this->IFID_txt);
    this->addItem(this->IDEX_txt);
    this->addItem(this->EXMEM_txt);
    this->addItem(this->MEMWB_txt);

    //  ========== MUXs ================
    this->MUX_txt = new QGraphicsTextItem*[MUXs_SIZE] ;
    for(int i =0 ; i< MUXs_SIZE ; i++)
        this->MUX_txt[i] = new QGraphicsTextItem("M\nU\nX");
    // Set Font Size and Bold State
    for (int i = 0 ; i< MUXs_SIZE ; i++)
    {
        this->MUX_txt[i]->setFont(QFont("Arial",FONT_SIZE,QFont::Bold));
        this->MUX_txt[i]->setDefaultTextColor(QColor(TEXT_COLOR));
    }
    // Set Position
    this->MUX_txt[0]->setPos(-147,-246);
    this->MUX_txt[1]->setPos(187,-85);
    this->MUX_txt[2]->setPos(187,75);
    this->MUX_txt[3]->setPos(187,290);
    this->MUX_txt[4]->setPos(828,18);
    this->MUX_txt[5]->setPos(-813,-280);

    for (int i =0 ; i< MUXs_SIZE; i++)
        this->addItem(this->MUX_txt[i]);
    // ====================================
}
void myScene::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Space)
        cout << endl;
}
void myScene::addNewItem(QGraphicsItem * item)
{
    this->addItem(item);
}
vector<string> myScene :: split_string(string s,string splitter)
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
QString myScene::parasePC_Text(string pc)
{
    try {
        return QString::fromStdString(this->code[stoi(pc)]);
    } catch (...) {
        return QString("xxxx");
    }
}
void myScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QPointF point = event->scenePos();
    // string the point
    QString s = QString::number(point.x()) + "," + QString :: number(point.y()) ;
    this->cursor->setPlainText(s);
    // print the QGraphicText Item on the Scene
    //    this->cursor->setPos(QPointF(point.x()+10,point.y())); // +10 for visualization only
    //    cout << "Scene(" << point.x() << "," << point.y() << ")" << endl;
}
void myScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    QPointF point = mouseEvent->scenePos();
    QString s = QString::number(point.x()) + "," + QString :: number(point.y()) ;
    this->cursor->setPlainText(s);
    this->cursor->setPos(QPointF(point.x()+10,point.y())); // +10 for visualization only
    cout << point.x() << "," << point.y() << endl ;
}
//void myScene::drawBackground(QPainter *painter, const QRectF &rect)
//{
//    painter->drawImage(QRectF(-1000,-475,1800,1000),this->image);
//}
//void myScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
//{
//    QPointF point = event->scenePos();
//    // string the point
//    QString s = QString::number(point.x()) + "," + QString :: number(point.y()) ;
//    this->cursor->setPlainText(s);
//    // print the QGraphicText Item on the Scene
//    //    this->cursor->setPos(QPointF(point.x()+10,point.y())); // +10 for visualization only
//    cout << "\"" << point.x() << "," << point.y() << "\" ," ;
//}
