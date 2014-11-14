#ifndef CODEEDITOR
#define CODEEDITOR

#include <QPlainTextEdit>
#include <QPaintEvent>
#include <QResizeEvent>
#include <QSize>
#include <QWidget>
#include <QPainter>
#include "CodeHighlighter.hpp"

class LineHighlighting;


/**
 * @brief The CodeEditor class
 * @author Veit Heller(s0539501) & Tobias Brosge(s0539713)
 *
 * A subclass of QPlainTextEdit that is optimized for code;
 * with Syntax Highlighting, line numbers and highlighting of
 * the current line.
 */
class CodeEditor : public QPlainTextEdit{
    Q_OBJECT

public:
    CodeEditor(QWidget *parent = 0, int file = 0);
    void lineHighlightingPaintEvent(QPaintEvent *event);
    int lineHighlightingWidth();
    void highlightErroredLine(int);
    void setHighlighting(int highlighting);

protected:
    void resizeEvent(QResizeEvent *event);
    void keyPressEvent(QKeyEvent *e);

private Q_SLOTS:
    void updatelineHighlightingWidth();
    void highlightCurrentLine();
    void updatelineHighlighting(const QRect &, int);

private:
    QWidget *lineHighlighting;
    CodeHighlighter *syntaxEngine;
};



/**
 * @brief The LineHighlighting helper class
 * @author Veit Heller(s0539501) & Tobias Brosge(s0539713)
 *
 * A helping subclass of QWidget for the codeEditor that is
 * responsible for highlighting the current line.
 */
class LineHighlighting : public QWidget{
public:
    LineHighlighting(CodeEditor *editor) : QWidget(editor){
        codeEditor = editor;
    }

    virtual QSize sizeHint() const{
        return QSize(codeEditor->lineHighlightingWidth(), 0);
    }

 protected:
    virtual void paintEvent(QPaintEvent *event) {
        codeEditor->lineHighlightingPaintEvent(event);
    }

private:
    CodeEditor *codeEditor;

};


#endif
