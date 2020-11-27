#include <QApplication>
#include <QDebug>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QSystemTrayIcon>

#include "FileReader.h"
#include "Task.h"
#include "TasksListModel.h"

#include <QCommandLineParser>
#include <QFileInfo>
#include <QSystemTrayIcon>

int main(int argc, char* argv[]) {

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication app(argc, argv);

    QCommandLineParser parser;
    parser.setApplicationDescription("Yet Another ToDo Application");
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("input", QCoreApplication::translate("main", "Todo List full input file path."));
    parser.process(app);

    const QStringList args = parser.positionalArguments();
    const QFileInfo input{args.at(0)};
    if (!input.exists()) {
        qDebug() << "inpupt file does not exist.";
        parser.showHelp(1);
    }

    qmlRegisterType<TasksListModel>("com.eykop.Yata", 1, 0, "TasksListModel");

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(
        &engine, &QQmlApplicationEngine::objectCreated, &app,
        [url](QObject* obj, const QUrl& objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);

    TasksListModel* tasksListModel = new TasksListModel(&app);
    FileReader fr{input.absoluteFilePath().toStdString()};
    while (fr.init() && !fr.end()) {
        const std::string line = fr.next();
        const auto task = new Task(line, tasksListModel);
        tasksListModel->pushBack(task);
    }
    engine.rootContext()->setContextProperty("tasksListModel", tasksListModel);
    engine.load(url);

    QSystemTrayIcon* trayIcon = new QSystemTrayIcon(engine.rootObjects().at(0));
    trayIcon->setIcon(QIcon(":/todo.png"));
    trayIcon->show();

    return app.exec();
}
