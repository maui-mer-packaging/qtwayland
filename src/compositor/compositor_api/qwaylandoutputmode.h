/****************************************************************************
**
** Copyright (C) 2015 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
** Contact: http://www.qt.io/licensing/
**
** This file is part of the Qt Compositor.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
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

#ifndef QWAYLANDOUTPUTMODE_H
#define QWAYLANDOUTPUTMODE_H

#include <QtCore/QMap>
#include <QtCore/QObject>
#include <QtCore/QRect>
#include <QtCore/QSize>

#include <QtCompositor/qwaylandexport.h>

QT_BEGIN_NAMESPACE

class QWaylandOutputModePrivate;

class Q_COMPOSITOR_EXPORT QWaylandOutputMode : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QWaylandOutputMode)
    Q_PROPERTY(QString id READ id CONSTANT)
    Q_PROPERTY(QString name READ name NOTIFY nameChanged)
    Q_PROPERTY(QSize size READ size NOTIFY sizeChanged)
    Q_PROPERTY(int refreshRate READ refreshRate NOTIFY refreshRateChanged)
public:
    QWaylandOutputMode(const QString &id, const QSize &size, int refreshRate);

    QString id() const;

    QString name() const;

    QSize size() const;
    void setSize(const QSize &size);

    int refreshRate() const;
    void setRefreshRate(int refreshRate);

    bool operator==(QWaylandOutputMode &other);
    bool operator!=(QWaylandOutputMode &other);

Q_SIGNALS:
    void nameChanged();
    void sizeChanged();
    void refreshRateChanged();
};

typedef QList<QWaylandOutputMode *> QWaylandOutputModeList;
typedef QMap<QString, QWaylandOutputMode *> QWaylandOutputModeMap;

Q_DECLARE_METATYPE(QWaylandOutputModeList)
Q_DECLARE_METATYPE(QWaylandOutputModeMap)

QT_END_NAMESPACE

#endif // QWAYLANDOUTPUTMODE_H