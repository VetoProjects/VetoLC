#include <exception>
#include <QException>
#include <QApplication>
#include "CodeEditorTest.hpp"
#include "EditorWindowTest.hpp"
#include "BackendTest.hpp"
#include "RendererTest.hpp"
#include "SettingsBackendTest.hpp"
#include "RendererTest.hpp"
#include "SoundGeneratorTest.hpp"
#ifdef WITH_PYTHON
#include "PySoundGeneratorTest.hpp"
#endif

struct Tests{
    QString *name;
    QObject *(*testclass)();
};

template<typename T>
QObject* factory(){
    return new T();
}

int main(int argc, char** argv){
    QApplication app(argc, argv);
    int status = 0, statusSum = 0;

    struct Tests testcases[] = {
            {new QString("CodeEditor"), factory<CodeEditorTest>},
            {new QString("EditorWindow"), factory<EditorWindowTest>},
            {new QString("AudioInputProcessor"), factory<AudioInputProcessor>},
            {new QString("Backend"), factory<BackendTest>},
            {new QString("SoundGenerator"), factory<SoundGeneratorTest>},
            {new QString("SettingsBackend"), factory<SettingsBackendTest>},
            {new QString("Renderer"), factory<RendererTest>},
            {new QString("SoundGenerator"), factory<SoundGeneratorTest>}
#ifdef with_python
           ,{new QString("PySoundGenerator"), factory<PySoundGeneratorTest>}
#endif							
            };
	    
    unsigned int size = sizeof(testcases)/sizeof(Tests);
    int* codes = new int[size]();

    auto testRun = [&status, &statusSum, &codes](QObject* testObject, QString* name, int i) {
                        try{
                            status = QTest::qExec(testObject, 0, 0);
                        }catch(QException e){
                            qDebug() << *name << "Test threw QException:" << e.what();
                        }catch(const std::exception& e){
                            qDebug() << *name << "Test threw STL Exception:" << e.what();
                        }
                        if(status != 0){
                            codes[i-1] = status;
                            statusSum += codes[i-1];
                            status = 0;
                        }
                        qDebug() << "";
                        delete testObject;
                    };

    if(argc == 1 || QString(argv[1]) == "runall")
        for(unsigned int i = 0; i < size; i++)
            testRun(testcases[i].testclass(), testcases[i].name, i);
    else
        for(int i = 1; i < argc; i++){
            for(struct Tests &t: testcases)
                if(argv[i] == *t.name){
                    testRun(t.testclass(), t.name, i-1);
                    break;
                }
        }

    for(unsigned int i = 0; i < size; i++)
        if(codes[i] != 0)
            qDebug() << "Test" << *(testcases[i+1].name) << "failed" << codes[i] << "test(s).";
    qDebug() << "Total failed tests:" << statusSum;

    for(struct Tests &t: testcases)
        delete t.name;
    delete[] codes;

    return statusSum;
}
