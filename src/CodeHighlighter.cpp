#include "CodeHighlighter.hpp"
#include <QDebug>

/**
 * @brief CodeHighlighter::CodeHighlighter
 * @param parent
 *
 * The constructor of the syntax highlighter.
 */
CodeHighlighter::CodeHighlighter(QTextDocument *parent, int file) : QSyntaxHighlighter(parent){
    setupHighlighting(file);
}

/**
 * @brief CodeHighlighter::setupHighlighting
 * @param file
 *
 * Sets up the highlighting rules.
 * TODO: Make it more flexible - File format?
 * TODO: Highlight bracket-pairs
 * TODO: Build quot-pairs ( "bla " bla " bla " -> all is highlighted)
 * TODO: don't let the multi line comment trust preview format (see function highlightBlock(...) )
 */
bool CodeHighlighter::setupHighlighting(int file){
    python = false;
    QFile highlighting;
    if(file == 0 || file == 3)
        highlighting.setFileName(":/highlighting/python");
    else if(file == 1)
        highlighting.setFileName(":/highlighting/qt");
    else if(file == 2)
        highlighting.setFileName(":/highlighting/glsl");
    else
        return false;
    highlighting.open(QFile::ReadOnly | QFile::Text);
    Rule rule;
    Rules.clear();

    // Keyword
    rule.format.setForeground(Qt::blue); // rule.format already freshly initialized
//    rule.format.setFontWeight(QFont::Bold);
    QStringList keywordPatterns;
    QString keywords = QTextStream(&highlighting).readAll();
    foreach(QString in, keywords.split("\n")){
        if(in != "")
            keywordPatterns << in;
    }
    foreach(const QString &pattern, keywordPatterns){
        rule.pattern = QRegExp(pattern);
        Rules.append(rule);
    }

    // Branch
    rule.format = QTextCharFormat();
    rule.format.setForeground(Qt::blue);

    rule.pattern = QRegExp("\\b(for(each)?|while|if|else)\\s*(?=\\()");
    Rules.append(rule);

    rule.pattern = QRegExp("\\bdo\\s*(?=\\{)");
    Rules.append(rule);


    // Digits
    rule.format = QTextCharFormat();
    rule.format.setForeground(QColor(255, 128, 0));
    rule.pattern = QRegExp("\\b[0-9]+|[0-9]*\\.[0-9]+\\b");
    Rules.append(rule);

    QString filename = QFileInfo(highlighting.fileName()).fileName();

    if(filename == "python"){
        python = true;
            // Single line comment
            rule.format = QTextCharFormat();
            rule.format.setForeground(Qt::darkGreen);
            rule.pattern = QRegExp("^\\s*#.*$");
            Rules.append(rule);

            rule.format = QTextCharFormat();
            rule.format.setForeground(Qt::darkMagenta);
            rule.pattern = QRegExp("\\bimport\\b");
            Rules.append(rule);

            rule.format = QTextCharFormat();
            rule.format.setForeground(Qt::darkMagenta);
            rule.pattern = QRegExp("\\s*from(?=\\s*import)");
            Rules.append(rule);
    }else if(filename == "glsl"){
        // Single line comment
        rule.format = QTextCharFormat();
        rule.format.setForeground(Qt::darkGreen);
        rule.pattern = QRegExp("//.*$");
        Rules.append(rule);

        // Multi line comment
            // rule.format is already comment
        multiLineCommentNotation = rule.format; // needed for later highlight
        //work in progress; make for more elaborate matching
        //commentStartExpression = QRegExp("[^/(//[^\\r\\n]+)]?/\\*");
        commentStartExpression = QRegExp("/\\*");
        commentEndExpression = QRegExp("\\*/");

        // Directive
        rule.format = QTextCharFormat();
        rule.format.setForeground(Qt::darkMagenta);
        //highlight the whole line provided theres a # at the beginning
        rule.pattern = QRegExp("^\\s*#.*$");
        Rules.append(rule);

        // GL
        rule.format = QTextCharFormat();
        //rule.format.setFontWeight(QFont::Bold);
        rule.format.setForeground(Qt::darkCyan);
        rule.pattern = QRegExp("\\b(GL|gl)(_)?[A-Za-z]+\\b");
        Rules.append(rule);
   }else if(filename == "qt"){
        // Single line comment
        rule.format = QTextCharFormat();
        rule.format.setForeground(Qt::darkGreen);
        rule.pattern = QRegExp("//.*$");
        Rules.append(rule);

        // Multi line comment
        // rule.format is already comment
        multiLineCommentNotation = rule.format; // needed for later highlight
        //work in progress; make for more elaborate matching
        //commentStartExpression = QRegExp("[^/(//[^\\r\\n]+)]?/\\*");
        commentStartExpression = QRegExp("/\\*");
        commentEndExpression = QRegExp("\\*/");

        // Directive
        rule.format = QTextCharFormat();
        rule.format.setForeground(Qt::darkMagenta);
        //highlight the whole line provided theres a # at the beginning
        rule.pattern = QRegExp("^\\s*#.*$");
        Rules.append(rule);

        // Temporary Qt
        //temporary highlighting of QT; needed?
        rule.format = QTextCharFormat();
        //rule.format.setFontWeight(QFont::Bold);
        rule.format.setForeground(Qt::darkCyan);
        rule.pattern = QRegExp("\\bQ[A-Za-z]+\\b");
        Rules.append(rule);
    }
    // Quotation
    rule.format = QTextCharFormat();
    rule.format.setFontItalic(true);
    rule.format.setForeground(Qt::darkRed);
    rule.pattern = QRegExp("\"(\\\"|.)*\"");
    Rules.append(rule);
    return true;
}

CodeHighlighter::~CodeHighlighter(){
}

/**
 * @brief CodeHighlighter::highlightBlock
 * @param text
 *
 * Highlights blocks(duh) by evaluating the regexes
 * created in the constructor.
 * TODO: Code and time complexity are suboptimal.
 *       Is there a better way?
 */
void CodeHighlighter::highlightBlock(const QString &text){
    /*
     * goes through the text once for each rule and
     * highlights the code according to those
     */
    foreach(const Rule &rule, Rules){
        QRegExp regEx(rule.pattern);
        int i = regEx.indexIn(text);
        while(i >= 0){
            int length = regEx.matchedLength();
            setFormat(i, length, rule.format);
            i = regEx.indexIn(text, i + length);
        }
    }
    setCurrentBlockState(0);

    if(!python){
        /*
         * goes through the text once more for the slightly
         * more complex multiline comment rule: searches for
         * a start of the comment and the accoring end and
         * highlights them and the text between.
         *
         * MAINTAINABILITY NOTES:
         * Requires keeping track of three indices:
         *      si = start index
         *      ei = end index
         *      commentLen = length of the comment(i.e. offset ; needed?)
         */
        int si = 0;
        if(previousBlockState() != 1)
            si = commentStartExpression.indexIn(text);

    // TODO: don't trust the preview format
        while(si >= 0 && this->format(si) != multiLineCommentNotation){
            int ei = commentEndExpression.indexIn(text, si);
            int commentLen;
            if(ei == -1){
                setCurrentBlockState(1);
                commentLen = text.length() - si;
            }else
                commentLen = ei - si + commentEndExpression.matchedLength();

            setFormat(si, commentLen, multiLineCommentNotation);
            si = commentStartExpression.indexIn(text, si + commentLen);
        }
    }
}
