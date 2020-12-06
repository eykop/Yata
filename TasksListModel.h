#pragma once
#include "Task.h"

#include <QAbstractTableModel>

class FileReader;

class TasksListModel : public QAbstractTableModel {
    Q_OBJECT

public:
    explicit TasksListModel(QObject* parent = nullptr);

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Basic functionality:
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    void pushBack(Task* task);

private:
    std::vector<Task*> m_tasks;
    QHash<int, QByteArray> m_roleNames;

    // QAbstractItemModel interface
public:
    QHash<int, QByteArray> roleNames() const override;

    // QAbstractItemModel interface
public:
    int columnCount(const QModelIndex& parent) const override;
};
