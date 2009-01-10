/***************************************************************************
 *                                                                         *
 *   copyright : (C) 2008 The University of Toronto                        *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "overridelabeltab.h"

#include "application.h"
#include "objectstore.h"
#include "mainwindow.h"
#include "document.h"

namespace Kst {

OverrideLabelTab::OverrideLabelTab(QString title, QWidget *parent)
  : DialogTab(parent) {

  setupUi(this);
  _bold->setIcon(QPixmap(":kst_bold.png"));
  _bold->setFixedWidth(32);
  _italic->setIcon(QPixmap(":kst_italic.png"));
  _italic->setFixedWidth(32);
  _labelColor->setFixedWidth(32);
  _labelColor->setFixedHeight(32);

  setTabTitle(title);

  connect(_fontSize, SIGNAL(valueChanged(double)), this, SIGNAL(modified()));
  connect(_bold, SIGNAL(toggled(bool)), this, SIGNAL(modified()));
  connect(_italic, SIGNAL(toggled(bool)), this, SIGNAL(modified()));
  connect(_family, SIGNAL(currentIndexChanged(int)), this, SIGNAL(modified()));
  connect(_labelColor, SIGNAL(changed(const QColor &)), this, SIGNAL(modified()));
  connect(_useDefault, SIGNAL(toggled(bool)), this, SIGNAL(useDefaultChanged(bool)));
}


OverrideLabelTab::~OverrideLabelTab() {
}

QFont OverrideLabelTab::labelFont() const {
  QFont font(_family->currentFont());
  font.setItalic(_italic->isChecked());
  font.setBold(_bold->isChecked());
  return font;
}


void OverrideLabelTab::setLabelFont(const QFont &font) {
  _family->setCurrentFont(font);
  _bold->setChecked(font.bold());
  _italic->setChecked(font.italic());
}


qreal OverrideLabelTab::labelFontScale() const {
  return _fontSize->value();
}


void OverrideLabelTab::setLabelFontScale(const qreal scale) {
  _fontSize->setValue(scale);
}


QColor OverrideLabelTab::labelColor() const { 
  return _labelColor->color();
}


void OverrideLabelTab::setLabelColor(const QColor &color) {
  _labelColor->setColor(color);
}

void OverrideLabelTab::setFontSpecsIfDefault(const QFont &font, const qreal scale, const QColor &color) {
  if (_useDefault->isChecked()) {
    setLabelFontScale(scale);
    setLabelFont(font);
    setLabelColor(color);
  }
}

void OverrideLabelTab::setUseDefault(bool use_default) {
  _useDefault->setChecked(use_default);
}

bool OverrideLabelTab::useDefault() const {
  return (_useDefault->isChecked());
}

}

// vim: ts=2 sw=2 et