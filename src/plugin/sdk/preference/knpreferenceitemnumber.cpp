/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include <QSlider>
#include <QSpinBox>
#include <QBoxLayout>

#include "knpreferenceitemnumber.h"

KNPreferenceItemNumber::KNPreferenceItemNumber(QWidget *parent) :
    KNPreferenceItemBase(parent)
{
    //Initial the spin box.
    m_spinBox=new QSpinBox(this);
    QPalette pal=m_spinBox->palette();
    pal.setColor(QPalette::Base, QColor(0x40, 0x40, 0x40));
    pal.setColor(QPalette::Button, QColor(0x50, 0x50, 0x50));
    pal.setColor(QPalette::Highlight, QColor(0x60, 0x60, 0x60));
    pal.setColor(QPalette::HighlightedText, QColor(0xf7, 0xcf, 0x3d));
    pal.setColor(QPalette::Text, QColor(0xA0, 0xA0, 0xA0));
    m_spinBox->setPalette(pal);

    //Initial the slider.
    m_slider=new QSlider(Qt::Horizontal, this);
    m_slider->setFixedWidth(200);
    pal.setColor(QPalette::Button, QColor(0x60, 0x60, 0x60));
    pal.setColor(QPalette::Highlight, QColor(0xf7, 0xcf, 0x3d));
    m_slider->setPalette(pal);

    //Link the control.
    connect(m_slider, &QSlider::valueChanged,
            this, &KNPreferenceItemNumber::synchronizeValue);
    connect(m_spinBox, SIGNAL(valueChanged(int)), //Signal has QString and int.
            this, SLOT(synchronizeValue(int)));

    //Insert the widget to preference item.
    insertStretch();
    insertWidget(m_slider);
    insertSpacing(5);
    insertWidget(m_spinBox);
}

void KNPreferenceItemNumber::setRange(int min, int max)
{
    m_spinBox->setRange(min, max);
    m_slider->setRange(min, max);
}

QVariant KNPreferenceItemNumber::defaultValue() const
{
    return m_defaultValue;
}

QVariant KNPreferenceItemNumber::value() const
{
    return m_spinBox->value();
}

void KNPreferenceItemNumber::setDefaultValue(const QVariant &defaultValue)
{
    //Set the default value.
    m_defaultValue=defaultValue.toInt();
    //When default value changed, set the value to the default value.
    synchronizeValue(m_defaultValue);
}

void KNPreferenceItemNumber::setValue(const QVariant &value)
{
    synchronizeValue(value.toInt());
}

void KNPreferenceItemNumber::synchronizeValue(const int &value)
{
    //Block the signal.
    m_spinBox->blockSignals(true);
    m_slider->blockSignals(true);
    //Set the value.
    m_spinBox->setValue(value);
    m_slider->setValue(value);
    //Release the signal.
    m_spinBox->blockSignals(false);
    m_slider->blockSignals(false);
}
