#ifndef JIEBACONTROLLER_H
#define JIEBACONTROLLER_H

#include <QObject>
#include <QMap>
#include <QTimer>
#include <QDebug>
#include <QProcess>
#include <QJsonDocument>
#include <QJsonObject>

typedef struct{
    QString name;
    int times;
}SELECT_ITEM;

class JiebaController : public QObject
{
    Q_OBJECT
public:
    explicit JiebaController(QObject *parent = nullptr);

    int appState = 0;
    QString txtToProcess;                        /*待处理的文本*/
    QStringList highWord;                       /*词频分析结果*/

    QProcess gzip;
    QByteArray result;
    QJsonDocument jsonDocument;
    QString jsonString;
    QVariantMap vmap;

    QList<SELECT_ITEM> selectList;

    QTimer * jiebaTimer = nullptr;
signals:

public slots:
    void InvokeJiebaProcess();
    void SetTxtToProcess(QString txt);
    void MapConventer();
    QStringList GetHighWord();
};

/*针对一个表的单例工厂*/
class JiebaControllerFactory
{
public:
    static JiebaController * instance(QObject *parent = nullptr)
    {
        if (m_pInstance == NULL)
        {
            m_pInstance = new JiebaController(parent);
        }
        return m_pInstance;
    }

    static void Release()
    {
        if (m_pInstance != NULL)
        {
            delete m_pInstance;
            m_pInstance = NULL;
        }
    }
public:
    static JiebaController * m_pInstance;
};


#endif // JIEBACONTROLLER_H
