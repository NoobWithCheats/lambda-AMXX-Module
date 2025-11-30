# [Lambda](https://github.com/NoobWithCheats/lambda-AMXX-Module) [![Download](https://img.shields.io/github/downloads/NoobWithCheats/lambda-AMXX-Module/latest/total)](https://github.com/NoobWithCheats/lambda-AMXX-Module/releases/latest) [![Downloads](https://img.shields.io/github/downloads/NoobWithCheats/lambda-AMXX-Module/total)]() 

AMX Mod X module that allows you to abandon the use of access flags and replace them with `Permissions` and `Groups`.

## Example

```cpp
new g_GrpPtr, g_PermPtr;

public plugin_init()
{
    g_GrpPtr = lx_group_create("Test");
    g_PermPtr = lx_permission_create("Perm");
    lx_group_add_permission(g_GrpPtr, g_PermPtr);
}

public client_connect(id)
{
    lx_player_add_group(iPlayer, g_GrpPtr);

    new szName[32];
    if (lx_player_find_group(id, g_GrpPtr));
    {
        lx_group_get_name(g_GrpPtr, szName, charsmax(szName));
        server_print("player %d (%n) add group: %s", id, id, szName);

        if (lx_group_find_permission(g_GrpPtr, g_PermPtr))
        {
            lx_permission_get_name(g_PermPtr, szName, charsmax(szName));
            server_print(" Group Permission: %s", szName);
        }
    }

    g_PermPtr = lx_permission_create("Perm Custom");
    lx_player_add_permission(id, g_PermPtr);
}

public client_disconnected(id)
{
    if (lx_player_get_group_count(id)) lx_player_clear_groups(id);
    if (lx_player_get_permission_count(id, false)) lx_player_clear_permissions(id);
}
```

## Build instructions
### Checking requirements

#### Windows
<pre>
Visual Studio 2017 (C++17 standard) and later
</pre>

#### Linux
<pre>
cmake >= 3.15
GCC >= 9.4 (Optional)
ninja-build
</pre>

### Building
Use `Visual Studio` to build, open the module, select `CMakeLists.txt`, and use `Release` or `Debug` in the list of solution configurations.
