#include "FileReader.h"
#include "Task.h"
#include "TasksListModel.h"

#ifdef Q_OS_LINUX
#include <libnotifymm.h>
#endif

#include <QApplication>
#include <QCommandLineParser>
#include <QDebug>
#include <QFileInfo>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>
#include <QSystemTrayIcon>

void defineCmdLineArgs(const QApplication& app, QCommandLineParser& parser);
std::string getTodoInputFilePathArg(QCommandLineParser& parser);
TasksListModel* parseTodoInputFile(const std::string& inputFile);

int main(int argc, char* argv[]) {

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication app(argc, argv);
    QQuickStyle::setStyle("Material");
    QCommandLineParser cmdParser;
    defineCmdLineArgs(app, cmdParser);

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(
        &engine, &QQmlApplicationEngine::objectCreated, &app,
        [url](QObject* obj, const QUrl& objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);

    TasksListModel* tasksListModel = parseTodoInputFile(getTodoInputFilePathArg(cmdParser));
    tasksListModel->setParent(&app);

    qmlRegisterType<TasksListModel>("com.eykop.Yata", 1, 0, "TasksListModel");
    engine.rootContext()->setContextProperty(QStringLiteral("tasksListModel"), tasksListModel);
    engine.load(url);

#ifdef Q_OS_LINUX
    Notify::init("Hello world!");
    Notify::Notification Hello("Hello world", "This is an example notification.", "dialog-information");
    Hello.show();
#endif

    return app.exec();
}

void defineCmdLineArgs(const QApplication& app, QCommandLineParser& parser) {
    parser.setApplicationDescription(QStringLiteral("Yet Another ToDo Application"));
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument(QStringLiteral("input"),
                                 QCoreApplication::translate("main", "Todo List full input file path."));
    parser.process(app);
}

std::string getTodoInputFilePathArg(QCommandLineParser& parser) {
    const QStringList args = parser.positionalArguments();
    const QFileInfo input{args.at(0)};
    if (!input.exists()) {
        qDebug() << "inpupt file does not exist.";
        parser.showHelp(1);
    }
    return input.absoluteFilePath().toStdString();
}

TasksListModel* parseTodoInputFile(const std::string& inputFile) {
    TasksListModel* tasksListModel = new TasksListModel();
    FileReader fr{inputFile};
    while (fr.init() && !fr.end()) {
        const std::string line = fr.next();
        const auto task = new Task(line, tasksListModel);
        tasksListModel->pushBack(task);
    }
    return tasksListModel;
}
