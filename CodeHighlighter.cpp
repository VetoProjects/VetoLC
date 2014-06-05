#include "CodeHighlighter.hpp"

/**
 * @brief CodeHighlighter::CodeHighlighter
 * @param parent
 *
 * The constructor of the syntax highlighter.
 * Sets up the highlighting rules.
 * TODO: Make it more flexible - File format?
 * TODO: Highlight bracket-pairs
 * TODO: Build quot-pairs ( "bla " bla " bla " -> all is highlighted)
 * TODO: don't let the multi line comment trust preview format (see function highlightBlock(...) )
 */
CodeHighlighter::CodeHighlighter(QTextDocument *parent) : QSyntaxHighlighter(parent){
    Rule rule;

    // Keyword
    rule.format.setForeground(Qt::blue); // rule.format already fresh initialized
//    rule.format.setFontWeight(QFont::Bold);
    QStringList keywordPatterns;
    keywordPatterns << "\\battribute\\b" << "\\bbool\\b" << "\\bchar\\b"
                    << "\\bclass\\b" << "\\bconst\\b" << "\\bdouble\\b"
                    << "\\bdelete\\b" << "\\benum\\b" << "\\bexplicit\\b"
                    << "\\bfalse\\b" << "\\bfriend\\b" << "\\bin\\b"
                    << "\\binline\\b" <<"\\binout\\b" << "\\bint\\b"                      
                    << "\\blong\\b" << "\\bmat[234](x[234])?\\b" << "\\bnamespace\\b"
                    << "\\bnew\\b" << "\\boperator\\b" << "\\bout\\b"
                    << "\\bprivate\\b" << "\\bprotected\\b" << "\\bpublic\\b"
                    << "\\breturn\\b" << "\\bshort\\b" << "\\bsigned\\b"
                    << "\\bstatic\\b" << "\\bstruct\\b" << "\\btemplate\\b"
                    << "\\btrue\\b" << "\\btypedef\\b" << "\\btypename\\b"
                    << "\\buniform\\b" << "\\bunion\\b" << "\\bunsigned\\b"
                    << "\\bvarying\\b" << "\\b[biu]?vec[234]\\b" << "\\bvirtual\\b"
                    << "\\bvoid\\b" << "\\bvolatile\\b"
                    << "\\b(float|(a?(sin|cos|tan))|max|min|abs|break|length|distance)\\b";
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

    // Quotation
    rule.format = QTextCharFormat();
    rule.format.setFontItalic(true);
    rule.format.setForeground(Qt::darkRed);
    rule.pattern = QRegExp("\"(\\\"|.)*\"");
    Rules.append(rule);

    // Directive
    rule.format = QTextCharFormat();
    rule.format.setForeground(Qt::darkMagenta);
    //highlight the whole line provided theres a # at the beginning
    rule.pattern = QRegExp("^\\s*#.*$");
    Rules.append(rule);

    // GL
    rule.format = QTextCharFormat();
//    rule.format.setFontWeight(QFont::Bold);
    rule.format.setForeground(Qt::darkCyan);
    rule.pattern = QRegExp("\\b(GL|gl)(_)?[A-Za-z]+\\b");
    Rules.append(rule);

    // Temporary Qt
    //temporary highlighting of QT; needed?
    rule.format = QTextCharFormat();
//    rule.format.setFontWeight(QFont::Bold);
    rule.format.setForeground(Qt::darkCyan);
    rule.pattern = QRegExp("\\bQ[A-Za-z]+\\b");
    Rules.append(rule);
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
