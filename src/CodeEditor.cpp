#include "CodeEditor.hpp"

/**
 * @brief CodeEditor::CodeEditor
 * @param parent
 * @param file
 *
 * The constructor of the code editor.
 * Sets up the highlighting of syntax and current line
 * and connects slots and signals. Needs a highlighting
 * file.
 */
CodeEditor::CodeEditor(QWidget *parent, int file) : QPlainTextEdit(parent){
    lineHighlighting = new LineHighlighting(this);
    syntaxEngine = new CodeHighlighter(this->document(), file);

    connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updatelineHighlightingWidth()));
    connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(updatelineHighlighting(QRect,int)));
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));

    updatelineHighlightingWidth();
    highlightCurrentLine();

    QFont font("Inconsolata");
    font.setPointSize(10);
    font.setBold(true);
    font.setStyleStrategy(QFont::PreferAntialias);
    setFont(font);
}

/**
 * @brief CodeEditor::lineHighlightingWidth
 * @return the width of the line to be highlighted
 *
 * Helper function that computes the width of the line that is
 * to be highlighted.
 */
int CodeEditor::lineHighlightingWidth(){
    int digits = 1,
    maxLen = qMax(1, blockCount());
    //computes how many blocks we have
    while(maxLen >= 10){
        maxLen /= 10;
        ++digits;
    }
    //computes the width with respect to the font
    int space = 3 + fontMetrics().width(QLatin1Char('9')) * digits;

    return space;
}

/**
 * @brief CodeEditor::updatelineHighlightingWidth
 *
 * sets the highlighting whenever the block count changes(SLOT)
 */
void CodeEditor::updatelineHighlightingWidth(){
    setViewportMargins(lineHighlightingWidth(), 0, 0, 0);
}

/**
 * @brief CodeEditor::updatelineHighlighting
 * @param rect
 * @param delta_y
 *
 * updates the line numbers whenever an update
 * is requested(SLOT)
 */
void CodeEditor::updatelineHighlighting(const QRect &rect, int delta_y){
    if(delta_y)
        lineHighlighting->scroll(0, delta_y);
    else
        lineHighlighting->update(0, rect.y(), lineHighlighting->width(), rect.height());

    if(rect.contains(viewport()->rect()))
        updatelineHighlightingWidth();
}


/**
 * @brief CodeEditor::resizeEvent
 * @param e
 *
 * catches the resize signal and customizes the update(SLOT)
 */
void CodeEditor::resizeEvent(QResizeEvent *e){
    QPlainTextEdit::resizeEvent(e);

    QRect rect = contentsRect();
    lineHighlighting->setGeometry(QRect(rect.left(), rect.top(), lineHighlightingWidth(), rect.height()));
}

/**
 * @brief CodeEditor::highlightCurrentLine
 *
 * highlights the current line in red
 * TODO: set the selections list up in the if-branch and only
 *       call setExtraSelections in there. is that correct?
 */
void CodeEditor::highlightCurrentLine(){
    if(!isReadOnly()){
        QTextEdit::ExtraSelection selection;

        QColor lineColor = QColor(Qt::lightGray).lighter(120);

        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.format.setBackground(lineColor);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        QList<QTextEdit::ExtraSelection> selections;
        selections.append(selection);
        setExtraSelections(selections);
    }
}

/**
 * @brief CodeEditor::lineHighlightingPaintEvent
 * @param event
 *
 * updates the line highlighting whenever scrolling happens;
 * invoked from updateLineHighlighting().
 */
void CodeEditor::lineHighlightingPaintEvent(QPaintEvent *event){
    //computes first and last block numbers
    QTextBlock textBlock = firstVisibleBlock();
    int textBlockNum = textBlock.blockNumber();
    int top = (int) blockBoundingGeometry(textBlock).translated(contentOffset()).top();
    int bottom = top + (int) blockBoundingRect(textBlock).height();

    //paints the background to where line nums will appear
    QPainter painter(lineHighlighting);
    painter.fillRect(event->rect(), Qt::lightGray);

    //sets the numbers for all the lines visible
    while(textBlock.isValid() && top <= event->rect().bottom()){
        //sets line num starting for current line
        if(textBlock.isVisible() && bottom >= event->rect().top()){
            QString num = QString::number(textBlockNum + 1);
            painter.setPen(Qt::black);
            painter.drawText(0, top, lineHighlighting->width(), fontMetrics().height(),
                            Qt::AlignRight, num);
        }

        //selects next line and computes its' numbers
        textBlock = textBlock.next();
        ++textBlockNum;
        top = bottom;
        bottom = top + (int)blockBoundingRect(textBlock).height();
    }
}

/**
 * @brief CodeEditor::highlightErroredLine
 * @param lineno
 *
 * highlights the line given as argument as errored.
 */
void CodeEditor::highlightErroredLine(int lineno){
    QTextCursor cursor(document()->findBlockByLineNumber(lineno-1));
    QTextEdit::ExtraSelection selection;

    cursor.movePosition(QTextCursor::EndOfLine);
    setTextCursor(cursor);

    QColor lineColor = QColor(Qt::red).lighter(180);

    selection.format.setProperty(QTextFormat::FullWidthSelection, true);
    selection.format.setBackground(lineColor);
    selection.cursor = cursor;
    selection.cursor.clearSelection();
    QList<QTextEdit::ExtraSelection> selections;
    selections.append(selection);
    setExtraSelections(selections);

}

/**
 * @brief CodeEditor::setHighlighting
 * @param highlighting
 *
 * Tells the syntax highlighter to change highlighting by
 * using a code where 0 and 3 stand for Python, 1 stands for Qt
 * and 2 stands for GLSL.
 */
void CodeEditor::setHighlighting(int highlighting){
    syntaxEngine->setupHighlighting(highlighting);
}

/**
 * @brief CodeEditor::keyPressEvent
 * @param e
 *
 * intercepts the keyPressEvent e so that a tab is rendered
 * as 4 spaces.
 */
void CodeEditor::keyPressEvent(QKeyEvent *e){
    if(e->key() == Qt::Key_Tab){
        insertPlainText("    ");
        e->accept();
    }
    else
        QPlainTextEdit::keyPressEvent(e);
}
