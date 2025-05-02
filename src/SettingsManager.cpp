#include "SettingsManager.h"
#include "stm32f4xx_hal.h"
#include <string.h>

SettingsManager appSettings;
static const uint8_t defaultAngles[6] = {15, 45, 75, 105, 135, 165};

void SettingsManager::load() {
  // Указатель на структуру в памяти Flash
  AppSettings *flashData = (AppSettings *)FLASH_SETTINGS_ADDR;

  // Проверяем контрольное число
  if (flashData->MagicNum == SETTINGS_MAGIC_NUM) {
    // Flash содержит валидные данные — копируем в оперативную память
    memcpy(&data, flashData, sizeof(AppSettings));
  } else {
    // Нет валидных данных — сбрасываем на значения по умолчанию
    memset(&data, 0, sizeof(AppSettings));
    data.MagicNum = SETTINGS_MAGIC_NUM;
    data.calibration = 2000;
    memcpy(data.angles, defaultAngles, 6);
    memset(data.doses, 100, 6); // Устанавливаем все дозы = 100
    data.isDebug = 0;
    save();
  }
};

bool SettingsManager::save() {
  // Разрешаем доступ к Flash
  HAL_FLASH_Unlock();

  // Структура для параметров стирания
  FLASH_EraseInitTypeDef erase;
  uint32_t pageError;

  erase.TypeErase = FLASH_TYPEERASE_SECTORS;  // Стираем по секторам
  erase.Sector = FLASH_SECTOR_5;              // Используем sector 7
  erase.NbSectors = 1;                        // Только один сектор
  erase.VoltageRange = FLASH_VOLTAGE_RANGE_3; // Диапазон напряжения 2.7–3.6В

  // Стираем сектор (обязательно перед записью)
  if (HAL_FLASHEx_Erase(&erase, &pageError) != HAL_OK) {
    HAL_FLASH_Lock(); // Обязательно заблокировать Flash при ошибке
    return false;
  }

  // Записываем структуру по 4 байта
  uint32_t *dt = (uint32_t *)&data;
  for (size_t i = 0; i < sizeof(AppSettings) / 4; i++) {
    if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, FLASH_SETTINGS_ADDR + i * 4,
                          dt[i]) != HAL_OK) {
      HAL_FLASH_Lock(); // Заблокировать Flash при ошибке записи
      return false;
    }
  }

  // Блокируем доступ к Flash (обязательно)
  HAL_FLASH_Lock();

  AppSettings *check = (AppSettings *)FLASH_SETTINGS_ADDR;
  if (check->MagicNum != SETTINGS_MAGIC_NUM) {
    // Запись не сработала
    return false;
  }

  return true;
};

bool SettingsManager::saveIfChanged() {
  AppSettings *flashData = (AppSettings *)FLASH_SETTINGS_ADDR;

  // Сравниваем байт в байт с тем, что лежит в Flash
  if (memcmp(&data, flashData, sizeof(AppSettings)) == 0) {
    // Данные совпадают — не нужно перезаписывать
    return true;
  }

  // Данные разные — вызываем обычный save()
  return save();
}