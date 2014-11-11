#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <QOpenGLShaderProgram>
#include <QOpenGLContext>
#include <QWindow>
#include <QOpenGLPaintDevice>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLDebugLogger>
#include <QTime>
#include <QMutex>
#include <QAudioInput>
#include <QBuffer>
#include <QOpenGLShader>
#include <QCoreApplication>

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
    void doneSignal(QString);
    void errored(QString, int);

public slots:
    void renderNow();
    void renderLater();
    bool updateCode(const QString &, const QString &);
    void updateAudioData(QByteArray);
    void onMessageLogged(QOpenGLDebugMessage message);

protected:
    virtual bool event(QEvent *);
    virtual void exposeEvent(QExposeEvent *);

private:
    bool init();
    void render();
    bool initShaders(const QString &);
    QColor clearColor;
    QOpenGLContext *context;
    QOpenGLPaintDevice *device;
    QTime *time;
    bool pendingUpdate;

    QOpenGLVertexArrayObject *vao;
    GLuint vertexBuffer, uvBuffer, audioLeftTexture, audioRightTexture;
    GLint vertexAttr, uvAttr,
        timeUniform, mouseUniform, rationUniform, samplerLeft, samplerRight;
    QOpenGLShaderProgram *shaderProgram;
    QMutex shaderProgramMutex;
    QString fragmentSource;

    AudioInputProcessor *audio;

    QOpenGLDebugLogger* m_logger;


//    static bool mapFormat(float *target, char *source, int count, const QAudioFormat &format);
//    template <typename T>
//    static void convertArray(float *target, const T *source, int count, qreal div, qreal sub = 0);
    static const char *defaultVertexShader, *defaultFragmentShader;
};

#endif // RENDERER_HPP
