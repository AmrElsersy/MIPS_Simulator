#ifndef FILESTESTER_H
#define FILESTESTER_H
#include <bits/stdc++.h>
#include <QObject>
#include <QCoreApplication>
using namespace std;
class filesTester :public QObject
{
    Q_OBJECT
private:
    string regs[32] = {"$zero","$at","$v0","$v1","$a0","$a1","$a2","$a3","$t0","$t1","$t2","$t3","$t4","$t5","$t6","$t7","$s0","$s1","$s2","$s3","$s4","$s5","$s6","$s7","$t8","$t9","$k0","$k1","$gp","$sp","$fp","ra"};
    int correct_values[32];
    int new_values[32];
    string correct_data_dis;
    string correct_reg_dis;
    map<int, int> correct_data;
    map<int, int> new_data;
    vector<string> reg_warnings;
    vector<string> data_warnings;

    string path;
    vector<string>  split_string(string s,string splitter);
    void clear();
public:
    filesTester(QObject* parent = nullptr);
    void set_paths(string regfile, string data);
    bool get_status();
    void check_reg();
    void check_data();
    string fixpath(string s1);

signals:
    void set_status(bool);
    void set_reg_warnings(vector<string>);
    void set_data_warnings(vector<string>);
public slots:
    void StartTest();
};
#endif // FILESTESTER_H
