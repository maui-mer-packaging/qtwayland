/****************************************************************************
**
** Copyright (C) 2014 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
** Copyright (C) 2013 Klarälvdalens Datakonsult AB (KDAB).
** Contact: http://www.qt-project.org/legal
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
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
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

#include <private/qobject_p.h>

#include "wayland_wrapper/qwlcompositor_p.h"
#include "wayland_wrapper/qwloutput_p.h"
#include "qwaylandcompositor.h"
#include "qwaylandoutput.h"
#include "qwaylandoutput_p.h"

QWaylandOutputPrivate::QWaylandOutputPrivate(QWaylandCompositor *compositor, QWindow *window)
    : QtWayland::Output(compositor->handle(), window)
{
}


QWaylandOutput::QWaylandOutput(QWaylandCompositor *compositor, QWindow *window)
    : QObject(*new QWaylandOutputPrivate(compositor, window))
{
    Q_D(QWaylandOutput);
    d->m_output = this;
    d->compositor()->addOutput(this);
}

QWaylandOutput::~QWaylandOutput()
{
    Q_D(QWaylandOutput);
    d->compositor()->removeOutput(this);
}

QWaylandOutput *QWaylandOutput::fromResource(wl_resource *resource)
{
    if (!resource)
        return Q_NULLPTR;

    QtWayland::OutputResource *outputResource = static_cast<QtWayland::OutputResource *>(
        QtWayland::Output::Resource::fromResource(resource));
    if (!outputResource)
        return Q_NULLPTR;

    QtWayland::Output *output = static_cast<QtWayland::Output *>(outputResource->output_object);
    if (!output)
        return Q_NULLPTR;

    return output->output();
}

QString QWaylandOutput::manufacturer() const
{
    Q_D(const QWaylandOutput);

    return d->manufacturer();
}

void QWaylandOutput::setManufacturer(const QString &manufacturer)
{
    Q_D(QWaylandOutput);

    if (d->manufacturer() == manufacturer)
        return;

    d->setManufacturer(manufacturer);
    Q_EMIT manufacturerChanged();
}

QString QWaylandOutput::model() const
{
    Q_D(const QWaylandOutput);

    return d->model();
}

void QWaylandOutput::setModel(const QString &model)
{
    Q_D(QWaylandOutput);

    if (d->model() == model)
        return;

    d->setModel(model);
    Q_EMIT modelChanged();
}

QRect QWaylandOutput::geometry() const
{
    Q_D(const QWaylandOutput);

    return d->geometry();
}

void QWaylandOutput::setGeometry(const QRect &geometry)
{
    Q_D(QWaylandOutput);

    if (d->geometry() == geometry)
        return;

    d->setGeometry(geometry);
    Q_EMIT geometryChanged();
}

QRect QWaylandOutput::availableGeometry() const
{
    Q_D(const QWaylandOutput);

    if (!d->availableGeometry().isValid())
        return d->geometry();

    return d->availableGeometry();
}

void QWaylandOutput::setAvailableGeometry(const QRect &availableGeometry)
{
    Q_D(QWaylandOutput);

    if (d->availableGeometry() == availableGeometry)
        return;

    d->setAvailableGeometry(availableGeometry);
    Q_EMIT availableGeometryChanged();
}

QSize QWaylandOutput::physicalSize() const
{
    Q_D(const QWaylandOutput);

    return d->physicalSize();
}

void QWaylandOutput::setPhysicalSize(const QSize &size)
{
    Q_D(QWaylandOutput);

    if (d->physicalSize() == size)
        return;

    d->setPhysicalSize(size);
    Q_EMIT physicalSizeChanged();
}

int QWaylandOutput::refreshRate() const
{
    Q_D(const QWaylandOutput);

    return d->refreshRate();
}

void QWaylandOutput::setRefreshRate(int refreshRate)
{
    Q_D(QWaylandOutput);

    if (d->refreshRate() == refreshRate)
        return;

    d->setRefreshRate(refreshRate);
    Q_EMIT refreshRateChanged();
}

QWaylandOutput::Subpixel QWaylandOutput::subpixel() const
{
    Q_D(const QWaylandOutput);

    return d->subpixel();
}

void QWaylandOutput::setSubpixel(const Subpixel &subpixel)
{
    Q_D(QWaylandOutput);

    if (d->subpixel() == subpixel)
        return;

    d->setSubpixel(subpixel);
    Q_EMIT subpixelChanged();
}

QWaylandOutput::Transform QWaylandOutput::transform() const
{
    Q_D(const QWaylandOutput);

    return d->transform();
}

void QWaylandOutput::setTransform(const Transform &transform)
{
    Q_D(QWaylandOutput);

    if (d->transform() == transform)
        return;

    d->setTransform(transform);
    Q_EMIT transformChanged();
}

int QWaylandOutput::scaleFactor() const
{
    Q_D(const QWaylandOutput);

    return d->scaleFactor();
}

void QWaylandOutput::setScaleFactor(int scale)
{
    Q_D(QWaylandOutput);

    if (d->scaleFactor() == scale)
        return;

    d->setScaleFactor(scale);
    Q_EMIT scaleFactorChanged();

}

QWindow *QWaylandOutput::window() const
{
    Q_D(const QWaylandOutput);

    return d->window();
}

QtWayland::Output *QWaylandOutput::handle()
{
    Q_D(QWaylandOutput);

    return d;
}
