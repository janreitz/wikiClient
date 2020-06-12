#ifndef SEARCHRESULT_H
#define SEARCHRESULT_H

#include <QObject>

class SearchResult : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString title READ title NOTIFY titleChanged)
    Q_PROPERTY(QString matchContext READ matchContext NOTIFY matchContextChanged)

public:
    explicit SearchResult(const QString& title = QString(),
                          const QString& matchContext = QString());

signals:

    void titleChanged() const;
    void matchContextChanged() const;

private:
    QString m_title;
    QString m_matchContext;
};
Q_DECLARE_METATYPE(SearchResult)
#endif // SEARCHRESULT_H
