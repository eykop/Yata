#include "TasksListModel.h"

TasksListModel::TasksListModel(QObject* parent) : QAbstractTableModel(parent) {
    m_roleNames[Task::Section::STATUS] = "Status";
    m_roleNames[Task::Section::CONTEXTS] = "Contexts";
    m_roleNames[Task::Section::PRIORITY] = "Priority";
    m_roleNames[Task::Section::PROJECTS] = "Projects";
    m_roleNames[Task::Section::DESCRIPTION] = "Description";
    m_roleNames[Task::Section::CREATION_DATE] = "Created";
    m_roleNames[Task::Section::COMPLETION_DATE] = "Completed";
    m_roleNames[Task::Section::DUE] = "Due";
    m_roleNames[Task::Section::RECURING] = "Recuring";
}

QVariant TasksListModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        return m_roleNames[static_cast<Task::Section>(section) + Qt::UserRole + 1];
    } else {
    }
    return QVariant();
}

int TasksListModel::rowCount(const QModelIndex& parent) const {
    // For list models only the root node (an invalid parent) should return the
    // list's size. For all other (valid) parents, rowCount() should return 0 so
    // that it does not become a tree model.
    if (parent.isValid())
        return 0;

    return static_cast<int>(m_tasks.size());
}

QVariant TasksListModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid())
        return QVariant();

    //  role = static_cast<Task::Section>(role);
    auto a = m_tasks[static_cast<std::size_t>(index.row())]->get(static_cast<Task::Section>(role));
    return a;
}

void TasksListModel::pushBack(Task* task) {
    m_tasks.push_back(task);
}

QHash<int, QByteArray> TasksListModel::roleNames() const {
    return m_roleNames;
}

int TasksListModel::columnCount(const QModelIndex& parent) const {
    return static_cast<int>(m_roleNames.size());
}
