#pragma once
#include <jni.h>

unsigned char* GetRes(const char *resName, size_t* size0);
int			   GetDPI(int type);
float		   GetDensity(void);







#ifdef __cplusplus
extern "C" {
#endif
void Java_com_example_opengltest_TestGLSurfaceView_HandleActionDown(JNIEnv *, jobject, jint, jfloat, jfloat);
void Java_com_example_opengltest_TestGLSurfaceView_HandleActionUp(JNIEnv *, jobject, jint, jfloat, jfloat);
void Java_com_example_opengltest_TestGLSurfaceView_HandleActionMove(JNIEnv *, jobject, jintArray, jfloatArray, jfloatArray);
void Java_com_example_opengltest_TestGLSurfaceView_HandleActionCancel(JNIEnv *, jobject, jintArray, jfloatArray, jfloatArray);
#ifdef __cplusplus
}
#endif
