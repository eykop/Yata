#include "Task.h"

#include <QDateTime>
#include <QDebug>

#include <regex>

std::string Task::m_taskRegex = "";

Task::Task(const std::string& task, QObject* parent) : QObject(parent), m_line(task), m_id{QUuid::createUuid()} {
    parse();
}

void Task::parse() {
    std::vector<std::string> sections;
    std::string section;
    std::regex self_regex(m_taskRegex);
    std::smatch matches;

    if (std::regex_search(m_line, matches, self_regex)) {
        for (std::size_t i = 1; i < matches.size(); ++i) {
            const auto match = matches[i].str();
            if (static_cast<int>(Section::DUE) > static_cast<int>(i)) {
                m_sections[static_cast<Section>(static_cast<int>(Section::STATUS) + static_cast<int>(i) - 1)] =
                    QString::fromStdString(match).trimmed();
            }
        }
    }
}

QString Task::get(Section section) const {
    return m_sections[section];
}

void Task::setRegexp(const std::string& regexp) {
    m_taskRegex = regexp;
}
