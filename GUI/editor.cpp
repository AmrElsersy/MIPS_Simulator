#include "editor.h"

Editor::Editor(QColor clr,QWidget *parent) : QPlainTextEdit(parent)
{
    this->color = clr;
    this->lineNumberArea = new LineNumberArea(this);
    connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth(int)));
    connect(this, SIGNAL(updateRequest(QRect,int)), this,SLOT(updateLineNumberArea(QRect,int)));
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));

    this->updateLineNumberAreaWidth(0);
    this->highlightCurrentLine();
    this->setFont(QFont("Arial",10));
}


// ============= Text Editor ======================
void Editor::Write_Code_Text_Editor(vector<string> s)
{
    string PlainText;
// ================= Onther Approach =================
//    for (uint i =0 ; i < s.size(); ++ i)
//    {
//        PlainText + s[i]+ "\n"  ;
//    }
//    this->Code_TextEdit->setPlainText(   QString::fromStdString(PlainText)   );

    // ================= Onther Approach =================
    for (int i =0 ; i<s.size();i++) {
        this->appendPlainText(QString::fromStdString( s[i] ) );
    }
//    this->Code_TextEdit->undo();  // undo the previews change in text editor
//    this->Code_TextEdit->append("RRRRRRRRRRRR");  // add the string to the end of the text editor
}
vector<string> Editor::Read_Code_Text_Editor()
{
    QString PlainText = this->toPlainText();
    string plaintext = PlainText.toStdString();
    vector<string> instructions =split_string(plaintext,"\n");
    return instructions;}

void Editor::Clear()
{
    this->clear();
}

vector<string> split_string(string s,string splitter)
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
void print(vector<string> x)
{
    for (int i = 0; i < x.size()-1; ++i) {
        cout << x[i] << "-";
    }
    cout << x[x.size()-1] << endl;

}
int Editor::lineNumberAreaWidth()
{
    int digits = 1;
    int max = qMax(1, blockCount());
    while (max >= 10) {
        max /= 10;
        ++digits;
    }

    int space = 3 + fontMetrics().width( QLatin1Char('9')) * digits;

    return space;
}
void Editor::updateLineNumberAreaWidth(int /* newBlockCount */)
{
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);}
void Editor::updateLineNumberArea(const QRect &rect, int dy)
{
    if (dy)
        lineNumberArea->scroll(0, dy);
    else
        lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());

    if (rect.contains(viewport()->rect()))
        updateLineNumberAreaWidth(0);}

void Editor::highlight(uint pos)
{
    // you cant acess the widget's cursor directly you must create a new cursor then set the cursor
    QTextCursor cursor = QTextCursor(this->textCursor());
    // there is no functions that moves you to a specific line position so
    // move the cursor to the first pos in the Editor ... then go down pos times
    cursor.movePosition(QTextCursor::Start);
    cursor.movePosition(QTextCursor::Down,QTextCursor::KeepAnchor,pos);
    // when you go down n times it selects all the text in the way
    cursor.clearSelection();

    this->setTextCursor(cursor);
}
void Editor::resizeEvent(QResizeEvent *e)
{
    QPlainTextEdit::resizeEvent(e);

    QRect cr = contentsRect();
    lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));}
void Editor::highlightCurrentLine()
{
    QList<QTextEdit::ExtraSelection> extraSelections;

    if (!isReadOnly()) {
//        cout << "pos from highlight=" << this->textCursor().position() << " vertical=" <<this->textCursor().verticalMovementX() << endl;
        QTextEdit::ExtraSelection selection;
        selection.format.setBackground(this->color);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = this->textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
    }

    this->setExtraSelections(extraSelections);
}
void Editor::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    QPainter painter(lineNumberArea);
    painter.fillRect(event->rect(), Qt::lightGray);
    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
    int bottom = top + (int) blockBoundingRect(block).height();
    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            QString number = QString::number(blockNumber + 1);
            painter.setPen(Qt::black);
            painter.drawText(0, top, lineNumberArea->width(), fontMetrics().height(),
                             Qt::AlignRight, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + (int) blockBoundingRect(block).height();
        ++blockNumber;
    }
}
