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

#include <QtCore/QCoreApplication>
#include <QtCore/QtMath>
#include <QtGui/QWindow>
#include <QtGui/QExposeEvent>
#include <private/qobject_p.h>

#include "wayland_wrapper/qwlcompositor_p.h"
#include "wayland_wrapper/qwloutput_p.h"
#include "qwaylandcompositor.h"
#include "qwaylandoutput.h"

class QWaylandOutputModePrivate : public QObjectPrivate
{
public:
    QWaylandOutputModePrivate() {}

    void resetName()
    {
        name = QStringLiteral("%1x%2@%3")
                .arg(size.width())
                .arg(size.height())
                .arg(refreshRate);
    }

    QString id;
    QString name;
    QSize size;
    int refreshRate;
};

/*!
    \class QWaylandOutputMode
    \inmodule QtCompositor
    \brief The QWaylandOutputMode class provides an output mode.

    This class is used by Wayland compositors to provide mode information
    to outputs.
*/
QWaylandOutputMode::QWaylandOutputMode(const QString &id, const QSize &size,
                                       int refreshRate)
    : QObject(*new QWaylandOutputModePrivate())
{
    Q_D(QWaylandOutputMode);
    d->id = id;
    d->size = size;
    d->refreshRate = refreshRate;
    d->resetName();
}

QString QWaylandOutputMode::id() const
{
    Q_D(const QWaylandOutputMode);
    return d->id;
}

QString QWaylandOutputMode::name() const
{
    Q_D(const QWaylandOutputMode);
    return d->name;
}

QSize QWaylandOutputMode::size() const
{
    Q_D(const QWaylandOutputMode);
    return d->size;
}

void QWaylandOutputMode::setSize(const QSize &size)
{
    Q_D(QWaylandOutputMode);

    if (d->size == size)
        return;

    d->size = size;
    Q_EMIT sizeChanged();

    d->resetName();
    Q_EMIT nameChanged();
}

int QWaylandOutputMode::refreshRate() const
{
    Q_D(const QWaylandOutputMode);
    return d->refreshRate;
}

void QWaylandOutputMode::setRefreshRate(int refreshRate)
{
    Q_D(QWaylandOutputMode);

    if (d->refreshRate == refreshRate)
        return;

    d->refreshRate = refreshRate;
    Q_EMIT refreshRateChanged();

    d->resetName();
    Q_EMIT nameChanged();
}

bool QWaylandOutputMode::operator==(QWaylandOutputMode &other)
{
    return (size() == other.size() &&
            refreshRate() == other.refreshRate());
}

bool QWaylandOutputMode::operator!=(QWaylandOutputMode &other)
{
    return (size() != other.size() ||
            refreshRate() != other.refreshRate());
}
