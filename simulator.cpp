#include "simulator.h"

Simulator::Simulator()
{
//    this->file_assembly_path   = "/home/amrelsersy/ins.txt";
//    this->file_regFile_path    = "/home/amrelsersy/regFile.txt";
//    this->file_dataMemory_path = "/home/amrelsersy/dataMemory.txt";

    this->file_assembly_path   = "C:\\Users\\user\\Desktop\\ins.txt";
    this->file_regFile_path    = "C:\\Users\\user\\Desktop\\regFile.txt";
    this->file_dataMemory_path = "C:\\Users\\user\\Desktop\\dataMemory.txt";

    this->modelsim_process = new QProcess();
    this->modelsim_path = "C:\\Modeltech_pe_edu_10.4a\\examples\\mips_project";
    this->python_path  =  "C:\\Users\\user\\Desktop\\modelsim.py" ;

    // ================== ModelSim Running Settings ===========================
//    this->modelsim_process->setWorkingDirectory("C:\\Users\\user\\Desktop");
//    this->modelsim_process->setProgram("python");
//    this->modelsim_process->setArguments(QStringList() << "modelsim.py");

    this->modelsim_process->setWorkingDirectory(this->modelsim_path);
    this->modelsim_process->setProgram("vsim");
    this->modelsim_process->setArguments(QStringList() << "-c" << "-do" << "'run'" << "work.mips_processor");
    // =========================================================================

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
}

void Simulator::update_GUI()
{
    emit update_assembled_instruction();
    emit update_registers();
}

void Simulator::Modelsim()
{
    emit file_assembled_instructions(this->file_assembly_path);
    this->Run_Modelsim();
    emit file_regFile_data(this->file_regFile_path);
    emit file_dataMemory_data(this->file_dataMemory_path);
}

void Simulator::Run_Modelsim()
{

    // ================== python ====================================
    this->modelsim_process->start();
}
void Simulator::Simulate()
{

    clear();
    Read_Instruction_Editor();
    Assemble_Instructions();
//    ALU_Logic();

//    emit print_registers();
    print(this->Lables);
    print(this->assembler->get_assembled_strings());

    this->Modelsim();
    this->update_GUI();
}

void Simulator::Simulate(string path)
{
    this->code_path = path;
    clear();

    Read_Instruction();
    Assemble_Instructions();
//    ALU_Logic();


    emit print_registers();
    print(this->Lables);
    print(this->assembler->get_assembled_strings());

    this->Modelsim();
    this->update_GUI();
}

void Simulator::Read_Instruction()
{
    file.open(code_path);
    if (!file.is_open())
    {
        cout << "file can't open ya ray2" << endl;
        exit(1);
    }

    // Read line by line in the file and parase it then add it to the Instructions Vector
    string s;
    uint address = 0;

    this->Read_Data();


    while( getline(file,s) )
    {
        if(s == "")
            continue;

        Split_Instruction(s,address);
        this->instructions[address].push_back( to_string(address*4) ) ; // add adress to the instruction
        address++;

    }
    this->instructions.push_back(vector<string> {"end"});
    this->print_all();
    cout << "======================" << endl;}

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
            Split_Instruction(s,address);
            this->instructions[address].push_back( to_string(address*4) ) ; // add adress to the instruction (for just the address)
            address++;

    }
    this->instructions.push_back(vector<string> {"end"});

}

void Simulator::Split_Instruction(string s,uint index)
{
    if(check_for_Lable(s,index))
        return;
    if(check_for_specials(s))
        return;

    vector<string> x;
    this->code.push_back(s);

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

void Simulator::Read_Data()
{
    string s ;
    getline(file,s);
    int data_found = s.find(".data");
    if (data_found == -1)
    {
        this->file.close();
        this->file.open(this->code_path);
        return ;
    }
    while( getline(file,s) )
    {
        if(s==".text")
            return ;
        else if(s == "")
            continue;

        // split the dot data to get the name and type and value .....  my_int: .word 25
        vector<string> dot_data = split_string(s," ");
        if (dot_data.size() != 3)
        {
            cout << "error in .data" << endl;
            continue;
        }
        // remove space noise
        for (int i =0 ;i<3;i++)
            for (int j =0 ; j <dot_data.size();j++)
                if ( s[i] == ' ')
                    s.erase(i,1);
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
    vector<string> x;   x.push_back(Lable_name);
    this->instructions.push_back(x);
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
        address = PC;
        // Set the PC with the next instruction address (befor) the ALU Operation >> so that to allow ALU to change PC in some cases(beq,j..)
        Set_PC(address+1);
        // ====================================================================
        // check for Lables it has size 2 ==== Label Name + address
        if (instructions[address].size()==2 && instructions[address][0] != SYSCALL)
            continue;
        // check for end of program =====> instructions has end vector to check
        else if (instructions[address].size() == 1  )
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
    connect(this,           SIGNAL(Assemble_Instruction(vector<string>)),   this-> assembler,SLOT(Assemble(vector<string>)) );
    connect(this,           SIGNAL(print_assembled_instruction()) ,         this-> assembler,SLOT(print_all() ) );
    connect(this,           SIGNAL(ALU_Instruction(vector<string>)),        this-> Alu,SLOT(ALU_Operation(vector<string>) ) );
    connect(this,           SIGNAL(print_registers()) ,                     this->register_file, SLOT(print_all()) );
    connect(this,           SIGNAL(file_assembled_instructions(string)),    this->assembler, SLOT(File_assembled_instructions(string)) );
    connect(this,           SIGNAL(file_regFile_data(string)),              this->register_file, SLOT(read_regFile_data(string)) );
    connect(this,           SIGNAL(file_dataMemory_data(string)),           this->data_memory, SLOT(file_read_data_mem(string)) );

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
    this->Program_Counter->setValue((address*4)+4);}
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
