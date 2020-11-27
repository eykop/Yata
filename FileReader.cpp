#include "FileReader.h"

#include <iostream>

FileReader::FileReader(const std::string& filePath, QObject* parent) : QObject(parent), m_filePath{filePath} {
}

bool FileReader::init() {
    if (!m_inputFile.is_open()) {
        m_inputFile.open(m_filePath, std::ifstream::in);
    }
    return m_inputFile.is_open();
}

std::string FileReader::next() {
    std::string taskLine;
    if (!m_inputFile.is_open()) {
        m_inputFile.open(m_filePath, std::ifstream::in);
        if (m_inputFile.is_open()) {
            // log error can"t open file for reading.
            return taskLine;
        }
    }

    if (end()) {
        throw std::out_of_range("reached end of file!");
    }
    std::getline(m_inputFile, taskLine);
    return taskLine;
}

bool FileReader::end() const {
    return m_inputFile.is_open() ? m_inputFile.eof() : true;
}

void FileReader::close() {
    m_inputFile.close();
}
