#ifndef Editor_H
#define Editor_H

#include <bits/stdc++.h>
#include <QObject>
#include <QWidget>
#include <QTabWidget>
#include <QTextEdit>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QStringList>
#include <QPlainTextEdit>
#include <QRect>
#include <QColor>
#include <QFontMetrics>
#include <QPainter>
#include <QTextBlock>


using namespace std;
class GUI ; class LineNumberWidget;

class Editor : public QPlainTextEdit
{
    Q_OBJECT
    friend class GUI;
private:
    QWidget *lineNumberArea;
    QColor color;

public:
    Editor(QColor clr = QColor(Qt::white),QWidget *parent = nullptr);
    void lineNumberAreaPaintEvent(QPaintEvent *event);
    int lineNumberAreaWidth();

protected:
    void resizeEvent(QResizeEvent *event) override;

public slots:
    void Write_Code_Text_Editor(vector<string>);
    vector<string> Read_Code_Text_Editor();
    void Clear();
private slots:
    void updateLineNumberAreaWidth(int newBlockCount);
    void highlightCurrentLine();
    void updateLineNumberArea(const QRect &, int);
    void highlight(uint);
};

vector<string> split_string(string s,string splitter);

class LineNumberArea :public QWidget
{
    Q_OBJECT
private:
    Editor* codeEditor;

public:
    LineNumberArea(Editor *editor) : QWidget(editor) {
        this->codeEditor = editor;
    }

    QSize sizeHint() const override {
        return QSize(this->codeEditor->lineNumberAreaWidth(), 0);
    }

protected:
    void paintEvent(QPaintEvent *event) override {
        this->codeEditor->lineNumberAreaPaintEvent(event);
    }


};

#endif // Editor_H
