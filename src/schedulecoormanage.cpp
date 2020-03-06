/*
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
#include "schedulecoormanage.h"
#include <QTime>
CScheduleCoorManage::CScheduleCoorManage()
{

}

CScheduleCoorManage::~CScheduleCoorManage()
{

}

void CScheduleCoorManage::setRange( int w, int h, QDate begindate, QDate enddate, int rightmagin)
{
    m_width = w;
    m_height = h;
    m_rightmagin = rightmagin;
    m_begindate = begindate;
    m_enddate = enddate;
    m_totalDay = begindate.daysTo(enddate) + 1;
}

void CScheduleCoorManage::setSenceRange( int w, int h )
{
    m_width = w;
    m_height = h;
}

void CScheduleCoorManage::setDateRange( QDate begindate, QDate enddate )
{
    m_begindate = begindate;
    m_enddate = enddate;
    m_totalDay = begindate.daysTo(enddate) + 1;
}

QRect CScheduleCoorManage::getDrawRegion( QDateTime begintime, QDateTime endtime )
{
    QRect rect;
    QString bb = begintime.toString("yyyyMMddhhmmsszzz");
    QString ee = endtime.toString("yyyyMMddhhmmsszzz");

    if (begintime > endtime) return rect;


    QDate begindate = begintime.date();
    QDate enddate = endtime.date();
    QTime beginzero(0, 0, 0);
    QTime beginScheduleT = begintime.time();
    QTime endScheduleT = endtime.time();
    if (begindate < m_begindate || enddate > m_enddate) return rect;
    int beginday = m_begindate.daysTo(begindate) + 1;

    int day = begindate.daysTo(enddate) + 1;
    int ScheduleBT = beginzero.secsTo(beginScheduleT);
    int ScheduleET = beginzero.secsTo(endScheduleT);

    int rWidth = m_width * (1.0 * day / m_totalDay);
    int rHeight = m_height * ((ScheduleET - ScheduleBT) / 86400.0);
    int posX = m_width * (1.0 * (beginday - 1) / m_totalDay);
    int posY = m_height * (ScheduleBT / 86400.0);
    rect = QRect(posX, posY, rWidth, rHeight);
    return rect;
}

QRect CScheduleCoorManage::getDrawRegion(QDateTime begintime, QDateTime endtime, int index, int coount)
{
    QRect rect;
    QString bb = begintime.toString("yyyyMMddhhmmsszzz");
    QString ee = endtime.toString("yyyyMMddhhmmsszzz");

    if (begintime > endtime) return rect;


    QDate begindate = begintime.date();
    QDate enddate = endtime.date();
    QTime beginzero(0, 0, 0);
    QTime beginScheduleT = begintime.time();
    QTime endScheduleT = endtime.time();
    if (begindate < m_begindate || enddate > m_enddate) return rect;
    int beginday = m_begindate.daysTo(begindate) + 1;

    int day = begindate.daysTo(enddate) + 1;
    int ScheduleBT = beginzero.secsTo(beginScheduleT);
    int ScheduleET = beginzero.secsTo(endScheduleT);

    int rWidth = m_width * (1.0 * day / m_totalDay) / coount;
    int rHeight = m_height * ((ScheduleET - ScheduleBT) / 86400.0);
    int posX = m_width * (1.0 * (beginday - 1) / m_totalDay) + (index - 1) * rWidth;
    int posY = m_height * (ScheduleBT / 86400.0);
    rect = QRect(posX, posY, rWidth, rHeight);
    return rect;
}

QRect CScheduleCoorManage::getDrawRegion(QDate date, QDateTime begintime, QDateTime endtime, int index, int coount, int maxnum, int type)
{
    QRect rect;
    QString bb = begintime.toString("yyyyMMddhhmmsszzz");
    QString ee = endtime.toString("yyyyMMddhhmmsszzz");

    if (begintime > endtime) return rect;


    QDate begindate = begintime.date();
    QDate enddate = endtime.date();


    QTime beginzero(0, 0, 0);
    QTime beginScheduleT = begintime.time();
    QTime endScheduleT = endtime.time();

    if (begindate < date) {
        begindate = date;
        beginScheduleT = beginzero;
    }
    if (enddate > date) {
        enddate = date;
        endScheduleT = QTime(23, 59, 59);
    }
    //if (begindate < m_begindate || enddate > m_enddate) return rect;
    int beginday = m_begindate.daysTo(begindate) + 1;

    int day = begindate.daysTo(enddate) + 1;
    int ScheduleBT = beginzero.secsTo(beginScheduleT);
    int ScheduleET = beginzero.secsTo(endScheduleT);

    int rWidth = m_width * (1.0 * day / m_totalDay) / coount;
    int rHeight = m_height * ((ScheduleET - ScheduleBT) / 86400.0);
    int posX = m_width * (1.0 * (beginday - 1) / m_totalDay) + (index - 1) * rWidth;
    int posY = m_height * (ScheduleBT / 86400.0);
    if (coount > maxnum && type == 0) {

        float sscale = 27.0 / (m_width * (1.0 * day / m_totalDay));
        if (index < maxnum + 1) {
            rWidth = m_width * (1.0 * day / m_totalDay) * sscale + 0.5;
            posX = m_width * (1.0 * (beginday - 1) / m_totalDay) + (index - 1) * rWidth;
        } else {
            int trWidth = m_width * (1.0 * day / m_totalDay) * sscale + 0.5;
            rWidth = m_width * (1.0 * day / m_totalDay) - (index - 1) * trWidth;
            posX = m_width * (1.0 * (beginday - 1) / m_totalDay) + (index - 1) * trWidth;
        }
    }
    if (rHeight < 20) {
        if (posY + 20 > m_height)
            posY = m_height - 20;
        rHeight = 20;
    }
    if (posX < 1) {
        posX = 1;
        rWidth = rWidth - posX;
    }
    rect = QRect(posX, posY, rWidth, rHeight);
    return rect;
}

QRectF CScheduleCoorManage::getDrawRegionF( QDateTime begintime, QDateTime endtime )
{
    QRectF rectf;
    if (begintime > endtime) return rectf;

    QDate begindate = begintime.date();
    QDate enddate = endtime.date();
    QTime beginzero(0, 0, 0);
    QTime beginScheduleT = begintime.time();
    QTime endScheduleT = endtime.time();
    if (begindate < m_begindate || enddate > m_enddate) return rectf;
    int beginday = m_begindate.daysTo(begindate) + 1;

    int day = begindate.daysTo(enddate) + 1;
    int ScheduleBT = beginzero.secsTo(beginScheduleT);
    int ScheduleET = beginzero.secsTo(endScheduleT);

    float rWidth = m_width * (1.0 * day / m_totalDay);
    float rHeight = m_height * ((ScheduleET - ScheduleBT) / 86400.0);
    float posX = m_width * (1.0 * (beginday - 1) / m_totalDay);
    float posY = m_height * (ScheduleBT / 86400.0);
    rectf = QRectF(posX, posY, rWidth, rHeight);
    return rectf;
}

QRect CScheduleCoorManage::getAllDayDrawRegion(QDate begin, QDate end)
{
    QRect rect;
    if (begin > end) return rect;

    QDate begindate = begin;
    QDate enddate = end;

    if (begindate < m_begindate) begindate = m_begindate;
    if (enddate > m_enddate)   enddate = m_enddate;
    //if (begindate < m_begindate || enddate > m_enddate) return rect;
    int beginday = m_begindate.daysTo(begindate) + 1;

    int day = begindate.daysTo(enddate) + 1;


    float rWidth = m_width * (1.0 * day / m_totalDay) - 12;
    float rHeight = m_height;
    float posX = m_width * (1.0 * (beginday - 1) / m_totalDay);
    float posY = 0;
    rect = QRect(posX, posY, rWidth - m_rightmagin, rHeight);
    return rect;
}

QDateTime CScheduleCoorManage::getDate(QPointF pos)
{
    QDateTime begintime;
    int day = (1.0 * pos.x() / m_width) * m_totalDay;
    int time = (1.0 * pos.y() / m_height) * 86400.0;
    int hours = time / 3600;
    int minutes = (time - 3600 * hours) / 60;
    int secss = time - 3600 * hours - 60 * minutes;
    QDate date = m_begindate.addDays(day);
    begintime.setDate(date);
    begintime.setTime(QTime(hours, minutes, secss));
    return begintime;
}

QDate CScheduleCoorManage::getsDate(QPointF pos)
{
    int day = (1.0 * pos.x() / m_width) * m_totalDay;
    QDate date = m_begindate.addDays(day);
    return date;
}

float CScheduleCoorManage::getHeight(QTime time)
{
    QTime beginzero(0, 0, 0);
    int ScheduleBT = beginzero.secsTo(time);
    float posY = m_height * (ScheduleBT / 86400.0);
    return posY;
}
