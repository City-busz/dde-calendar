﻿/*
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
#pragma once

#include <DComboBox>
#include <QTimeEdit>
#include <DListWidget>
#include <DLineEdit>
#include <QTime>
DWIDGET_USE_NAMESPACE
class CTimeVerticalScroll;
class CCaYearMonthEdit : public DComboBox
{
    Q_OBJECT

public:
    CCaYearMonthEdit(QWidget *parent = nullptr);
    ~CCaYearMonthEdit();
    void  setRange(int minv, int maxv);
    void  setPadStr(QString vstr);
    int getNum();
private:
    void initUI();
    void initConnection();
    void showPopup();
    void hidePopup();
signals:
    void signalsCurrentValueChange(int value);
public slots:
    void setCurrentNum(int num);
    void slotcurrentValueChanged(int value);
    void slotcurrentValueChangedClose(int value);
private:
    QLineEdit                 *m_timeEdit;
    CTimeVerticalScroll       *m_verticalScroll;
    DListWidget               *m_pListWidget;
    int                       m_num;
    int                       m_min;
    int                       m_max;
    QString                   m_paddingStr;
};
