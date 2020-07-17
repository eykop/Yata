#include "FileReader.h"

#include <iostream>

FileReader::FileReader(QObject* parent) : QObject(parent) {
}

void FileReader::setPath(std::string filePath) {
    m_filePath = filePath;
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
