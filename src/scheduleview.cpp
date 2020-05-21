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
#include "scheduleview.h"
#include <DPalette>
#include <QGridLayout>
#include <QShortcut>
#include <DHorizontalLine>
#include <QVBoxLayout>
#include <QApplication>
#include "alldayeventview.h"
#include "graphicsview.h"
#include "schceduledlg.h"
#include "schedulecoormanage.h"
#include "scheduledatamanage.h"
DGUI_USE_NAMESPACE

static int hourTextWidth = 50;
static int hourTextHeight = 20;
CScheduleView::CScheduleView(QWidget *parent, int viewType)
    : DFrame(parent)
    , m_viewType(viewType)
{
    setAutoFillBackground(true);
    initUI();
    initConnection();
    setFrameRounded(true);
    // setFrameShape(QFrame::NoFrame);
    setLineWidth(0);
}

CScheduleView::~CScheduleView() {}

void CScheduleView::setviewMagin(int left, int top, int right, int bttom)
{
    m_leftMagin = left;
    m_topMagin = top;
    m_rightmagin = right;
    m_graphicsView->setMargins(left, 0, right, bttom);
    m_alldaylist->setMargins(left, 0, 0, 0);
}

void CScheduleView::setRange(int w, int h, QDate begin, QDate end)
{
    if (!(w > 0)) {
        return;
    }
    m_TotalDay = begin.daysTo(end) + 1;
    m_graphicsView->setRange(w, scheduleViewHegith(), begin, end, m_rightmagin);
    m_alldaylist->setRange(w, 22, m_beginDate, m_endDate, m_rightmagin);
    update();
}

void CScheduleView::setRange(QDate begin, QDate end)
{
    m_TotalDay = begin.daysTo(end) + 1;
    m_graphicsView->setRange(begin,end);
    m_alldaylist->setRange(begin,end);
    m_beginDate = begin;
    m_endDate = end;
    updateSchedule();
}

void CScheduleView::setFirstWeekday(int weekday)
{
    m_firstWeekDay = weekday;
    m_graphicsView->setFirstWeekday(weekday);
}

void CScheduleView::setTheMe(int type)
{
    if (type == 0 || type == 1) {
        DPalette palette(this->palette());
        palette.setColor(DPalette::Background, "#FFFFFF");
        this->setPalette(palette);
        setBackgroundRole(DPalette::Background);
        m_linecolor = "#000000";
        m_linecolor.setAlphaF(0.1);
        m_weekColor = "#00429A";
        m_weekColor.setAlphaF(0.05);
        m_ALLDayColor = "#303030";
        m_timeColor = "#7D7D7D";

    } else if (type == 2) {
        DPalette palette(this->palette());
        QColor tbcolor = "#282828";
        // tbcolor.setAlphaF(0.05);
        palette.setColor(DPalette::Background, tbcolor);
        this->setPalette(palette);
        setBackgroundRole(DPalette::Background);
        m_linecolor = "#000000";
        m_linecolor.setAlphaF(0.1);
        m_weekColor = "#4F9BFF";
        m_weekColor.setAlphaF(0.1);
        m_ALLDayColor = "#7D7D7D";
        m_timeColor = "#7D7D7D";
    }
    m_graphicsView->setTheMe(type);
    m_alldaylist->setTheMe(type);
    update();
}

void CScheduleView::setLunarVisible(bool state)
{
    m_alldaylist->setLunarVisible(state);
}

void CScheduleView::setTime(QTime time)
{
    m_graphicsView->setTime(time);
}

void CScheduleView::setSelectSchedule(const ScheduleDtailInfo &scheduleInfo)
{
    if (scheduleInfo.allday) {
        m_alldaylist->setSelectSchedule(scheduleInfo);
    } else {
        m_graphicsView->setSelectSchedule(scheduleInfo);
    }
}

void CScheduleView::updateHigh()
{
    m_graphicsView->updateHigh();
    m_alldaylist->updateHigh();
}

void CScheduleView::slotsupdatescheduleD(QVector<ScheduleDateRangeInfo> &data)
{
    updateAllday();
    m_graphicsView->upDateInfoShow();

    m_graphicsView->update();
    m_graphicsView->getSence()->update();
    if (m_viewType == 1) {
        if (QDate::currentDate() == m_beginDate) {
            m_graphicsView->setTime(QTime::currentTime());
        } else {
            if (data.isEmpty()) {
                m_graphicsView->setTime(QTime(13, 0));
            } else {
                QVector<ScheduleDtailInfo> scheduleInfolist;

                for (int i = 0; i < data.at(0).vData.count(); i++) {
                    if (data.at(0).vData.at(i).allday)
                        continue;
                    scheduleInfolist.append(data.at(0).vData.at(i));
                }
                if (scheduleInfolist.isEmpty()) {
                    m_graphicsView->setTime(QTime(13, 0));
                } else {
                    qSort(scheduleInfolist.begin(),scheduleInfolist.end(),
                    [](const ScheduleDtailInfo &s1, const ScheduleDtailInfo &s2) ->bool {
                        return s1.beginDateTime < s2.beginDateTime;
                    });
                    QTime time = scheduleInfolist.at(0).beginDateTime.time();
                    if (scheduleInfolist.at(0).beginDateTime.date() != m_beginDate) {
                        time = QTime(0, 0);
                    }
                    if (time.hour() + 4 >= 24) {
                        time = QTime(20, 0);
                    } else {
                        time = time.addSecs(14400);
                    }
                    m_graphicsView->setTime(time);
                }
            }
        }
    }
    setEnabled(true);
}

void CScheduleView::slotCreateSchedule()
{
    CSchceduleDlg dlg(1, this);
    QDateTime tDatatime;
    tDatatime.setDate(m_currteDate);
    tDatatime.setTime(QTime::currentTime());
    dlg.setDate(tDatatime);
    if (dlg.exec() == DDialog::Accepted) {
        slotupdateSchedule();
    }
}

void CScheduleView::setDate(QDate date)
{
    m_currteDate = date;
    updateAllday();
}

void CScheduleView::slotupdateSchedule(int id)
{
    updateSchedule();
    if (id !=1) {
        emit signalsUpdateShcedule(id);
    }
}

void CScheduleView::slotPosHours(QVector<int> vPos, QVector<int> vHours, int cuttrnttimetype)
{
    m_vHours = vHours;
    m_vPos = vPos;
    m_cuttrnttimetype = cuttrnttimetype;
    update();
}

void CScheduleView::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QFont font;
//    font.setFamily("SourceHanSansSC");
    font.setWeight(QFont::Normal);
    font.setPixelSize(11);
    if (m_vPos.isEmpty())
        return;
    QLocale locale;
    if (locale.language() == QLocale::Chinese) {
        QRect tinrect((m_leftMagin - hourTextWidth) / 2 - 5,
                      m_topMagin - 8 + m_vPos[m_vPos.count() - 1], hourTextWidth, hourTextHeight);
        if (m_cuttrnttimetype == 0) {
            painter.save();
            painter.setFont(font);
            painter.setPen(m_timeColor);
            for (int i = 0; i < m_vPos.size(); i++) {
                if (m_vHours[i] == 0)
                    continue;
                if (m_vHours[i] == 24)
                    continue;
                if (m_topMagin - 8 + m_vPos[i] < m_topMagin)
                    continue;
//                if (m_vHours[i] > 12) {
//                    painter.drawText(
//                        QRect((m_leftMagin - hourTextWidth) / 2 - 5, m_topMagin - 8 + m_vPos[i],
//                              hourTextWidth, hourTextHeight),
//                        Qt::AlignRight, ("下午 ") + QString::number(m_vHours[i] - 12) + (" 时"));
//                } else {
//                    painter.drawText(
//                        QRect((m_leftMagin - hourTextWidth) / 2 - 5, m_topMagin - 8 + m_vPos[i],
//                              hourTextWidth, hourTextHeight),
//                        Qt::AlignRight, ("上午 ") + QString::number(m_vHours[i]) + (" 时"));
//                }
                painter.drawText(
                    QRect((m_leftMagin - hourTextWidth) / 2 - 5, m_topMagin - 8 + m_vPos[i],
                          hourTextWidth, hourTextHeight),
                    /*Qt::AlignRight | */Qt::AlignCenter, QTime(m_vHours[i], 0).toString("AP h 时"));
            }
            painter.restore();
        } else {
            painter.save();
            painter.setFont(font);
            painter.setPen(m_timeColor);
            for (int i = 0; i < m_vPos.size() - 1; i++) {
                if (m_vHours[i] == 0)
                    continue;
                if (m_vHours[i] == 24)
                    continue;
                if (m_topMagin - 8 + m_vPos[i] < m_topMagin)
                    continue;
                QRect rr((m_leftMagin - hourTextWidth) / 2 - 5, m_topMagin - 8 + m_vPos[i],
                         hourTextWidth, hourTextHeight);
                if (rr.intersects(tinrect) && m_viewType == 1 &&
                        m_beginDate == QDate::currentDate()) {
                    continue;
                }
//                if (m_vHours[i] > 12) {
//                    painter.drawText(
//                        QRect((m_leftMagin - hourTextWidth) / 2 - 5, m_topMagin - 8 + m_vPos[i],
//                              hourTextWidth, hourTextHeight),
//                        Qt::AlignRight, ("下午 ") + QString::number(m_vHours[i] - 12) + (" 时"));
//                } else {
//                    painter.drawText(
//                        QRect((m_leftMagin - hourTextWidth) / 2 - 5, m_topMagin - 8 + m_vPos[i],
//                              hourTextWidth, hourTextHeight),
//                        Qt::AlignRight, ("上午 ") + QString::number(m_vHours[i]) + (" 时"));
//                }
                painter.drawText(
                    QRect((m_leftMagin - hourTextWidth) / 2 - 5, m_topMagin - 8 + m_vPos[i],
                          hourTextWidth, hourTextHeight),
                    /*Qt::AlignRight | */Qt::AlignCenter, QTime(m_vHours[i], 0).toString("AP h 时"));
            }
            painter.restore();
            if (m_viewType == 1 && m_beginDate == QDate::currentDate()) {
                painter.save();
                painter.setFont(font);
                painter.setPen(m_currenttimecolor);
                //                QString str = QTime::currentTime().toString("AP HH:mm");
                QString str = QTime::currentTime().toString("AP hh:mm");
                painter.drawText(QRect((m_leftMagin - hourTextWidth) / 2 - 5,
                                       m_topMagin - 8 + m_vPos[m_vPos.count() - 1], hourTextWidth,
                                       hourTextHeight),
                                 /*Qt::AlignRight | */Qt::AlignCenter, str);
                painter.restore();
            }
        }
    } else {
        if (m_cuttrnttimetype == 0) {
            painter.save();
            painter.setFont(font);
            painter.setPen(m_timeColor);
            for (int i = 0; i < m_vPos.size(); i++) {
                if (m_vHours[i] == 0)
                    continue;
                if (m_vHours[i] == 24)
                    continue;
                if (m_topMagin - 8 + m_vPos[i] < m_topMagin)
                    continue;
//                if (m_vHours[i] > 12) {
//                    painter.drawText(
//                        QRect((m_leftMagin - hourTextWidth) / 2 - 5, m_topMagin - 8 + m_vPos[i],
//                              hourTextWidth, hourTextHeight),
//                        Qt::AlignRight, ("PM ") + QTime(m_vHours[i] - 12, 0).toString("hh:mm"));
//                } else {
//                    painter.drawText(
//                        QRect((m_leftMagin - hourTextWidth) / 2 - 5, m_topMagin - 8 + m_vPos[i],
//                              hourTextWidth, hourTextHeight),
//                        Qt::AlignRight, ("AM ") + QTime(m_vHours[i], 0).toString("hh:mm"));
//                }
                painter.drawText(
                    QRect((m_leftMagin - hourTextWidth) / 2 - 5, m_topMagin - 8 + m_vPos[i],
                          hourTextWidth + 2, hourTextHeight),
                    /*Qt::AlignRight | */Qt::AlignCenter, QTime(m_vHours[i], 0).toString(/*"AP */"HH:mm"));
            }
            painter.restore();
        } else {
            painter.save();
            painter.setFont(font);
            painter.setPen(m_timeColor);
            QRect tinrect((m_leftMagin - hourTextWidth) / 2 - 5,
                          m_topMagin - 8 + m_vPos[m_vPos.count() - 1], hourTextWidth + 2,
                          hourTextHeight);

            for (int i = 0; i < m_vPos.size() - 1; i++) {
                if (m_vHours[i] == 0)
                    continue;
                if (m_vHours[i] == 24)
                    continue;
                if (m_topMagin - 8 + m_vPos[i] < m_topMagin)
                    continue;
                QRect rr((m_leftMagin - hourTextWidth) / 2 - 5, m_topMagin - 8 + m_vPos[i],
                         hourTextWidth + 2, hourTextHeight);
                if (rr.intersects(tinrect) && m_viewType == 1 &&
                        m_beginDate == QDate::currentDate())
                    continue;
//                if (m_vHours[i] > 12) {
//                    painter.drawText(
//                        QRect((m_leftMagin - hourTextWidth) / 2 - 5, m_topMagin - 8 + m_vPos[i],
//                              hourTextWidth, hourTextHeight),
//                        Qt::AlignRight, ("PM ") + QTime(m_vHours[i] - 12, 0).toString("hh:mm"));
//                } else {
//                    painter.drawText(
//                        QRect((m_leftMagin - hourTextWidth) / 2 - 5, m_topMagin - 8 + m_vPos[i],
//                              hourTextWidth, hourTextHeight),
//                        Qt::AlignRight, ("AM ") + QTime(m_vHours[i], 0).toString("hh:mm"));
//                }
                painter.drawText(
                    QRect((m_leftMagin - hourTextWidth) / 2 - 5, m_topMagin - 8 + m_vPos[i],
                          hourTextWidth + 2, hourTextHeight),
                    /*Qt::AlignRight | */Qt::AlignCenter, QTime(m_vHours[i], 0).toString(/*"AP */"HH:mm"));
            }
            painter.restore();
            if (m_viewType == 1 && m_beginDate == QDate::currentDate()) {
                painter.save();
                painter.setFont(font);
                painter.setPen(m_currenttimecolor);
                //                QString str = QTime::currentTime().toString("AP HH:mm");
                QString str = QTime::currentTime().toString(/*"AP */"hh:mm");
                if (m_topMagin - 8 + m_vPos[m_vPos.count() - 1] >= m_topMagin)
                    painter.drawText(QRect((m_leftMagin - hourTextWidth) / 2 - 5,
                                           m_topMagin - 8 + m_vPos[m_vPos.count() - 1],
                                           hourTextWidth + 2, hourTextHeight),
                                     /*Qt::AlignRight | */Qt::AlignCenter, str);
                painter.restore();
            }
        }
    }

    painter.save();
    QFont alldayfont;
//    alldayfont.setFamily("SourceHanSansSC");
    alldayfont.setWeight(QFont::Medium);
    alldayfont.setPixelSize(14);
    painter.setFont(alldayfont);
    painter.setPen(m_ALLDayColor);
    painter.drawText(QRect(0, 0, m_leftMagin - 2, m_topMagin - 2), Qt::AlignCenter, tr("ALL DAY"));
    painter.restore();

    // painter.setCompositionMode(QPainter::CompositionMode_Difference  ); //设置混合模式
    int t_width = width();
//    int t_height = height();
//    painter.save();
//    painter.setPen(Qt::SolidLine);
//    painter.setPen(m_linecolor);
//    painter.drawLine(QPoint(0, m_topMagin), QPoint(t_width, m_topMagin));
//    painter.restore();
    float intenval = 1.0 * (t_width - m_leftMagin) / m_TotalDay;
    if (m_TotalDay > 1) {
        painter.save();
        painter.setPen(Qt::SolidLine);
        painter.setPen(m_linecolor);

        for (float i = intenval; i < width() - m_leftMagin; i = i + intenval) {
            painter.drawLine(QPoint(i + m_leftMagin + 1, 1),
                             QPoint(i + m_leftMagin + 1, m_topMagin + 1));
        }

        // for (int i = 1; i < m_TotalDay; i++) {
        //   painter.drawLine(QPoint(m_leftMagin + i * intenval + 1, 1), QPoint(m_leftMagin  + i *
        //   intenval + 1, m_topMagin + 1));
        //}
        painter.restore();
        painter.save();
        for (int i = 0; i != 7; ++i) {
            int d = checkDay(i - m_firstWeekDay);
            painter.setBrush(m_weekColor);
            painter.setPen(Qt::NoPen);
            if (d == 6) {
                painter.drawRect(QRect(m_leftMagin + i * intenval + 1, 0,
                                       width() - m_leftMagin - i * intenval, this->height()));
            }
            if (d == 7) {
                painter.drawRect(
                    QRect(m_leftMagin + i * intenval + 2, 0, intenval, this->height()));
            }
        }
        painter.restore();
    }
    painter.end();
}

void CScheduleView::resizeEvent(QResizeEvent *event)
{
//    int tt = 0.0968 * height() + 0.5;
    if (m_viewType == 0) {
        m_sMaxNum = ((width() - m_leftMagin) / 7) / 27;
//        m_graphicsView->setRange(width() - m_leftMagin, scheduleViewHegith(), m_beginDate,
//                                 m_endDate, 0);
//        //   m_alldaylist->setFixedSize(width() - m_leftMagin, m_topMagin - 5);
//        m_alldaylist->setRange(width() - m_leftMagin, 22, m_beginDate, m_endDate, 0);
    } else {
//        m_graphicsView->setRange(width() - m_leftMagin,
//                                 scheduleViewHegith(), m_beginDate, m_endDate, m_rightmagin);
//        //  m_alldaylist->setFixedSize(width() - m_leftMagin, m_topMagin - 10);
//        m_alldaylist->setRange(width() - m_leftMagin, 22, m_beginDate, m_endDate, m_rightmagin);
    }
    m_graphicsView->setMaxNum(m_sMaxNum);
    m_graphicsView->setRange(width() - m_leftMagin,
                             scheduleViewHegith(), m_beginDate, m_endDate, m_rightmagin);
    m_alldaylist->setRange(width() - m_leftMagin, 22, m_beginDate, m_endDate, m_rightmagin);
    update();
    QFrame::resizeEvent(event);
    updateSchedule();
}

void CScheduleView::initUI()
{
    DHorizontalLine *m_hline = new DHorizontalLine;
    m_layout = new QVBoxLayout;
    m_layout->setSpacing(0);
    m_layout->setMargin(0);
    m_alldaylist = new CAllDayEventWeekView(this, 1);
//    m_alldaylist->move(72, 5);
    m_layout->addWidget(m_alldaylist);
    m_layout->addWidget(m_hline);
    m_graphicsView = new CGraphicsView(this, m_viewType);
//    m_layout->setContentsMargins(0, m_space, 0, 0);
    connect(m_graphicsView, SIGNAL(signalsPosHours(QVector<int>, QVector<int>, int)), this,
            SLOT(slotPosHours(QVector<int>, QVector<int>, int)));
    m_layout->addWidget(m_graphicsView);
    setLayout(m_layout);
    m_graphicsView->scrollBarValueChangedSlot();

    m_ScheduleRemindWidget = new SchecduleRemindWidget(this);
}

void CScheduleView::initConnection()
{
    connect(m_graphicsView, &CGraphicsView::signalsUpdateShcedule, this,
            &CScheduleView::slotupdateSchedule);
    // connect(m_graphicsView, &CGraphicsView::signalsUpdateShcedule, this,
    // &CScheduleView::signalsUpdateShcedule);
    connect(m_alldaylist, &CAllDayEventWeekView::signalsUpdateShcedule, this,
            &CScheduleView::slotupdateSchedule);
    connect(m_graphicsView, &CGraphicsView::signalsCurrentScheduleDate, this,
            &CScheduleView::slotCurrentScheduleDate);

    connect(m_alldaylist, &CAllDayEventWeekView::signalViewtransparentFrame, this,
            &CScheduleView::signalViewtransparentFrame);
    connect(m_graphicsView, &CGraphicsView::signalViewtransparentFrame, this,
            &CScheduleView::signalViewtransparentFrame);

    connect(m_graphicsView
            , &CGraphicsView::signalScheduleShow
            , this, &CScheduleView::slotScheduleShow);

    connect(m_alldaylist, &CAllDayEventWeekView::signalScheduleShow
            , this, &CScheduleView::slotScheduleShow);

    connect(m_alldaylist,&CAllDayEventWeekView::signalUpdatePaint,
            this,&CScheduleView::slotUpdatePaint);
    connect(m_alldaylist,&CAllDayEventWeekView::signalScene,
            this,&CScheduleView::slotUpdateScene);
    connect(m_graphicsView,&CGraphicsView::signalScene,
            this,&CScheduleView::slotUpdateScene);


    QShortcut *shortcut = new QShortcut(this);
    shortcut->setKey(QKeySequence(QLatin1String("Ctrl+N")));
    connect(shortcut, SIGNAL(activated()), this, SLOT(slotCreateSchedule()));

    QShortcut *dshortcut = new QShortcut(this);
    dshortcut->setKey(QKeySequence(QLatin1String("Delete")));
    connect(dshortcut, SIGNAL(activated()), this, SLOT(slotDeleteitem()));



}
void CScheduleView::slotCtrlSchceduleUpdate(QDate date, int type)
{
    updateSchedule();
}


void CScheduleView::slotDeleteitem()
{
    m_graphicsView->slotDeleteItem();
    m_alldaylist->slotDeleteItem();
}

void CScheduleView::slotCurrentScheduleDate(QDate date)
{
    if (m_viewType == 1)
        return;
    emit signalsCurrentScheduleDate(date);
}

void CScheduleView::slotScheduleShow(const bool isShow, const ScheduleDtailInfo &out)
{
    if (isShow) {
        QPoint pos22 = QCursor::pos();
        CSchedulesColor gdcolor = CScheduleDataManage::getScheduleDataManage()->getScheduleColorByType(
                                      out.type.ID);
        QDesktopWidget *w = QApplication::desktop();
        m_ScheduleRemindWidget->setData(out, gdcolor);
        if ((pos22.x() + m_ScheduleRemindWidget->width() + 15) > w->width()) {
            m_ScheduleRemindWidget->setArrowDirection(DArrowRectangle::ArrowRight);
            m_ScheduleRemindWidget->show(pos22.x() - 15, pos22.y());
        } else {
            m_ScheduleRemindWidget->setArrowDirection(DArrowRectangle::ArrowLeft);
            m_ScheduleRemindWidget->show(pos22.x() + 15, pos22.y());
        }
    } else {
        m_ScheduleRemindWidget->hide();
    }
}

void CScheduleView::slotUpdatePaint(const int topM)
{
    m_topMagin = topM;
    update();
}

void CScheduleView::slotUpdateScene()
{
    m_graphicsView->slotUpdateScene();
    m_alldaylist->slotUpdateScene();
}

void CScheduleView::updateSchedule()
{
    m_graphicsView->clearSchdule();
    CScheduleDataCtrl *scheduleDataCtrl =
        CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl();
    QDateTime bdate = QDateTime(m_beginDate);
    QDateTime edate = QDateTime(m_endDate);
    edate.setTime(QTime(23, 59, 59));
    QVector<ScheduleDateRangeInfo> data;
    scheduleDataCtrl->queryScheduleInfo("", bdate, edate, data);
    QVector<ScheduleDtailInfo> allInfo;
    QVector<ScheduleDtailInfo> nonAllInfo;
    for (int i = 0; i < data.size(); ++i) {
        for (int j = 0; j< data.at(i).vData.size(); ++j) {
            if (data.at(i).vData.at(j).allday) {
                if (!allInfo.contains(data.at(i).vData.at(j))) {
                    allInfo.append(data.at(i).vData.at(j));
                }
            } else {
                if (!nonAllInfo.contains(data.at(i).vData.at(j))) {
                    nonAllInfo.append(data.at(i).vData.at(j));
                }
            }
        }
    }
    m_alldaylist->setInfo(allInfo);
    m_graphicsView->setInfo(nonAllInfo);
    slotsupdatescheduleD(data);
}

void CScheduleView::updateAllday()
{
    m_topMagin = m_alldaylist->upDateInfoShow();
    update();
    m_graphicsView->resize(m_graphicsView->width(),this->height()-m_alldaylist->height());
}

int CScheduleView::checkDay(int weekday)
{
    if (weekday <= 0)
        return weekday += 7;

    if (weekday > 7)
        return weekday -= 7;

    return weekday;
}

int CScheduleView::scheduleViewHegith()
{
//    rHeight = m_height * ((ScheduleET - ScheduleBT) / 86400.0)

    int mheight = 0;
    if (m_viewType == 0) {
        mheight = 24 * (0.0968 * height() + 0.5);
    } else {
        mheight = 24 * (0.083 * height() + 0.5);
    }
    //现在最小高度为20;
    int m_minTime = (20.0/mheight)*86400;
    m_graphicsView->setMinTime(m_minTime);
    return  mheight;
}
