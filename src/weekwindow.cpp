/*
 * Copyright (C) 2015 ~ 2018 Deepin Technology Co., Ltd.
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
#include "weekwindow.h"
#include "scheduleview.h"
#include "constants.h"
#include "calendardbus.h"
#include "weekheadview.h"
#include "weekview.h"
#include <QMessageBox>
#include <QDate>
#include <QHBoxLayout>
#include <DPalette>
DGUI_USE_NAMESPACE
CWeekWindow::CWeekWindow(QWidget *parent): QMainWindow (parent)
{
    initUI();
    initConnection();
}

CWeekWindow::~CWeekWindow()
{

}

void CWeekWindow::setDate(QDate date)
{
    m_currentdate = date;
    m_weekview->setCurrentDate(date);
}

void CWeekWindow::setFirstWeekday(int weekday)
{
    m_weekview->setFirstWeekDay(weekday);
    m_weekHeadView->setFirstWeekday(weekday);
    m_scheduleView->setFirstWeekday(weekday);
}

void CWeekWindow::setLunarVisible(bool state)
{
    m_weekHeadView->setLunarVisible(state);
}

void CWeekWindow::initUI()
{
    m_contentBackground = new QFrame;
    m_contentBackground->setObjectName("CalendarBackground");
    DPalette pa = m_contentBackground->palette();
    pa.setColor(DPalette::Background, Qt::white);
    m_contentBackground->setAutoFillBackground(true);
    m_contentBackground->setPalette(pa);

    // m_contentBackground->setStyleSheet("QFrame#CalendarBackground { "
    //                                   "background:white;"
    //                                    "}");
//

    m_today = new DPushButton;
    m_today->setText(tr("Return today"));
    m_today->setFixedSize(DDEWeekCalendar::WTodayWindth, DDEWeekCalendar::WTodayHeight);
    DPalette todaypa = m_today->palette();
    todaypa.setColor(DPalette::ButtonText, QColor("#0098FF"));
    todaypa.setColor(DPalette::Dark, Qt::white);
    todaypa.setColor(DPalette::Light, Qt::white);
    m_today->setPalette(todaypa);
    m_prevButton = new DImageButton(this);
    //m_prevButton->setArrowDirection(DArrowButton::ArrowLeft);
    m_prevButton->setHoverPic(":/resources/icon/previous_hover.svg");
    m_prevButton->setNormalPic(":/resources/icon/previous_normal.svg");
    m_prevButton->setPressPic(":/resources/icon/previous_press.svg");
    m_prevButton->setFixedWidth(DDEWeekCalendar::WWeekCellWidth);
    //m_prevButton->setFixedSize(DDEWeekCalendar::WWeekCellWidth, DDEWeekCalendar::WWeekCellHeight);

    m_nextButton = new DImageButton(this);
    //m_nextButton->setArrowDirection(DArrowButton::ArrowRight);
    m_nextButton->setHoverPic(":/resources/icon/next_hover.svg");
    m_nextButton->setNormalPic(":/resources/icon/next_normal.svg");
    m_nextButton->setPressPic(":/resources/icon/next_press.svg");
    m_nextButton->setFixedWidth(DDEWeekCalendar::WWeekCellWidth);
    //m_nextButton->setFixedSize(DDEWeekCalendar::WWeekCellWidth, DDEWeekCalendar::WWeekCellHeight);

    m_YearLabel = new DLabel();
    m_YearLabel->setFixedHeight(DDEWeekCalendar::W_YLableHeight);
    //m_currentMouth->setStyleSheet("border: 1px solid rgba(0, 0, 0, 0.05);");

    QFont t_labelF;
    t_labelF.setFamily("SourceHanSansSC-Medium");
    t_labelF.setPixelSize(24);
    m_YearLabel->setFont(t_labelF);
    DPalette Lunadpa = m_YearLabel->palette();
    Lunadpa.setColor(DPalette::WindowText, QColor("#3B3B3B"));
    m_YearLabel->setPalette(Lunadpa);
    //m_YearLabel->setStyleSheet("color:#3B3B3B;");

    m_YearLunarLabel = new DLabel(m_contentBackground);
    m_YearLunarLabel->setFixedSize(DDEWeekCalendar::W_YLunatLabelWindth, DDEWeekCalendar::W_YLunatLabelHeight);

    m_weekview  = new CWeekView(this);
    // m_weekview->setFixedHeight(DDEWeekCalendar::W_YLunatLabelHeight + 8);

    m_weekLabel = new DLabel();
    m_weekLabel->setFixedHeight(DDEWeekCalendar::W_YLableHeight);
    QFont weeklabelF;
    weeklabelF.setFamily("PingFangSC-Medium");
    weeklabelF.setPixelSize(14);
    m_weekLabel->setFont(weeklabelF);
    DPalette wpa = m_weekLabel->palette();
    wpa.setColor(DPalette::WindowText, QColor("#717171"));
    m_weekLabel->setPalette(wpa);
    m_weekLabel->setText(tr("Week"));

    QFont ylabelF;
    ylabelF.setFamily("SourceHanSansSC-Medium");
    ylabelF.setPixelSize(14);
    m_YearLunarLabel->setFont(ylabelF);
    DPalette YearLpa = m_YearLunarLabel->palette();
    YearLpa.setColor(DPalette::WindowText, QColor("#8A8A8A"));

    m_YearLunarLabel->setPalette(YearLpa);
    m_YearLunarLabel->move(116, 27);
    QHBoxLayout *yeartitleLayout = new QHBoxLayout;
    yeartitleLayout->setMargin(0);
    yeartitleLayout->setSpacing(0);
    yeartitleLayout->setContentsMargins(12, 12, 10, 12);
    yeartitleLayout->addWidget(m_YearLabel);
    yeartitleLayout->addWidget(m_YearLunarLabel);
    QSpacerItem *spaceitem = new QSpacerItem(30, DDEWeekCalendar::WTodayHeight, QSizePolicy::Expanding, QSizePolicy::Fixed);
    yeartitleLayout->addSpacerItem(spaceitem);
    yeartitleLayout->addWidget(m_prevButton);
    yeartitleLayout->addWidget(m_weekview);
    yeartitleLayout->addWidget(m_nextButton);
    yeartitleLayout->addWidget(m_weekLabel);
    QSpacerItem *spaceitem1 = new QSpacerItem(30, DDEWeekCalendar::WTodayHeight, QSizePolicy::Expanding, QSizePolicy::Fixed);
    yeartitleLayout->addSpacerItem(spaceitem1);
    yeartitleLayout->addWidget(m_today);

    m_weekHeadView = new CWeekHeadView(this);
    m_scheduleView = new CScheduleView(this);
    m_weekHeadView->setFixedWidth(840);
    m_scheduleView->setviewMagin(72, 57, 0, 0);
    m_scheduleView->setRange(763, 1032, QDate(2019, 8, 12), QDate(2019, 8, 18));
    m_scheduleView->setFixedSize(838, 450);

    QVBoxLayout *mhLayout = new QVBoxLayout;
    mhLayout->setMargin(0);
    mhLayout->setSpacing(0);
    mhLayout->setContentsMargins(8, 0, 10, 8);
    mhLayout->addWidget(m_weekHeadView);
    mhLayout->addWidget(m_scheduleView);
    QVBoxLayout *hhLayout = new QVBoxLayout;
    hhLayout->addLayout(yeartitleLayout);
    hhLayout->addLayout(mhLayout);
    m_contentBackground->setLayout(hhLayout);
    setCentralWidget(m_contentBackground);
}

void CWeekWindow::initConnection()
{
    connect(m_prevButton, &DImageButton::clicked, this, &CWeekWindow::slotprev);
    connect(m_today, &DPushButton::clicked, this, &CWeekWindow::slottoday);
    connect(m_nextButton, &DImageButton::clicked, this, &CWeekWindow::slotnext);
    connect(m_weekview, &CWeekView::signalsSelectDate, this, &CWeekWindow::slotCurrentWeek);
    connect(m_weekHeadView, &CWeekHeadView::signalcurrentLunarDateChanged, this, &CWeekWindow::slotcurrentDateLunarChanged);
    connect(m_weekHeadView, &CWeekHeadView::signalcurrentDateChanged, this, &CWeekWindow::slotcurrentDateChanged);
    connect(m_scheduleView, &CScheduleView::signalsUpdateShcedule, this, &CWeekWindow::slotTransitSchedule);
}

void CWeekWindow::slotReturnTodayUpdate()
{
    setDate(QDate::currentDate());
}

void CWeekWindow::slotupdateSchedule(int id)
{
    m_scheduleView->slotupdateSchedule(id);
}

void CWeekWindow::slotTransitSchedule(int id)
{
    emit signalsWUpdateShcedule(this, id);
}

void CWeekWindow::slotprev()
{
    if (m_currentdate.year() > 1900) {
        m_currentdate = m_currentdate.addDays(-7);
        setDate(m_currentdate);
    } else {
        QMessageBox::information(this, tr("infomation"), tr("Year less than 1900!"));
    }
}

void CWeekWindow::slotnext()
{
    m_currentdate = m_currentdate.addDays(7);;
    setDate(m_currentdate);
}

void CWeekWindow::slottoday()
{
    emit signalsReturnTodayUpdate(this);
    setDate(QDate::currentDate());
}

void CWeekWindow::slotCurrentWeek(QDate date, QDate currentDate)
{
    QVector<QDate> vDate;
    for (int i = 0; i < 7; i++)
        vDate.append(date.addDays(i));
    m_currentdate = currentDate;
    m_weekHeadView->setWeekDay(vDate);
    m_weekHeadView->setCurrentDate(m_currentdate);
    m_scheduleView->setRange(date, date.addDays(6));
    //m_scheduleView->setDate(m_currentdate);
}

void CWeekWindow::slotcurrentDateLunarChanged(QDate date,  CaLunarDayInfo detail, int type)
{
    if (date != QDate::currentDate()) {
        m_today->setVisible(true);
    } else {
        m_today->setVisible(false);
    }
    if (detail.mLunarFestival.isEmpty()) {
        m_scheduleView->setDate(m_currentdate, detail.mSolarFestival);
    } else {
        m_scheduleView->setDate(m_currentdate, detail.mLunarFestival + "    " + detail.mSolarFestival);
    }
    m_currentdate = date;
    if (type == 1) {
        m_YearLabel->setText(QString::number(date.year()) + tr("Y"));
        m_YearLunarLabel->setText("-" + detail.mGanZhiYear + detail.mZodiac + "年-");
    }
}

void CWeekWindow::slotcurrentDateChanged(QDate date)
{
    if (date != QDate::currentDate()) {
        m_today->setVisible(true);
    } else {
        m_today->setVisible(false);
    }
    m_currentdate = date;
    m_scheduleView->setDate(date);
}