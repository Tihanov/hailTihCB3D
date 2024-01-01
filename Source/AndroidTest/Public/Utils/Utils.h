#pragma once

#define CHECK_ON_TRUE_DO_TASK(COND, TASK) \
	if(COND) \
	{\
		ULog::Error(__FILE__ ": " #COND, LO_Both); TASK; \
	} (void)0


#define CHECK_ON_TRUE_JUST_RETURN(COND) CHECK_ON_TRUE_DO_TASK(COND, return)
#define CHECK_ON_TRUE_RETURN(COND, TO_RET) CHECK_ON_TRUE_DO_TASK(COND, return TO_RET)
