#pragma once
#include "overridemodule\PsetLibraryCommon.h" 

#define	SCE_S_IRWXU	0000700			/* RWX mask for owner */
#define	SCE_S_IRUSR	0000400			/* R for owner */
#define	SCE_S_IWUSR	0000200			/* W for owner */
#define	SCE_S_IXUSR	0000100			/* X for owner */

#define	SCE_S_IRWXG	0000070			/* RWX mask for group */
#define	SCE_S_IRGRP	0000040			/* R for group */
#define	SCE_S_IWGRP	0000020			/* W for group */
#define	SCE_S_IXGRP	0000010			/* X for group */
#define	SCE_S_IRWXO	0000007			/* RWX mask for other */
#define	SCE_S_IROTH	0000004			/* R for other */
#define	SCE_S_IWOTH	0000002			/* W for other */
#define	SCE_S_IXOTH	0000001			/* X for other */

#define	SCE_S_IFMT	 0170000		/* type of file mask */
#define	SCE_S_IFIFO	 0010000		/* named pipe (fifo) */
#define	SCE_S_IFCHR	 0020000		/* character special */
#define	SCE_S_IFDIR	 0040000		/* directory */
#define	SCE_S_IFBLK	 0060000		/* block special */
#define	SCE_S_IFREG	 0100000		/* regular */
#define	SCE_S_IFLNK	 0120000		/* symbolic link */
#define	SCE_S_IFSOCK 0140000		/* socket */
#define	SCE_S_ISVTX	 0001000		/* save swapped text even after use */

// open mode
#define SCE_KERNEL_S_INONE         0

#define SCE_KERNEL_S_IRUSR         (SCE_S_IRUSR | SCE_S_IRGRP | SCE_S_IROTH | SCE_S_IXUSR | \
									SCE_S_IXGRP | SCE_S_IXOTH)
#define SCE_KERNEL_S_IWUSR         (SCE_S_IWUSR | SCE_S_IWGRP | SCE_S_IWOTH | SCE_S_IXUSR | \
									SCE_S_IXGRP | SCE_S_IXOTH)
#define SCE_KERNEL_S_IXUSR         (SCE_S_IXUSR | SCE_S_IXGRP | SCE_S_IXOTH)
#define SCE_KERNEL_S_IRWXU         (SCE_KERNEL_S_IRUSR | SCE_KERNEL_S_IWUSR)
// read write
#define SCE_KERNEL_S_IRWU          (SCE_KERNEL_S_IRUSR | SCE_KERNEL_S_IWUSR)
// read
#define SCE_KERNEL_S_IRU           (SCE_KERNEL_S_IRUSR)

#define SCE_KERNEL_S_IFMT          SCE_S_IFMT
#define SCE_KERNEL_S_IFDIR         SCE_S_IFDIR
#define SCE_KERNEL_S_IFREG         SCE_S_IFREG


int PSET_SYSV_ABI Pset_sceKernelOpen(const char* path, int flags, uint16_t mode);
int PSET_SYSV_ABI Pset_sceKernelClose(int fileHandle);
int64_t PSET_SYSV_ABI Pset_sceKernelLseek(int fileHandle, int64_t offset, int origin);
int64_t PSET_SYSV_ABI Pset_sceKernelRead(int fileHandle, void* destBuffer, size_t maxCharCount);
int PSET_SYSV_ABI Pset_sceKernelWrite(void);
int PSET_SYSV_ABI Pset_sceKernelMkdir(char* path,int32_t mode);
