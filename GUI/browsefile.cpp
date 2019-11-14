#include "browsefile.h"

BrowseFile::BrowseFile(string name ,QWidget *parent) : QWidget(parent)
{
    this->layout = new QHBoxLayout();
    this->label = new QLabel();
    this->label->setStyleSheet("background-color:#035c70;");
    this->label->setText(QString::fromStdString(name));
    this->label->setMinimumWidth(MIN_WIDTH_LABEL);
    this->entry = new QLineEdit(this);
    this->entry->setReadOnly(true);
    this->BrowseBtn = new QPushButton("Browse");
    this->BrowseBtn->setStyleSheet("background-color: rgb(0,0,128);"
                                   "color: white;"
                                  "border-radius: 5%;"
                                  "font-weight: bold;");
    this->file_dialog = new QFileDialog(this);
    this->SelectedPath = false;
    this->layout->addWidget(this->label);
    this->layout->addWidget(this->entry);
    this->layout->addWidget(this->BrowseBtn);
    this->setLayout(this->layout);

    this->file_dialog->setDirectory(DIRECTORY); // set the open directory
    this->file_dialog->setFileMode(QFileDialog::ExistingFile); // select existing one file only
    this->file_dialog->setNameFilter("*.txt");                  // show only txt extentions
    this->file_dialog->setOption(QFileDialog::ReadOnly);        // readonly mode dosn't support deleting or writing

    this->ObserverPattern();
    this->show();
}

void BrowseFile::ObserverPattern()
{
    connect(this->BrowseBtn,SIGNAL(clicked()),this,SLOT(show_dialog()));
    connect( this->file_dialog,SIGNAL(filesSelected(QStringList) ),this , SLOT(SetFilePath(QStringList) ) );
}

bool BrowseFile::isSelectedPath()
{
    return this->SelectedPath;
}

void BrowseFile::clear()
{
    this->entry ->clear();
    this->SelectedPath = false;
    this->path = "";
}

string BrowseFile::getText()
{
    return this->entry->text().toStdString();
}

void BrowseFile::show_dialog()
{
    cout << "Browse File"<< endl;
    this->file_dialog->show();
}

void BrowseFile::SetFilePath(QStringList paths)
{
    cout << this->isSelectedPath() << endl;
    this->path = paths[0].toStdString();
    this->entry->setText(paths[0]);
    this->SelectedPath = true;
    cout << "path="<<path << endl;
    cout << this->isSelectedPath() << endl;
}
