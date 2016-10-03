
extern "C" {
	#include <string.h>
	#include <time.h>

	#include "vmtype.h"
	#include "vmsystem.h"
	#include "vmlog.h"
	#include "vmtimer.h"
	#include "vmbt_cm.h"
	#include "vmbt_spp.h"
	#include "vmstdlib.h"
	#include "string.h"
	#include "vmmemory.h"
	#include "vmthread.h"

	#include "lua.h"
	#include "lauxlib.h"
	#include "shell.h"
}


static cb_func_param_ortobio_t ortobio;




static int _test(lua_State *L){
    g_shell_result = 0;
    strcpy(ortobio.buff, "non mi fido molto");
    remote_lua_call(CB_FUNC_ORTOBIO_DONE, &ortobio);

}







static int test(lua_State *L)
{

    if (ortobio.cb_ref != LUA_NOREF) {
    	luaL_unref(L, LUA_REGISTRYINDEX, ortobio.cb_ref);
    	ortobio.cb_ref = LUA_NOREF;
    }

    if ((lua_type(L, 1) == LUA_TFUNCTION) || (lua_type(L, 1) == LUA_TLIGHTFUNCTION)) {
		lua_pushvalue(L, 1);
		ortobio.cb_ref = luaL_ref(L, LUA_REGISTRYINDEX);
    }

    g_shell_result = -9;
	CCwait = 20000;
	remote_CCall(L, _test);
	if (g_shell_result < 0) { // no response or error
        lua_pushinteger(L, g_shell_result);
		g_shell_result = 1;
	}
	else {
		g_shell_result = 1;
		lua_pushinteger(L, 0);
	}
	return g_shell_result;
}






extern "C"{
	#undef MIN_OPT_LEVEL
	#define MIN_OPT_LEVEL 0
	#include "lrodefs.h"

	const LUA_REG_TYPE ortobio_map[] = {
			{LSTRKEY("test"), LFUNCVAL(test)},
	        {LNILKEY, LNILVAL}
	};


	LUALIB_API int luaopen_ortobio(lua_State *L) {
	 	ortobio.cb_ref=LUA_NOREF;
	 	luaL_register(L, "ortobio", ortobio_map);
	  	return 1;
	}

}