#ifndef CODEHIGHLIGHTER
#define CODEHIGHLIGHTER

#include <QSyntaxHighlighter>
#include <QHash>
#include <QTextCharFormat>
#include <QTextStream>
#include <QFileInfo>

/**
 * @brief The CodeHighlighter class
 * @author Veit Heller(s0539501) & Tobias Brosge(s0539713)
 *
 * A subclass of QSyntaxHighlighter that implements Syntax
 * Highlighting(duh!) for the Code Editor class.
 */
class CodeHighlighter : public QSyntaxHighlighter{
    Q_OBJECT

public:
    CodeHighlighter(QTextDocument *parent = 0, int file = 0);
    bool setupHighlighting(int file);

protected:
    void highlightBlock(const QString &text);

private:
    struct Rule{
        QRegExp pattern;
        QTextCharFormat format;
    };
    QVector<Rule> Rules;
    int python;

    QRegExp commentStartExpression;
    QRegExp commentEndExpression;

//    QTextCharFormat keywordNotation;
//    QTextCharFormat branchNotation;
//    QTextCharFormat singleLineCommentNotation;
    QTextCharFormat multiLineCommentNotation;
//    QTextCharFormat directiveNotation;
//    QTextCharFormat glNotation;
//    QTextCharFormat temporaryQtNotation;
//    QTextCharFormat quotationNotation;
//    QTextCharFormat digitNotation;
};

#endif
