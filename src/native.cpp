#include "sdk/amxxmodule.h"
#include "lambda.h"

std::unique_ptr<AccessMngr> g_AccessMngr;
std::array<std::unique_ptr<PlayerMngr>, MAX_PLAYERS + 1> g_PlayerMngr;

int Fwd_GroupCreated;
int	Fwd_GroupDestroyed;
int	Fwd_GroupAddedPermission;
int	Fwd_GroupRemovedPermission;
int	Fwd_GroupClearedPermissions;
int	Fwd_GroupSetImmunity;
int	Fwd_PermissionCreated;
int	Fwd_PermissionDestroyed;
int	Fwd_PlayerAddedGroup;
int	Fwd_PlayerRemovedGroup;
int	Fwd_PlayerClearedGroups;
int	Fwd_PlayerAddedPermission;
int	Fwd_PlayerRemovedPermission;
int	Fwd_PlayerClearedPermissions;
int	Fwd_PlayerSetImmunity;

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

// native GroupCreate(szGroup[]);
static cell AMX_NATIVE_CALL GroupCreate(AMX* amx, cell* params)
{
    enum args { arg_group = 1 };
    
    char* name = MF_GetAmxString(amx, params[arg_group], 0, nullptr);

    auto ptr = (cell)g_AccessMngr->createGroup(name);
    MF_ExecuteForward(Fwd_GroupCreated, name, ptr);
    return ptr;
}

// native GroupFind(szGroup[]);
static cell AMX_NATIVE_CALL GroupFind(AMX* amx, cell* params)
{
    enum args { arg_group = 1 };

    char* name = MF_GetAmxString(amx, params[arg_group], 0, nullptr);

    return (cell)g_AccessMngr->findGroup(name);
}

// native GroupGetName(pGroup, szGroup[], iLen);
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

// native GroupGetCount();
static cell AMX_NATIVE_CALL GroupGetCount(AMX* amx, cell* params)
{
    return g_AccessMngr->getGroupCount();
}

// native lambda_handle:GroupDestroy(pGroup);
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
       if (g_PlayerMngr.at(i)->findGroup(group))
       {
           g_PlayerMngr.at(i)->removeGroup(group);
       }
    }

  
    g_AccessMngr->destroyGroup(params[arg_group]);
    MF_ExecuteForward(Fwd_GroupDestroyed, group->getName().data(), params[arg_group]);

    return lambda_Done;
}

// native lambda_handle:GroupAddPermission(pGroup, pPermission);
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

// native lambda_handle:GroupRemovePermission(pGroup, pPermission);
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

// native lambda_handle:GroupClearPermissions(pGroup);
static cell AMX_NATIVE_CALL GroupClearPermissions(AMX* amx, cell* params)
{
    enum args { arg_group = 1 };

    auto group = g_AccessMngr->findGroup(params[arg_group]);

    if (!group)
    {
        return lambda_InvalidGroup;
    }

    auto result = group->clearPermissions();

    if (result)
    {
        MF_ExecuteForward(Fwd_GroupClearedPermissions, group->getName().data(), params[arg_group]);
        return result;
    }

    return lambda_Error;
}

// native GroupFindPermission(pGroup, pPermission);
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

// native GroupPermissionCount(pGroup);
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

// native GroupGetImmunity(pGroup);
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

// native lambda_handle:GroupSetImmunity(pGroup, iImmunity);
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

// native PermissionCreate(szPermission[]);
static cell AMX_NATIVE_CALL PermissionCreate(AMX* amx, cell* params)
{
    enum args { arg_permission = 1 };

    char* name = MF_GetAmxString(amx, params[arg_permission], 0, nullptr);

    auto ptr = (cell)g_AccessMngr->createPermission(name);
    MF_ExecuteForward(Fwd_PermissionCreated, name, ptr);
    return ptr;
}

// native PermissionFind(szPermission[]);
static cell AMX_NATIVE_CALL PermissionFind(AMX* amx, cell* params)
{
    enum args { arg_permission = 1 };

    char* name = MF_GetAmxString(amx, params[arg_permission], 0, nullptr);

    return (cell)g_AccessMngr->findPermission(name);
}

// native PermissionGetName(pPermission, szPermission[], iLen);
static cell AMX_NATIVE_CALL PermissionGetName(AMX* amx, cell* params)
{
    enum args { arg_permission = 1, arg_string = 2, arg_len };

    auto permission = g_AccessMngr->findPermission(params[arg_permission]);

    if (!permission)
    {
        return lambda_InvalidGroup;
    }

    auto name = permission->data();

    return MF_SetAmxString(amx, params[arg_string], name, params[arg_len]);
}

// native PermissionGetCount();
static cell AMX_NATIVE_CALL PermissionGetCount(AMX* amx, cell* params)
{
    return g_AccessMngr->getPermissionCount();
}

// native lambda_handle:PermissionDestroy(pPermission);
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
        if (g_PlayerMngr.at(i)->findPermission(permission, false))
        {
            g_PlayerMngr.at(i)->removePermission(permission);
        }
    }

    g_AccessMngr->destroyPermission(params[arg_permission]);

    MF_ExecuteForward(Fwd_PermissionDestroyed, permission->data(), params[arg_permission]);
    return lambda_Done;
}

// native lambda_handle:PlayerAddGroup(iPlayer, pGroup);
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

// native PlayerFindGroup(iPlayer, pGroup);
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

// native lambda_handle:PlayerRemoveGroup(iPlayer, pGroup);
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

// native bool:PlayerClearGroups(iPlayer);
static cell AMX_NATIVE_CALL PlayerClearGroups(AMX* amx, cell* params)
{
    enum args { arg_player = 1 };

    CHECK_PLAYER(params[arg_player]);

    auto& player = g_PlayerMngr.at(params[arg_player]);

    auto result = player->clearGroups();

    if (result)
    {
        MF_ExecuteForward(Fwd_PlayerClearedGroups, params[arg_player]);
    }

    return result;
}

// native PlayerGetGroupCount(iPlayer);
static cell AMX_NATIVE_CALL PlayerGetGroupCount(AMX* amx, cell* params)
{
    enum args { arg_player = 1 };

    CHECK_PLAYER(params[arg_player]);

    auto& player = g_PlayerMngr.at(params[arg_player]);

    return player->getGroupCount();
}

// native lambda_handle:PlayerAddPermission(iPlayer, pPermission);
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

// native PlayerFindPermission(iPlayer, pPermission, bool:bGroup);
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

// native lambda_handle:PlayerRemovePermission(iPlayer, pPermission);
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

// native bool:PlayerClearPermissions(iPlayer);
static cell AMX_NATIVE_CALL PlayerClearPermissions(AMX* amx, cell* params)
{
    enum args { arg_player = 1 };

    CHECK_PLAYER(params[arg_player]);

    auto& player = g_PlayerMngr.at(params[arg_player]);

    auto result = player->clearPermissions();

    if (result)
    {
        MF_ExecuteForward(Fwd_PlayerClearedPermissions, params[arg_player]);
    }

    return result;
}

// native bool:PlayerGetPermissionCount(iPlayer, bool: bGroup);
static cell AMX_NATIVE_CALL PlayerGetPermissionCount(AMX* amx, cell* params)
{
    enum args { arg_player = 1, arg_group = 2 };

    CHECK_PLAYER(params[arg_player]);

    auto& player = g_PlayerMngr.at(params[arg_player]);

    return player->getPermissionCount(params[arg_group]);
}

// native PlayerGetImmunity(iPlayer);
static cell AMX_NATIVE_CALL PlayerGetImmunity(AMX* amx, cell* params)
{
    enum args { arg_player = 1 };

    CHECK_PLAYER(params[arg_player]);

    auto& player = g_PlayerMngr.at(params[arg_player]);

    return (cell)player->getImmunity();
}

// native lambda_handle:PlayerSetImmunity(iPlayer, iImmunity);
static cell AMX_NATIVE_CALL PlayerSetImmunity(AMX* amx, cell* params)
{
    enum args { arg_player = 1, arg_immunity = 2 };

    CHECK_PLAYER(params[arg_player]);

    auto& player = g_PlayerMngr.at(params[arg_player]);

    player->setImmunity(params[arg_immunity]);

    MF_ExecuteForward(Fwd_PlayerSetImmunity, params[arg_player], params[arg_immunity]);
    return lambda_Done;
}

AMX_NATIVE_INFO nativeInfoLambda[] =
{
    {"lx_group_create",                 GroupCreate},
    {"lx_group_find",                   GroupFind},
    {"lx_group_get_name",               GroupGetName},
    {"lx_group_size",                   GroupGetCount},
    {"lx_group_destroy",                GroupDestroy},
    {"lx_group_add_permission",         GroupAddPermission},
    {"lx_group_remove_permission",      GroupRemovePermission},
    {"lx_group_clear_permissions",      GroupClearPermissions},
    {"lx_group_find_permission",        GroupFindPermission},
    {"lx_group_permission_count",       GroupPermissionCount},
    {"lx_group_get_immunity",           GroupGetImmunity},
    {"lx_group_set_immunity",           GroupSetImmunity},
    {"lx_permission_create",            PermissionCreate},
    {"lx_permission_find",              PermissionFind},
    {"lx_permission_get_name",          PermissionGetName},
    {"lx_permission_destroy",           PermissionDestroy},
    {"lx_permission_size",              PermissionGetCount},
    {"lx_player_add_group",             PlayerAddGroup},
    {"lx_player_find_group",            PlayerFindGroup},
    {"lx_player_remove_group",          PlayerRemoveGroup},
    {"lx_player_clear_groups",          PlayerClearGroups},
    {"lx_player_get_group_count",       PlayerGetGroupCount},
    {"lx_player_add_permission",        PlayerAddPermission},
    {"lx_player_find_permission",       PlayerFindPermission},
    {"lx_player_remove_permission",     PlayerRemovePermission},
    {"lx_player_clear_permissions",     PlayerClearPermissions},
    {"lx_player_get_permission_count",  PlayerGetPermissionCount},
    {"lx_player_get_immunity",          PlayerGetImmunity},
    {"lx_player_set_immunity",          PlayerSetImmunity},
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
    // forward lx_on_group_removed_permission(szGroup[], pGroup, szPermission[], pPermission);
    Fwd_GroupRemovedPermission      = MF_RegisterForward("lx_on_group_removed_permission", ET_IGNORE, FP_STRING, FP_CELL, FP_STRING, FP_CELL, FP_DONE);
    // forward lx_on_group_added_permission(szGroup[], pGroup, szPermission[], pPermission);
    Fwd_GroupAddedPermission        = MF_RegisterForward("lx_on_group_added_permission", ET_IGNORE, FP_STRING, FP_CELL, FP_STRING, FP_CELL, FP_DONE);
    // forward lx_on_group_cleared_permission(szGroup[], pGroup);
    Fwd_GroupClearedPermissions     = MF_RegisterForward("lx_on_group_cleared_permission", ET_IGNORE, FP_STRING, FP_CELL, FP_DONE);
    // forward lx_on_group_set_immunity(szGroup[], pGroup, immunity);
    Fwd_GroupSetImmunity            = MF_RegisterForward("lx_on_group_set_immunity", ET_IGNORE, FP_STRING, FP_CELL, FP_CELL, FP_DONE);
    // forward lx_on_permission_created(szPermission[], pPermission);
    Fwd_PermissionCreated           = MF_RegisterForward("lx_on_permission_created", ET_IGNORE, FP_STRING, FP_CELL, FP_DONE);
    // forward lx_on_permission_destroyed(szPermission[], pPermission);
    Fwd_PermissionDestroyed         = MF_RegisterForward("lx_on_permission_destroyed", ET_IGNORE, FP_STRING, FP_CELL, FP_DONE);
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
}