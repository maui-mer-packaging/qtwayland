/****************************************************************************
**
** Copyright (C) 2014 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
** Copyright (C) 2014 Jolla Ltd, author: <giulio.camuffo@jollamobile.com>
** Contact: http://www.qt-project.org/legal
**
** This file is part of the plugins of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QQuickWindow>

#include "qwaylandclient.h"
#include "qwaylandquickcompositor.h"
#include "qwaylandquickcompositor_p.h"
#include "qwaylandquicksurface.h"
#include "qwaylandsurfaceitem.h"
#include "qwaylandoutput.h"
#include "wayland_wrapper/qwlsurface_p.h"

QT_BEGIN_NAMESPACE

QWaylandQuickCompositorPrivate::QWaylandQuickCompositorPrivate(QWaylandQuickCompositor *compositor, QWaylandCompositor::ExtensionFlags extensions)
    : QtWayland::Compositor(compositor, extensions)
    , updateScheduled(false)
{
}

void QWaylandQuickCompositorPrivate::compositor_create_surface(Resource *resource, uint32_t id)
{
    // Ask the compositor to create a surface, it may want to create a custom
    // surface object with some output management logic or non standard
    // features
    QWaylandQuickSurface *surface = static_cast<QWaylandQuickCompositor *>(m_qt_compositor)->createSurface(resource->client(), id);
    m_surfaces << surface->handle();
    //BUG: This may not be an on-screen window surface though
    m_qt_compositor->surfaceCreated(surface);
}

void QWaylandQuickCompositorPrivate::updateStarted()
{
    updateScheduled = false;
    m_qt_compositor->frameStarted();
    m_qt_compositor->cleanupGraphicsResources();
}

void QWaylandQuickCompositorPrivate::outputAdded(QWaylandOutput *output)
{
    if (!output->window())
        return;

    QQuickWindow *window = qobject_cast<QQuickWindow *>(output->window());
    if (!window)
        return;

    connect(window, &QQuickWindow::beforeSynchronizing,
            this, &QWaylandQuickCompositorPrivate::updateStarted,
            Qt::DirectConnection);

    QWaylandQuickCompositor *compositor = static_cast<QWaylandQuickCompositor *>(m_qt_compositor);
    if (compositor)
        Q_EMIT compositor->outputsChanged();
}

void QWaylandQuickCompositorPrivate::outputRemoved(QWaylandOutput *output)
{
    Q_UNUSED(output);

    QWaylandQuickCompositor *compositor = static_cast<QWaylandQuickCompositor *>(m_qt_compositor);
    if (compositor)
        Q_EMIT compositor->outputsChanged();
}


QWaylandQuickCompositor::QWaylandQuickCompositor(const char *socketName, ExtensionFlags extensions)
                       : QWaylandCompositor(socketName, new QWaylandQuickCompositorPrivate(this, extensions))
{
    qmlRegisterUncreatableType<QWaylandSurfaceItem>("QtCompositor", 1, 0, "WaylandSurfaceItem", QObject::tr("Cannot create instance of WaylandSurfaceItem"));
    qmlRegisterUncreatableType<QWaylandQuickSurface>("QtCompositor", 1, 0, "WaylandQuickSurface", QObject::tr("Cannot create instance of WaylandQuickSurface"));
    qmlRegisterUncreatableType<QWaylandClient>("QtCompositor", 1, 0, "WaylandClient", QObject::tr("Cannot create instance of WaylandClient"));
    qmlRegisterUncreatableType<QWaylandOutput>("QtCompositor", 1, 0, "WaylandOutput", QObject::tr("Cannot create instance of WaylandOutput"));

    connect(this, &QWaylandQuickCompositor::outputAdded,
            d_ptr(), &QWaylandQuickCompositorPrivate::outputAdded);
    connect(this, &QWaylandQuickCompositor::outputRemoved,
            d_ptr(), &QWaylandQuickCompositorPrivate::outputRemoved);
}

QWaylandQuickCompositorPrivate *QWaylandQuickCompositor::d_ptr()
{
    return static_cast<QWaylandQuickCompositorPrivate *>(m_compositor);
}

void QWaylandQuickCompositor::update()
{
    if (!d_ptr()->updateScheduled) {
        Q_FOREACH (QWaylandOutput *output, outputs()) {
            QQuickWindow *window = qobject_cast<QQuickWindow *>(output->window());
            if (window)
                window->update();
        }

        d_ptr()->updateScheduled = true;
    }
}

QWaylandQuickSurface *QWaylandQuickCompositor::createSurface(wl_client *client, quint32 id)
{
    return new QWaylandQuickSurface(client, id, this);
}

QWaylandSurfaceView *QWaylandQuickCompositor::createView(QWaylandSurface *surf)
{
    return new QWaylandSurfaceItem(static_cast<QWaylandQuickSurface *>(surf));
}

QQmlListProperty<QWaylandOutput> QWaylandQuickCompositor::outputList()
{
    return QQmlListProperty<QWaylandOutput>(this, 0, outputsCount, outputAt);
}

int QWaylandQuickCompositor::outputsCount(QQmlListProperty<QWaylandOutput> *p)
{
    QWaylandQuickCompositor *parent = static_cast<QWaylandQuickCompositor *>(p->object);
    if (!parent)
        return 0;
    return parent->outputs().count();
}

QWaylandOutput *QWaylandQuickCompositor::outputAt(QQmlListProperty<QWaylandOutput> *p, int index)
{
    QWaylandQuickCompositor *parent = static_cast<QWaylandQuickCompositor *>(p->object);
    if (!parent)
        return Q_NULLPTR;
    return parent->outputs().at(index);
}

QT_END_NAMESPACE
