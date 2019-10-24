#ifndef REGISTER_FILE_H
#define REGISTER_FILE_H
#include <bits/stdc++.h>
#include <QObject>
#include "register.h"
using namespace std;

#define WORD_SIZE sizeof(int)
#define MEMORYSIZE 8192
#define STACK_SIZE (WORD_SIZE*MEMORYSIZE)

class Register_File : public QObject
{
    Q_OBJECT

private:
    map<string,Register*> Registers;
    stack<int> Stack_Pointer;
public:
    Register_File(QObject *parent = nullptr);
    void add_register(string Name,uint Num , int Value = 0 );
    void clear();
public slots:
    map<string,Register*> registers_reading();
    int read_register(string name);
    uint get_register_num(string name);
    void write_register(string name,int Value=0);
    void print_all();

    void push(string name);
    void pop (string name);
    void stack_write(string name);
    void stack_read (string name);
signals:
};

#endif // REGISTER_FILE_H
