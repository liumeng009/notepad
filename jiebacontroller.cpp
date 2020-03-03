#include "jiebacontroller.h"

JiebaController * JiebaControllerFactory::m_pInstance = nullptr;

JiebaController::JiebaController(QObject *parent) : QObject(parent)
  ,jiebaTimer(new QTimer(this))
{
    connect(jiebaTimer,SIGNAL(timeout()),this,SLOT(InvokeJiebaProcess()));
    jiebaTimer->start(1000);  //每1000ms执行一次
}
void JiebaController::SetTxtToProcess(QString txt)
{
    txtToProcess = txt;
}

void JiebaController::InvokeJiebaProcess()
{
    qDebug()<<"appState"<<appState;
    switch (appState) {

    case 0:
        gzip.start("./leon.exe", QStringList() << txtToProcess);
        appState = 1;
        break;
    case 1:
        if (!gzip.waitForStarted())
            appState = 2;
        break;
    case 2:
        //gzip.write("人物、情节、环境是小说的三要素。情节一般包括开端、发展、高潮、结局四部分，有的包括序幕、尾声。环境包括自然环境和社会环境。 小说按照篇幅及容量可分为长篇、中篇、短篇和微型小说（小小说）。按照表现的内容可分为神话、仙侠、武侠、科幻、悬疑、古传、当代、浪漫青春、游戏竞技等。按照体制可分为章回体小说、日记体小说、书信体小说、自传体小说。按照语言形式可分为文言小说和白话小说。");
        gzip.closeWriteChannel();
        appState = 3;
        break;
    case 3:
        if (!gzip.waitForFinished())
            appState = 4;
        break;
    case 4:
        result = gzip.readAll();

        jsonString = QString::fromLocal8Bit(result);

        jsonDocument = QJsonDocument::fromJson(jsonString.toUtf8());
        if(jsonDocument.isNull())
        {
            qDebug()<< "String NULL"<< jsonString.toLocal8Bit().data();
        }
        qDebug()<<jsonDocument.object();
        vmap = jsonDocument.object().toVariantMap();
        MapConventer();
        appState = 0;
        break;
    default:break;
    }
}

static bool FreqCompare(SELECT_ITEM a, SELECT_ITEM b){
    return (a.times > b.times);
}

void JiebaController::MapConventer()
{
    QVariantMap::iterator it;
    SELECT_ITEM item;

    selectList.clear();
    for (it = vmap.begin(); it != vmap.end(); it++) {
        item.name = it.key();
        item.times = it.value().toInt();
        selectList.append(item);
    }

    std::sort(selectList.begin(),selectList.end(),FreqCompare);

    highWord.clear();
    foreach(item ,selectList){
        if(item.times > 1)
        {
            highWord << item.name;
        }
    };
}

QStringList JiebaController::GetHighWord()
{
    return highWord;
}
