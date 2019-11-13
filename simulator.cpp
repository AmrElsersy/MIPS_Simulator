#include "simulator.h"

Simulator::Simulator()
{
    // ===================== Linux ==================================
    //    this->file_assembly_path   = "/home/amrelsersy/ins.txt";
    //    this->file_regFile_path    = "/home/amrelsersy/regFile.txt";
    //    this->file_dataMemory_path = "/home/amrelsersy/dataMemory.txt";

    this->modelsim_path = "C:\\MIPS";
    this->modelsim_pipeline_path = "C:\\Pipeline";
    this->file_assembly_path   = (this->modelsim_path + "\\ins.txt").toStdString();
    this->file_regFile_path    = (this->modelsim_path + "\\regFile.txt").toStdString();
    this->file_dataMemory_path = (this->modelsim_path + "\\dataMemory.txt").toStdString();

    this->file_assembly_path_pipeline   = (this->modelsim_pipeline_path + "\\ins.txt").toStdString();
    this->file_regFile_path_pipeline    = (this->modelsim_pipeline_path + "\\regFile.txt").toStdString();
    this->file_dataMemory_path_pipeline = (this->modelsim_pipeline_path + "\\dataMemory.txt").toStdString();

    this->modelsim_process = new QProcess();
    this->modelsim_pipeline_process = new QProcess();
    // ==================== Run ModelSim ============================
    this->modelsim_process->setWorkingDirectory(this->modelsim_path);
    this->modelsim_pipeline_process->setWorkingDirectory(this->modelsim_pipeline_path);
    this->modelsim_command = "vsim -c -do \"run -all\" work.MIPS";
    this->modelsim_pipeline_command ="vsim -c -do \"run -all\" work.Pipeline_MIPS";

    this->mode = "MIPS";

    this->Program_Counter = new Register("PC",100,0);
    this->Alu = new ALU(this->Program_Counter);
    this->assembler = new Assembler();
    this->register_file = new Register_File();
    this->data_memory = new Data_Memory();
    Observer_Pattern();
}

Simulator::~Simulator()
{
    delete this->register_file;
    delete this->assembler;
    delete this->Alu;
    delete this->Program_Counter;
    this->file.close();
    this->modelsim_process->kill();
}
void Simulator::clear()
{

    this->code.clear();
    for (uint i =0 ; i < instructions.size();i++)
        instructions[i].clear();
    this->instructions.clear();
    this->Lables.clear();
    this->data_asciiz.clear();
    this->data_word.clear();
    this->assembler->assembled_Instructions.clear();
    this->assembler->assembled_Instr_Strings.clear();
    this->Program_Counter->clear();
    this->Alu->clear();
    this->register_file->clear();
    this->data_memory->clear();
    this->address = 0;   // pc = 0
    emit clear_data_memory(); // clear the data memory

    this->lines.clear();

}

void Simulator::update_GUI()
{
    emit update_assembled_instruction();
    emit update_registers();
}

void Simulator::Modelsim()
{
    if (this->mode == "MIPS")
    {
        emit file_assembled_instructions(this->file_assembly_path); // write file with assembledd instructions
        this->modelsim_process->start(this->modelsim_command);// run modelsim to read the assembly file and write in the dataMemory and regFile files
        this->modelsim_process->waitForStarted();
        this->modelsim_process->terminate();
        this->modelsim_process->waitForFinished();
        emit file_regFile_lines(this->file_regFile_path);
        emit file_dataMemory_lines(this->file_dataMemory_path); // read dataMemory and load it into Data Memory Widget
        emit file_regFile_data();// read regFile and load it into reg File Widget
        emit file_dataMemory_data(); // read dataMemory and load it into Data Memory Widget
    }
    else if (this->mode == "Pipeline")
    {
        emit file_assembled_instructions(this->file_assembly_path_pipeline); // write file with assembledd instructions
        this->modelsim_pipeline_process->start(this->modelsim_pipeline_command);// run modelsim to read the assembly file and write in the dataMemory and regFile files
        this->modelsim_pipeline_process->waitForStarted();
        this->modelsim_pipeline_process->terminate();
        this->modelsim_pipeline_process->waitForFinished();
        emit updatePipelineAssemblyCode(this->code);
    }
    else if (this->mode == "Debugger")
    {
        emit file_assembled_instructions(this->file_assembly_path); // write file with assembledd instructions
        this->modelsim_process->start(this->modelsim_command);// run modelsim to read the assembly file and write in the dataMemory and regFile files
        this->modelsim_process->waitForStarted();
        this->modelsim_process->terminate();
        this->modelsim_process->waitForFinished();
        this->index = 2; //anwar b2ol kda
        emit file_regFile_lines(this->file_regFile_path);
        emit file_dataMemory_lines(this->file_dataMemory_path);
        this->debugg_pc();
        this->max_clocks = register_file->get_regClocks();
        this->updateState(index);
        emit highlight(0);
    }
}

void Simulator::Simulate()
{
    clear();
    Read_Instruction_Editor();
    Assemble_Instructions();
    //    ALU_Logic();

    print(this->Lables);
    print(this->assembler->get_assembled_strings());
    for(uint i =0 ; i<instructions.size();i++)
        print(instructions[i]);

    this->Modelsim();
    this->update_GUI();
}
void Simulator::debugg_pc()
{
    this->PCs.clear();
    this->PCs.push_back(11); //dummy
    this->PCs.push_back(11); //dummy
    ifstream file_pc;
    file_pc.open( (this->modelsim_path+"\\pc.txt").toStdString());
    if (!file_pc.is_open())
    {
        cout << "Cannot open Debug PC" << endl;
        return;
    }
    string s;
    while(getline(file_pc,s)) // read line by line
    {
        this->PCs.push_back(stoi(s));
    }
    file_pc.close();
}

void Simulator::updateState(int direction)
{
    if (this->max_clocks <= 2)
        return;
    cout << "index  " << index << " clocks " << max_clocks <<endl;
    if(direction == 1) // right
    {
        if (this->index == (this->max_clocks -1)) // if index is the last index in clocks vector
        {
            cout << "End of Execution" << endl;
            return;
        }
        this->index++;
    }
    else if (direction == -1) // backward
    {
        if(this->index == 2) // if it is the first state
        {
            cout << "First State cannot go backword" << endl;
            return;
        }
        this->index --;
    }
//    cout << this->lines[PCs[this->index-2]] << endl;
    for(int l=0; l < lines.size();l++)
        cout << lines[l] << " ";
    cout << endl;
    emit highlight(this->lines[PCs[this->index]]);
    emit updateDataMem(this->index); // updates both data memory and data_mem table
    emit updateRegFile(this->index);
    emit update_registers();
}
void Simulator::Simulate(string path)
{
    this->code_path = path;
    file.open(code_path);
    if (!file.is_open())
    {
        cout << "file can't open ya ray2" << endl;
        emit ERROR_Output("File Cannot Open");
    }
    vector<string> all_code_to_copy_in_editor;
    string s;
    while( getline(file,s) )
        all_code_to_copy_in_editor.push_back(s);

    emit clearTextEditor();
    emit update_Text_Editor(all_code_to_copy_in_editor);
}


void Simulator::Read_Instruction_Editor()
{
    vector<string> code_from_editor = emit getInstruction_Editor();

    string s;
    uint address = 0;
    uint start = this->Read_Data_Editor(code_from_editor);

    for (uint i =start ;i< code_from_editor.size();i++)
    {
        s = code_from_editor[i];
        if (s=="")
            continue;
        if(check_for_Lable(s,address))
            continue;

        Split_Instruction(s,i);
        this->instructions[address].push_back( to_string(address*4) ) ; // add adress to the instruction (for just the address)
        address++;
    }
    this->instructions.push_back(vector<string> {"end"});


}

void Simulator::Split_Instruction(string s,uint i)
{
    if(check_for_specials(s))
        return;

    vector<string> x;
    this->code.push_back(s);
    this->lines.push_back(i);

    // get the first operand
    uint pos = s.find_first_of(" ");
    x.push_back(s.substr(0,pos));
    s.erase(0,pos);

    // remove space noise
    for (int i =0 ;i<s.length();i++) {
        if ( s[i] == ' ')
            s.erase(i,1);
    }

    //check if it is a special format
    if(x[0] == "sw" || x[0] == "lw")
    {
        uint pos_practice = s.find_first_of("(");
        s.replace(pos_practice,1,",");  // to help in splitting
        s.erase(s.length()-1,1);          // remove the ')'
    }

    // get instruction's elements
    vector<string> inst = split_string(s,",");
    x.insert(x.begin()+1,inst.begin(),inst.end() );

    this->instructions.push_back(x);}

uint Simulator::Read_Data_Editor(vector<string> editor_code)
{
    string s = editor_code[0];
    int data_found = s.find(".data");
    if (data_found == -1)
        return 0;

    for(uint i=1 ; i< editor_code.size();i++)
    {
        s= editor_code[i];
        if(s==".text")
            return i+1; // return the pos to start from to read the data (pos of ".text" +1 )
        else if(s == "")
            continue;

        // split the dot data to get the name and type and value .....  my_int: .word 25
        vector<string> dot_data_ = split_string(s," ");
        // remove space noise
        vector<string> dot_data;
        for (int i =0 ;i < dot_data_.size(); i++)
            if(dot_data_[i] != "")
                dot_data.push_back(dot_data_[i]);

        print(dot_data);

        if (dot_data.size() != 3)
        {
            cout << "error in .data" << endl;
            continue;
        }

        if(dot_data[1] == ".asciiz")
        {
            string name = dot_data[0];
            name.erase(name.size()-1); // delete the ":"
            data_asciiz[name] = dot_data[2];
        }
        else if (dot_data[1] == ".word")
        {
            string name = dot_data[0];
            name.erase(name.size()-1); // delete the ":"
            data_word[name] = stoi(dot_data[2]);
        }
    }
}


bool Simulator::check_for_Lable(string s,uint index)
{
    int pos = s.find_first_of(":");
    if (pos == -1)
        return false;
    string Lable_name = s.substr(0,pos);
    this->Lables[Lable_name] = index;

    return true;
}

void Simulator::Assemble_Instructions()
{
    for (uint i =0; i < this->instructions.size();i++) {
        // check for lables it has a size 2 ==== 1 for label + 1 for address
        if (instructions[i].size()<=2)
            continue;
        emit Assemble_Instruction(instructions[i]);
    }
    emit print_assembled_instruction();
    cout << "=============R=============" << endl;
}

void Simulator::ALU_Logic()
{
    while(true)
    {
        // PC is program counter / 4 and we use it as an index for the instructions
        address = PC_;
        // Set the PC with the next instruction address (befor) the ALU Operation >> so that to allow ALU to change PC in some cases(beq,j..)
        Set_PC(address+1);
        // ====================================================================
        // check for Lables it has size 2 ==== Label Name + address
        if (instructions[address].size()==2 && instructions[address][0] != SYSCALL)
            continue;
        // check for end of program =====> instructions has end vector to check
        else if ( (instructions[address].size() == 1)  || (PC_ >= instructions.size()) )
        {
            cout << " ============= End of Excution ============== " << endl;
            return;
        }

        emit ALU_Instruction(this->instructions[address]);
        cout << "instruction ";
        print(instructions[address]);
        cout << "address = " << address << endl;
    }
}
bool Simulator::check_for_specials(string s)
{
    if (s == "syscall")
    {
        vector<string> x;
        x.push_back(s);
        this->instructions.push_back(x);
        return true;
    }
    return false;
}

void Simulator::Observer_Pattern()
{

    connect(this,           SIGNAL(updateDataMem(int) ),                    this->data_memory , SLOT( file_read_data_mem(int) ) );
    connect(this,           SIGNAL(updateRegFile(int) ),                    this->register_file , SLOT( read_regFile_data(int) ) );
    connect(this,           SIGNAL(Assemble_Instruction(vector<string>)),   this-> assembler,SLOT(Assemble(vector<string>)) );
    connect(this,           SIGNAL(print_assembled_instruction()) ,         this-> assembler,SLOT(print_all() ) );
    connect(this,           SIGNAL(ALU_Instruction(vector<string>)),        this-> Alu,SLOT(ALU_Operation(vector<string>) ) );
    connect(this,           SIGNAL(print_registers()) ,                     this->register_file, SLOT(print_all()) );
    connect(this,           SIGNAL(file_assembled_instructions(string)),    this->assembler, SLOT(File_assembled_instructions(string)) );
    connect(this,           SIGNAL(file_regFile_lines(string)),              this->register_file, SLOT(set_lines(string)) );
    connect(this,           SIGNAL(file_regFile_data()),              this->register_file, SLOT(read_regFile_data()) );
    connect(this,           SIGNAL(file_dataMemory_lines(string)),           this->data_memory, SLOT(set_lines(string)) );
    connect(this,           SIGNAL(file_dataMemory_data()),           this->data_memory, SLOT(file_read_data_mem()) );
    connect(this->assembler,SIGNAL(get_register_num(string)),               this->register_file,SLOT(get_register_num(string)));
    connect(this->assembler,SIGNAL(get_PC()),                               this->Program_Counter,SLOT(getValue()));
    connect(this->assembler,SIGNAL(get_label_address(string)),              this,SLOT(get_Label(string)));
    connect(this->assembler,SIGNAL(get_data_word(string)),                  this,SLOT(get_dataWord(string)));
    connect(this->assembler,SIGNAL(check_for_word(string)),                 this,SLOT(check_data_words(string)));

    connect(this->Alu ,     SIGNAL(get_fun_format(string)),                 this->assembler,SLOT(get_fun(string)));
    connect(this->Alu ,     SIGNAL(read_register(string)) ,                 this->register_file, SLOT(read_register(string)) );
    connect(this->Alu ,     SIGNAL(set_register(string,int)) ,              this->register_file, SLOT(write_register(string,int)) );
    connect(this->Alu ,     SIGNAL(change_PC_Label(string)) ,               this,SLOT(set_Program_Counter(string)));
    connect(this->Alu ,     SIGNAL(change_PC_address(int)) ,                this,SLOT(set_Program_Counter(int)));
    connect(this->Alu ,     SIGNAL(PC_current_instr()),                     this->Program_Counter,SLOT(getValue()));
    connect(this->Alu ,     SIGNAL(Push(string)),                           this->register_file,SLOT(push(string)));
    connect(this->Alu ,     SIGNAL(Pop (string)),                           this->register_file,SLOT(pop(string)));
    connect(this->Alu ,     SIGNAL(get_data_word(string)),                  this,SLOT(get_dataWord(string)));
    connect(this->Alu ,     SIGNAL(check_for_word(string)),                 this,SLOT(check_data_words(string)));
    connect(this->Alu ,     SIGNAL(get_value_data_memory(uint)),            this->data_memory,SLOT(read_memory(uint)));
    connect(this->Alu ,     SIGNAL(set_value_data_memory(uint,int)),        this->data_memory,SLOT(write_memory(uint ,int)));

}
vector<string> Simulator :: split_string(string s,string splitter)
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
void Simulator::print(vector<string> x)
{
    if(x.size()==0)
        return;
    for (int i = 0; i < x.size()-1; ++i) {
        cout << x[i] << "-";
    }
    cout << x[x.size()-1] << endl;}
void Simulator::print_all()
{
    for (int i =0 ;i < instructions.size();i++)
        print(instructions[i]);
    cout << endl;}
void Simulator::set_Program_Counter(string label)
{
    uint address = this->Lables[label];
    this->Program_Counter->setValue(address*4);}
void Simulator::set_Program_Counter(int address)
{
    this->Program_Counter->setValue(address);
}

uint Simulator::get_Label(string label)
{
    return this->Lables[label];}
vector<string> Simulator::get_instructions()
{
    vector<string> code_cleaned;
    for (uint i =0;i< this->code.size();i++) {
        int pos = this->code[i].find("syscall");
        if (pos == -1)
            code_cleaned.push_back(this->code[i]);
    }
    return code_cleaned;}
bool Simulator::check_data_words(string s)
{
    auto found = this->data_word.find(s);
    if (found != this->data_word.end())
        return true;
    return false;}
int Simulator::get_dataWord(string s)
{
    return this->data_word[s];}
void Simulator::print(map<string,uint> x)
{
    for (auto i = x.begin();i != x.end(); i ++)
        cout << "(" << i->first << "," << i->second << ") ";
    cout << endl;}
void Simulator::print(deque<string> x)
{
    for (int i = 0; i < x.size()-1; ++i) {
        cout << x[i] << "-";
    }
    cout << x[x.size()-1] << endl;

}
