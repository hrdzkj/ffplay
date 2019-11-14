#include <stdio.h>
#include <stdlib.h>

#ifdef WIN32
#include <windows.h>
//#include <wtypes.h>
//#include <winbase.h>
#else
#include <dlfcn.h>
#endif

//将版本信息转换成具体的数字
//#define AV_VERSION_INT(a,b,c) ((a)<<16 | (b)<<8 | (c))
void transport_version(unsigned int version_num, char *pname)
{
	int version_major, version_minor, version_micro;

	version_major = (version_num >> 16) & 0xff;
	version_minor = (version_num >> 8) & 0xff;
	version_micro = (version_num) & 0xff;

	printf("%s: %d, %d, %d\n", pname, version_major, version_minor, version_micro);
}

//需要从so或dll文件中导出的函数声明
typedef unsigned(*avcodec_version_fuc)(void);

//动态库名字加路径
char *psoavutil = "libavutil.so";
char *psoavcodec = "libavcodec.so";
char *psoavformat = "libavformat.so";
char *psoavdevice = "libavdevice.so";
char *psoavfilter = "libavfilter.so";
char *psoavscale = "libswscale.so";

char *pdllavutil = "libavutil.dll";
char *pdllavcodec = "libavcodec.dll";
//char *pdllavformat = "libavformat.dll";
char *pdllavformat = "avformat-57.dll";
char *pdllavdevice = "libavdevice.dll";
char *pdllavfilter = "libavfilter.dll";
char *pdllavscale = "libswscale.dll";

#ifdef WIN32
void get_lib_version(char *pdllname, char *function)
{
	HINSTANCE pdllHandle;
	avcodec_version_fuc avcodec_version = NULL;//函数指针

	pdllHandle = LoadLibrary(pdllname);
	if (pdllHandle == NULL) {
		printf("Cant open %s\n", pdllHandle);
		return;
	}

	avcodec_version = (avcodec_version_fuc)GetProcAddress(pdllHandle, function);
	if (avcodec_version == NULL) {
		printf("Cant load symbol %s\n", function);
		FreeLibrary(pdllHandle);
		return;
	}
	transport_version(avcodec_version(), function);

	//关闭so文件
	FreeLibrary(pdllHandle);
}



void get_ffmpeg_version(char *pdllname, char *function)
{
	HINSTANCE pdllHandle;
	char *avcodec_version = NULL;//函数指针

	pdllHandle = LoadLibrary(pdllname);
	if (pdllHandle == NULL) {
		printf("Cant open %s\n", pdllHandle);
		return;
	}

	avcodec_version = (avcodec_version_fuc)GetProcAddress(pdllHandle, function);
	if (avcodec_version == NULL) {
		printf("Cant load symbol %s\n", function);
		FreeLibrary(pdllHandle);
		return;
	}
	printf("%s\n", avcodec_version);

	//关闭so文件
	FreeLibrary(pdllHandle);
}

//windows动态加载方法
//void main(int argc, char *argv[])
void getVersion()
{
	//get_lib_version(pdllavutil, "avutil_version");
	//get_lib_version(pdllavcodec, "avcodec_version");
	get_lib_version(pdllavformat, "avformat_version");
	//get_lib_version(pdllavdevice, "avdevice_version");
	//get_lib_version(pdllavfilter, "avfilter_version");
	//get_lib_version(pdllavscale, "swscale_version");
	//get_ffmpeg_version(pdllavcodec, "av_codec_ffversion");
}

#else
void get_lib_version(char *psoname, char *function)
{
	void *psoHandle;
	avcodec_version_fuc avcodec_version = NULL;//函数指针

	psoHandle = dlopen(psoname, RTLD_NOW | RTLD_GLOBAL);
	if (psoHandle == NULL) {
		printf("Cant open %s\n", psoHandle);
		return;
	}

	avcodec_version = dlsym(psoHandle, function);
	if (avcodec_version == NULL) {
		printf("Cant load symbol %s\n", function);
		dlclose(psoHandle);
		return;
	}
	transport_version(avcodec_version(), function);

	//关闭so文件
	dlclose(psoHandle);
}


void get_ffmpeg_version(char *psoname, char *function)
{
	void *psoHandle;
	char *avcodec_version = NULL;//函数指针

	psoHandle = dlopen(psoname, RTLD_NOW | RTLD_GLOBAL);
	if (psoHandle == NULL) {
		printf("Cant open %s\n", psoHandle);
		return;
	}

	avcodec_version = dlsym(psoHandle, function);
	if (avcodec_version == NULL) {
		printf("Cant load symbol %s\n", function);
		dlclose(psoHandle);
		return;
	}
	printf("%s\n", avcodec_version);

	//关闭so文件
	dlclose(psoHandle);
}

//linux动态加载方法
/*
void main(int argc, char *argv[])
{
	get_lib_version(psoavutil, "avutil_version");
	get_lib_version(psoavcodec, "avcodec_version");
	get_lib_version(psoavformat, "avformat_version");
	get_lib_version(psoavdevice, "avdevice_version");
	get_lib_version(psoavfilter, "avfilter_version");
	get_lib_version(psoavscale, "swscale_version");
	get_ffmpeg_version(psoavcodec, "av_codec_ffversion");
	
}
*/
#endif