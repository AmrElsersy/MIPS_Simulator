#ifndef DATA_MEMORY_H
#define DATA_MEMORY_H

#include <QObject>
#include <QTableWidget>
#include <iostream>
#include <bits/stdc++.h>
#include "register.h"

#define MEM_SIZE 8192

using namespace std;
class Data_Memory : public QObject
{
    Q_OBJECT
private:
    int* memory ;
    vector<string> lines;
    ifstream file;
public:
    explicit Data_Memory(QObject *parent = nullptr);
    void clear();
    vector<string> split_string(string s,string splitter);

signals:
    void update_dataMemory_GUI(uint);

public slots:

    void set_lines(string);
    void  write_memory(uint address , int value);
    int   read_memory (uint address);
    int  *get_Memory_Access();

    void file_read_data_mem();
    void file_read_data_mem(int);
    void updateDataMemPipeline(string dataMemClock);
};

#endif // DATA_MEMORY_H
