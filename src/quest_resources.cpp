/*
 * Copyright (C) 2014 Christopho, Solarus - http://www.solarus-games.org
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
#include "editor_exception.h"
#include "quest.h"
#include "quest_resources.h"
#include <QFile>
#include <QMap>
#include <QTextStream>

namespace {

/**
 * Description of each resource type.
 */
const QMap<Solarus::ResourceType, QString> resource_type_friendly_names = {
  { Solarus::ResourceType::MAP,      QuestResources::tr("Map")           },
  { Solarus::ResourceType::TILESET,  QuestResources::tr("Tileset")       },
  { Solarus::ResourceType::SPRITE,   QuestResources::tr("Sprite")        },
  { Solarus::ResourceType::MUSIC,    QuestResources::tr("Music")         },
  { Solarus::ResourceType::SOUND,    QuestResources::tr("Sound")         },
  { Solarus::ResourceType::ITEM,     QuestResources::tr("Item")          },
  { Solarus::ResourceType::ENEMY,    QuestResources::tr("Enemy")         },
  { Solarus::ResourceType::ENTITY,   QuestResources::tr("Custom entity") },
  { Solarus::ResourceType::LANGUAGE, QuestResources::tr("Language")      },
  { Solarus::ResourceType::FONT,     QuestResources::tr("Font")          },
};

}

/**
 * @brief Creates an empty resource list for the specified quest.
 * @param quest The quest.
 */
QuestResources::QuestResources(Quest& quest):
  quest(quest) {

  connect(&quest, SIGNAL(root_path_changed(const QString&)),
          this, SLOT(reload()));
  reload();
}

/**
 * @brief Reads project_db.dat and rebuilds the model.
 */
void QuestResources::reload() {

  resources.clear();

  if (quest.is_valid()) {
    resources.load_from_file(quest.get_resource_list_path().toStdString());
    // TODO throw an exception in case of error
  }
}

/**
 * @brief Saves the resource list to the project_db.dat file of the quest.
 * @throws EditorException If the save operation failed.
 */
void QuestResources::save() const {

  if (!quest.is_valid()) {
    throw EditorException("No quest");
  }

  QString file_name = quest.get_resource_list_path();
  QFile file(file_name);
  if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
    throw EditorException("Cannot open file '" + file_name + "' for writing");
  }

  QTextStream out(&file);
  out.setCodec("UTF-8");

  // Save each resource.
  const std::map<ResourceType, std::string> resource_type_names =
      Solarus::QuestResources::get_resource_type_names();
  for (const auto& kvp: resource_type_names) {

    const Solarus::QuestResources::ResourceMap& resource =
        resources.get_elements(kvp.first);
    for (const auto& element: resource) {

      const std::string& id = element.first;
      const std::string& description = element.second;

      out << QString::fromStdString(kvp.second)
          << "{ id = \""
          << QString::fromStdString(id.c_str())
          << "\", description = \""
          << QString::fromStdString(description.c_str())
          << "\" }\n";
    }
    out << "\n";
  }
}

/**
 * @brief Returns whether a resource element exists.
 * @param type A type of resource.
 * @param id The id to lookup.
 * @return @c true if such an element exists in the resource.
 */
bool QuestResources::exists(ResourceType type, const QString& id) const {

  return resources.exists(type, id.toStdString());
}

/**
 * @brief Returns the description of a resource element.
 * @param type A type of resource.
 * @param id Id of the element to get.
 * @return The description of this element.
 * Returns an empty string if the element does not exist.
 */
QString QuestResources::get_description(
    ResourceType type, const QString& id) const {

  return QString::fromStdString(
        resources.get_description(type, id.toStdString()));
}

/**
 * @brief Changes the description of a resource element.
 * @param type A type of resource.
 * @param id Id of the element to change.
 * @param description The new description to set.
 */
void QuestResources::set_description(
    ResourceType type, const QString& id, const QString& description) {

  resources.set_description(type, id.toStdString(), description.toStdString());
  // TODO resources.save_to_file();
  emit element_description_changed(type, id, description);
}

/**
 * @brief Returns the Lua name for the specified resource type.
 * @param resource_type A type of resources.
 * @return The Lua name of this resource type.
 */
QString QuestResources::get_lua_name(ResourceType resource_type) {
  return QString::fromStdString(
        Solarus::QuestResources::get_resource_type_name(resource_type));
}

/**
 * @brief Returns a user-friendly name for the specified resource type.
 * @param resource_type A type of resources.
 * @return The human-readable name of this resource type.
 */
QString QuestResources::get_friendly_name(ResourceType resource_type) {
  return resource_type_friendly_names[resource_type];
}
