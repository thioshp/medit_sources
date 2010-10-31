#ifndef MOM_SCRIPT_LUA_H
#define MOM_SCRIPT_LUA_H

#include "lua/moolua.h"
#include "mooscript-api.h"

namespace mom {

bool lua_setup(lua_State *L, bool default_init, bool enable_callbacks) throw();
void lua_cleanup(lua_State *L) throw();

} // namespace mom

#endif // MOM_SCRIPT_LUA_H
