#pragma once

#include <QObject>

#include <fstream>
#include <string>

class FileReader : public QObject {
    Q_OBJECT
public:
    explicit FileReader(const std::string& filePath, QObject* parent = nullptr);
    virtual ~FileReader() {
        close();
    };
    bool init();
    std::string next();
    [[nodiscard]] bool end() const;

signals:

private:
    void close();

private:
    std::ifstream m_inputFile;
    std::string m_filePath;
};
