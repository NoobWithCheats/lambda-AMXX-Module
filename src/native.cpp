#include "sdk/amxxmodule.h"
#include "lambda.h"

std::unique_ptr<AccessMngr> g_AccessMngr;
std::array<std::unique_ptr<PlayerMngr>, MAX_PLAYERS + 1> g_PlayerMngr;
std::map<AMX*, std::array<tempMngr, MAX_PLAYERS + 1>> g_TempMngr;

int Fwd_GroupCreated;
int	Fwd_GroupDestroyed;
int Fwd_GroupClears;
int	Fwd_GroupAddedPermission;
int	Fwd_GroupRemovedPermission;
int	Fwd_GroupClearedPermissions;
int	Fwd_GroupSetImmunity;
int Fwd_GroupSetPrefix;
int	Fwd_PermissionCreated;
int	Fwd_PermissionDestroyed;
int Fwd_PermissionClears;
int	Fwd_PlayerAddedGroup;
int	Fwd_PlayerRemovedGroup;
int	Fwd_PlayerClearedGroups;
int	Fwd_PlayerAddedPermission;
int	Fwd_PlayerRemovedPermission;
int	Fwd_PlayerClearedPermissions;
int	Fwd_PlayerSetImmunity;
int Fwd_PlayerSetPrefix;

void CreateModule()
{
    g_AccessMngr = std::make_unique<AccessMngr>();
    for (auto i = 1; i <= MAX_PLAYERS; i++)
    {
        auto& ptr = g_PlayerMngr.at(i);
        ptr = std::make_unique<PlayerMngr>();
    }

}

void DestroyModule()
{
    g_AccessMngr.reset();
    for (auto i = 1; i <= MAX_PLAYERS; i++)
    {
        auto& ptr = g_PlayerMngr.at(i);
        ptr = nullptr;
    }
}

// native lx_group_create(szGroup[]);
static cell AMX_NATIVE_CALL GroupCreate(AMX* amx, cell* params)
{
    enum args { arg_group = 1 };
    
    char* name = MF_GetAmxString(amx, params[arg_group], 0, nullptr);

    auto ptr = (cell)g_AccessMngr->createGroup(name);
    MF_ExecuteForward(Fwd_GroupCreated, name, ptr);
    return ptr;
}

// native lx_group_find(szGroup[]);
static cell AMX_NATIVE_CALL GroupFind(AMX* amx, cell* params)
{
    enum args { arg_group = 1 };

    char* name = MF_GetAmxString(amx, params[arg_group], 0, nullptr);

    return (cell)g_AccessMngr->findGroup(name);
}

// native lx_group_get_name(pGroup, szGroup[], iLen);
static cell AMX_NATIVE_CALL GroupGetName(AMX* amx, cell* params)
{
    enum args { arg_group = 1, arg_string = 2, arg_len };

    auto group = g_AccessMngr->findGroup(params[arg_group]);

    if (!group)
    {
        return lambda_InvalidGroup;
    }

    auto name = group->getName().data();

    return MF_SetAmxString(amx, params[arg_string], name, params[arg_len]);
}

// native lx_group_get_pointer(pos);
static cell AMX_NATIVE_CALL GroupGetPtr(AMX* amx, cell* params)
{
    enum args { arg_pos = 1 };

    if (params[arg_pos] < 0 || params[arg_pos] > g_AccessMngr->getGroupCount(false))
    {
        return lambda_InvalidGroup;
    }

    return g_AccessMngr->getGroupPtr(params[arg_pos]);
}

// native lx_group_size();
static cell AMX_NATIVE_CALL GroupGetCount(AMX* amx, cell* params)
{
    return g_AccessMngr->getGroupCount(false);
}

// native lambda_handle:lx_group_destroy(pGroup);
static cell AMX_NATIVE_CALL GroupDestroy(AMX* amx, cell* params)
{
    enum args { arg_group = 1 };

    auto group = g_AccessMngr->findGroup(params[arg_group]);

    if (!group)
    {
        return lambda_InvalidGroup;
    }

    for (auto i = 1; i <= MAX_PLAYERS; i++)
    {
       g_PlayerMngr.at(i)->removeGroup(group);
    }

    for (auto i = 0; i <= MAX_PLAYERS; i++)
    {
        for (auto& [key, value] : g_TempMngr)
        {
            value.at(i).removeGroup(group);
        }
    }

    MF_ExecuteForward(Fwd_GroupDestroyed, group->getName().data(), params[arg_group]);
    g_AccessMngr->destroyGroup(params[arg_group]);

    return lambda_Done;
}

// native lx_group_clears();
static cell AMX_NATIVE_CALL GroupClears(AMX* amx, cell* params)
{
    size_t size = g_AccessMngr->getGroupCount(true);
    for (size_t i = 1; i <= size; i++)
    {
        auto group = g_AccessMngr->findGroup(i);

        if (group == nullptr)
        {
            continue;
        }
        
        for (auto j = 1; j <= MAX_PLAYERS; j++)
        {
            g_PlayerMngr.at(j)->removeGroup(group);
        }

        for (auto i = 0; i <= MAX_PLAYERS; i++)
        {
            for (auto& [key, value] : g_TempMngr)
            {
                value.at(i).removeGroup(group);
            }
        }

        g_AccessMngr->destroyGroup(i);
    }

    MF_ExecuteForward(Fwd_GroupDestroyed);
    return lambda_Done;
}

// native lambda_handle:lx_group_add_permission(pGroup, pPermission);
static cell AMX_NATIVE_CALL GroupAddPermission(AMX* amx, cell* params) {
    enum args { arg_group = 1, arg_permission = 2 };

    auto group = g_AccessMngr->findGroup(params[arg_group]);

    if (!group)
    {
        return lambda_InvalidGroup;
    }

    auto permission = g_AccessMngr->findPermission(params[arg_permission]);


    if (!permission)
    {
        return lambda_InvalidPermission;
    }

    auto result = group->addPermission(permission);

    if (result == lambda_Done)
    {
        MF_ExecuteForward(Fwd_GroupAddedPermission, group->getName().data(), params[arg_group],
            permission.get()->data(), params[arg_permission]);
    }

    return result;
}

// native lambda_handle:lx_group_remove_permission(pGroup, pPermission);
static cell AMX_NATIVE_CALL GroupRemovePermission(AMX* amx, cell* params)
{
    enum args { arg_group = 1, arg_permission = 2 };

    auto group = g_AccessMngr->findGroup(params[arg_group]);

    if (!group)
    {
        return lambda_InvalidGroup;
    }

    auto permission = g_AccessMngr->findPermission(params[arg_permission]);

    if (!permission)
    {
        return lambda_InvalidPermission;
    }

    auto result = group->removePermission(permission);

    if (result == lambda_Done)
    {
        MF_ExecuteForward(Fwd_GroupRemovedPermission, group->getName().data(), params[arg_group],
            permission.get()->data(), params[arg_permission]);
    }
    
    return result;
}

// native lambda_handle:lx_group_clear_permissions(pGroup);
static cell AMX_NATIVE_CALL GroupClearPermissions(AMX* amx, cell* params)
{
    enum args { arg_group = 1 };

    auto group = g_AccessMngr->findGroup(params[arg_group]);

    if (!group)
    {
        return lambda_InvalidGroup;
    }

    auto result = group->clearPermissions();

    if (result == lambda_Done)
    {
        MF_ExecuteForward(Fwd_GroupClearedPermissions, group->getName().data(), params[arg_group]);
    }

    return result;
}

// native lx_group_find_permission(pGroup, pPermission);
static cell AMX_NATIVE_CALL GroupFindPermission(AMX* amx, cell* params)
{
    enum args { arg_group = 1, arg_permission = 2 };

    auto group = g_AccessMngr->findGroup(params[arg_group]);

    if (!group)
    {
        return lambda_InvalidGroup;
    }

    auto permission = g_AccessMngr->findPermission(params[arg_permission]);

    if (!permission)
    {
        return lambda_InvalidPermission;
    }

    return group->findPermission(permission);
}

// native lx_group_get_permission_pointer(pGroup, pos);
static cell AMX_NATIVE_CALL GroupGetPermissionPtr(AMX* amx, cell* params)
{
    enum args { arg_group = 1, arg_pos = 2 };

    auto group = g_AccessMngr->findGroup(params[arg_group]);

    if (!group)
    {
        return lambda_InvalidGroup;
    }

    if (params[arg_pos] < 0 || params[arg_pos] > group->getPermissionCount())
    {
        return lambda_InvalidPermission;
    }

    return g_AccessMngr->findPermission(group->getPermission(params[arg_pos]));
}

// native lx_group_permission_count(pGroup);
static cell AMX_NATIVE_CALL GroupPermissionCount(AMX* amx, cell* params)
{
    enum args { arg_group = 1};

    auto group = g_AccessMngr->findGroup(params[arg_group]);

    if (!group)
    {
        return lambda_InvalidGroup;
    }

    return group->getPermissionCount();
}

// native lx_group_get_immunity(pGroup);
static cell AMX_NATIVE_CALL GroupGetImmunity(AMX* amx, cell* params)
{
    enum args { arg_group = 1 };

    auto group = g_AccessMngr->findGroup(params[arg_group]);

    if (!group)
    {
        return lambda_InvalidGroup;
    }

    return (cell)group->getImmunity();
}

// native lambda_handle:lx_group_set_immunity(pGroup, iImmunity);
static cell AMX_NATIVE_CALL GroupSetImmunity(AMX* amx, cell* params)
{
    enum args { arg_group = 1, arg_immunity = 2 };

    auto group = g_AccessMngr->findGroup(params[arg_group]);

    if (!group)
    {
        return lambda_InvalidGroup;
    }

    group->setImmunity(params[arg_immunity]);
    MF_ExecuteForward(Fwd_GroupSetImmunity, group->getName().data(), params[arg_group], params[arg_immunity]);

    return lambda_Done;
}

// native lambda_handle:lx_group_get_prefix(pGroup, szPrefix[], iLen)
static cell AMX_NATIVE_CALL GroupGetPrefix(AMX* amx, cell* params)
{
    enum args { arg_group = 1, arg_string = 2, arg_len };

    auto group = g_AccessMngr->findGroup(params[arg_group]);

    if (!group)
    {
        return lambda_InvalidGroup;
    }

    auto prefix = group->getPrefix().data();

    return MF_SetAmxString(amx, params[arg_string], prefix, params[arg_len]);
}

// native lambda_handle:lx_group_set_prefix(pGroup, szPrefix[])
static cell AMX_NATIVE_CALL GroupSetPrefix(AMX* amx, cell* params)
{
    enum args { arg_group = 1, arg_string = 2, arg_len };

    auto group = g_AccessMngr->findGroup(params[arg_group]);

    if (!group)
    {
        return lambda_InvalidGroup;
    }

    char* prefix = MF_GetAmxString(amx, params[arg_string], 0, nullptr);

    group->setPrefix(prefix);
    MF_ExecuteForward(Fwd_GroupSetPrefix, group->getName().data(), params[arg_group], params[arg_string]);

    return lambda_Done;
}

// native lx_permission_create(szPermission[]);
static cell AMX_NATIVE_CALL PermissionCreate(AMX* amx, cell* params)
{
    enum args { arg_permission = 1 };

    char* name = MF_GetAmxString(amx, params[arg_permission], 0, nullptr);

    auto ptr = (cell)g_AccessMngr->createPermission(name);
    MF_ExecuteForward(Fwd_PermissionCreated, name, ptr);
    return ptr;
}

// native lx_permission_find(szPermission[]);
static cell AMX_NATIVE_CALL PermissionFind(AMX* amx, cell* params)
{
    enum args { arg_permission = 1 };

    char* name = MF_GetAmxString(amx, params[arg_permission], 0, nullptr);

    return (cell)g_AccessMngr->findPermission(name);
}

// native lx_permission_get_name(pPermission, szPermission[], iLen);
static cell AMX_NATIVE_CALL PermissionGetName(AMX* amx, cell* params)
{
    enum args { arg_permission = 1, arg_string = 2, arg_len };

    auto permission = g_AccessMngr->findPermission(params[arg_permission]);

    if (!permission)
    {
        return lambda_InvalidPermission;
    }

    auto name = permission->data();

    return MF_SetAmxString(amx, params[arg_string], name, params[arg_len]);
}

// native lx_permission_get_pointer(pos);
static cell AMX_NATIVE_CALL PermissionGetPtr(AMX* amx, cell* params)
{
    enum args { arg_pos = 1 };

    if (params[arg_pos] < 0 || params[arg_pos] > g_AccessMngr->getPermissionCount(false))
    {
        return lambda_InvalidPermission;
    }

    return g_AccessMngr->getPermissionPtr(params[arg_pos]);
}

// native lx_permission_size();
static cell AMX_NATIVE_CALL PermissionGetCount(AMX* amx, cell* params)
{
    return g_AccessMngr->getPermissionCount(false);
}

// native lambda_handle:lx_permission_destroy(pPermission);
static cell AMX_NATIVE_CALL PermissionDestroy(AMX* amx, cell* params)
{
    enum args { arg_permission = 1 };

    auto permission = g_AccessMngr->findPermission(params[arg_permission]);

    if (!permission)
    {
        return lambda_InvalidPermission;
    }

    for (auto i = 1; i <= MAX_PLAYERS; i++)
    {
        g_PlayerMngr.at(i)->removePermission(permission);
    }

    for (auto i = 0; i <= MAX_PLAYERS; i++)
    {
        for (auto& [key, value] : g_TempMngr)
        {
            value.at(i).removePermission(permission);
        }
    }

    MF_ExecuteForward(Fwd_PermissionDestroyed, permission->data(), params[arg_permission]);
    g_AccessMngr->destroyPermission(params[arg_permission]);

    return lambda_Done;
}


// native lx_permission_clears();
static cell AMX_NATIVE_CALL PermissionClears(AMX* amx, cell* params)
{
    size_t size = g_AccessMngr->getPermissionCount(true);

    for (size_t i = 1; i <= size; i++)
    {
        auto permission = g_AccessMngr->findPermission(i);

        for (auto j = 1; j <= MAX_PLAYERS; j++)
        {
            if (g_PlayerMngr.at(j)->findPermission(permission, false))
            {
                g_PlayerMngr.at(j)->removePermission(permission);
            }
        }

        for (auto i = 0; i <= MAX_PLAYERS; i++)
        {
            for (auto& [key, value] : g_TempMngr)
            {
                value.at(i).removePermission(permission);
            }
        }

        g_AccessMngr->destroyPermission(i);
    }

    MF_ExecuteForward(Fwd_PermissionClears);
    return lambda_Done;
}

// native lambda_handle:lx_player_add_group(iPlayer, pGroup);
static cell AMX_NATIVE_CALL PlayerAddGroup(AMX* amx, cell* params)
{
    enum args { arg_player = 1, arg_group = 2 };

    CHECK_PLAYER(params[arg_player]);

    auto& player = g_PlayerMngr.at(params[arg_player]);

    auto group = g_AccessMngr->findGroup(params[arg_group]);

    if (!group)
    {
        return lambda_InvalidGroup;
    }

    auto result = player->addGroup(group);

    if (result == lambda_Done)
    {
        MF_ExecuteForward(Fwd_PlayerAddedGroup, params[arg_player], group->getName().data(), params[arg_group]);
    }
    
    return result;
}

// native lx_player_find_group(iPlayer, pGroup);
static cell AMX_NATIVE_CALL PlayerFindGroup(AMX* amx, cell* params)
{
    enum args { arg_player = 1, arg_group = 2 };

    CHECK_PLAYER(params[arg_player]);

    auto& player = g_PlayerMngr.at(params[arg_player]);

    auto group = g_AccessMngr->findGroup(params[arg_group]);

    if (!group)
    {
        return lambda_InvalidGroup;
    }

    return player->findGroup(group);
}

// native lambda_handle:lx_player_remove_group(iPlayer, pGroup);
static cell AMX_NATIVE_CALL PlayerRemoveGroup(AMX* amx, cell* params)
{
    enum args { arg_player = 1, arg_group = 2 };

    CHECK_PLAYER(params[arg_player]);

    auto& player = g_PlayerMngr.at(params[arg_player]);

    auto group = g_AccessMngr->findGroup(params[arg_group]);

    if (!group)
    {
        return lambda_InvalidGroup;
    }

    auto result = player->removeGroup(group);

    if (result == lambda_Done)
    {
        MF_ExecuteForward(Fwd_PlayerRemovedGroup, params[arg_player], group->getName().data(), params[arg_group]);
    }

    return result;
}

// native bool:lx_player_clear_groups(iPlayer);
static cell AMX_NATIVE_CALL PlayerClearGroups(AMX* amx, cell* params)
{
    enum args { arg_player = 1 };

    CHECK_PLAYER(params[arg_player]);

    auto& player = g_PlayerMngr.at(params[arg_player]);

    auto result = player->clearGroups();

    if (result == lambda_Done)
    {
        MF_ExecuteForward(Fwd_PlayerClearedGroups, params[arg_player]);
    }

    return result;
}

// native lx_player_get_group(iPlayer, pos);
static cell AMX_NATIVE_CALL PlayerGetGroup(AMX* amx, cell* params)
{
    enum args { arg_player = 1, arg_pos = 2 };

    CHECK_PLAYER(params[arg_player]);

    auto& player = g_PlayerMngr.at(params[arg_player]);

    if (params[arg_pos] < 0 || params[arg_pos] > player->getGroupCount())
    {
        return lambda_InvalidGroup;
    }

    return g_AccessMngr->findGroup(player->getGroup(params[arg_pos]));
}

// native lx_player_get_group_count(iPlayer);
static cell AMX_NATIVE_CALL PlayerGetGroupCount(AMX* amx, cell* params)
{
    enum args { arg_player = 1 };

    CHECK_PLAYER(params[arg_player]);

    auto& player = g_PlayerMngr.at(params[arg_player]);

    return player->getGroupCount();
}

// native lambda_handle:lx_player_add_permission(iPlayer, pPermission);
static cell AMX_NATIVE_CALL PlayerAddPermission(AMX* amx, cell* params)
{
    enum args { arg_player = 1, arg_permission = 2 };

    CHECK_PLAYER(params[arg_player]);

    auto& player = g_PlayerMngr.at(params[arg_player]);

    auto permission = g_AccessMngr->findPermission(params[arg_permission]);

    if (!permission)
    {
        return lambda_InvalidPermission;
    }

    auto result = player->addPermission(permission);

    if (result == lambda_Done)
    {
        MF_ExecuteForward(Fwd_PlayerAddedPermission, params[arg_player], permission->data(), params[arg_permission]);
    }

    return result;
}

// native lx_player_find_permission(iPlayer, pPermission, bool:bGroup);
static cell AMX_NATIVE_CALL PlayerFindPermission(AMX* amx, cell* params)
{
    enum args { arg_player = 1, arg_permission = 2, arg_group };

    CHECK_PLAYER(params[arg_player]);

    auto& player = g_PlayerMngr.at(params[arg_player]);

    auto permission = g_AccessMngr->findPermission(params[arg_permission]);

    if (!permission)
    {
        return lambda_InvalidPermission;
    }

    return player->findPermission(permission, params[arg_group]);
}

// native lambda_handle:lx_player_remove_permission(iPlayer, pPermission);
static cell AMX_NATIVE_CALL PlayerRemovePermission(AMX* amx, cell* params)
{
    enum args { arg_player = 1, arg_permission = 2 };

    CHECK_PLAYER(params[arg_player]);

    auto& player = g_PlayerMngr.at(params[arg_player]);

    auto permission = g_AccessMngr->findPermission(params[arg_permission]);

    if (!permission)
    {
        return lambda_InvalidPermission;
    }

    auto result = player->removePermission(permission);

    if (result == lambda_Done)
    {
        MF_ExecuteForward(Fwd_PlayerRemovedPermission, params[arg_player], permission->data(), params[arg_permission]);
    }

    return result;
}

// native bool:lx_player_clear_permissions(iPlayer);
static cell AMX_NATIVE_CALL PlayerClearPermissions(AMX* amx, cell* params)
{
    enum args { arg_player = 1 };

    CHECK_PLAYER(params[arg_player]);

    auto& player = g_PlayerMngr.at(params[arg_player]);

    auto result = player->clearPermissions();

    if (result == lambda_Done)
    {
        MF_ExecuteForward(Fwd_PlayerClearedPermissions, params[arg_player]);
    }

    return result;
}

// native lx_player_get_permission(iPlayer, index);
static cell AMX_NATIVE_CALL PlayerGetPermission(AMX* amx, cell* params)
{
    enum args { arg_player = 1, arg_index = 2 };

    CHECK_PLAYER(params[arg_player]);

    auto& player = g_PlayerMngr.at(params[arg_player]);

    if (params[arg_index] < 0 || params[arg_index] > player->getPermissionCount(false))
    {
        return lambda_InvalidPermission;
    }

    return g_AccessMngr->findPermission(player->getPermission(params[arg_index]));
}

// native lx_player_get_permission_count(iPlayer, bool: bGroup);
static cell AMX_NATIVE_CALL PlayerGetPermissionCount(AMX* amx, cell* params)
{
    enum args { arg_player = 1, arg_group = 2 };

    CHECK_PLAYER(params[arg_player]);

    auto& player = g_PlayerMngr.at(params[arg_player]);

    return player->getPermissionCount(params[arg_group]);
}

// native lx_player_get_immunity(iPlayer);
static cell AMX_NATIVE_CALL PlayerGetImmunity(AMX* amx, cell* params)
{
    enum args { arg_player = 1 };

    CHECK_PLAYER(params[arg_player]);

    auto& player = g_PlayerMngr.at(params[arg_player]);

    return (cell)player->getImmunity();
}

// native lambda_handle:lx_player_set_immunity(iPlayer, iImmunity);
static cell AMX_NATIVE_CALL PlayerSetImmunity(AMX* amx, cell* params)
{
    enum args { arg_player = 1, arg_immunity = 2 };

    CHECK_PLAYER(params[arg_player]);

    auto& player = g_PlayerMngr.at(params[arg_player]);

    player->setImmunity(params[arg_immunity]);

    MF_ExecuteForward(Fwd_PlayerSetImmunity, params[arg_player], params[arg_immunity]);
    return lambda_Done;
}

// native lambda_handle:lx_player_get_prefix(iPlayer, szPrefix[], iLen);
static cell AMX_NATIVE_CALL PlayerGetPrefix(AMX* amx, cell* params)
{
    enum args { arg_player = 1, arg_string = 2, arg_len };

    CHECK_PLAYER(params[arg_player]);
    auto& player = g_PlayerMngr.at(params[arg_player]);
    auto prefix = player->getPrefix().data();

    return MF_SetAmxString(amx, params[arg_string], prefix, params[arg_len]);
}

// native lambda_handle:lx_player_set_prefix(iPlayer, szPrefix[]);
static cell AMX_NATIVE_CALL PlayerSetPrefix(AMX* amx, cell* params)
{
    enum args { arg_player = 1, arg_string = 2, arg_len };

    CHECK_PLAYER(params[arg_player]);
    auto& player = g_PlayerMngr.at(params[arg_player]);
    char* prefix = MF_GetAmxString(amx, params[arg_string], 0, nullptr);
    player->setPrefix(prefix);


    MF_ExecuteForward(Fwd_PlayerSetPrefix, params[arg_player], params[arg_string]);
    return lambda_Done;
}

// native lambda_handle:lx_temp_add_group(pGroup, index = 0);
static cell AMX_NATIVE_CALL TempAddGroup(AMX* amx, cell* params)
{
    enum args { arg_group = 1, arg_index = 2 };

    auto group = g_AccessMngr->findGroup(params[arg_group]);

    if (!group)
    {
        return lambda_InvalidGroup;
    }

    int id = params[arg_index];

    if (id < 0 || id > MAX_PLAYERS)
    {
        id = 0;
    }

    return g_TempMngr[amx].at(id).addGroup(group);
}

// native lambda_handle:lx_temp_remove_group(pGroup, index = 0);
static cell AMX_NATIVE_CALL TempRemoveGroup(AMX* amx, cell* params)
{
    enum args { arg_group = 1, arg_index = 2 };

    auto group = g_AccessMngr->findGroup(params[arg_group]);

    int id = params[arg_index];

    if (id < 0 || id > MAX_PLAYERS)
    {
        id = 0;
    }

    return g_TempMngr[amx].at(id).removeGroup(group);
}

// native lx_temp_get_group(pos, index = 0);
static cell AMX_NATIVE_CALL TempGetGroup(AMX* amx, cell* params)
{
    enum args { arg_pos = 1,  arg_index = 2 };

    int id = params[arg_index];

    if (id < 0 || id > MAX_PLAYERS)
    {
        id = 0;
    }

    if (params[arg_pos] < 0 || params[arg_pos] > g_TempMngr[amx].at(id).getGroupCount())
    {
        return lambda_InvalidGroup;
    }


    return g_AccessMngr->findGroup(g_TempMngr[amx].at(id).getGroup(params[arg_pos]));
}

// native lx_temp_get_group_count(index);
static cell AMX_NATIVE_CALL TempGetGroupCount(AMX* amx, cell* params)
{
    enum args { arg_index = 1 };

    int id = params[arg_index];

    if (id < 0 || id > MAX_PLAYERS)
    {
        id = 0;
    }

    return g_TempMngr[amx].at(id).getGroupCount();
}

// native lambda_handle:lx_temp_copy_groups(iPlayer, index = 0);
static cell AMX_NATIVE_CALL TempCopyGroups(AMX* amx, cell* params)
{
    enum args { arg_player = 1, arg_index = 2 };

    CHECK_PLAYER(params[arg_player]);
    auto& player = g_PlayerMngr.at(params[arg_player]);

    int id = params[arg_index];

    if (id < 0 || id > MAX_PLAYERS)
    {
        id = 0;
    }

    g_TempMngr[amx].at(id).copyGroups(player->getGroups());
    return lambda_Done;
}

// native lambda_handle:lx_temp_clear_groups(index = -1);
static cell AMX_NATIVE_CALL TempClearGroups(AMX* amx, cell* params)
{
    enum args { arg_index = 1 };

    int id = params[arg_index];

    if (id == -1)
    {
        for (auto i = 0; i <= MAX_PLAYERS; i++)
        {
            if (!g_TempMngr[amx].at(i).clearGroups())
            {
                return lambda_Error;
            }
        }

        return lambda_Done;
    }
    else
    if (id < 0 || id > MAX_PLAYERS)
    {
        id = 0;
    }

    return g_TempMngr[amx].at(id).clearGroups();
}

// native lambda_handle:lx_temp_add_permission(pPermission, index = 0);
static cell AMX_NATIVE_CALL TempAddPermission(AMX* amx, cell* params)
{
    enum args { arg_permission = 1, arg_index = 2 };

    auto permission = g_AccessMngr->findPermission(params[arg_permission]);

    if (!permission)
    {
        return lambda_InvalidPermission;
    }

    int id = params[arg_index];

    if (id < 0 || id > MAX_PLAYERS)
    {
        id = 0;
    }

    return g_TempMngr[amx].at(id).addPermission(permission);
}

// native lambda_handle:lx_temp_remove_permission(pPermission, index = 0);
static cell AMX_NATIVE_CALL TempRemovePermission(AMX* amx, cell* params)
{
    enum args { arg_permission = 1, arg_index = 2 };

    
    auto permission = g_AccessMngr->findPermission(params[arg_permission]);

    int id = params[arg_index];

    if (id < 0 || id > MAX_PLAYERS)
    {
        id = 0;
    }

    return g_TempMngr[amx].at(id).removePermission(permission);
}

// native lx_temp_get_permission(pos, index = 0);
static cell AMX_NATIVE_CALL TempGetPermission(AMX* amx, cell* params)
{
    enum args { arg_pos = 1, arg_index = 2 };

    int id = params[arg_index];

    if (id < 0 || id > MAX_PLAYERS)
    {
        id = 0;
    }

    if (params[arg_pos] < 0 || params[arg_pos] > g_TempMngr[amx].at(id).getPermissionCount())
    {
        return lambda_Error;
    }

    return g_AccessMngr->findPermission(g_TempMngr[amx].at(id).getPermission(params[arg_pos]));
}

// native lx_temp_get_permission_count(index);
static cell AMX_NATIVE_CALL TempGetPermissionCount(AMX* amx, cell* params)
{
    enum args { arg_index = 1 };

    int id = params[arg_index];

    if (id < 0 || id > MAX_PLAYERS)
    {
        id = 0;
    }

    return g_TempMngr[amx].at(id).getPermissionCount();
}

// native lambda_handle:lx_temp_copy_permissions(iPlayer, index = 0);
static cell AMX_NATIVE_CALL TempCopyPermissions(AMX* amx, cell* params)
{
    enum args { arg_player = 1, arg_index = 2};

    CHECK_PLAYER(params[arg_player]);
    auto& player = g_PlayerMngr.at(params[arg_player]);

    int id = params[arg_index];

    if (id < 0 || id > MAX_PLAYERS)
    {
        id = 0;
    }

    g_TempMngr[amx].at(id).copyPermissions(player->getPermissions());
    return lambda_Done;
}

// native bool: lx_temp_clear_permissions(index = -1);
static cell AMX_NATIVE_CALL TempClearPermissions(AMX* amx, cell* params)
{
    enum args { arg_index = 1 };

    int id = params[arg_index];

    if (id == -1)
    {
        for (auto i = 0; i <= MAX_PLAYERS; i++)
        {
            if (!g_TempMngr[amx].at(i).clearPermissions())
            {
                return lambda_Error;
            }
        }

        return lambda_Done;
    }
    else
    if (id < 0 || id > MAX_PLAYERS)
    {
        id = 0;
    }

    return g_TempMngr[amx].at(id).clearPermissions();
}

AMX_NATIVE_INFO nativeInfoLambda[] =
{
    {"lx_group_create",                 GroupCreate},
    {"lx_group_find",                   GroupFind},
    {"lx_group_get_name",               GroupGetName},
    {"lx_group_get_pointer",            GroupGetPtr},
    {"lx_group_size",                   GroupGetCount},
    {"lx_group_destroy",                GroupDestroy},
    {"lx_group_clears",                 GroupClears},
    {"lx_group_add_permission",         GroupAddPermission},
    {"lx_group_remove_permission",      GroupRemovePermission},
    {"lx_group_clear_permissions",      GroupClearPermissions},
    {"lx_group_find_permission",        GroupFindPermission},
    {"lx_group_get_permission_pointer", GroupGetPermissionPtr},
    {"lx_group_permission_count",       GroupPermissionCount},
    {"lx_group_get_immunity",           GroupGetImmunity},
    {"lx_group_set_immunity",           GroupSetImmunity},
    {"lx_group_get_prefix",             GroupGetPrefix},
    {"lx_group_set_prefix",             GroupSetPrefix},
    {"lx_permission_create",            PermissionCreate},
    {"lx_permission_find",              PermissionFind},
    {"lx_permission_get_name",          PermissionGetName},
    {"lx_permission_get_pointer",       PermissionGetPtr},
    {"lx_permission_size",              PermissionGetCount},
    {"lx_permission_destroy",           PermissionDestroy},
    {"lx_permission_clears",            PermissionClears},
    {"lx_player_add_group",             PlayerAddGroup},
    {"lx_player_find_group",            PlayerFindGroup},
    {"lx_player_remove_group",          PlayerRemoveGroup},
    {"lx_player_clear_groups",          PlayerClearGroups},
    {"lx_player_get_group",             PlayerGetGroup},
    {"lx_player_get_group_count",       PlayerGetGroupCount},
    {"lx_player_add_permission",        PlayerAddPermission},
    {"lx_player_find_permission",       PlayerFindPermission},
    {"lx_player_remove_permission",     PlayerRemovePermission},
    {"lx_player_clear_permissions",     PlayerClearPermissions},
    {"lx_player_get_permission",        PlayerGetPermission},
    {"lx_player_get_permission_count",  PlayerGetPermissionCount},
    {"lx_player_get_immunity",          PlayerGetImmunity},
    {"lx_player_set_immunity",          PlayerSetImmunity},
    {"lx_player_get_prefix",            PlayerGetPrefix},
    {"lx_player_set_prefix",            PlayerSetPrefix},
    {"lx_temp_add_group",               TempAddGroup},
    {"lx_temp_remove_group",            TempRemoveGroup},
    {"lx_temp_get_group",               TempGetGroup},
    {"lx_temp_get_group_count",         TempGetGroupCount},
    {"lx_temp_copy_groups",             TempCopyGroups},
    {"lx_temp_clear_groups",            TempClearGroups},
    {"lx_temp_add_permission",          TempAddPermission},
    {"lx_temp_remove_permission",       TempRemovePermission},
    {"lx_temp_get_permission",          TempGetPermission},
    {"lx_temp_get_permission_count",    TempGetPermissionCount},
    {"lx_temp_copy_permissions",        TempCopyPermissions},
    {"lx_temp_clear_permissions",       TempClearPermissions},
    {nullptr,                           nullptr}
};

void OnAmxxAttach()
{
    MF_AddNatives(nativeInfoLambda);
    CreateModule();
}

void OnAmxxDetach()
{
    DestroyModule();
}

void OnPluginsLoaded()
{
    // forward lx_on_group_created(szGroup[], pGroup);
    Fwd_GroupCreated                = MF_RegisterForward("lx_on_group_created", ET_IGNORE, FP_STRING, FP_CELL, FP_DONE);
    // forward lx_on_group_destroyed(szGroup[], pGroup);
    Fwd_GroupDestroyed              = MF_RegisterForward("lx_on_group_destroyed", ET_IGNORE, FP_STRING, FP_CELL, FP_DONE);
    // forward lx_on_group_clears();
    Fwd_GroupClears                 = MF_RegisterForward("lx_on_group_cleared", ET_IGNORE, FP_DONE);
    // forward lx_on_group_removed_permission(szGroup[], pGroup, szPermission[], pPermission);
    Fwd_GroupRemovedPermission      = MF_RegisterForward("lx_on_group_removed_permission", ET_IGNORE, FP_STRING, FP_CELL, FP_STRING, FP_CELL, FP_DONE);
    // forward lx_on_group_added_permission(szGroup[], pGroup, szPermission[], pPermission);
    Fwd_GroupAddedPermission        = MF_RegisterForward("lx_on_group_added_permission", ET_IGNORE, FP_STRING, FP_CELL, FP_STRING, FP_CELL, FP_DONE);
    // forward lx_on_group_cleared_permission(szGroup[], pGroup);
    Fwd_GroupClearedPermissions     = MF_RegisterForward("lx_on_group_cleared_permission", ET_IGNORE, FP_STRING, FP_CELL, FP_DONE);
    // forward lx_on_group_set_immunity(szGroup[], pGroup, immunity);
    Fwd_GroupSetImmunity            = MF_RegisterForward("lx_on_group_set_immunity", ET_IGNORE, FP_STRING, FP_CELL, FP_CELL, FP_DONE);
    // forward lx_on_group_set_prefix(szGroup[], pGroup, szPrefix[]);
    Fwd_GroupSetPrefix              = MF_RegisterForward("lx_on_group_set_prefix", ET_IGNORE, FP_STRING, FP_CELL, FP_STRING, FP_DONE);
    // forward lx_on_permission_created(szPermission[], pPermission);
    Fwd_PermissionCreated           = MF_RegisterForward("lx_on_permission_created", ET_IGNORE, FP_STRING, FP_CELL, FP_DONE);
    // forward lx_on_permission_destroyed(szPermission[], pPermission);
    Fwd_PermissionDestroyed         = MF_RegisterForward("lx_on_permission_destroyed", ET_IGNORE, FP_STRING, FP_CELL, FP_DONE);
    // forward lx_on_permission_clears();
    Fwd_PermissionClears            = MF_RegisterForward("lx_on_permission_cleared", ET_STOP, FP_DONE);
    // forward lx_on_player_added_group(iPlayer, szGroup[], pGroup);
    Fwd_PlayerAddedGroup            = MF_RegisterForward("lx_on_player_added_group", ET_IGNORE, FP_CELL, FP_STRING, FP_CELL, FP_DONE);
    // forward lx_on_player_removed_group(iPlayer, szGroup[], pGroup);
    Fwd_PlayerRemovedGroup          = MF_RegisterForward("lx_on_player_removed_group", ET_IGNORE, FP_CELL, FP_STRING, FP_CELL, FP_DONE);
    // forward lx_on_player_cleared_groups(iPlayer);
    Fwd_PlayerClearedGroups         = MF_RegisterForward("lx_on_player_cleared_groups", ET_IGNORE, FP_CELL, FP_DONE);
    // forward lx_on_player_added_permission(iPlayer, szPermission[], pPermission);
    Fwd_PlayerAddedPermission       = MF_RegisterForward("lx_on_player_added_permission", ET_IGNORE, FP_CELL, FP_STRING, FP_CELL, FP_DONE);
    // forward lx_on_player_removed_permission(iPlayer, szPermission[], pPermission);
    Fwd_PlayerRemovedPermission     = MF_RegisterForward("lx_on_player_removed_permission", ET_IGNORE, FP_CELL, FP_STRING, FP_CELL, FP_DONE);
    // forward lx_on_player_cleared_permissions(iPlayer);
    Fwd_PlayerClearedPermissions    = MF_RegisterForward("lx_on_player_cleared_permissions", ET_IGNORE, FP_CELL, FP_DONE);
    // forward lx_on_player_set_immunity(iPlayer, immunity);
    Fwd_PlayerSetImmunity           = MF_RegisterForward("lx_on_player_set_immunity", ET_IGNORE, FP_CELL, FP_CELL, FP_DONE);
    // forward lx_on_player_set_prefix(iPlayer, szPrefix[]);
    Fwd_PlayerSetPrefix             = MF_RegisterForward("lx_on_player_set_prefix", ET_IGNORE, FP_CELL, FP_STRING, FP_DONE);
}