#include "filestester.h"

filesTester::filesTester(QObject *parent) : QObject (parent)
{
    this->path = (QCoreApplication::applicationDirPath()+"\\MIPS\\").toStdString() ;
//    this->path = (QCoreApplication::applicationDirPath()+"\\Pipeline\\").toStdString() ;
}
string filesTester::fixpath(string s1)
{
    for(int i=0;i <s1.length();i++)
    {
        if(s1[i] == '/')
           s1.replace(i,1,"\\");
    }
    return s1;
}
void filesTester::set_paths(string regfile, string data)
{
    this -> correct_reg_dis = fixpath(regfile);
    this -> correct_data_dis = fixpath(data);
   // cout << "****done******" << endl <<correct_reg_dis << endl << correct_data_dis << endl;
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
    if((data_warnings.size() + reg_warnings.size()) == 0)
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
        if (s == "")
            continue;
        try {
        vector<string> k = split_string(s," ");
        for (uint i =0 ; i <k.size(); i++)
        {
                vector<string> address_value = split_string(k[i],","); if (address_value.size() == 1) {continue;}
                int address =  stoi(address_value[0]);
                int value    = stoi(address_value[1]);
                this -> correct_values[address] =  value;
        }
        }
        catch (...) {}

    }

    file1.close();

    while(getline(file2,s)) // read line by line
    {
        if (s == "")
            continue;
        try {
            vector<string> k = split_string(s," ");
            for (uint i =0 ; i <k.size(); i++)
            {
                    vector<string> address_value = split_string(k[i],","); if (address_value.size() == 1) {continue;}
                    int address =  stoi(address_value[0]);
                    int value    = stoi(address_value[1]);
                    this -> new_values[address] =  value;
            }
        }
        catch (...) {}

    }

    file2.close();

    for(int i=0; i< 32; i++)
    {
        if((correct_values[i] != new_values[i]) && ((i == 0) || (i == 1) || (i == 26) || (i == 27)))
        {
            string error;
            error = "Access denied - You tried to access a reserved register " + regs[i];
            this -> reg_warnings.push_back(error);
        }
        else if(correct_values[i] != new_values[i])
        {
            string error;
            error = "Register " + regs[i] + " correct value should be " + to_string(correct_values[i]) + " not " + to_string(new_values[i]);
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
        if (s == "")
            continue;
        try {
        vector<string> k = split_string(s," ");
        for (uint i =0 ; i <k.size(); i++)
        {
            vector<string> address_value = split_string(k[i],","); if (address_value.size() == 1) {continue;}
            int address =  stoi(address_value[0]);
            int value    = stoi(address_value[1]);
            this -> correct_data[address] = value;
        }
        }
        catch (...) {}
    }

    file1.close();

    while(getline(file2,s)) // read line by line
    {
        if (s == "")
            continue;
        try {
        vector<string> k = split_string(s," ");
        for (uint i =0 ; i <k.size(); i++)
        {
            vector<string> address_value = split_string(k[i],","); if (address_value.size() == 1) {continue;}
            int address =  stoi(address_value[0]) ;
            int value    = stoi(address_value[1]);
            this -> new_data[address] =  value;
         }}
        catch (...) {}

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
                    error = "Address: 0x" + to_string(i->first) + " correct value should be " + to_string(i->second) + " not " + to_string(j->second);
                    this -> data_warnings.push_back(error);
                }
            }

        }
        if(flag == 0)
        {
            string error;
            error = "Address: 0x" + to_string(i->first) + " not found";
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
    //emit show("Testing Started ..");
    emit set_status( this->get_status() );
    emit set_reg_warnings(this->reg_warnings);
    emit set_data_warnings(this->data_warnings);
}
