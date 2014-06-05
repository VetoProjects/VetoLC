#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <QOpenGLShaderProgram>
#include <QOpenGLContext>
#include <QWindow>
#include <QOpenGLPaintDevice>
#include <QOpenGLFunctions>
#include <QTime>
#include <QMutex>
//#include <QAudioBuffer>
//#include <QAudioRecorder>
//#include <QAudioProbe>
#include <QAudioInput>
#include <QBuffer>

#include "QtGlException.hpp"
#include "AudioInputProcessor.hpp"

/**
 * @brief The Renderer class
 *
 * A subclass of QWindow and QOPenGLFunctions that implements
 * a GLSL fragment shader renderer.
 */
class Renderer : public QWindow, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit Renderer(QWindow *parent = 0);
    explicit Renderer(const QString &, const QString &, QWindow *parent = 0);
    ~Renderer();

signals:
    void doneSignal(QtGlException);

public slots:
    void renderNow();
    void renderLater();
    bool updateCode(const QString &, const QString &);
    void updateAudioData(QByteArray);

protected:
    virtual bool event(QEvent *);
    virtual void exposeEvent(QExposeEvent *);

private:
    bool init();
    void render();
    void render(QPainter*);
    bool initShaders(const QString &);
    QColor clearColor;
    QOpenGLContext *context;
    QOpenGLPaintDevice *device;
    QTime *time;
    bool pendingUpdate;

    GLuint vertexBuffer, uvBuffer, audioLeftTexture, audioRightTexture;
    int vertexAttr, uvAttr,
        timeUniform, audioLeftUniform, audioRightUniform;
    QOpenGLShaderProgram *shaderProgram;
    QMutex shaderProgramMutex;
    QString fragmentSource;

    AudioInputProcessor *audio;


//    static bool mapFormat(float *target, char *source, int count, const QAudioFormat &format);
//    template <typename T>
//    static void convertArray(float *target, const T *source, int count, qreal div, qreal sub = 0);
    static const char *defaultVertexShader, *defaultFragmentShader, *juliaFragmentShader, *mandelbrotFragmentShader;
};

#endif // RENDERER_HPP
