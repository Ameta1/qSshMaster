/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Hierarchy: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the hierarchy form at https://www.qt.io/hierarchy-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>

extern "C"
{
   #include<gost-engine.h>
}

#include "hierarchymodel.h"
#include <openssl/obj_mac.h>
#include <openssl/x509.h>
#include <openssl/err.h>
#include <openssl/ssl.h>
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/engine.h>
#include "ssldefs.h"
#include "sslfunctions.h"
#include "certificatemodel.h"

void debug() {
    Sslinfo sslinfo {
        "RU",
        "77",
        "zelenograd",
        "miet",
        "localhost",
        3650
    };
    EVP_PKEY_ptr pkey (EVP_PKEY_new(), EVP_PKEY_free);
    EVP_PKEY *pkeyDP = pkey.get();
    qDebug() <<"generateGOSTKey"<< sslFunctions::generateGOSTKey(&pkeyDP, "A");
    qDebug() <<"saveEVPKey"<< sslFunctions::saveEVPPrivateKey(&pkeyDP, "/home/gregory/MY_COOL_KEY.pem");
    qDebug() <<"saveEVPKey"<< sslFunctions::saveEVPPublicKey(&pkeyDP, "/home/gregory/MY_COOL_KEY.pem");
    qDebug() << "createRootCert" << sslFunctions::createRootX509Cert("/home/gregory/MY_COOL_KEY.pem", "/home/gregory/MY_COOL_CERT.crt", sslinfo);
};

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    ENGINE_load_gost();
    ENGINE_ptr engine (ENGINE_by_id("gost"), ENGINE_free);
    if(!engine.get())
    {
        return Fail; //Failed to get structural reference to engine
    }
    ENGINE_set_default(engine.get(), ENGINE_METHOD_ALL);

    QQuickStyle::setStyle("Material");
    qmlRegisterType<certificateModel>("Custom.CertificateModel", 1, 0, "CertificateModel");
    QScopedPointer<HierarchyModel> hierarchyModel(new HierarchyModel);
    QQmlApplicationEngine qmlApplicationEngine;
    qmlRegisterSingletonInstance("Custom.HierarchyModel", 1, 0, "HierarchyModel", hierarchyModel.get());
//    qmlEngine.rootContext()->setContextProperty("hierarchyModel", hierarchyModel);
    qmlApplicationEngine.load(QUrl(QStringLiteral("qrc:/hierarchylist.qml")));

    return app.exec();
}

