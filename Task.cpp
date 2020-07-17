#include "Task.h"

#include <iostream>
#include <regex>

const std::string Task::m_taskRegex = "(x)? ?(\\([A-Z]{1}\\) )?([0-9]{4}\\-[0-9]{2}\\-[0-9]{2} "
                                      ")?([0-9]{4}\\-[0-9]{2}\\-[0-9]{2} )?([a-zA-Z\\ "
                                      "\\.\\-0-9]+)(\\@[A-Za-z]+)? "
                                      "?(\\+[A-Za-z]+)?";

Task::Task(const std::string& task, QObject* parent) : QObject(parent), m_line(task) {
    parse();
}

void Task::parse() {
    std::vector<std::string> sections;
    std::string section;
    std::regex self_regex(m_taskRegex);
    std::smatch matches;

    if (std::regex_search(m_line, matches, self_regex)) {
        for (std::size_t i = 1; i < matches.size(); ++i) {
            m_sections[static_cast<Section>(static_cast<int>(Section::STATUS) + static_cast<int>(i) - 1)] =
                QString::fromStdString(matches[i].str()).trimmed();
        }
    }
}

QString Task::get(Section section) const {
    return m_sections[section];
}
