#pragma once

#include "sdk/amxxmodule.h"
#include <set>
#include <string>
#include <vector>
#include <array>
#include <memory>

#define MAX_PLAYERS 32


enum lambda_handle
{
    lambda_Error = -3,
    lambda_InvalidPermission = -2,
    lambda_InvalidGroup = -1,
    lambda_Ready,
    lambda_Done
};
/*
    Как это работает:
    - при создании группы/разрешения создаётся указатель на них.
    - в остальном мы добавляем в массив эти указатели. Т.е. всё будет ссылаться на конкретный
    элемент, благодаря чему не будем иметь миллион копий.
*/

class GroupMngr
{
  public:
    GroupMngr(std::string_view name);
    ~GroupMngr();

    // Добавить разрешение в группу
    lambda_handle addPermission(std::shared_ptr<std::string> permission);
    // Удалить разрешение из группы
    lambda_handle removePermission(std::shared_ptr<std::string> permission);
    // Поиск разрешения в группе
    bool findPermission(std::shared_ptr<std::string> permission);
    // Очистить разрешения из группы
    bool clearPermissions();
    // Кол-во разрешений в группе
    size_t getPermissionCount() const;
    // Установить иммунитет группе
    void setImmunity(size_t immunity);
    
    const auto& getPermissions() const
    {
        return m_permissions;
    }

    std::string_view getName() const;
    // Получить иммунитет группы
    size_t getImmunity() const;
    // Установить префикс группе
    void setPrefix(std::string_view name);
    // Получить префикс группы
    std::string_view getPrefix() const;

  private:
    // имя группы
    std::string m_name;
    // иммунитет группы
    size_t m_immunity = 0;
    // Префикс группы
    std::string m_prefix;
    // разрешения группы
    std::set<std::shared_ptr<std::string>> m_permissions;
};

class AccessMngr
{
  public:

    AccessMngr(){}
    ~AccessMngr(){}
    
    // Создать группу, возращает указатель на группу
    size_t createGroup(std::string_view name);
    // Поиск группы по названию, возращает указатель на группу
    size_t findGroup(std::string_view name);
    // Поиск группы по указателю
    std::shared_ptr<GroupMngr> findGroup(size_t group);
    // Удаляет группу
    void destroyGroup(size_t group);
    // Получает кол-во групп
    size_t getGroupCount(bool size) const;

    // Создать разрешение, возращаеть указатель на разрешение
    size_t createPermission(std::string_view name);
    // Поиск разрешения по названию, возрашает указатель на разрешение
    size_t findPermission(std::string_view name);
    // Поиск разрешения по указателю
    std::shared_ptr<std::string> findPermission(size_t permission);
    // Удалет разрешение
    void destroyPermission(size_t permission);
    // Получает кол-во разрешений
    size_t getPermissionCount(bool size) const;

    // Полная очиcтка Групп и Разрешений.
    void clear();

  private:
    // вектор с группами
    std::vector<std::shared_ptr<GroupMngr>> m_groups;
    // Вектор с разрешениями
    std::vector<std::shared_ptr<std::string>> m_permissions;
};


class PlayerMngr
{
  public:
    PlayerMngr() {}
    ~PlayerMngr() {}

    // Добавить группу игроку
    lambda_handle addGroup(std::shared_ptr<GroupMngr> group);
    // Поиск группы у игрока
    bool findGroup(std::shared_ptr<GroupMngr> group);
    // Удалить группу у игрока
    lambda_handle removeGroup(std::shared_ptr<GroupMngr> group);
    // Удалить группы у игрока
    bool clearGroups();
    // Получает кол-во групп у игрока
    size_t getGroupCount() const;

    // Добавить разрешение игроку
    lambda_handle addPermission(std::shared_ptr<std::string> permission);
    // Поиск разрешения у игрока
    bool findPermission(std::shared_ptr<std::string> permission, bool group);
    // Удалить разрешение у игрока
    lambda_handle removePermission(std::shared_ptr<std::string> permission);
    // Удалить все разрешения у игрока
    bool clearPermissions();
    // Получает кол-во разрешений у игрока
    size_t getPermissionCount(bool group) const;

    // Установить иммунитет игроку
    void setImmunity(size_t immunity);
    // Получить иммунитет игрока
    size_t getImmunity() const;
    // Установить префикс игроку
    void setPrefix(std::string_view name);
    // Получить префикс игрока
    std::string_view getPrefix() const;

    void clear();

  private:
    // Иммунитет игрока
    size_t m_immunity = 0;
    // вектор с группами игрока
    std::set<std::shared_ptr<GroupMngr>> m_groups;
    // вектор с разрешениями игрока
    std::set<std::shared_ptr<std::string>> m_permissions;
    // Префикс игрока
    std::string m_prefix;
};

#define CHECK_PLAYER(x)                                                         \
    if (!MF_IsPlayerValid(x)) {                                                 \
        MF_LogError(amx, AMX_ERR_NATIVE, "Invalid player %d", x);               \
        return 0;                                                               \
    }
