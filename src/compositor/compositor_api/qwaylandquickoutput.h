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

#ifndef QWAYLANDQUICKOUTPUT_H
#define QWAYLANDQUICKOUTPUT_H

#include <QtQuick/QQuickWindow>
#include <QtCompositor/qwaylandoutput.h>

QT_BEGIN_NAMESPACE

class QWaylandQuickCompositor;
class QWaylandQuickOutputPrivate;
class QQuickWindow;

class Q_COMPOSITOR_EXPORT QWaylandQuickOutput : public QWaylandOutput
{
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<QWaylandOutputMode> availableModes READ availableModes NOTIFY availableModesChanged)
public:
    QWaylandQuickOutput(QWaylandCompositor *compositor, QQuickWindow *window,
                        const QString &manufacturer, const QString &model);

    QQuickWindow *quickWindow() const;

    QQmlListProperty<QWaylandOutputMode> availableModes();

    void update() Q_DECL_OVERRIDE;

Q_SIGNALS:
    void availableModesChanged();

public Q_SLOTS:
    void updateStarted();

private:
    Q_DECLARE_PRIVATE(QWaylandQuickOutput)
    QWaylandQuickOutputPrivate *const d_ptr;
};

QT_END_NAMESPACE

#endif
