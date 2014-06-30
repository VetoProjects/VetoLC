#include "Renderer.hpp"

#include <QOpenGLShader>
#include <QMessageBox>
#include <typeinfo>
#include <QCoreApplication>
#include <QPainter>
#include <QThread>

#include <QAudioDeviceInfo>

static GLfloat vertices[] = {
    1, 1,0,  1,-1,0, -1,1,0,
    1,-1,0, -1,-1,0, -1,1,0
};
static GLfloat uvs[] = {
    1,0, 1,1, 0,0,
    1,1, 0,1, 0,0
};

const char * Renderer::defaultVertexShader =
        "#version 330 core\n"
        "\n"
        "in vec3 position;\n"
        "in vec2 texCoord;\n"
        "\n"
        "uniform int _time;\n"
        "\n"
        "out vec2  uv;\n"
        "out float time;\n"
        "\n"
        "void main(){\n"
        "   time = float(_time);\n"
        "   gl_Position = vec4(position, 1);\n"
        "   uv = texCoord;\n"
        "}";

const char * Renderer::defaultFragmentShader =
        "#version 330 core\n"
        "\n"
        "out vec4 color;\n"
        "\n"
        "uniform in vec2  uv;\n"
        "uniform in float time;\n"
        "\n"
        "void main(){\n"
        "     color = vec4(cos(uv.x * 5 - time / 1000) / 2 + .5, 0, sin(uv.x * 5 - time / 1000) / 2 + .5, 1);\n"
        "}";

const char * Renderer::mandelbrotFragmentShader =
        "#version 330 core\n"
        "\n"
        "in vec2  uv;\n"
        "in float time;\n"
        "\n"
        "vec2 center = vec2(0,0);\n"
        "float scale = 4;\n"
        "int iter = 10;\n"
        "\n"
        "void main() {\n"
        "    gl_FragColor = vec4(0,0,0,1);\n"
        "    vec2 z, c;\n"
        " \n"
        "    c.x = 1.3333 * (uv.x - 0.5) * scale - center.x;\n"
        "    c.y = (uv.y - 0.5) * scale - center.y;\n"
        " \n"
        "    int i;\n"
        "    z = c;\n"
        "    for(i=0; i < iter; ++i) {\n"
        "        float x = (z.x * z.x - z.y * z.y) + c.x;\n"
        "        float y = (z.y * z.x + z.x * z.y) + c.y;\n"
        " \n"
        "        if((x * x + y * y) > 4.0)\n"
        "            break;\n"
        "        z.x = x;\n"
        "        z.y = y;\n"
        "    }\n"
        "    if(i < iter)\n"
        "        gl_FragColor.r = float(i)/10.0;\n"
        "}\n";

const char * Renderer::juliaFragmentShader =
        "#version 330 core\n"
        "\n"
        "in vec2  uv;\n"
        "in float time;\n"
        "uniform in float[10] audioData;\n"
        "\n"
        "//uniform highp vec2 c = vec2(.1,.1);\n"
        "int iter = 100;\n"
        "out vec4 color;\n"
        "\n"
        "void main() {\n"
//        "   float audioVal = audioData[int(uv.x * 1764)];\n"
        "   float y = 1 - uv.y * 2;"
        "   if(y < max(0, audioVal) && y > min(0, audioVal)){\n"
        "       color = vec4(1.0);\n"
        "       return;\n"
        "   }\n"
        "    vec2 c = vec2(sin(time / 2000.0), cos(time / 1500.0));\n"
        "    color = vec4(0,audioData[int(uv.x * 1764)],0,1);\n"
        "    vec2 z;\n"
        " \n"
        "    z.x = 3.0 * (uv.x - 0.5);\n"
        "    z.y = 2.0 * (uv.y - 0.5);\n"
        " \n"
        "    int i;\n"
        "    for(i=0; i < iter; ++i) {\n"
        "        float x = (z.x * z.x - z.y * z.y) + c.x;\n"
        "        float y = (z.y * z.x + z.x * z.y) + c.y;\n"
        " \n"
        "        if((x * x + y * y) > 4.0)\n"
        "            break;\n"
        "        z.x = x;\n"
        "        z.y = y;\n"
        "    }\n"
        "    if(i < iter){\n"
        "        color.b = float(i) * 5.0 / float(iter);\n"
        "        color.g = (float(i) * 5.0 / float(iter) - .3) / .7;\n"
        "        color.r = (float(i) * 5.0 / float(iter) - .7) / .3;\n"
        "    }\n"
        "}\n";

Renderer::Renderer(QWindow *parent) : Renderer::Renderer("new", defaultFragmentShader, parent){ }

Renderer::Renderer(const QString &filename, const QString &instructions, QWindow *parent) :
    QWindow(parent),
    clearColor(Qt::black),
    context(0), device(0),
    time(0),
    pendingUpdate(false),
    vao(0), uvBuffer(0), audioLeftTexture(0), audioRightTexture(0),
    vertexAttr(0), uvAttr(0), timeUniform(0),
    shaderProgram(0),
    fragmentSource(instructions)
{
    setTitle(filename);

    m_logger = new QOpenGLDebugLogger( this );

    connect(m_logger, SIGNAL(messageLogged(QOpenGLDebugMessage)),
             this, SLOT(onMessageLogged(QOpenGLDebugMessage)),
             Qt::DirectConnection );

    time = new QTime();
    time->start();

    setSurfaceType(QWindow::OpenGLSurface);

    audio = new AudioInputProcessor(this);
    connect(audio, SIGNAL(processData(QByteArray)), this, SLOT(updateAudioData(QByteArray)));
    audio->start();

    QSurfaceFormat format;
    format.setMajorVersion(3);
    format.setMinorVersion(3);
    format.setSamples(4);
    format.setProfile(QSurfaceFormat::CoreProfile);
    setFormat(format);

    resize(800, 600);
    show();
}

Renderer::~Renderer(){
    glDeleteBuffers(1, &uvBuffer);
    glDeleteTextures(1, &audioLeftTexture);
    glDeleteTextures(1, &audioRightTexture);
}

bool Renderer::init(){
    delete vao;
    vao = new QOpenGLVertexArrayObject(this);
    vao->create();
    vao->bind();

    glDeleteBuffers(1, &vertexBuffer);
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glDeleteBuffers(1, &uvBuffer);
    glGenBuffers(1, &uvBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(uvs), uvs, GL_STATIC_DRAW);

    glEnable(GL_TEXTURE_1D);

    glDeleteTextures(1, &audioLeftTexture);
    glGenTextures(1, &audioLeftTexture);
    glDeleteTextures(1, &audioRightTexture);
    glGenTextures(1, &audioRightTexture);

    glClearColor(0,0,.3,1);
    return initShaders(fragmentSource);

    vao->release();
}

bool Renderer::initShaders(const QString &fragmentShader){
    QOpenGLShaderProgram *newShaderProgram = new QOpenGLShaderProgram(this);
    if(!newShaderProgram->addShaderFromSourceCode(QOpenGLShader::Vertex, defaultVertexShader)){
        int infologLength;
        glGetShaderiv(newShaderProgram->programId(), GL_INFO_LOG_LENGTH, &infologLength);
        char *infoLog = new char[infologLength + 1];
        glGetShaderInfoLog(newShaderProgram->programId(), infologLength + 1, &infologLength, infoLog);

        qDebug() << infoLog;

        delete[] infoLog;


//        qDebug() << newShaderProgram->log();
        delete newShaderProgram;
        return false;
    }
    if(!newShaderProgram->addShaderFromSourceCode(QOpenGLShader::Fragment,fragmentShader)){
        qDebug() << newShaderProgram->log();
        delete newShaderProgram;
        return false;
    }
    if(!newShaderProgram->link()){
        qDebug() << newShaderProgram->log();
        delete newShaderProgram;
        return false;
    }
    shaderProgramMutex.lock();
        if(shaderProgram)
            delete shaderProgram;
        shaderProgram = newShaderProgram;

        vertexAttr   = shaderProgram->attributeLocation("position");
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        shaderProgram->setAttributeBuffer(vertexAttr, GL_FLOAT, 0, 3);
        shaderProgram->enableAttributeArray(vertexAttr);

        uvAttr       = shaderProgram->attributeLocation("texCoord");
        glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
        shaderProgram->setAttributeBuffer("texCoord", GL_FLOAT, 0, 2);
        shaderProgram->enableAttributeArray(uvAttr);

        timeUniform  = shaderProgram->uniformLocation("_time");

        audioLeftUniform = shaderProgram->uniformLocation("audioDataLeft");
        audioRightUniform = shaderProgram->uniformLocation("audioDataRight");
        shaderProgram->setUniformValue(audioLeftUniform, 0);
        shaderProgram->setUniformValue(audioRightUniform, 0);
        fragmentSource = fragmentShader;
    shaderProgramMutex.unlock();

//    qDebug() << "vertexAttr" << vertexAttr;
//    qDebug() << "uvAttr" << uvAttr;
//    qDebug() << "timeUniform" << timeUniform;
//    qDebug() << "audioUniform" << audioUniform;
    return true;
}

void Renderer::render(){
    if(!device)
        device = new QOpenGLPaintDevice();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    device->setSize(size());
    QPainter painter;
    //QPainter painter(device);
    render(&painter);
}

void Renderer::render(QPainter *){
//    qDebug() << QLatin1String(reinterpret_cast<const char*>(glGetString(GL_VERSION))) << " " << QLatin1String(reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION)));
    const qreal retinaScale = devicePixelRatio();
    glViewport(0, 0, width() * retinaScale, height() * retinaScale);

    glClear(GL_COLOR_BUFFER_BIT);

    vao->bind();
    shaderProgramMutex.lock();
    shaderProgram->bind();

    shaderProgram->setUniformValue(timeUniform, time->elapsed());

    glDrawArrays(GL_TRIANGLES, 0, 6);

    vao->release();
    shaderProgramMutex.unlock();
}

void Renderer::renderLater(){
    if(!pendingUpdate){
        QCoreApplication::postEvent(this, new QEvent(QEvent::UpdateRequest));
        pendingUpdate = true;
    }
}

void Renderer::renderNow(){
    pendingUpdate = false;

    if(!isExposed())
        return;

    bool needsInit = false;

    if(!context){
        context = new QOpenGLContext(this);
        context->setFormat(requestedFormat());
        context->create();

        needsInit = true;
    }

    context->makeCurrent(this);

    if(needsInit){
        if (m_logger->initialize()){
            m_logger->startLogging( QOpenGLDebugLogger::SynchronousLogging );
            m_logger->enableMessages();
        }
        initializeOpenGLFunctions();
        init();
    }


    if(!shaderProgram)
        initShaders(fragmentSource);

    if(shaderProgram)
        render();
    context->swapBuffers(this);

    renderLater();
}

bool Renderer::event(QEvent *event){
    switch(event->type()){
    case QEvent::UpdateRequest:
        QCoreApplication::postEvent(this, new QEvent(QEvent::UpdateRequest));
        renderNow();
        return true;
    case QEvent::Close:
        emit doneSignal("User closed window");
        // No fall throu?
        return true;
    default:
        return QWindow::event(event);
    }
}

void Renderer::exposeEvent(QExposeEvent *){
    if(isExposed())
        renderNow();
}

bool Renderer::updateCode(const QString &filename, const QString &code){
    if(!initShaders(code))
        return false;
    setTitle(filename);
    return true;
}

void Renderer::updateAudioData(QByteArray data){
    if(!shaderProgram)
        return;
    GLenum type, internalType;
    char typeSize;
    switch(audio->format().sampleType() + audio->format().sampleSize()){
        case  8: case 10: type = GL_UNSIGNED_BYTE;  internalType = GL_R8UI;  typeSize = 1; break;
        case  9:          type = GL_BYTE;           internalType = GL_R8I;   typeSize = 1; break;
        case 16: case 18: type = GL_UNSIGNED_SHORT; internalType = GL_R16UI; typeSize = 2; break;
        case 17:          type = GL_SHORT;          internalType = GL_R16I;  typeSize = 2; break;
        case 32: case 34: type = GL_UNSIGNED_INT;   internalType = GL_R32UI; typeSize = 4; break;
        case 33:          type = GL_INT;            internalType = GL_R32I;  typeSize = 4; break;
        case 35:          type = GL_FLOAT;          internalType = GL_R32F;  typeSize = 4; break;
        default: return;
    }
    char *left, *right;
    int count = data.size();
    if(audio->format().channelCount() == 2){
        count /= 2;
        left  = new char[count];
        right = new char[count];
        for(int i = 0; i < count; i += typeSize){
            for(int j = 0; j < typeSize; ++j){
                left [i+j] = data[i*2+j         ];
                right[i+j] = data[i*2+j+typeSize];
            }
        }
    }else
        left = right = data.data();


    shaderProgramMutex.lock();
    shaderProgram->bind();

    glBindTexture(GL_TEXTURE_1D, audioLeftTexture);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage1D(GL_TEXTURE_1D, 0, GL_R32F, count / typeSize, 0, GL_RED, type, left);

    glBindTexture(GL_TEXTURE_1D, audioRightTexture);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage1D(GL_TEXTURE_1D, 0, GL_R32F, count / typeSize, 0, GL_RED, type, right);

    shaderProgram->release();
    shaderProgramMutex.unlock();
    if(left != data.data())
        delete[] left;
    if(right != data.data())
        delete[] right;
}


void Renderer::onMessageLogged(QOpenGLDebugMessage message){
    qDebug() << message;
}
