#pragma once

#include <QObject>

#include <fstream>
#include <string>

class FileReader : public QObject {
    Q_OBJECT
public:
    explicit FileReader(QObject* parent = nullptr);
    virtual ~FileReader() {
        close();
    };
    void setPath(std::string filePath);
    std::string next();
    [[nodiscard]] bool end() const;

signals:

private:
    void close();

private:
    std::ifstream m_inputFile;
    std::string m_filePath;
};
