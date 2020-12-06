#pragma once

#include <QByteArray>
#include <QHash>
#include <QObject>
#include <QString>
#include <QUuid>

#include <string>

class Task : public QObject {

    Q_OBJECT

public:
    enum Section {
        STATUS = Qt::UserRole + 1,
        PRIORITY,
        COMPLETION_DATE,
        CREATION_DATE,
        DESCRIPTION,
        PROJECTS,
        CONTEXTS,
        DUE,
        RECURING
    };
    Q_ENUM(Section)

    explicit Task(const std::string& task, QObject* parent = nullptr);
    QString get(Section section) const;

    static void setRegexp(const std::string& regexp);

private:
    void parse();

private:
    std::string m_line;
    QHash<Section, QString> m_sections;
    QUuid m_id;
    static std::string m_taskRegex;
};
