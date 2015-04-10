/****************************************************************************
**
** Copyright (C) 2015 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the plugins of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL21$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file. Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** As a special exception, The Qt Company gives you certain additional
** rights. These rights are described in The Qt Company LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "qwaylandquickoutput.h"
#include "qwaylandquickoutput_p.h"
#include "qwaylandquickcompositor.h"

QT_BEGIN_NAMESPACE

QWaylandQuickOutputPrivate::QWaylandQuickOutputPrivate(QWaylandQuickOutput *q)
    : updateScheduled(false)
    , q(q)
{
}

int QWaylandQuickOutputPrivate::modesCount(QQmlListProperty<QWaylandOutputMode> *prop)
{
    QWaylandQuickOutput *parent = static_cast<QWaylandQuickOutput *>(prop->object);
    return parent->modes().count();
}

QWaylandOutputMode *QWaylandQuickOutputPrivate::modesAt(QQmlListProperty<QWaylandOutputMode> *prop,
                                   int index)
{
    QWaylandQuickOutput *parent = static_cast<QWaylandQuickOutput *>(prop->object);
    return parent->modes().at(index);
}


QWaylandQuickOutput::QWaylandQuickOutput(QWaylandCompositor *compositor, QQuickWindow *window,
                                         const QString &manufacturer, const QString &model)
    : QWaylandOutput(compositor, window, manufacturer, model)
    , d_ptr(new QWaylandQuickOutputPrivate(this))
{
    connect(window, &QQuickWindow::beforeSynchronizing,
            this, &QWaylandQuickOutput::updateStarted,
            Qt::DirectConnection);
    connect(this, &QWaylandQuickOutput::modesChanged,
            this, &QWaylandQuickOutput::availableModesChanged);
}

QQuickWindow *QWaylandQuickOutput::quickWindow() const
{
    return static_cast<QQuickWindow *>(window());
}

QQmlListProperty<QWaylandOutputMode> QWaylandQuickOutput::availableModes()
{
    return QQmlListProperty<QWaylandOutputMode>(this, Q_NULLPTR,
                                                QWaylandQuickOutputPrivate::modesCount,
                                                QWaylandQuickOutputPrivate::modesAt);
}

void QWaylandQuickOutput::update()
{
    Q_D(QWaylandQuickOutput);

    if (!d->updateScheduled) {
        quickWindow()->update();
        d->updateScheduled = true;
    }
}

void QWaylandQuickOutput::updateStarted()
{
    Q_D(QWaylandQuickOutput);

    d->updateScheduled = false;
    compositor()->frameStarted();
    compositor()->cleanupGraphicsResources();
}

QT_END_NAMESPACE
