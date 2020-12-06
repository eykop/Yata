#include "XmlParser.h"

#include <QDebug>
#include <QDir>

namespace {

    constexpr int DEFAULT_SECTIONS_COUNT = 8;
    namespace XmlTags {
        const QString XML_SECTION = QStringLiteral("section");
        const QString MAIN = QStringLiteral("main");
        const QString EXTRA = QStringLiteral("extra");
        const QString NAME = QStringLiteral("name");
        const QString REGEXP = QStringLiteral("regexp");
        const QString INDEX = QStringLiteral("index");
    } // namespace XmlTags

} // namespace

XmlParser::XmlParser(QObject* parent) : QObject(parent) {
    m_xmlSections.reserve(DEFAULT_SECTIONS_COUNT);
    connect(this, &XmlParser::canParse, this, &XmlParser::parse);
}

void XmlParser::init() {
    const auto xmlDataFilePath = QDir::currentPath() + "/data.xml";
    QFile fileReader{xmlDataFilePath};
    if (!fileReader.open(QIODevice::ReadOnly)) {
        // Error while loading file
        qDebug() << "Error while loading xml file";
        fileReader.close();
        return;
    }
    // Set data into the QDomDocument before processing
    QString erroMsg;
    if (!m_xmlDocument.setContent(&fileReader, &erroMsg)) {
        qDebug() << "Error while setting xml content" << erroMsg;
        fileReader.close();
        return;
    }
    fileReader.close();
    emit canParse();
}

void XmlParser::parse() {

    auto parseSections = [this](const QDomElement& sections) -> bool {
        QDomElement section = sections.firstChild().toElement();
        while (!section.isNull()) {
            Q_ASSERT(section.tagName() == XmlTags::XML_SECTION);
            if (section.tagName() == XmlTags::XML_SECTION) {
                const QString name = section.attribute(XmlTags::NAME);
                const QString regex = section.attribute(XmlTags::REGEXP);
                bool ok = false;
                const int index = section.attribute(XmlTags::INDEX).toInt(&ok);
                Q_ASSERT(ok && !name.isEmpty() && !regex.isEmpty());
                if (ok && !name.isEmpty() && !regex.isEmpty()) {
                    m_xmlSections.emplace_back(XmlSection{name, regex, index});
                } else {
                    m_xmlSections.clear();
                    emit loaded(LoadingResult::FAILED);
                    return false;
                }
            }
            // Next component
            section = section.nextSibling().toElement();
        }
        return true;
    };

    const QDomElement root = m_xmlDocument.documentElement();
    const QDomElement sections = root.firstChild().toElement();
    const QDomElement main_sections = sections.firstChild().toElement();
    const auto tagname = main_sections.tagName();
    Q_ASSERT(XmlTags::MAIN == tagname);
    if (parseSections(main_sections)) {
        const QDomElement extra_sections = main_sections.nextSibling().toElement();
        Q_ASSERT(XmlTags::EXTRA == extra_sections.tagName());
        if(parseSections(extra_sections)) {
            emit loaded(LoadingResult::SUCESS);
        }
    }
}

const std::vector<XmlSection>& XmlParser::xmlSections() const {
    return m_xmlSections;
}
