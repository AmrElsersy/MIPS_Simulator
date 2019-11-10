#include "data_memory.h"

Data_Memory::Data_Memory(QObject *parent) : QObject(parent)
{
    this->memory = new int [MEM_SIZE];
    cout << "address from data_mem = " << this->memory << endl;

}

void Data_Memory::clear()
{
    for(uint i =0 ; i<MEM_SIZE ; i ++)
        this->memory[i] = 0;
}

void Data_Memory::write_memory(uint address, int value)
{
    this->memory[address] = value;
    cout << "address " << address << " " << this->memory[address] << endl;
}

int Data_Memory::read_memory(uint address)
{
    return this->memory[address];
}

int* Data_Memory::get_Memory_Access()
{
    return this->memory;
}

void Data_Memory::file_read_data_mem(string path)
{
    this->file.open(path);
    if (!file.is_open())
    {
        cout << "Cannot open Data_MemoryFile" << endl;
        return;
    }

    string s ;
    while(getline(this->file,s)) // read line by line
    {
        vector<string> k = split_string(s," ");
        for (uint i =0 ; i <k.size(); i++)
        {
            vector<string> address_value = split_string(k[i],","); if (address_value.size() == 1) {continue;}
        if(address_value.size() != 2)
        {
            cout << "ERROR in Reading Data of Data_Mem_File (not the openning) " << endl;
            file.close();
            return;
        }

        uint address = uint( stoi(address_value[0]) );
        int value    = stoi (address_value[1] );
        this->write_memory(address,value);
        emit update_dataMemory_GUI(address);}
    }

    file.close();
}

void Data_Memory::updateDataMemPipeline(string dataMemClock)
{
    if (dataMemClock == "" || dataMemClock == " ")
        return;
    vector<string> s = split_string(dataMemClock," ");
    for (uint i =0 ; i <s.size(); i++)
    {
        try {
            vector<string> address_value = split_string(s[i],",");if (address_value.size() == 1) {continue;}
            uint address = uint( stoi(address_value[0]) );
            int value    = stoi (address_value[1] );
            this->write_memory(address,value);
            emit update_dataMemory_GUI(address);
        } catch (...) {
            cout << "error in data memory pipeline (" << s[i] << ")" << endl;
        }
    }
}

vector<string> Data_Memory :: split_string(string s,string splitter)
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
