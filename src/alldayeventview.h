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
#ifndef ALLDAYEVENTVIEW_H
#define ALLDAYEVENTVIEW_H

#include <DComboBox>
#include <DLabel>
#include <QMouseEvent>
#include <DPushButton>
#include "schedulestructs.h"
#include <DGraphicsView>
#include <QGraphicsScene>
#include <DFontSizeManager>

#include <DMenu>


#include "SchecduleRemindWidget.h"
#include "draginfoitem.h"
#include "draginfographicsview.h"
DWIDGET_USE_NAMESPACE
class QDrag;
class CAllDayEventWidgetItem;
class CScheduleCoorManage;
class CAllDayEventWeekView : public DragInfoGraphicsView
{
    Q_OBJECT
public:
    CAllDayEventWeekView(QWidget *parent = nullptr, int edittype = 0);
    ~CAllDayEventWeekView() Q_DECL_OVERRIDE;
    void setDayData(const QVector<QVector<ScheduleDtailInfo> > &vlistData);
    void setInfo(const QVector<ScheduleDtailInfo> &info);
    QVector<QVector<ScheduleDtailInfo> > &getListData()
    {
        return  m_vlistData;
    }
    void setRange(int w, int h, QDate begindate, QDate enddate, int rightmagin);
    void setRange(QDate begin, QDate end);
    CScheduleCoorManage *getCoorManage()
    {
        return m_coorManage;
    }
    void updateHigh();
    void setSelectSchedule(const ScheduleDtailInfo &info);
    void setMargins(int left, int top, int right, int bottom);
    void updateInfo();
signals:
    void signalUpdatePaint(const int topM);
    void signalSceneUpdate();
public slots:
    void slotDeleteItem();
    void slotUpdateScene();
private slots:
    void slotDoubleEvent();
public:
    void setTheMe(int type = 0) override;
private:

    bool MeetCreationConditions(const QDateTime &date) override;
    //判断时间是否相等
    bool IsEqualtime(const QDateTime &timeFirst,const QDateTime &timeSecond) override;
    //根据鼠标移动的距离判断是否创建日程
    bool JudgeIsCreate(const QPointF &pos)  override;
    void RightClickToCreate(QGraphicsItem *listItem,const QPoint &pos) override;
    void MoveInfoProcess(ScheduleDtailInfo &info,const QPointF &pos) override;
    QDateTime getDragScheduleInfoBeginTime(const QDateTime &moveDateTime) override;
    QDateTime getDragScheduleInfoEndTime(const QDateTime &moveDateTime) override;
    PosInItem getPosInItem(const QPoint &p,const QRectF &itemRect)override;
    QDateTime getPosDate(const QPoint &p)override;
    void upDateInfoShow(const DragStatus &status = NONE,const ScheduleDtailInfo &info =ScheduleDtailInfo())override;
protected:
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE;
    void paintEvent( QPaintEvent *event ) Q_DECL_OVERRIDE;
private:
    void updateDateShow();
    void createItemWidget(int index, bool average = false);
private:
    int                     itemHeight = 22;

    QVector<QVector<ScheduleDtailInfo> >         m_vlistData;
    QVector<ScheduleDtailInfo>                   m_scheduleInfo;
    QVector<CAllDayEventWidgetItem *>            m_baseShowItem;
    int                                          m_editType = 0;
    CScheduleCoorManage                         *m_coorManage;
    QDate                                       m_dianjiDay;

    int m_rightmagin = 0;
    bool                            m_updateDflag  = false;

    QDate                           m_beginDate;
    QDate                           m_endDate;


};

class CAllDayEventWidgetItem : public DragInfoItem
{
    Q_OBJECT
public:
    explicit CAllDayEventWidgetItem(QRect rect, QGraphicsItem *parent = nullptr, int edittype = 0);
    bool hasSelectSchedule(const ScheduleDtailInfo &info);
protected:
    void paintBackground(QPainter *painter,const QRectF &rect,const int isPixMap = false) override;
};
#endif // CSHCEDULEDAYVIEW_H

