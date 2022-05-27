// implemented in io.c
#ifndef QMOS_QMC_QM_IOBUF_H
#define QMOS_QMC_QM_IOBUF_H
#include <qmos/qmc/fs.h>

/**
 * Writes to a "write" buffer. e.g. for stdin this is the screen
 * but for a file open in read-only mode does nothing. Really only
 * used internally. So instead of relying on textmode_* using iobuf_*
 * functions makes it possible to write to  an io port or a socket.
 */
void iobuf_write(FILEBUF *fb, int c);

#endif//QMOS_QMC_QM_IOBUF_H
