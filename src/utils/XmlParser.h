#pragma once

#include "XmlSection.h"

#include <QFile>
#include <QObject>
#include <QString>
#include <QtXml>

#include <memory>
#include <vector>

class XmlParser : public QObject {
    Q_OBJECT
public:
    enum class LoadingResult { SUCESS, FAILED };

    XmlParser(QObject* parent = nullptr);

    [[nodiscard]] const std::vector<XmlSection>& xmlSections() const;

public:
    void parse();
    void init();

signals:
    void loaded(LoadingResult result);
    void canParse();

private:
    QDomDocument m_xmlDocument;
    std::vector<XmlSection> m_xmlSections;
};
