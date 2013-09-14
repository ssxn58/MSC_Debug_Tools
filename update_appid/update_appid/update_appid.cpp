// update_appid.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <string.h>
#include <stdlib.h>

/** 
 * @fn		update_appid
 * @brief	change appkey from a dll file
 * 
 *  find app_key_in from filename_in , and change to app_key_out , and save to filename_out.
 *  filename length must less than 255 , appkey length must equal 32 .
 * 
 * @return	int 						- Return 0 in success, otherwise return error code.
 * @param	const char* filename_out	- [in] the output filename .
 * @param	const char* filename_in		- [in] the input filename .
 * @param	const char* app_key_out		- [in] the output appkey .
 * @param	const char* app_key_in		- [in] the input appkey .
 * @see		
 */
int update_appid(const char* filename_out,const char* filename_in,const char* app_key_out,const char* app_key_in);

int _tmain(int argc, _TCHAR* argv[])
{
	const char* filename_in = NULL;
	const char* filename_out = NULL;
	const char* app_key_in = NULL;
	const char* app_key_out = NULL;

	if (argc >= 2)	filename_out = argv[1];
	if (argc >= 3)	filename_in = argv[2];
	if (argc >= 4)	app_key_out = argv[3];
	if (argc >= 5)	app_key_in = argv[4];

	update_appid(filename_out,filename_in,app_key_out,app_key_in);
	getchar();
	return 0;
}

int update_appid(const char* filename_out,const char* filename_in,const char* app_key_out,const char* app_key_in)
{
	FILE *fp_in,*fp_out;
	unsigned int i,k,fp_size,read_len,write_len;
	char *content,*find_pos;
	char key_in[33] = "4d23a5893b7712e05ce340e5f7263fbb";//原始
	char key_out[33] = "4c770a3e084cb1617fa106025e4a89d4";//50287829
	char fn_in[256] = "msc.dll";
	char fn_out[256] = "msc_new.dll";
	printf("--------------------------------------------------\n");
	printf("filename_out = %s\nfilename_in = %s\napp_key_out = %s\napp_key_in = %s\n"
					,filename_out?filename_out:"NULL"
					,filename_in?filename_in:"NULL"
					,app_key_out?app_key_out:"NULL"
					,app_key_in?app_key_in:"NULL" );
	printf("--------------------------------------------------\n");
	if (filename_in && strlen(filename_in)<=255)
	{
		strncpy(fn_in,filename_in,255);
	}
	if (filename_out && strlen(filename_out)<=255)
	{
		strncpy(fn_out,filename_out,255);
	}
	if (app_key_in && strlen(app_key_in)==32)
	{
		strncpy(key_in,app_key_in,32);
	}
	if (app_key_out && strlen(app_key_out)==32)
	{
		strncpy(key_out,app_key_out,32);
	}
	printf("fn_out = %s\nfn_in = %s\nkey_out = %s\nkey_in = %s\n"
		,fn_out,fn_in,key_out,key_in );
	printf("--------------------------------------------------\n");
	fp_in = fopen(fn_in, "rb");
	if (NULL == fp_in)
	{
		printf("open file \"%s\" failed!\n",fn_in);
		return -1;
	}
	fseek(fp_in, 0, SEEK_END);
	fp_size = ftell(fp_in);
	content = (char *)malloc(fp_size);
	fseek(fp_in, 0, SEEK_SET);
	read_len = fread(content, 1, fp_size, fp_in);
	if (read_len != fp_size)
	{
		free(content);
		fclose(fp_in);
		printf("read file \"%s\" failed!\n",fn_in);
		return -1;
	}
	fclose(fp_in);

	for (i = 0; i < fp_size; i++)
	{
		if (0 == memcmp(content+i, key_in, 32))
		{
			find_pos = content+i;
			for (k = 0; k < 32; k++)
			{
				find_pos[k] = key_out[k];
			}
			k = 0;
		}
	}

	if (NULL == find_pos)
	{
		free(content);
		printf("can't find the correspond string in the library, please recheck!\n");
		return -1;
	}

	fp_out = fopen(fn_out, "wb");
	if (NULL == fp_out)
	{
		free(content);
		printf("open file \"%s\" failed.\n",fn_out);
		return -1;
	}
	write_len = fwrite(content, 1, fp_size, fp_out);
	if (fp_size != write_len)
	{
		free(content);
		fclose(fp_out);
		printf("write file \"%s\" failed.\n",fn_out);
		return -1;
	}
	free(content);
	fclose(fp_out);
	return 0;
}