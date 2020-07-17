#include <QDebug>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QSystemTrayIcon>

#include "FileReader.h"
#include "Task.h"
#include "TasksListModel.h"

int main(int argc, char* argv[]) {
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);
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
    FileReader fr;
    fr.setPath("/home/eyekop/Yata/test_data/todo.txt");
    fr.next();
    while (!fr.end()) {
        const std::string line = fr.next();
        const auto task = new Task(line, tasksListModel);
        tasksListModel->pushBack(task);
    }
    engine.rootContext()->setContextProperty("tasksListModel", tasksListModel);

    engine.load(url);
    qDebug() << "isSystemTrayAvailable: " << QSystemTrayIcon::isSystemTrayAvailable();
    return app.exec();
}
