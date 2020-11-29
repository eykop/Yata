#ifdef Q_OS_LINUX
 #include <libnotifymm.h>
#endif

#include "FileReader.h"
#include "Task.h"
#include "TasksListModel.h"
#include "utils/XmlParser.h"
#include "utils/XmlSection.h"

#include <QApplication>
#include <QCommandLineParser>
#include <QDebug>
#include <QFileInfo>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>
#include <QSystemTrayIcon>

#include <algorithm>

void defineCmdLineArgs(const QApplication& app, QCommandLineParser& parser);
std::string getTodoInputFilePathArg(QCommandLineParser& parser);
TasksListModel* parseTodoInputFile(const std::string& inputFile);

int main(int argc, char* argv[]) {

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication app(argc, argv);
    QQuickStyle::setStyle("Material");
    QCommandLineParser cmdParser;
    defineCmdLineArgs(app, cmdParser);
    app.setQuitOnLastWindowClosed(false);

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));

    QObject::connect(
        &engine, &QQmlApplicationEngine::objectCreated, &app,
        [url](QObject* obj, const QUrl& objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);

    XmlParser parser;
    QObject::connect(&parser, &XmlParser::loaded,
                     [&parser, &cmdParser, &app, &engine, &url](XmlParser::LoadingResult result) {
                         if (result == XmlParser::LoadingResult::FAILED) {
                             qDebug() << "failed to load rules from xml";
                             return;
                         }

                         // build regexp and set in task
                         const auto& sections = parser.xmlSections();
                         QString fullRegex;
                         std::for_each(sections.cbegin(), sections.cend(),
                                       [&fullRegex](const XmlSection& section) { fullRegex.append(section.Regexp); });
                         Task::setRegexp(fullRegex.toStdString());

                         TasksListModel* tasksListModel = parseTodoInputFile(getTodoInputFilePathArg(cmdParser));
                         tasksListModel->setParent(&app);
                         engine.rootContext()->setContextProperty(QStringLiteral("tasksListModel"), tasksListModel);
                         engine.load(url);
                     });

    qmlRegisterType<TasksListModel>("com.eykop.Yata", 1, 0, "TasksListModel");
    parser.init();
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
    if(args.empty())
    {
        qDebug() << "inpupt file was not provided.";
        return {};
    }
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
