
extern "C" {

	#include "lua.h"
	#include <string.h>
	#include "lauxlib.h"
	#include "vmtype.h"
	#include "vmthread.h"

	int remote_CCall(lua_State *L, lua_CFunction func);
	void remote_lua_call(VMUINT16 type, void *params);
	extern int g_shell_result;
	extern int CCwait;

	typedef struct {
		int cb_ref;
		char buff[50];
	} cb_func_param_ortobio_t;
	
	#define CB_MESSAGE_ID		    400

	typedef enum
	{
	    CB_FUNC_INT = CB_MESSAGE_ID+1,
		CB_FUNC_TIMER,
		CB_FUNC_ADC,
		CB_FUNC_BT_RECV,
		CB_FUNC_BT_CONNECT,
		CB_FUNC_BT_DISCONNECT,

		CB_FUNC_ORTOBIO_DONE,

		CB_FUNC_SMS_LIST,
		CB_FUNC_SMS_READ,
		CB_FUNC_SMS_NEW,
		CB_FUNC_HTTPS_HEADER,
		CB_FUNC_HTTPS_DATA,
		CB_FUNC_DOTTY,
		CB_FUNC_REBOOT,
		CB_FUNC_MQTT_TIMER,
		CB_FUNC_MQTT_MESSAGE,
		CB_FUNC_MQTT_DISCONNECT,
		CB_FUNC_UART_RECV,
		CB_FUNC_NET,
		CB_FUNC_EINT,
		CB_FUNC_TOUCH
	} CB_FUNC_TYPE;

	cb_func_param_ortobio_t ortobio;



	extern VM_SIGNAL_ID g_shell_signal;


	static int _test(lua_State *L){
	    g_shell_result = 0;
	    strcpy(ortobio.buff, "non mi fido molto");
	    remote_lua_call(CB_FUNC_ORTOBIO_DONE, &ortobio);
	    vm_signal_post(g_shell_signal);
	    return 1;
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
		remote_CCall(L, &_test);
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