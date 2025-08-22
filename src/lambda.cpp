#include "lambda.h"


GroupMngr::GroupMngr(std::string_view name) : m_name(name) {}

GroupMngr::~GroupMngr()
{
    m_name.clear();
    m_immunity = 0;
    m_permissions.clear();
}

std::string_view GroupMngr::getName() const
{
    return m_name;
}

lambda_handle GroupMngr::addPermission(std::shared_ptr<std::string> permission)
{
    if (findPermission(permission))
    {
        return lambda_Ready;
    }

    m_permissions.insert(permission);
    return lambda_Done;
}

lambda_handle GroupMngr::removePermission(std::shared_ptr<std::string> permission)
{
    auto iter = m_permissions.find(permission);

    if (iter != m_permissions.end())
    {
        m_permissions.erase(iter);
    }

    if (!findPermission(permission))
    {
        return lambda_Done;
    }

    return lambda_Error;
}

bool GroupMngr::findPermission(std::shared_ptr<std::string> permission)
{
    if (m_permissions.find(permission) == m_permissions.end())
    {
        return false;
    }

    return true;
}

bool GroupMngr::clearPermissions()
{
    m_permissions.clear();
    
    if (m_permissions.size())
    {
        return false;
    }

    return true;
}

size_t GroupMngr::getPermissionCount() const
{
    return m_permissions.size();
}

void GroupMngr::setImmunity(size_t immuity)
{
    m_immunity = immuity;
}

size_t GroupMngr::getImmunity() const
{
    return m_immunity;
}

size_t AccessMngr::createGroup(std::string_view name)
{
    auto group = findGroup(name);

    if (group)
    {
        return group;
    }

    for (size_t index = 0; index < m_groups.size(); index++)
    {
        if (m_groups[index] == nullptr)
        {
            m_groups[index] = std::make_shared<GroupMngr>(name);
            return index + 1;
        }
    }
;
    m_groups.push_back(std::make_shared<GroupMngr>(name));
    return m_groups.size();
}

size_t AccessMngr::findGroup(std::string_view name)
{
    for (size_t i = 0; i < m_groups.size(); i++)
    {
        if (m_groups[i] == nullptr)
        {
            continue;
        }

        if (!m_groups[i]->getName().compare(name))
        {
            return i + 1;
        }
    }

    return 0;
}

std::shared_ptr<GroupMngr> AccessMngr::findGroup(size_t group)
{
    --group;

    if (group < 0 || group >= m_groups.size() || m_groups[group] == nullptr)
    {
        return nullptr;
    }

    return m_groups[group];
}

void AccessMngr::destroyGroup(size_t group)
{   
    --group;

    auto iter = m_groups.begin() + group;
    m_groups.erase(iter);

    iter = m_groups.begin() + group;
    m_groups.insert(iter, nullptr);
}

size_t AccessMngr::getGroupCount() const
{
    size_t count = 0;

    for (size_t i = 0; i < m_groups.size(); i++)
    {
        if (m_groups[i] == nullptr)
        {
            continue;
        }

        ++count;
    }

    return count;
}

size_t AccessMngr::createPermission(std::string_view name)
{
    auto permission = findPermission(name);

    if (permission)
    {
        return permission;
    }

    for (size_t index = 0; index < m_permissions.size(); index++)
    {
        if (m_permissions[index] == nullptr)
        {
            m_permissions[index] = std::make_shared<std::string>(name);
            return index + 1;
        }
    }

    m_permissions.push_back(std::make_shared<std::string>(name));
    return m_permissions.size();
}

size_t AccessMngr::findPermission(std::string_view name)
{
    for (size_t i = 0; i < m_permissions.size(); i++)
    {
        if (m_permissions[i] == nullptr)
        {
            continue;
        }

        if (!m_permissions[i]->compare(name))
        {
            return i + 1;
        }
    }

    return 0;
}

std::shared_ptr<std::string> AccessMngr::findPermission(size_t permission)
{
    --permission;

    if (permission >= m_permissions.size() || m_permissions[permission] == nullptr)
    {
        return nullptr;
    }

    return m_permissions[permission];
}

void AccessMngr::destroyPermission(size_t permission)
{
    --permission;

    auto iter = m_permissions.begin() + permission;
    m_permissions.erase(iter);

    iter = m_permissions.begin() + permission;
    m_permissions.insert(iter, nullptr);
}

size_t AccessMngr::getPermissionCount() const
{
    size_t count = 0;

    for (size_t i = 0; i < m_permissions.size(); i++)
    {
        if (m_permissions[i] == nullptr)
        {
            continue;
        }

        ++count;
    }

    return count;
}

void AccessMngr::clear()
{
    m_groups.clear();
    m_permissions.clear();
}

lambda_handle PlayerMngr::addGroup(std::shared_ptr<GroupMngr> group)
{
    if (findGroup(group))
    {
        return lambda_Ready;
    }

    m_groups.insert(group);
    return lambda_Done;
}

bool PlayerMngr::findGroup(std::shared_ptr<GroupMngr> group)
{
    if (m_groups.find(group) == m_groups.end())
    {
        return false;
    }

    return true;
}

lambda_handle PlayerMngr::removeGroup(std::shared_ptr<GroupMngr> group)
{
    if (!findGroup(group))
    {
        return lambda_InvalidGroup;
    }

    m_groups.erase(group);

    if (!findGroup(group))
    {
        return lambda_Done;
    }

    return lambda_Error;
}

bool PlayerMngr::clearGroups()
{
    m_groups.clear();

    if (m_groups.size())
    {
        return false;
    }

    return true;
}

size_t PlayerMngr::getGroupCount() const
{
    return m_groups.size();
}

lambda_handle PlayerMngr::addPermission(std::shared_ptr<std::string> permission)
{
    if (findPermission(permission, true))
    {
        return lambda_Ready;
    }

    m_permissions.insert(permission);
    return lambda_Done;
}

lambda_handle PlayerMngr::removePermission(std::shared_ptr<std::string> permission)
{
    if (!findPermission(permission, false))
    {
        return lambda_InvalidPermission;
    }

    m_permissions.erase(permission);

    if (!findPermission(permission, false))
    {
        return lambda_Done;
    }

    return lambda_Error;
}

bool PlayerMngr::findPermission(std::shared_ptr<std::string> permission, bool group)
{
    if (!group && m_permissions.find(permission) == m_permissions.end())
    {
        return false;
    }

    std::set<std::shared_ptr<std::string>> temp = m_permissions;

    for (auto iter = m_groups.begin(); iter != m_groups.end(); iter++)
    {
        auto permission = iter->get()->getPermissions();
        temp.insert(permission.begin(), permission.end());
    }

    if (temp.find(permission) == temp.end())
    {
        return false;
    }

    return true;
}

bool PlayerMngr::clearPermissions()
{
    m_permissions.clear();

    if (m_permissions.size())
    {
        return false;
    }

    return true;
}

size_t PlayerMngr::getPermissionCount(bool group) const
{ 
    if (!group)
    {
        return m_permissions.size();
    }

    std::set<std::shared_ptr<std::string>> temp = m_permissions;

    for (auto iter = m_groups.begin(); iter != m_groups.end(); iter++)
    {
        auto permission = iter->get()->getPermissions();
        temp.insert(permission.begin(), permission.end());
    }

    return temp.size();
}

void PlayerMngr::setImmunity(size_t immuity)
{
    m_immunity = immuity;
}

size_t PlayerMngr::getImmunity() const
{
    return m_immunity;
}

void PlayerMngr::clear()
{
    m_groups.clear();
    m_permissions.clear();
}