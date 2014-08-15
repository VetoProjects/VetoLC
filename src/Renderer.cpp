#include "Renderer.hpp"


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
        "out vec2  uv;\n"
        "\n"
        "void main(){\n"
        "   gl_Position = vec4(position, 1);\n"
        "   uv = texCoord;\n"
        "}";

const char * Renderer::defaultFragmentShader =
        "#version 330 core\n"
        "\n"
        "out vec4 color;\n"
        "\n"
        "in vec2 uv;\n"
        "uniform float time;\n"
        "uniform vec2 mouse;\n"
        "uniform float ration;\n"
        "\n"
        "void main(){\n"
        "     color = vec4(cos(uv.x * 5 - time / 1000) / 2 + .5, 0, sin(uv.x * 5 - time / 1000) / 2 + .5, 1);\n"
        "}";

/**
 * @brief Renderer::Renderer
 * @param parent Parent object of the render window
 *
 * Create a new Renderer with default shader
 */
Renderer::Renderer(QWindow *parent) : Renderer::Renderer("new", defaultFragmentShader, parent){ }

/**
 * @brief Renderer::Renderer
 * @param filename Name that should shown in the title
 * @param instructions Shader code for execution
 * @param parent Parent object of the render window
 *
 * Create a new Renderer with given code and set filename as title
 */
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

    setSurfaceType(QWindow::OpenGLSurface);


    time = new QTime();
    time->start();

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

/**
 * @brief Renderer::~Renderer
 *
 * Free resources
 */
Renderer::~Renderer(){
    glDeleteBuffers(1, &uvBuffer);
    glDeleteTextures(1, &audioLeftTexture);
    glDeleteTextures(1, &audioRightTexture);
    delete time;
    delete vao;
    delete device;
    delete m_logger;
}

/**
 * @brief Renderer::init
 * @return True on success, otherwise false
 *
 * Allocates grafic memory and initialize the shader program
 */
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
    bool result = initShaders(fragmentSource);

    vao->release();

    return result;
}

/**
 * @brief Renderer::initShaders
 * @param fragmentShader Code to compile as shader
 * @return True on success, otherwise false
 *
 * Initialze and compile the shader program
 */
bool Renderer::initShaders(const QString &fragmentShader){
    QOpenGLShaderProgram *newShaderProgram = new QOpenGLShaderProgram(this);
    if(!newShaderProgram->addShaderFromSourceCode(QOpenGLShader::Vertex, defaultVertexShader)){
        qDebug() << newShaderProgram->log();

        delete newShaderProgram;

        if(fragmentShader == defaultFragmentShader)
            qWarning() << tr("Failed to compile default shader.");
        else if(shaderProgram == 0)
            initShaders(defaultFragmentShader);

        return false;
    }
    if(!newShaderProgram->addShaderFromSourceCode(QOpenGLShader::Fragment,fragmentShader)){
        qDebug() << newShaderProgram->log();

        delete newShaderProgram;

        if(fragmentShader == defaultFragmentShader)
            qWarning() << tr("Failed to compile default shader.");
        else if(shaderProgram == 0)
            initShaders(defaultFragmentShader);

        return false;
    }
    if(!newShaderProgram->link()){
        qDebug() << newShaderProgram->log();

        delete newShaderProgram;

        if(fragmentShader == defaultFragmentShader)
            qWarning() << tr("Failed to compile default shader.");
        else if(shaderProgram == 0)
            initShaders(defaultFragmentShader);

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

        timeUniform  = shaderProgram->uniformLocation("time");
        mouseUniform = shaderProgram->uniformLocation("mouse");
        rationUniform = shaderProgram->uniformLocation("ration");

        fragmentSource = fragmentShader;
    shaderProgramMutex.unlock();

//    qDebug() << "vertexAttr" << vertexAttr;
//    qDebug() << "uvAttr" << uvAttr;
//    qDebug() << "timeUniform" << timeUniform;
//    qDebug() << "audioUniform" << audioUniform;
    return true;
}



/**
 * @brief Renderer::render
 *
 * Initialize output device, execute the shader and display the result
 */
void Renderer::render(){
    if(!device)
        device = new QOpenGLPaintDevice();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    device->setSize(size());

//    qDebug() << QLatin1String(reinterpret_cast<const char*>(glGetString(GL_VERSION))) << " " << QLatin1String(reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION)));
    const qreal retinaScale = devicePixelRatio();
    glViewport(0, 0, width() * retinaScale, height() * retinaScale);

    glClear(GL_COLOR_BUFFER_BIT);

    QPoint mouse = this->mapFromGlobal(QCursor::pos());
    QVector2D mousePosition((float)mouse.x() / (float)this->width(),
                            (float)mouse.y() / (float)this->height());
    float ration = (float)this->width() / (float)this->height();

    shaderProgramMutex.lock();
        vao->bind();
        shaderProgram->bind();

        shaderProgram->setUniformValue(mouseUniform,mousePosition);
        if(this->height() != 0)
            shaderProgram->setUniformValue(rationUniform, ration);
        shaderProgram->setUniformValue(timeUniform, (float)time->elapsed());

        glDrawArrays(GL_TRIANGLES, 0, 6);

        vao->release();
    shaderProgramMutex.unlock();
}

/**
 * @brief Renderer::renderLater
 *
 * Enqueue an update event to event queue
 */
void Renderer::renderLater(){
    if(!pendingUpdate){
        QCoreApplication::postEvent(this, new QEvent(QEvent::UpdateRequest));
        pendingUpdate = true;
    }
}

/**
 * @brief Renderer::renderNow
 *
 * Use the compiled shader to render on the widget
 */
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

/**
 * @brief Renderer::event
 * @param event The event that should be proccessed
 * @return True if the event was successful proccessed, otherwise false
 *
 * Called if a new event is poped from the event-queue to render on update event
 * and emit doneSignal on close event.
 */
bool Renderer::event(QEvent *event){
    switch(event->type()){
    case QEvent::UpdateRequest:
        QCoreApplication::postEvent(this, new QEvent(QEvent::UpdateRequest));
        renderNow();
        return true;
    case QEvent::Close:
        emit doneSignal(tr("User closed renderer"));
        return true;
    default:
        return QWindow::event(event);
    }
}

/**
 * @brief Renderer::exposeEvent
 *
 * Called if the window is ready to start rendering
 */
void Renderer::exposeEvent(QExposeEvent *){
    if(isExposed())
        renderNow();
}

/**
 * @brief Renderer::updateCode
 * @param filename Text for the title
 * @param code New shader program code
 * @return True on success, otherwise false.
 *
 * Set new title and compile new code for the shader program
 */
bool Renderer::updateCode(const QString &filename, const QString &code){
    if(!initShaders(code))
        return false;
    setTitle(filename);
    return true;
}

/**
 * @brief Renderer::updateAudioData
 * @param data New audio data
 *
 * Copy the new sound-data to the graphics memory for visualisation
 */
void Renderer::updateAudioData(QByteArray data){
    if(!shaderProgram)
        return;
    GLenum type, internalType;
    Q_UNUSED(internalType);
    char typeSize;
    switch(audio->format().sampleType() + audio->format().sampleSize()){
        case  8: case 10:
            type = GL_UNSIGNED_BYTE;
            internalType = GL_R8UI;
            typeSize = 1;
            break;
        case  9:
            type = GL_BYTE;
            internalType = GL_R8I;
            typeSize = 1;
            break;
        case 16: case 18:
            type = GL_UNSIGNED_SHORT;
            internalType = GL_R16UI;
            typeSize = 2;
            break;
        case 17:
            type = GL_SHORT;
            internalType = GL_R16I;
            typeSize = 2;
            break;
        case 32: case 34:
            type = GL_UNSIGNED_INT;
            internalType = GL_R32UI;
            typeSize = 4;
            break;
        case 33:
            type = GL_INT;
            internalType = GL_R32I;
            typeSize = 4;
            break;
        case 35:
            type = GL_FLOAT;
            internalType = GL_R32F;
            typeSize = 4;
            break;
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
                left [i+j] = data[i*2+j];
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

/**
 * @brief Renderer::onMessageLogged
 * @param message Message text
 *
 * Write the message text in the debug output
 */
void Renderer::onMessageLogged(QOpenGLDebugMessage message){
    qDebug() << message;
}
