#include "filestester.h"

filesTester::filesTester(QObject *parent) : QObject (parent)
{
    this->path = "C:\\MIPS\\" ;  // windows
//    this->path = "/home/amrelsersy/MIPS_Simulator/TestCases/" ;  // Linux
}
void filesTester::set_paths(string regfile, string data)
{
    this -> correct_reg_dis = regfile;
    this -> correct_data_dis = data;
}

vector<string> filesTester:: split_string(string s,string splitter)
{
    vector<string> splitted;
    unsigned int n = s.length();
    int pos = 0;
    int start = pos;
    while (pos != string::npos)
    {
        pos = s.find_first_of(splitter,pos+1);
        string splitted_string = s.substr(start,pos-start);
        splitted.push_back(splitted_string);
        start = pos+1;
    }
    return splitted;
}
bool filesTester:: get_status()
{
    if((data_warnings.size() + data_warnings.size()) == 0)
        return true;
    return false;
}
void filesTester:: check_reg()
{
    ifstream file1,file2;
    file2.open(this->path + "regFile.txt");
    file1.open(correct_reg_dis);

    string s;

    while(getline(file1,s)) // read line by line
    {
        vector<string> address_value = split_string(s,",");
        int address =  stoi(address_value[0]);
        int value    = stoi(address_value[1]);
        this -> correct_values[address] =  value;
    }

    file1.close();

    while(getline(file2,s)) // read line by line
    {
        vector<string> address_value = split_string(s,",");
        int address =  stoi(address_value[0]) ;
        int value    = stoi(address_value[1]);
        this -> new_values[address] =  value;
    }

    file2.close();

    for(int i=0; i< 32; i++)
    {
        if(correct_values[i] != new_values[i])
        {
            string error;
            error = "register: " + regs[i] + " correct value should be " + to_string(correct_values[i]) + " not " + to_string(new_values[i]);
            this -> reg_warnings.push_back(error);
        }
    }
}
void filesTester::check_data()
{
    ifstream file1,file2;
    file2.open(this->path+"dataMemory.txt");
    file1.open(correct_data_dis);

    string s;

    while(getline(file1,s)) // read line by line
    {
        vector<string> address_value = split_string(s,",");
        int address =  stoi(address_value[0]);
        int value    = stoi(address_value[1]);
        this -> correct_data[address] = value;
    }

    file1.close();

    while(getline(file2,s)) // read line by line
    {
        vector<string> address_value = split_string(s,",");
        int address =  stoi(address_value[0]) ;
        int value    = stoi(address_value[1]);
        this -> new_data[address] =  value;
    }

    file2.close();

    for(auto i=correct_data.begin(); i != correct_data.end() ; i++)
    {
        int flag = 0;
        for(auto j=new_data.begin(); j != new_data.end() ; j++)
        {
            if(i->first == j->first)
            {
                flag = 1;
                if(i->second != j->second)
                {

                    string error;
                    error = "address: 0x" + to_string(i->first) + " correct value should be " + to_string(i->second) + " not " + to_string(j->second);
                    this -> data_warnings.push_back(error);
                }
            }

        }
        if(flag == 0)
        {
            string error;
            error = "address: 0x" + to_string(i->first) + " not found";
            this -> data_warnings.push_back(error);
        }
    }
}

void filesTester::clear()
{
    this->reg_warnings.clear();
    this->data_warnings.clear();
}
void filesTester::StartTest()
{
    this->clear();
    this->check_reg();
    this->check_data();
    emit set_status( this->get_status() );
    emit set_reg_warnings(this->reg_warnings);
    emit set_data_warnings(this->data_warnings);
}
