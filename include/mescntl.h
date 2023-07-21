/*************************************************************************
	> File Name: mescntl.h
	> Author: 
	> Mail: 
	> Created Time: Thu 06 Oct 2022 02:09:31 PM CST
 ************************************************************************/

#ifndef _MESCNTL_H
#define _MESCNTL_H

void mesread(const char *path, char *date, int sendfd);
void meswrite(const char *path, char *mes, char *name);

#endif
