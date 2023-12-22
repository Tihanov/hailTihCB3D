#pragma once

#define CNR_TO_CSTR_PURE(x) #x
#define CNR_TO_CSTR(x) CNR_TO_CSTR_PURE(x)


#define CHECK_RETURN(COND, TO_RET) \
	if(COND) \
	{\
		ULog::Error(__FILE__ ":" CNR_TO_CSTR(__LINE__) ":" #COND, LO_Both); TO_RET; \
	} (void)0


#define CHECK_RETURN_ON_FAIL(COND) CHECK_RETURN(COND, return)

