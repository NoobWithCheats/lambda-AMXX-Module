#pragma once

#include "sdk/amxxmodule.h"
#include <set>
#include <string>
#include <vector>
#include <array>
#include <map>
#include <memory>

#define MAX_PLAYERS 32


enum lambda_handle
{
    lambda_Error = -4,
    lambda_InvalidPlayer = -3,
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
    lambda_handle clearPermissions();
    // Возращает указатель на разрешение группы
    std::shared_ptr<std::string> getPermission(size_t index) const;
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
    // Поиск группы, возращает указатель на группу
    size_t findGroup(std::shared_ptr<GroupMngr> ptr);
    // Поиск группы по указателю
    std::shared_ptr<GroupMngr> findGroup(size_t group);
    // Удаляет группу
    void destroyGroup(size_t group);
    // Получает указатель на группу из позиции в векторе
    size_t getGroupPtr(size_t index) const;
    // Получает кол-во групп
    size_t getGroupCount(bool size) const;
    

    // Создать разрешение, возращаеть указатель на разрешение
    size_t createPermission(std::string_view name);
    // Поиск разрешения по названию, возрашает указатель на разрешение
    size_t findPermission(std::string_view name);
    // Поиск группы, возращает указатель на разрешение
    size_t findPermission(std::shared_ptr<std::string> ptr);
    // Поиск разрешения по указателю
    std::shared_ptr<std::string> findPermission(size_t permission);
    // Удалет разрешение
    void destroyPermission(size_t permission);
    // Получает указатель на разрешение из позиции в векторе
    size_t getPermissionPtr(size_t index) const;
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
    lambda_handle clearGroups();
    // Получает кол-во групп у игрока
    size_t getGroupCount() const;
    // Возращает указатель на группу игрока
    std::shared_ptr<GroupMngr> getGroup(size_t index) const;

    // Добавить разрешение игроку
    lambda_handle addPermission(std::shared_ptr<std::string> permission);
    // Поиск разрешения у игрока
    bool findPermission(std::shared_ptr<std::string> permission, bool group);
    // Удалить разрешение у игрока
    lambda_handle removePermission(std::shared_ptr<std::string> permission);
    // Удалить все разрешения у игрока
    lambda_handle clearPermissions();
    // Получает кол-во разрешений у игрока
    size_t getPermissionCount(bool group) const;
    // Возращает указатель на разрешение игрока
    std::shared_ptr<std::string> getPermission(size_t index) const;

    // Установить иммунитет игроку
    void setImmunity(size_t immunity);
    // Получить иммунитет игрока
    size_t getImmunity() const;
    // Установить префикс игроку
    void setPrefix(std::string_view name);
    // Получить префикс игрока
    std::string_view getPrefix() const;

    auto& getGroups() { return m_groups; }
    auto& getPermissions() { return m_permissions; }

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

class tempMngr
{
  public:
    tempMngr() {}
    ~tempMngr() {}

    // Добавляет группу во временное хранилище
    lambda_handle addGroup(std::shared_ptr<GroupMngr> group);
    // Удаляет группу из временного хранилища
    lambda_handle removeGroup(std::shared_ptr<GroupMngr> group);
    // Очищает временное хранилище групп
    lambda_handle clearGroups();
    // Копирует (добавляет) группы из времменого хранилища
    void copyGroups(std::set<std::shared_ptr<GroupMngr>>& group);
    // Возращает кол-во групп во временном хранилище
    size_t getGroupCount() const;
    // Получает указатель на группу из позиции в временном хранилище
    std::shared_ptr<GroupMngr> getGroup(size_t index) const;

    // Добавляет разрешение во времменное хранилище
    lambda_handle addPermission(std::shared_ptr<std::string> premission);
    // Удаляет разрешение из временного хранилищ
    lambda_handle removePermission(std::shared_ptr<std::string> permission);
    // Очищает временное хранилище разрешений
    lambda_handle clearPermissions();
    // Копирует (добавляет) разрешения из времменого хранилища
    void copyPermissions(std::set<std::shared_ptr<std::string>>& permission);
    // Возращает кол-во разрешений во временном хранилище
    size_t getPermissionCount() const;
    // Получает указатель на разрешение из позиции в временном хранилище
    std::shared_ptr<std::string> getPermission(size_t index) const;

  private:
    // группы
    std::set<std::shared_ptr<GroupMngr>> m_groups;
    // разрешениям
    std::set<std::shared_ptr<std::string>> m_permissions;
};

#define CHECK_PLAYER(x)                                                         \
    if (!MF_IsPlayerValid(x)) {                                                 \
        MF_LogError(amx, AMX_ERR_NATIVE, "Invalid player %d", x);               \
        return lambda_InvalidPlayer;                                            \
    }
