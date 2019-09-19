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
#include "calendarmainwindow.h"
#include <QHBoxLayout>
#include <QSpacerItem>
#include <QWidget>
#include <DAboutDialog>
#include <DHiDPIHelper>
#include "yearwindow.h"
#include "monthwindow.h"
#include "dbuscalendar_adaptor.h"
#include <QSizePolicy>
#include "weekwindow.h"
#include "daywindow.h"
#include <DPalette>
#include "scheduledatamanage.h"
#include "myschceduleview.h"
DGUI_USE_NAMESPACE
static const int CalendarMTitleHeight = 50;

static const int CalendarMWidth = 860;
static const int CalendarMHeight = 634;

static const int WorkViewWidth = 860;
static const int WorkViewHeight = 584;
Calendarmainwindow::Calendarmainwindow()
{
    setContentsMargins(QMargins(0, 0, 0, 0));
    initUI();
    initConnection();
    initLunar();
    setFixedSize(CalendarMWidth, CalendarMHeight);
    setWindowTitle(tr("Deepin Calendar"));

    new CalendarAdaptor(this);
}

void Calendarmainwindow::slotTheme(int type)
{
    CScheduleDataManage::getScheduleDataManage()->setTheMe(type);
    m_yearwindow->setTheMe(type);
    m_monthWindow->setTheMe(type);
}

void Calendarmainwindow::openScheduleId(int id)
{
    ScheduleDtailInfo out;
    if (CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl()->getScheduleInfoById(id, out)) {
        CMySchceduleView dlg(this);
        dlg.setSchedules(out);
        dlg.exec();

    }
}
void Calendarmainwindow::initUI()
{
    QFrame *titleframe = new QFrame(this);
    titleframe->setObjectName("TitleBar");
    titleframe->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    m_icon = new DLabel(this);
    m_icon->setFixedSize(34, 34);
    m_icon->setPixmap(DHiDPIHelper::loadNxPixmap(":/resources/icon/dde-logo.svg")
                      .scaled(m_icon->size() * devicePixelRatioF()));
    //m_icon->move(12, 8);


    QStringList titlelist;
    titlelist << tr("Y") << tr("M") << tr("W") << tr("D");
    m_buttonBox = new DButtonBox(this);
    //DPalette wpa = m_segmentedControl->palette();
    //wpa.setColor(DPalette::ButtonText, QColor("#FFFFFF"));
    //wpa.setColor(DPalette::Button, QColor("#0081FF "));
    /// wpa.setColor(DPalette::Dark, QColor("#0081FF"));
    //wpa.setColor(DPalette::Light, QColor("#0081FF"));
    // wpa.setColor(DPalette::Window, QColor("#0081FF"));
    // wpa.setColor(DPalette::WindowText, QColor("#414D68"));
    //m_segmentedControl->setPalette(wpa);
    //m_segmentedControl->addSegmented(titlelist);
    // m_segmentedControl->setFixedSize(200, 36);
    m_yearButton = new DButtonBoxButton(tr("Y"), this);
    m_monthButton = new DButtonBoxButton(tr("M"), this);
    m_weekButton = new DButtonBoxButton(tr("W"), this);
    m_dayButton = new DButtonBoxButton(tr("D"), this);

    QList<DButtonBoxButton *> btlist;
    btlist.append(m_yearButton);
    btlist.append(m_monthButton);
    btlist.append(m_weekButton);
    btlist.append(m_dayButton);
    m_buttonBox->setButtonList(btlist, true);

    m_buttonBox->setId(m_yearButton, 0);
    m_buttonBox->setId(m_monthButton, 1);
    m_buttonBox->setId(m_weekButton, 2);
    m_buttonBox->setId(m_dayButton, 3);
    m_buttonBox->setFixedSize(200, 36);

    QHBoxLayout *titleLayout = new QHBoxLayout;
    titleLayout->setMargin(0);
    titleLayout->setSpacing(0);
    //titleLayout->setContentsMargins(0, 0, 0, 0);
    //titleLayout->addWidget(m_ybutton);
    // titleLayout->addWidget(m_mbutton);
    //titleLayout->addWidget(m_wbutton);
    titleLayout->addSpacing(10);
    titleLayout->addWidget(m_icon);
    titleLayout->addSpacing(18);
    titleLayout->addWidget(m_buttonBox);
    // QSpacerItem *lspaceitem = new QSpacerItem(30, CalendarMTitleHeight, QSizePolicy::Expanding, QSizePolicy::Fixed);
    //titleLayout->addSpacerItem(lspaceitem);
    m_searchEdit = new DSearchEdit;
    m_searchEdit->setFixedHeight(36);
    m_searchEdit->setMinimumWidth(240);
    m_searchEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    titleLayout->addSpacing(52);
    titleLayout->addWidget(m_searchEdit);
    //m_searchEdit->setSizePolicy(QSizePolicy::Expanding);
    //QSpacerItem *rspaceitem = new QSpacerItem(30, CalendarMTitleHeight, QSizePolicy::Expanding, QSizePolicy::Fixed);
    titleLayout->addStretch(10);
    //QWidget *buttonW = new QWidget;
    titleframe->setLayout(titleLayout);

    DTitlebar *titlebar = this->titlebar();
    //用于切换主题
    titlebar->setMenu(new DMenu);

    titlebar->setTitle("");
    titlebar->setFixedHeight(50);
    titlebar->addWidget(titleframe, Qt::AlignLeft | Qt::AlignVCenter);
    // titlebar->move(36, 3);

    m_stackWidget = new QStackedWidget;
    m_stackWidget->setFixedSize(WorkViewWidth, WorkViewHeight);
    createview();
    setCentralWidget(m_stackWidget);
    //m_bttongroup->button(0)->setChecked(true);
    m_yearButton->setFocus();
    m_yearButton->setChecked(true);
}

void Calendarmainwindow::initConnection()
{
    connect(m_buttonBox, &DButtonBox::buttonClicked, this, &Calendarmainwindow::slotstackWClicked);
    //connect(m_bttongroup, static_cast<void(QButtonGroup::*)(int)>(&QButtonGroup::buttonClicked), this, &Calendarmainwindow::slotstackWClicked);
    //connect(m_weekWindow, &CWeekWindow::signalsWUpdateShcedule, this, &Calendarmainwindow::slotWUpdateShcedule);
    //connect(m_monthWindow, &CMonthWindow::signalsWUpdateShcedule, this, &Calendarmainwindow::slotWUpdateShcedule);
    // connect(m_DayWindow, &CDayWindow::signalsWUpdateShcedule, this, &Calendarmainwindow::slotWUpdateShcedule);
    connect(m_searchEdit, &DSearchEdit::returnPressed, this, &Calendarmainwindow::slotSreturnPressed);
    connect(m_searchEdit, &DSearchEdit::textChanged, this, &Calendarmainwindow::slotStextChanged);
    connect(m_weekWindow, &CWeekWindow::signalsReturnTodayUpdate, this, &Calendarmainwindow::slotReturnTodyUpdate);
    connect(m_monthWindow, &CMonthWindow::signalsReturnTodayUpdate, this, &Calendarmainwindow::slotReturnTodyUpdate);
    connect(m_DayWindow, &CDayWindow::signalsReturnTodayUpdate, this, &Calendarmainwindow::slotReturnTodyUpdate);
    connect(m_yearwindow, &CYearWindow::signalsReturnTodayUpdate, this, &Calendarmainwindow::slotReturnTodyUpdate);
    //监听当前应用主题切换事件
    QObject::connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::paletteTypeChanged, this, &Calendarmainwindow::slotTheme);
}

void Calendarmainwindow::initLunar()
{
    m_yearwindow->setLunarVisible(QLocale::system().name().contains("zh"));
}

void Calendarmainwindow::createview()
{
    m_yearwindow = new CYearWindow;
    m_yearwindow->setDate(QDate::currentDate());
    m_stackWidget->addWidget(m_yearwindow);
#if 1
    m_monthWindow = new CMonthWindow;
    m_monthWindow->setDate(QDate::currentDate());
    m_monthWindow->setFirstWeekday(0);
    m_stackWidget->addWidget(m_monthWindow);
    m_weekWindow  = new CWeekWindow(this);
    m_weekWindow->setFirstWeekday(0);
    m_weekWindow->setDate(QDate::currentDate());
    m_stackWidget->addWidget(m_weekWindow);

    m_DayWindow = new CDayWindow;
    m_DayWindow->setDate(QDate::currentDate());
    m_stackWidget->addWidget(m_DayWindow);
#else
    CYearWindow *yearwindow1 = new CYearWindow;
    yearwindow1->setDate(QDate(2014, 2, 1));
    m_stackWidget->addWidget(yearwindow1);
    CYearWindow *yearwindow2 = new CYearWindow;
    yearwindow2->setDate(QDate(2014, 2, 1));
    m_stackWidget->addWidget(yearwindow2);

    CYearWindow *yearwindow3 = new CYearWindow;
    yearwindow3->setDate(QDate(2018, 2, 1));
    m_stackWidget->addWidget(yearwindow3);
#endif

}

DPushButton *Calendarmainwindow::createButon(QString name)
{
    DPushButton *button = new DPushButton();
    button->setText(name);
    button->setFixedSize(50, 40);
    //button->setPressIcon(DHiDPIHelper::loadNxPixmap(":/resources/icon/dde-choose30.svg")
    //.scaled(50, 40));
    return  button;
}

void Calendarmainwindow::slotstackWClicked(QAbstractButton *bt)
{
    int index = m_buttonBox->id(bt);
    if (index < 0 || index > m_stackWidget->depth() - 1) {

        return;
    }
    m_stackWidget->setCurrentIndex(index);
    switch (index) {
    case 0: {
        m_yearwindow->slotReturnTodayUpdate();
    }
    break;
    case 1: {
        m_monthWindow->slotReturnTodayUpdate();
        m_monthWindow->slotupdateSchedule(0);
    }
    break;
    case 2: {
        m_weekWindow->slotReturnTodayUpdate();
        m_weekWindow->slotupdateSchedule(0);
    }
    break;
    case 3: {
        m_DayWindow->slotReturnTodayUpdate();
        m_DayWindow->slotupdateSchedule(0);
    }
    break;
    }
}

void Calendarmainwindow::slotWUpdateShcedule(QMainWindow *w, int id)
{
    if (w != m_weekWindow)
        m_weekWindow->slotupdateSchedule(id);
    if (w != m_monthWindow)
        m_monthWindow->slotupdateSchedule(id);
    if (w != m_DayWindow)
        m_DayWindow->slotupdateSchedule(id);
}

void Calendarmainwindow::slotReturnTodyUpdate(QMainWindow *w)
{
    if (w != m_weekWindow)
        m_weekWindow->slotReturnTodayUpdate();
    if (w != m_monthWindow)
        m_monthWindow->slotReturnTodayUpdate();
    if (w != m_DayWindow)
        m_DayWindow->slotReturnTodayUpdate();
    if (w != m_yearwindow)
        m_yearwindow->slotReturnTodayUpdate();
}

void Calendarmainwindow::slotSreturnPressed()
{
    m_dayButton->click();
    //m_segmentedControl->setCurrentIndex(3);
    //m_stackWidget->setCurrentIndex(3);
    m_DayWindow->setSearchText(m_searchEdit->text());
}

void Calendarmainwindow::slotStextChanged()
{
    m_dayButton->click();
    //m_segmentedControl->setCurrentIndex(3);
    //m_stackWidget->setCurrentIndex(3);
    m_DayWindow->setSearchWFlag(!m_searchEdit->text().isEmpty());
}