/*
 * Copyright (C) 2014-2015 Christopho, Solarus - http://www.solarus-games.org
 *
 * Solarus Quest Editor is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Solarus Quest Editor is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef SOLARUSEDITOR_EDIT_ENTITY_DIALOG_H
#define SOLARUSEDITOR_EDIT_ENTITY_DIALOG_H

#include "ui_edit_entity_dialog.h"

class EditEntityDialog : public QDialog {
  Q_OBJECT

public:

  explicit EditEntityDialog(Quest& quest, QWidget* parent = nullptr);

private:

  Ui::EditEntityDialog ui;      /**< The widgets. */

};

#endif
