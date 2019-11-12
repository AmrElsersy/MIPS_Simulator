#include "data_mem_widget.h"

string Data_Mem_Widget::convert_int_to_hex_string(int num)
{
    // stringstream to convert that int to hex string
    stringstream stream;
    stream << hex << num;
    string result = "0x" + stream.str() ;
    return result;
}

Data_Mem_Widget::Data_Mem_Widget(QWidget *parent) : Tree_Widget(parent)
{
    // address + 16 =  COLOMN_SIZE
    this->setColumnCount(COLOMN_SIZE);
    // Set the Headers of the Colomns (Items)

    QStringList Headers;
    Headers.append(QString::fromStdString("     Address")); // Colomn 0 for Address Base
    for(uint i =0 ; i<COLOMN_SIZE-1;i++)
    {
        QString x1 = "Address " , x2= "(+", x3 = QString::number(i) , x4 = ")";
        Headers.append(x1 + x2 + x3 + x4);
    }

    this->setHeaderLabels(Headers);
    for (uint i =0 ; i<ROW_SIZE ; i++)
    {
        QStringList text_items;

        string address = convert_int_to_hex_string((COLOMN_SIZE-1)*i); // each line represents 16 word
        text_items.append( QString::fromStdString(address) );
        // initialize memory with garbage values
        for(uint j=0; j< COLOMN_SIZE; j++)
            text_items.append("x");
        // add the text to the row
        this->addItem(text_items);
    }
}

void Data_Mem_Widget::clear()
{
    cout << "CLEAR DATA MEMORY" << endl;

    for (uint i =0 ; i< ROW_SIZE ; i++)
        for(uint j =1 ; j< COLOMN_SIZE; j++)
            this->Items[i]->setText(j,  "x" )  ;

}


void Data_Mem_Widget::update_memory()
{
    this->memory = emit get_access_memory();

    cout << this->memory << " << " << endl;
    for (uint i =0 ; i< ROW_SIZE ; i++)
    {
        for(uint j =1 ; j< COLOMN_SIZE; j++)
        {
            uint index =  i * (COLOMN_SIZE-1) + (j-1) ;
            string text = convert_int_to_hex_string(this->memory[index])  ;
            this->Items[i]->setText(j,  QString::fromStdString( text ) )  ;
        }
    }
}

void Data_Mem_Widget::update_memory(uint address)
{
    this->memory = emit get_access_memory();

    string text = convert_int_to_hex_string(this->memory[address])  ;
    uint row = address / (COLOMN_SIZE-1);
    uint colomn = address % (COLOMN_SIZE-1);
    // colomn + 1 because of first colomn is for base address
    this->Items[row]->setText( colomn+1,  QString::fromStdString( text ) )  ;
}
