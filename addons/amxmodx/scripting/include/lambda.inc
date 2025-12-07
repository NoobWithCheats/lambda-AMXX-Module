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
    lambda_InvalidPlayer = -3,      // Invalid Player
    lambda_InvalidPermission = -2,	// Invalid Permission
    lambda_InvalidGroup = -1,		// Invaild Group
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
 * @param szGroup       The name of the group.
 * 
 * @return              A pointer to the group.
 * 
 * @note 			    If the group exists, it will return a pointer to it.
 */
native lx_group_create(szGroup[]);

/**
 * Checks if the group exists.
 * 
 * @param szGroup       The name of the group.
 * 
 * @return              Returns a pointer to the group, 0 if the group is not found.
 */
native lx_group_find(szGroup[]);

/**
 * Gets the name of the group.
 * 
 * @param pGroup        A pointer to the group.
 * @param szGroup       An array for the received name.
 * @param iLen          The size of the array.
 * 
 * @return              Number of characters
 * @return              lambda_InvalidGroup - the group does not exist
 */
native lx_group_get_name(pGroup, szGroup[], iLen);

/**
 * Gets the group pointer by its sequence number.
 * 
 * @param pos           sequence number 0 to lx_group_size - 1
 * 
 * @return              A pointer to the group.
 * @return              lambda_InvalidGroup - invalid position (going abroad)
 * @return              lambda_Error - system error (please report it)
 */
native lx_group_get_pointer(pos);

/**
 * Gets the count of groups.
 * 
 * @return              int
 */
native lx_group_size();

/**
 * Deletes the group.
 * 
 * @param pGroup        A pointer to the group.
 * 
 * @return              lambda_InvalidGroup - the group does not exist
 * @return              lambda_Done - the group was successfully deleted
 */
native lambda_handle:lx_group_destroy(pGroup);

/**
 * Clears groups.
 * 
 * @noreturn
 */
native lx_group_clears();

/**
 * Adds a permission to the group.
 * 
 * @param pGroup        A pointer to the group.
 * @param pPermission   A pointer to the permission.
 * 
 * @return              lambda_InvalidGroup - the group does not exist
 * @return              lambda_InvalidPermission - the permission does not exist
 * @return              lambda_Done - permission added
 * @return              lambda_Ready - the permission has already been added before
 */
native lambda_handle:lx_group_add_permission(pGroup, pPermission);

/**
 * Removes a permission from the group.
 * 
 * @param pGroup        A pointer to the group.
 * @param pPermission   A pointer to the permission.
 * 
 * @return              lambda_InvalidGroup - the group does not exist
 * @return              lambda_InvalidPermission - the permission does not exist
 * @return              lambda_Done - permission removed
 */
native lambda_handle:lx_group_remove_permission(pGroup, pPermission);

/**
 * Removes all permissions from the group.
 * 
 * @param pGroup        A pointer to the group.
 * 
 * @return              lambda_InvalidGroup - the group does not exist
 * @return              lambda_Done - permissions removed
 * @return              lambda_Error - system error (please report it)
 */
native lambda_handle:lx_group_clear_permissions(pGroup);

/**
 * Checks for permissions in the group.
 * 
 * @param pGroup        A pointer to the group.
 * @param pPermission   A pointer to the permission.
 * 
 * @return              lambda_InvalidGroup - the group does not exist
 * @return              lambda_InvalidPermission - the permission does not exist
 * @return              true - permission found
 * @return              false - permission no found
 */
native lx_group_find_permission(pGroup, pPermission);

/**
 * Gets a permission pointer by sequence number in the group.
 * 
 * @param pGroup        A pointer to the group.
 * @param pos           sequence number 0 to lx_group_permission_count - 1
 * 
 * @return              A pointer to the permission.
 * @return              lambda_InvalidGroup - the group does not exist
 * @return              lambda_InvalidPermission - invalid position (going abroad)
 * @return              lambda_Error - system error (please report it)
 */
native lx_group_get_permission_pointer(pGroup, pos);

/**
 * Gets the count of permissions in the group.
 * 
 * @param pGroup        A pointer to the group.
 * 
 * @return              Count of permissions
 * @return              lambda_InvalidGroup - the group does not exist
 */
native lx_group_permission_count(pGroup);

/**
 * Gets the group's immunity.
 * 
 * @param pGroup		A pointer to the group.
 * 
 * @return 				immunity
 * @return              lambda_InvalidGroup - the group does not exist
 */
native lx_group_get_immunity(pGroup);

/**
 * Set the group's immunity.
 * 
 * @param pGroup		A pointer to the group.
 * @param iImmunity     Immunity.
 * 
 * @return 				lambda_Done - immunity is set
 * @return              lambda_InvalidGroup - the group does not exist
 */
native lambda_handle:lx_group_set_immunity(pGroup, iImmunity);

/**
 * Gets the group's prefix.
 * 
 * @param pGroup		A pointer to the group.
 * @param szPrefix      An array for the received prefix.
 * @param iLen          The size of the array.
 * 
 * @return              Number of characters
 * @return 				lambda_InvalidGroup - the group does not exist
 */
native lx_group_get_prefix(pGroup, szPrefix[], iLen);

/**
 * Set the group's prefix.
 * 
 * @param pGroup		A pointer to the group.
 * @param szPrefix      The prefix for the groups.
 * 
 * @return 				lambdlambda_Done - the prefix is set 
 * @return              lambda_InvalidGroup - the group does not exist
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
 * @param pPermission   A pointer to the permission.
 * @param szPermission  An array for the received name.
 * @param iLen          The size of the array.
 * 
 * @return              Number of characters
 * @return              lambda_InvalidPermission - the permission does not exist
 */
native lx_permission_get_name(pPermission, szPermission[], iLen);

/**
 * Gets the permission pointer by its sequence number
 * 
 * @param pos           sequence number 0 to lx_permission_size - 1
 * 
 * @return              A pointer to the permission.
 * @return              lambda_InvalidPermission - invalid position (going abroad)
 * @return              lambda_Error - system error (please report it)
 * 
 */
native lx_permission_get_pointer(pos);

/**
 * Gets the count of permissions.
 * 
 * @return              int
 */
native lx_permission_size();

/**
 * Deletes the permission.
 * 
 * @param pPermission   A pointer to the permission.
 * 
 * @return              lambda_InvalidPermission - the permission does not exist
 * @return              lambda_Done - the permission removed
 */
native lambda_handle:lx_permission_destroy(pPermission);

/**
 * Clears permission.
 * 
 * @noreturn
 */
native lx_permission_clears();

/*---------------------------------------------------------------------------------------------------*/
//                                             Players                                               //
/*---------------------------------------------------------------------------------------------------*/

/**
 * Adds a group to the player.
 * 
 * @param iPlayer       The player's index.
 * @param pGroup        A pointer to the group.
 * 
 * @return              lambda_InvalidGroup - the group does not exist
 * @return              lambda_InvalidPlayer - not a valid player
 * @return              lambda_Done - group added
 * @return              lambda_Ready - the group has already been added before
 */
native lambda_handle:lx_player_add_group(iPlayer, pGroup);

/**
 * Checks if the player has a group.
 *
 * @param iPlayer       The player's index.
 * @param pGroup        A pointer to the group.
 * 
 * @return              lambda_InvalidGroup - the group does not exist
 * @return              lambda_InvalidPlayer - not a valid player
 * @return              true - group found
 * @return              false - group no found
 */
native lx_player_find_group(iPlayer, pGroup);

/**
 * Deletes a group for a player.
 * 
 * @param iPlayer       The player's index.
 * @param pGroup        A pointer to the group.
 * 
 * @return              lambda_InvalidGroup - the group does not exist
 * @return              lambda_InvalidPlayer - not a valid player
 * @return              lambda_Done - group removed
 * @return              lambda_Error - system error (please report it)
 */
native lambda_handle:lx_player_remove_group(iPlayer, pGroup);

/**
 * Deletes groups for the player.
 *
 * @param iPlayer       The player's index.
 * 
 * @return              lambda_InvalidPlayer - not a valid player
 * @return              lambda_Done - groups removed
 * @return              lambda_Error - system error (please report it)
 */
native lambda_handle:lx_player_clear_groups(iPlayer);

/**
 * Gets a pointer to a group from a player by sequence number.
 * 
 * @param iPlayer       The player's index.
 * @param pos           sequence number 0 to lx_player_get_group_count - 1
 * 
 * @return              A pointer to the group.
 * @return              lambda_InvalidGroup - the group does not exist
 * @return              lambda_InvalidPlayer - not a valid player
 * @return              lambda_Error - system error (please report it)
 */
native lx_player_get_group(iPlayer, pos);

/**
 * Gets the count of groups from the player.
 *
 * @param iPlayer       The player's index.
 * 
 * @return              Count of groups.
 * @return              lambda_InvalidPlayer - not a valid player
 */
native lx_player_get_group_count(iPlayer);

/**
 * Adds permission to the player.
 * 
 * @param iPlayer       The player's index.
 * @param pPermission   A pointer to the permission.
 * 
 * @return              lambda_InvalidPermission - the permission does not exist
 * @return              lambda_InvalidPlayer - not a valid player
 * @return              lambda_Done - permission added
 * @return              lambda_Error - system error (please report it)
 */
native lambda_handle:lx_player_add_permission(iPlayer, pPermission);

/**
 * Checks if the player has permission.
 *
 * @param iPlayer       The player's index.
 * @param pPermission   A pointer to the permission.
 * @param bGroup        Should permissions be taken into account in groups.
 * 
 * @return              lambda_InvalidPermission - the permission does not exist
 * @return              lambda_InvalidPlayer - not a valid player
 * @return              true - permission found
 * @return              false - permission no found
 */
native lx_player_find_permission(iPlayer, pPermission, bool: bGroup);

/**
 * Removes the permission to the player.
 * 
 * @param iPlayer       The player's index.
 * @param pPermission   A pointer to the permission.
 * 
 * @return              lambda_InvalidPermission - the permission does not exist
 * @return              lambda_InvalidPlayer - not a valid player
 * @return              lambda_Done - permission removed
 * @return              lambda_Error - system error (please report it)
 */
native lambda_handle:lx_player_remove_permission(iPlayer, pPermission);

/**
 * Removes permissions for the player.
 *
 * @param iPlayer       The player's index.
 * 
 * @return              lambda_InvalidPlayer - not a valid player
 * @return              lambda_Done - groups removed
 * @return              lambda_Error - system error (please report it)
 */
native bool:lx_player_clear_permissions(iPlayer);

/**
 * Gets a pointer to a group from a player by sequence number.
 * 
 * @param iPlayer       The player's index.
 * @param pos           sequence number 0 to lx_player_get_permission_count(false) - 1
 * 
 * @return              A pointer to the group.
 * @return              lambda_InvalidPermission- the permission does not exist
 * @return              lambda_InvalidPlayer - not a valid player
 * @return              lambda_Error - system error (please report it)
 */
native lx_player_get_permission(iPlayer, pos);

/**
 * Gets the number of permissions from the player.
 *
 * @param iPlayer       The player's index.
 * @param bGroup        Should permissions be taken into account in groups.
 * 
 * @return              Count of permission.
 * @return              lambda_InvalidPlayer - not a valid player
 */
native lx_player_get_permission_count(iPlayer, bool: bGroup);

/**
 * Gets the player's immunity.
 * 
 * @param iPlayer		The player's index.
 * 
 * @return				Gets the player's immunity.
 * @return              lambda_InvalidPlayer - not a valid player
 */
native lx_player_get_immunity(iPlayer);

/**
 * Sets immunity to the player.
 * 
 * @param iPlayer		The player's index.
 * @param iImmunity		Immunity.
 * 
 * @return              lambda_Done - immunity is set
 * @return              lambda_InvalidPlayer - not a valid player
 */
native lambda_handle:lx_player_set_immunity(iPlayer, iImmunity);

/**
 * Gets the player's prefix.
 * 
 * @param iPlayer		The player's index.
 * @param szPrefix      An array for the received prefix.
 * @param iLen          The size of the array.
 * 
 * @return              Number of characters
 * @return 				lambda_InvalidPlayer - not a valid player
 */
native lambda_handle:lx_player_get_prefix(iPlayer, szPrefix[], iLen);

/**
 * Sets the player's prefix.
 * 
 * @param iPlayer		The player's index.
 * @param szPrefix      The prefix for the player.
 * 
 * @return 				lambdlambda_Done - the prefix is set 
 * @return              lambda_InvalidPlayer - not a valid player
 */
native lambda_handle:lx_player_set_prefix(iPlayer, szPrefix[]);

/*---------------------------------------------------------------------------------------------------*/
//                                              Temp                                                 //
/*---------------------------------------------------------------------------------------------------*/

/**
 * Adds a group to a temp array. 
 * 
 * @param pGroup        A pointer to the group.
 * @param index         temp array id, ranging from 0 to 32
 * 
 * @return              lambda_InvalidGroup - the group does not exist
 * @return              lambda_Done - group added
 * @return              lambda_Ready - the group has already been added before
 */
native lambda_handle:lx_temp_add_group(pGroup, index = 0);

/**
 * Removes the group to a temp array. 
 * 
 * @param pGroup        A pointer to the group.
 * @param index         temp array id, ranging from 0 to 32
 * 
 * @return              lambda_InvalidGroup - the group does not exist
 * @return              lambda_Done - group removed
 * @return              lambda_Error - system error (please report it)
 */
native lambda_handle:lx_temp_remove_group(pGroup, index = 0);

/**
 * Gets a group from temporary storage by sequence number
 * 
 * @param pos           sequence number 0 to lx_temp_get_group_count - 1
 * @param index         temp array id, ranging from 0 to 32
 * 
 * @return              A pointer to the group.
 * @return              lambda_InvalidGroup - the group does not exist
 * @return              lambda_Error - system error (please report it)
 */
native lx_temp_get_group(pos, index = 0);

/**
 * Gets the number of groups in the temporary storage
 * 
 * @param index         temp array id, ranging from 0 to 32
 * 
 * @noreturn
 */
native lx_temp_get_group_count(index);

/**
 * Adds groups to the player.
 * 
 * @param iPlayer		The player's index.
 * @param index         temp array id, ranging from 0 to 32
 *    
 * @return 				lambdlambda_Done - group added
 * @return              lambda_InvalidPlayer - not a valid player
 * @return              lambda_Error - system error (please report it)
 */
native lambda_handle:lx_temp_copy_groups(iPlayer, index = 0);

/**
 * Removes groups in temp array.
 * 
 * @param index         temp array id, ranging from -1 to 32.
 * 
 * @note                using index -1 for removed all arrays
 * 
 * @return 				lambdlambda_Done - groups removed
 * @return              lambda_Error - system error (please report it)
 */
native lambda_handle:lx_temp_clear_groups(index = -1);

/**
 * Adds a permission to a temp array. 
 * 
 * @param pPermission   A pointer to the permission.
 * @param index         temp array id, ranging from 0 to 32
 * 
 * @return              lambda_InvalidPermission - the permission does not exist
 * @return              lambda_Done - permission added
 * @return              lambda_Ready - the permission has already been added before
 */
native lambda_handle:lx_temp_add_permission(pPermission, index = 0);

/**
 * Removes the permissions to a temp array. 
 * 
 * @param pPermission   A pointer to the permission.
 * @param index         temp array id, ranging from 0 to 32
 * 
 * @return              lambda_InvalidPermission - the permission does not exist
 * @return              lambda_Done - permission removed
 * @return              lambda_Error - system error (please report it)
 */
native lambda_handle:lx_temp_remove_permission(pPermission, index = 0);

/**
 * Gets a permission from temporary storage by sequence number
 * 
 * @param pos           sequence number 0 to lx_temp_get_permission_count - 1
 * @param index         temp array id, ranging from 0 to 32
 * 
 * @return              A pointer to the permission.
 * @return              lambda_InvalidPermission - the permission does not exist
 * @return              lambda_Error - system error (please report it)
 */
native lx_temp_get_permission(pos, index = 0);

/**
 * Gets the number of permission in the temporary storage
 * 
 * @param index         temp array id, ranging from 0 to 32
 * 
 * @noreturn
 */
native lx_temp_get_permission_count(index);

/**
 * Adds permissions to the player.
 * 
 * @param iPlayer		The player's index.
 * @param index         temp array id, ranging from 0 to 32
 * 
 * @return 				lambdlambda_Done - permission added
 * @return              lambda_InvalidPlayer - not a valid player
 * @return              lambda_Error - system error (please report it)
 */
native lambda_handle:lx_temp_copy_permissions(iPlayer, index = 0);

/**
 * Removes permissions in temp array.
 * 
 * @param index         temp array id, ranging from -1 to 32
 * 
 * @note                using index -1 for removed all arrays
 * 
 * @return 				lambdlambda_Done - permissions removed
 * @return              lambda_Error - system error (please report it)
 */
native lambda_handle:lx_temp_clear_permissions(index = -1);
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