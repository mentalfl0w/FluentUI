#include "FluHttp.h"

#include <QThreadPool>
#include "HttpClient.h"
#include "FluApp.h"

using namespace  AeaQt;

FluHttp::FluHttp(QObject *parent)
    : QObject{parent}
{
    enabledBreakpointDownload(false);
}

Q_INVOKABLE void FluHttp::postString(QString params,QVariantMap headers){
    QVariantMap request = invokeIntercept(params,headers,"postString").toMap();
    QThreadPool::globalInstance()->start([=](){
        HttpClient client;
        Q_EMIT start();
        client.post(_url)
            .bodyWithRaw(request[params].toString().toUtf8())
            .headers(request["headers"].toMap())
            .onSuccess([=](QString result) {
                Q_EMIT success(result);
                Q_EMIT finish();
            })
            .onFailed([=](QNetworkReply* reply) {
                Q_EMIT error(reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt(),reply->errorString());
                Q_EMIT finish();
            })
            .block()
            .exec();
    });
}

void FluHttp::post(QVariantMap params,QVariantMap headers){
    QVariantMap request = invokeIntercept(params,headers,"post").toMap();
    QThreadPool::globalInstance()->start([=](){
        HttpClient client;
        Q_EMIT start();
        client.post(_url)
            .headers(headers)
            .bodyWithFormData(request["params"].toMap())
            .headers(request["headers"].toMap())
            .onSuccess([=](QString result) {
                Q_EMIT success(result);
                Q_EMIT finish();
            })
            .onFailed([=](QNetworkReply* reply) {
                Q_EMIT error(reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt(),reply->errorString());
                Q_EMIT finish();
            })
            .block()
            .exec();
    });
}

void FluHttp::postJson(QVariantMap params,QVariantMap headers){
    QVariantMap request = invokeIntercept(params,headers,"postJson").toMap();
    QThreadPool::globalInstance()->start([=](){
        HttpClient client;
        Q_EMIT start();
        client.post(_url)
            .headers(headers)
            .bodyWithRaw(QJsonDocument::fromVariant(request["params"]).toJson())
            .headers(request["headers"].toMap())
            .onSuccess([=](QString result) {
                Q_EMIT success(result);
                Q_EMIT finish();
            })
            .onFailed([=](QNetworkReply* reply) {
                Q_EMIT error(reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt(),reply->errorString());
                Q_EMIT finish();
            })
            .block()
            .exec();
    });
}

void FluHttp::get(QVariantMap params,QVariantMap headers){
    QVariantMap request = invokeIntercept(params,headers,"get").toMap();
    QThreadPool::globalInstance()->start([=](){
        HttpClient client;
        Q_EMIT start();
        client.get(_url)
            .queryParams(request["params"].toMap())
            .headers(request["headers"].toMap())
            .onSuccess([=](QString result) {
                Q_EMIT success(result);
                Q_EMIT finish();
            })
            .onFailed([=](QNetworkReply* reply) {
                Q_EMIT error(reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt(),reply->errorString());
                Q_EMIT finish();
            })
            .block()
            .exec();
    });
}

Q_INVOKABLE void FluHttp::download(QString path,QVariantMap params,QVariantMap headers){
    QVariantMap request = invokeIntercept(params,headers,"download").toMap();
    QThreadPool::globalInstance()->start([=](){
        HttpClient client;
        Q_EMIT start();
        client.get(_url)
            .download(path)
            .enabledBreakpointDownload(_enabledBreakpointDownload)
            .queryParams(request["params"].toMap())
            .headers(request["headers"].toMap())
            .onDownloadProgress([=](qint64 recv, qint64 total) {
                Q_EMIT downloadProgress(recv,total);
            })
            .onDownloadFileSuccess([=](QString result) {
                Q_EMIT success(result);
                Q_EMIT finish();
            })
            .onDownloadFileFailed([=](QString errorString) {
                Q_EMIT error(-1,errorString);
                Q_EMIT finish();
            })
            .block()
            .exec();
    });
}

QVariant FluHttp::invokeIntercept(const QVariant& params,const QVariant& headers,const QString& method){
    QVariantMap requet = {
        {"params",params},
        {"headers",headers},
        {"method",method}
    };
    if(!FluApp::getInstance()->httpInterceptor()){
        return requet;
    }
    QVariant target;
    QMetaObject::invokeMethod(FluApp::getInstance()->httpInterceptor(), "onIntercept",Q_RETURN_ARG(QVariant,target),Q_ARG(QVariant, requet));
    return target;
}
