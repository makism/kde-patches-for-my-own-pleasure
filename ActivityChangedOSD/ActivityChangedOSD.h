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

#ifndef ACTIVITYCHANGEOSD_H
#define ACTIVITYCHANGEOSD_H

#include <QGraphicsView>
#include <QGraphicsItem>
#include <QTimer>
#include <KDE/Plasma/FrameSvg>
#include <KDE/Plasma/Theme>
#include <KDE/Plasma/WindowEffects>
#include <QWeakPointer>
#include "ActivityManager.h"

class QGraphicsScene;
class QPropertyAnimation;

class ActivityChangedText : public QGraphicsItem
{
public:
    ActivityChangedText(ActivityManager* am);
    ~ActivityChangedText();
    
    enum { Type = UserType + 2 };
    
    inline void setWidth(float width) {
        m_width = width;
    };
    
    inline void setHeight(float height) {
        m_height = height;
    };
    
    virtual QRectF boundingRect() const;
    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*);
    inline virtual int type() const {
        return Type;
    };
    
private:
    ActivityManager* m_activityman;
    float m_height;
    float m_width;
};

class ActivityChangedOSD : public QGraphicsView
{
public:
    ActivityChangedOSD(ActivityManager* am);
    ~ActivityChangedOSD();

    void reconfigure();
    void activityChanged(const QString& id);

    inline Plasma::FrameSvg* itemFrame() {
        return &m_item_frame;
    };
    
    inline int& getDelayTime() {
        return m_delayTime;
    };

protected:
    virtual void hideEvent(QHideEvent*);
    virtual void drawBackground(QPainter* painter, const QRectF& rect);

private:
    void resize();
    ActivityManager* m_activityman;
    Plasma::FrameSvg m_frame;
    Plasma::FrameSvg m_item_frame;
    QGraphicsScene* m_scene;
    bool m_active;
    QTimer m_delayedHideTimer;
    bool m_show;
    int m_delayTime;
    bool m_smallView;
};

// class ActivityChangedItem : public QObject, public QGraphicsItem
// {
//     Q_OBJECT
//     
//     Q_INTERFACES(QGraphicsItem)
// public:
//     ActivityChangedItem();
//     ~ActivityChangedItem();
//     
//     virtual QRectF boundingRect() const;
//     virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget);
// };

#endif // ACTIVITYCHANGEOSD_H
