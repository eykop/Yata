#pragma once

#include <QByteArray>
#include <QHash>
#include <QObject>
#include <QString>

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
        CONTEXTS
    };
    Q_ENUM(Section)

    explicit Task(const std::string& task, QObject* parent = nullptr);
    QString get(Section section) const;

signals:
private:
    void parse();

private:
    std::string m_line;
    QHash<Section, QString> m_sections;
    static const std::string m_taskRegex;
};
