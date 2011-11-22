/********************************************************************
 KWin - the KDE window manager
 This file is part of the KDE project.

Copyright (C) 2009 Martin Gräßlin <kde@martin-graesslin.com>

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*********************************************************************/


#include "ActivityChangedOSD.h"

#include <QDebug>
#include <QGraphicsScene>
#include <QRect>
#include <QDesktopWidget>
#include <KDE/Plasma/Theme>
#include <KDE/Plasma/PaintUtils>
#include <kiconloader.h>
#include <kicon.h>

//*******************************
// ActivityChangedOSD
//*******************************
ActivityChangedOSD::ActivityChangedOSD(ActivityManager* am)
        : QGraphicsView()
        , m_activityman(am)
        , m_scene(0)
        , m_active(false)
        , m_show(false)
        , m_delayTime(0)
        , m_smallView(true)
{
    setWindowFlags(Qt::X11BypassWindowManagerHint);
    setFrameStyle(QFrame::NoFrame);
    viewport()->setAutoFillBackground(false);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setAttribute(Qt::WA_TranslucentBackground);
    m_frame.setImagePath("dialogs/background");
    m_frame.setCacheAllRenderedFrames(true);
    m_frame.setEnabledBorders(Plasma::FrameSvg::AllBorders);

    m_item_frame.setImagePath("widgets/pager");
    m_item_frame.setCacheAllRenderedFrames(true);
    m_item_frame.setEnabledBorders(Plasma::FrameSvg::AllBorders);

    m_delayedHideTimer.setSingleShot(true);
    connect(&m_delayedHideTimer, SIGNAL(timeout()), this, SLOT(hide()));

    m_scene = new QGraphicsScene(0);
    setScene(m_scene);

    reconfigure();

    m_scene->addItem(new ActivityChangedText(m_activityman));
}

ActivityChangedOSD::~ActivityChangedOSD()
{
    delete m_scene;
}

void ActivityChangedOSD::reconfigure()
{
    m_smallView = true;
    m_show = true;
    m_delayTime = 3000;
}

void ActivityChangedOSD::activityChanged(const QString& id)
{
    if (!m_show)
        return;

    if (m_active) {
        resize();
        update();
    } else {
        m_active = true;
        resize();
        show();
        raise();
    }

    m_delayedHideTimer.start(m_delayTime);
}

void ActivityChangedOSD::hideEvent(QHideEvent*)
{
    m_delayedHideTimer.stop();
    m_active = false;
}

void ActivityChangedOSD::drawBackground(QPainter* painter, const QRectF& rect)
{
    painter->save();
    painter->setCompositionMode(QPainter::CompositionMode_Source);
    qreal left, top, right, bottom;
    m_frame.getMargins(left, top, right, bottom);
    m_frame.paintFrame(painter, rect.adjusted(-left, -top, right, bottom));
    painter->restore();
}

void ActivityChangedOSD::resize()
{
    const int screenId = qApp->desktop()->screenNumber(this);
    const QRect screenRect = qApp->desktop()->availableGeometry(screenId);

    qreal left, top, right, bottom;
    m_frame.getMargins(left, top, right, bottom);
    
    float width = 0.0f;
    float height = 0.0f;

    QRect r = fontMetrics().boundingRect(
                  m_activityman->ActivityName(m_activityman->CurrentActivity())
              );
    height = 32.0f + top + bottom + 4.0f;
    width = r.width() + 32 + left + right + 16.0f;

    QRect rect = QRect(screenRect.x() + (screenRect.width() - width) / 2,
                       screenRect.y() + (screenRect.height() - height) / 2,
                       width,
                       height);
    setGeometry(rect);
    m_scene->setSceneRect(0, 0, width, height);
    m_frame.resizeFrame(QSize(width, height));

    if (Plasma::Theme::defaultTheme()->windowTranslucencyEnabled()) {
        // blur background
        Plasma::WindowEffects::enableBlurBehind(winId(), true, m_frame.mask());
        Plasma::WindowEffects::overrideShadow(winId(), true);
    } else {
        // do not trim to mask with compositing enabled, otherwise shadows are cropped
        setMask(m_frame.mask());
    }
    
    foreach (QGraphicsItem * it, m_scene->items()) {
        ActivityChangedText* text = qgraphicsitem_cast<ActivityChangedText*>(it);
        if (text) {
            text->setPos(32.0f / 2.0f, 32.0f / 2.0f);
        }
    }
}

//*******************************
// ActivityChangedText
//*******************************
ActivityChangedText::ActivityChangedText(ActivityManager* am)
        : QGraphicsItem()
        , m_activityman(am)
        , m_width(0.0f)
        , m_height(0.0f)
{

}

ActivityChangedText::~ActivityChangedText()
{

}

QRectF ActivityChangedText::boundingRect() const
{
    return QRectF(0, 0, m_width, m_height);
}

void ActivityChangedText::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*)
{
    painter->setPen(Plasma::Theme::defaultTheme()->color(Plasma::Theme::TextColor));

    KIcon icon = KIcon(m_activityman->ActivityIcon(m_activityman->CurrentActivity()));
    icon.paint(painter, 0, 0, 32, 32);

    painter->drawText(32.0f + 4.0f, (32.0f / 2.0f) - (boundingRect().height() / 2.0f) , m_activityman->ActivityName(m_activityman->CurrentActivity()));
}

