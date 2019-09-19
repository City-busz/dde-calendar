﻿/*
 * Copyright (C) 2017 ~ 2018 Deepin Technology Co., Ltd.
 *
 * Author:     kirigaya <kirigaya@mkacg.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef SCHEDULEVIEW_H
#define SCHEDULEVIEW_H

#include <QFrame>
#include <QDate>
#include "schedulestructs.h"

typedef struct _tagScheduleclassificationInfo {
    QDateTime begindate;
    QDateTime enddate;
    QVector<ScheduleDtailInfo> vData;
} ScheduleclassificationInfo;

class CGraphicsView;
class CSchceduleAllDayView;
class CScheduleView : public QFrame
{
    Q_OBJECT

public:
    CScheduleView(QWidget *parent = 0, int viewType = 0);
    ~CScheduleView();
    void setviewMagin(int left, int top, int right, int bttom);
    void setRange(int w, int h, QDate begin, QDate end);
    void setRange(QDate begin, QDate end);
    void setFirstWeekday(int weekday);
    void scheduleClassificationType(QVector<ScheduleDtailInfo> &scheduleInfolist, QVector<ScheduleclassificationInfo> &info);
public slots:
    void slotsupdatescheduleD(QWidget *w, QVector<ScheduleDateRangeInfo> &data);
signals:
    void signalsupdatescheduleD(QWidget *w, QDate begin, QDate end);
signals:
    void signalsUpdateShcedule(int id = 0);
public slots:
    void slotPosHours(QVector<int> vPos, QVector<int> vHours);
    void setDate(QDate date);
    void setDate(QDate date,  QString solarDay);
    void slotupdateSchedule(int id = 0);
    void slotCtrlSchceduleUpdate(QDate date, int type = 0);
protected:
    void paintEvent( QPaintEvent *event );
    void resizeEvent(QResizeEvent *event);
private:
    void initUI();
    void initConnection();
    void updateSchedule(int id = 0);
    void updateAllday(int id = 0);
    int checkDay(int weekday);
private:
    CGraphicsView            *m_graphicsView;
    QVector<int>              m_vPos;
    QVector<int>              m_vHours;
    CSchceduleAllDayView     *m_alldaylist;
    int                       m_leftMagin;
    int                       m_topMagin;
    int                       m_TotalDay;
    QDate                     m_currteDate;
    int                       m_firstWeekDay;
    QDate                     m_beginDate;
    QDate                     m_endDate;
    int                       m_viewType = 0;
};

#endif // SCHEDULEVIEW_H