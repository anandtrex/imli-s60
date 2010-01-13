/*************************************************************
 * conf.c
 *
 * This file is part of libimli
 *
 * Licensed under the GNU Library General Public License v2
 *
 * Author: V.Krishnakumar <vkrishnakumar@users.sf.net>
 *
 * Copyright Ushodaya Enterprises Ltd., 2007
 *************************************************************/

#define __IMLI_IMPL__

#include "libimli.h"
#include "imli_private.h"

#include <string.h>
#include <stdlib.h>

#ifdef WIN32

#define WIN32_MEAN_AND_LEAN

#include <windows.h>
#include <stdlib.h>

static char* get_home_dir_from_registry ()
{
    char* direc = NULL;
    HKEY sdb;
	
    int index = 0;
    char value_name[512];
    memset(value_name, 0, sizeof(value_name));
    unsigned long value_name_size;
    unsigned char value_data[512];
    memset(value_data, 0, sizeof(value_data));
    unsigned long value_data_size;
    DWORD type_code = 0;

    if(RegOpenKeyEx(HKEY_LOCAL_MACHINE,
		    "SOFTWARE\\IMLI2",
		    0,
		    KEY_READ,
		    &sdb) 
       != ERROR_SUCCESS) 
    {
	return NULL;
    }

    while(1)
    {
	value_name_size = 512;
	value_data_size = 512;

	if(RegEnumValue(sdb,
			index,
			value_name,
			&value_name_size,
			0,
			&type_code,
			value_data,
			&value_data_size) == ERROR_SUCCESS)
	{
	    if (0 == strcmp(value_name, "IMLI_HOME"))
	    {
		direc = strdup((char*)value_data);
	    }
	    ++index;
	} 
	else 
	{
	    break;
	}
    }
    RegCloseKey(sdb);      
    return direc;
}

#endif 

char* get_home_dir()
{
	char* direc = (char*)g_get_user_data_dir();
	/*if(!direc)
	//TODO get the home directory value from some config file.
		direc = "C:\\private\\e6a4c06a"; //o_O*/
    return strdup(direc);
}
