#pragma once

#define CHECK_RETURN(COND, TO_RET) \
	if(COND) \
	{\
		ULog::Error(#COND, LO_Both); TO_RET; \
	} (void)0


#define CHECK_RETURN_ON_FAIL(COND) CHECK_RETURN(COND, return)

