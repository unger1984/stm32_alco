#pragma once

#include "app_shared.h"

#define FLASH_SETTINGS_ADDR 0x08020000 // Последний сектор (128 КБ, sector 5)
#define SETTINGS_MAGIC_NUM                                                     \
  0xDEADBEEF // Контрольное число для проверки корректности

// Основной класс настроек
class SettingsManager {
public:
  AppSettings data;
  void load();
  bool save();
  bool saveIfChanged();
};

extern SettingsManager appSettings;
