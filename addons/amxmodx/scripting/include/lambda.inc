#if defined _lambda_included
	#endinput
#endif

#define _lambda_included

#pragma reqlib lambda
#if !defined AMXMODX_NOAUTOLOAD
	#pragma loadlib lambda
#endif

enum lambda_handle
{
    lambda_Error = -4,				// Error
    lambda_InvalidPermission = -3,	// Invalid Permission
    lambda_InvalidGroup = -2,		// Invaild Group
    lambda_Ready, 					// It already exists
    lambda_Done 					// Done
};

/*---------------------------------------------------------------------------------------------------*/
//                                              Native                                               //
/*---------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------*/
//                                              Groups                                               //
/*---------------------------------------------------------------------------------------------------*/

/**
 * Creates a group.
 * 
 * @param szGroup   The name of the group.
 * 
 * @return          A pointer to the group.
 * 
 * @note 			If the group exists, it will return a pointer to it.
 */
native lx_group_create(szGroup[]);

/**
 * Checks if the group exists.
 * 
 * @param szGroup   The name of the group.
 * 
 * @return          Returns a pointer to the group, 0 if the group is not found.
 */
native lx_group_find(szGroup[]);

/**
 * Gets the name of the group.
 * 
 * @param pGroup    A pointer to the group.
 * @param szGroup   An array for the received name.
 * @param iLen      The size of the array.
 * 
 * @return          Number of characters, lambda_handle
 */
native lx_group_get_name(pGroup, szGroup[], iLen);

/**
 * Gets the count of groups.
 * 
 * @return          int
 */
native lx_group_size();

/**
 * Deletes the group.
 * 
 * @param pGroup    A pointer to the group.
 * 
 * @return          lambda_handle
 */
native lambda_handle:lx_group_destroy(pGroup);

/**
 * Clears groups.
 * 
 * @return          lamnda_handle
 */
native lambda_handle:lx_group_clears();

/**
 * Adds a permission to the group.
 * 
 * @param pGroup        A pointer to the group.
 * @param pPermission   A pointer to the permission.
 * 
 * @return              lambda_handle
 */
native lambda_handle:lx_group_add_permission(pGroup, pPermission);

/**
 * Removes a permission from the group.
 * 
 * @param pGroup        A pointer to the group.
 * @param pPermission   A pointer to the permission.
 * 
 * @return              lambda_handle
 */
native lambda_handle:lx_group_remove_permission(pGroup, pPermission);

/**
 * Removes all permissions from the group.
 * 
 * @param pGroup        A pointer to the group.
 * 
 * @return              lambda_handle
 */
native lambda_handle:lx_group_clear_permissions(pGroup);

/**
 * Checks for permissions in the group.
 * 
 * @param pGroup        A pointer to the group.
 * @param pPermission   A pointer to the permission.
 * 
 * @return              1/0, lambda_handle
 */
native lx_group_find_permission(pGroup, pPermission);

/**
 * Gets the count of permissions in the group.
 * 
 * @param pGroup        A pointer to the group.
 * 
 * @return              Count of permissions, lambda_handle
 */
native lx_group_permission_count(pGroup);

/**
 * Gets the group's immunity.
 * 
 * @param pGroup		A pointer to the group.
 * 
 * @return 				immunity, lambda_handle
 */
native lx_group_get_immunity(pGroup);

/**
 * Set the group's immunity.
 * 
 * @param pGroup		A pointer to the group.
 * @param immunity      Immunity.
 * 
 * @return 				lambda_handle
 */
native lambda_handle:lx_group_set_immunity(pGroup, immunity);

/**
 * Gets the group's prefix.
 * 
 * @param pGroup		A pointer to the group.
 * @param szPrefix      An array for the received prefix.
 * @param iLen          The size of the array.
 * 
 * @return 				lambda_handle
 */
native lambda_handle:lx_group_get_prefix(pGroup, szPrefix[], iLen);

/**
 * Set the group's prefix.
 * 
 * @param pGroup		A pointer to the group.
 * @param szPrefix      The prefix for the groups.
 * 
 * @return 				lambda_handle
 */
native lambda_handle:lx_group_set_prefix(pGroup, szPrefix[]);

/*---------------------------------------------------------------------------------------------------*/
//                                            Permission                                             //
/*---------------------------------------------------------------------------------------------------*/

/**
 * Creates a permission.
 * 
 * @param szPermission   The name of the permission.
 * 
 * @return               A pointer to the permission.
 * 
 * @note				 Returns a pointer to the permission, 0 if no permission is found.
 */
native lx_permission_create(szPermission[]);

/**
 * Checks if the permission exists.
 * 
 * @param szPermission  The name of the permission.
 * 
 * @return              Returns a pointer to the permission, 0 if no permission is found.
 */
native lx_permission_find(szPermission[]);

/**
 * Get the name of the permission.
 * 
 * @param pPermission    A pointer to the permission.
 * @param szPermission   An array for the received name.
 * @param iLen           The size of the array.
 * 
 * @return              Number of characters, lambda_handle
 */
native lx_permission_get_name(pPermission, szPermission[], iLen);

/**
 * Gets the count of permissions.
 * 
 * @return          int
 */
native lx_permission_size();

/**
 * Deletes the permission.
 * 
 * @param pPermission   A pointer to the permission.
 * 
 * @return              lambda_handle
 */
native lambda_handle:lx_permission_destroy(pPermission);

/**
 * Clears permission.
 * 
 * @return              lambda_handle
 */
native lambda_handle:lx_permission_clears();

/*---------------------------------------------------------------------------------------------------*/
//                                             Players                                               //
/*---------------------------------------------------------------------------------------------------*/

/**
 * Adds a group to the player.
 * 
 * @param iPlayer       The player's index.
 * @param pGroup        A pointer to the group.
 * 
 * @return              lambda_handle
 */
native lambda_handle:lx_player_add_group(iPlayer, pGroup);

/**
 * Checks if the player has a group.
 *
 * @param iPlayer       The player's index.
 * @param pGroup        A pointer to the group.
 * 
 * @return              True/false
 */
native lx_player_find_group(iPlayer, pGroup);

/**
 * Deletes a group for a player.
 * 
 * @param iPlayer       The player's index.
 * @param pGroup        A pointer to the group.
 * 
 * @return              lambda_handle
 */
native lambda_handle:lx_player_remove_group(iPlayer, pGroup);

/**
 * Deletes groups for the player.
 *
 * @param iPlayer       The player's index.
 * 
 * @return              True/false
 */
native bool:lx_player_clear_groups(iPlayer);

/**
 * Gets the count of groups from the player.
 *
 * @param iPlayer       The player's index.
 * 
 * @return              Count of groups.
 */
native lx_player_get_group_count(iPlayer);

/**
 * Adds permission to the player.
 * 
 * @param iPlayer       The player's index.
 * @param pPermission   A pointer to the permission.
 * 
 * @return              lambda_handle
 */
native lambda_handle:lx_player_add_permission(iPlayer, pPermission);

/**
 * Checks if the player has permission.
 *
 * @param iPlayer       The player's index.
 * @param pPermission   A pointer to the permission.
 * @param bGroup        Should permissions be taken into account in groups.
 * 
 * @return              True/false, lambda_handle
 */
native lx_player_find_permission(iPlayer, pPermission, bool: bGroup);

/**
 * Removes the permission to the player.
 * 
 * @param iPlayer       The player's index.
 * @param pPermission   A pointer to the permission.
 * 
 * @return              lambda_handle
 */
native lambda_handle:lx_player_remove_permission(iPlayer, pPermission);

/**
 * Removes permissions for the player.
 *
 * @param iPlayer       The player's index.
 * 
 * @return              True/false.
 */
native bool:lx_player_clear_permissions(iPlayer);

/**
 * Gets the number of permissions from the player.
 *
 * @param iPlayer       The player's index.
 * @param bGroup        Should permissions be taken into account in groups.
 * 
 * @return              Count of permissions.
 */
native lx_player_get_permission_count(iPlayer, bool: bGroup);

/**
 * Gets the player's immunity.
 * 
 * @param iPlayer		The player's index.
 * 
 * @return				Gets the player's immunity.
 */
native lx_player_get_immunity(iPlayer);

/**
 * Sets immunity to the player.
 * 
 * @param iPlayer		The player's index.
 * @param immunity		Immunity.
 * 
 * @return				lambda_handle
 */
native lambda_handle:lx_player_set_immunity(iPlayer, immunity);

/**
 * Gets the player's prefix.
 * 
 * @param iPlayer		The player's index.
 * @param szPrefix      An array for the received prefix.
 * @param iLen          The size of the array.
 * 
 * @return 				lambda_handle
 */
native lambda_handle:lx_player_get_prefix(iPlayer, szPrefix[], iLen);

/**
 * Sets the player's prefix.
 * 
 * @param iPlayer		The player's index.
 * @param szPrefix      The prefix for the player.
 * 
 * @return 				lambda_handle
 */
native lambda_handle:lx_player_set_prefix(iPlayer, szPrefix[]);

/*---------------------------------------------------------------------------------------------------*/
//                                              Temp                                                 //
/*---------------------------------------------------------------------------------------------------*/

/**
 * Adds a group to a temp array. 
 * 
 * @param pGroup        A pointer to the group.
 * 
 * @return              lambda_handle
 */
native lambda_handle:lx_temp_add_group(pGroup);

/**
 * Removes the group to a temp array. 
 * 
 * @param pGroup        A pointer to the group.
 * 
 * @return              lambda_handle
 */
native lambda_handle:lx_temp_remove_group(pGroup);

/**
 * Adds groups to the player.
 * 
 * @param iPlayer		The player's index.
 * 
 * @return				lambda_handle (lamda_Done)
 */
native lambda_handle:lx_temp_copy_groups(iPlayer);

/**
 * Removes groups in temp array.
 * 
 * @return              lambda_handle
 */
native bool:lx_temp_clear_groups();

/**
 * Adds a permission to a temp array. 
 * 
 * @param pPermission   A pointer to the permission.
 * 
 * @return              lambda_handle
 */
native lambda_handle:lx_temp_add_permission(pPermission);

/**
 * Removes the permissions to a temp array. 
 * 
 * @param pPermission   A pointer to the permission.
 * 
 * @return              lambda_handle
 */
native lambda_handle:lx_temp_remove_permission(pPermission);

/**
 * Adds permissions to the player.
 * 
 * @param iPlayer		The player's index.
 * 
 * @return				lambda_handle (lamda_Done)
 */
native lambda_handle:lx_temp_copy_permissions(iPlayer);

/**
 * Removes permissions in temp array.
 * 
 * @return              lambda_handle
 */
native bool: lx_temp_clear_permissions();
/*---------------------------------------------------------------------------------------------------*/
//                                             Forwards                                              //
/*---------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------*/
//                                              Groups                                               //
/*---------------------------------------------------------------------------------------------------*/

/**
 * Called after the group is created.
 * 
 * @param szGroup   The name of the group.
 * @param pGroup    A pointer to the group.
 * 
 * @noreturn
 */
forward lx_on_group_created(szGroup[], pGroup);

/**
 * Called after destroyed a group.
 * 
 * @param szGroup   The name of the group.
 * @param pGroup    A pointer to the group.
 * 
 * @noreturn
 */
forward lx_on_group_destroyed(szGroup[], pGroup);

/**
 * Called after groups are cleared.
 * 
 * @noreturn
 */
forward lx_on_group_cleared();

/**
 * Called after removing a permission from a group.
 * 
 * @param szGroup           The name of the group.
 * @param pGroup            A pointer to the group.
 * @param szPermission      The name of the permission.
 * @param pPermission       A pointer to the permission.
 * 
 * @noreturn
 */
forward lx_on_group_removed_permission(szGroup[], pGroup, szPermission[], pPermission);

/**
 * It is called after adding a permission to a group.
 * 
 * @param szGroup           The name of the group.
 * @param pGroup            A pointer to the group.
 * @param szPermission      The name of the permission.
 * @param pPermission       A pointer to the permission.
 * 
 * @noreturn
 */
forward lx_on_group_added_permission(szGroup[], pGroup, szPermission[], pPermission);

/**
 * It is called when all group permissions are cleared.
 * 
 * @param szGroup           The name of the group.
 * @param pGroup            A pointer to the group.
 * 
 * @noreturn
 */
forward lx_on_group_cleared_permission(szGroup[], pGroup);

/**
 * It is called after the group's immunity is set.
 * 
 * @param szGroup           The name of the group.
 * @param pGroup            A pointer to the group.
 * @param immunity          Immunity.
 * 
 * @noreturn
 */
forward lx_on_group_set_immunity(szGroup[], pGroup, immunity);

/**
 * It is called after the group's prefix is set.
 * 
 * @param szGroup           The name of the group.
 * @param pGroup            A pointer to the group.
 * @param szPrefix          An array for the received prefix.
 * 
 * @noreturn
 */
forward lx_on_group_set_prefix(szGroup[], pGroup, szPrefix[]);

/*---------------------------------------------------------------------------------------------------*/
//                                            Permission                                             //
/*---------------------------------------------------------------------------------------------------*/

/**
 * Called after creating a permission.
 * 
 * @param szPermission   The name of the permission.
 * @param pPermission    A pointer to the permission.
 * 
 * @noreturn
 */
forward lx_on_permission_created(szPermission[], pPermission);

/**
 * Called after deleting the permission.
 * 
 * @param szPermission   The name of the permission.
 * @param pPermission    A pointer to the permission.
 * 
 * @noreturn
 */
forward lx_on_permission_destroyed(szPermission[], pPermission);

/**
 * Called after permissions are cleared.
 * 
 * @noreturn 
 */
forward lx_on_permission_cleared();

/*---------------------------------------------------------------------------------------------------*/
//                                             Players                                               //
/*---------------------------------------------------------------------------------------------------*/

/**
 * It is called after adding a group to a player.
 * 
 * @param iPlayer   The player's index.
 * @param szGroup   The name of the permission.
 * @param pGroup    A pointer to the permission.
 * 
 * @noreturn
 */
forward lx_on_player_added_group(iPlayer, szGroup[], pGroup);

/**
 * It is called after the player's group is removed.
 * 
 * @param iPlayer   The player's index.
 * @param szGroup   The name of the permission.
 * @param pGroup    A pointer to the permission.
 * 
 * @noreturn
 */
forward lx_on_player_removed_group(iPlayer, szGroup[], pGroup);

/**
 * It is called after all groups are cleared from the player.
 * 
 * @param iPlayer   The player's index.
 * @param szGroup   The name of the permission.
 * @param pGroup    A pointer to the permission.
 * 
 * @noreturn
 */
forward lx_on_player_cleared_groups(iPlayer);

/**
 * It is called after adding a permission to the player.
 * 
 * @param iPlayer        The player's index.
 * @param szPermission   The name of the permission.
 * @param pPermission    A pointer to the permission.
 * 
 * @noreturn
 */
forward lx_on_player_added_permission(iPlayer, szPermission[], pPermission);

/**
 * It is called after the player's permission is removed.
 * 
 * @param iPlayer        The player's index.
 * @param szPermission   The name of the permission.
 * @param pPermission    A pointer to the permission.
 * 
 * @noreturn
 */
forward lx_on_player_removed_permission(iPlayer, szPermission[], pPermission);

/**
 * It is called after all player permissions are removed.
 * 
 * @param iPlayer        The player's index.
 * 
 * @noreturn
 */
forward lx_on_player_cleared_permissions(iPlayer);

/**
 * It is called after the player's immunity is set.
 * 
 * @param iPlayer       The player's index.
 * @param immunity      Immunity.
 * 
 * @noreturn
 */
forward lx_on_player_set_immunity(iPlayer, immunity);

/**
 * It is called after the player's immunity is prefix.
 * 
 * @param iPlayer       The player's index.
 * @param szPrefix      An array for the received prefix.
 * 
 * @noreturn
 */
forward lx_on_player_set_prefix(iPlayer, szPrefix[]);

/*--------------END--------------END--------------END--------------END--------------END--------------*/