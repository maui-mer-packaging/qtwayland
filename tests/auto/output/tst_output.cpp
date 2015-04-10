/****************************************************************************
**
** Copyright (C) 2015 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
** Contact: http://www.qt.io/licensing/
**
** This file is part of the test suite of the Qt Toolkit.
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

#include "testcompositor.h"

#include "QtCompositor/qwaylandoutput.h"

#include <QtTest/QtTest>

class Output : public QWaylandOutput
{
    Q_OBJECT
public:
    Output(QWaylandCompositor *compositor, bool createDefaultMode = true)
        : QWaylandOutput(compositor, Q_NULLPTR, "", "")
    {
        if (createDefaultMode) {
            QWaylandOutputMode *mode = new QWaylandOutputMode("defaultMode",
                                                              QSize(1024, 768), 60000);
            setModes(QWaylandOutputModeList() << mode);
        }
    }

    void addModes(const QWaylandOutputModeList &list)
    {
        setModes(list);
    }
};

class tst_Output : public QObject
{
    Q_OBJECT
public:
    tst_Output()
    {
        setenv("XDG_RUNTIME_DIR", ".", 1);
    }

private Q_SLOTS:
    void outputsCount();
    void defaultMode();
    void modes();
};

void tst_Output::outputsCount()
{
    TestCompositor compositor(false);

    QCOMPARE(compositor.outputs().count(), 0);

    Output *output1 = new Output(&compositor);
    QCOMPARE(compositor.outputs().count(), 1);

    QVERIFY(compositor.primaryOutput() == output1);

    Output *output2 = new Output(&compositor);
    QCOMPARE(compositor.outputs().count(), 2);

    compositor.setPrimaryOutput(output2);
    QVERIFY(compositor.primaryOutput() == output2);

    delete output2;
    QCOMPARE(compositor.outputs().count(), 1);

    delete output1;
    QCOMPARE(compositor.outputs().count(), 0);
}

void tst_Output::defaultMode()
{
    TestCompositor compositor(false);

    Output *output = new Output(&compositor);

    QCOMPARE(output->modes().count(), 1);
    QCOMPARE(output->currentMode()->id(), QStringLiteral("defaultMode"));
    QCOMPARE(output->currentMode()->size(), QSize(1024, 768));
    QCOMPARE(output->currentMode()->refreshRate(), 60000);

    delete output;
}

void tst_Output::modes()
{
    TestCompositor compositor(false);

    Output *output = new Output(&compositor, false);
    QSignalSpy modesChangedSpy(output, SIGNAL(modesChanged()));
    QSignalSpy currentModeChangedSpy(output, SIGNAL(currentModeChanged()));
    QSignalSpy preferredModeChangedSpy(output, SIGNAL(preferredModeChanged()));

    QWaylandOutputModeList list;
    list << new QWaylandOutputMode("mode1", QSize(1024, 768), 100000);
    list << new QWaylandOutputMode("mode2", QSize(1366, 768), 60000);
    list << new QWaylandOutputMode("mode3", QSize(1920, 1080), 59000);
    output->addModes(list);

    QTRY_COMPARE(modesChangedSpy.count(), 1);
    QCOMPARE(output->modes().count(), 3);
    QVERIFY(output->currentMode() == output->preferredMode());
    QCOMPARE(output->currentMode()->id(), output->preferredMode()->id());
    QVERIFY(output->currentMode() == list.at(2));

    QCOMPARE(output->preferredMode()->id(), QStringLiteral("mode3"));
    QCOMPARE(output->preferredMode()->size(), QSize(1920, 1080));
    QCOMPARE(output->preferredMode()->refreshRate(), 59000);

    QCOMPARE(output->currentMode()->id(), QStringLiteral("mode3"));
    QCOMPARE(output->currentMode()->size(), QSize(1920, 1080));
    QCOMPARE(output->currentMode()->refreshRate(), 59000);

    output->setCurrentMode(list.at(1));
    QVERIFY(output->currentMode() == list.at(1));
    QTRY_COMPARE(currentModeChangedSpy.count(), 1);
    QTRY_COMPARE(preferredModeChangedSpy.count(), 0);

    QCOMPARE(output->currentMode()->id(), QStringLiteral("mode2"));
    QCOMPARE(output->currentMode()->size(), QSize(1366, 768));
    QCOMPARE(output->currentMode()->refreshRate(), 60000);

    QCOMPARE(output->preferredMode()->name(), QStringLiteral("1920x1080@59000"));
    QCOMPARE(output->currentMode()->name(), QStringLiteral("1366x768@60000"));
}

#include <tst_output.moc>
QTEST_MAIN(tst_Output);
