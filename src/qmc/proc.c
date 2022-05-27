#include <qmos/qmc/proc.h>

PROC *__qmc_current_proc;
PROC *get_this_proc() { return __qmc_current_proc; }
