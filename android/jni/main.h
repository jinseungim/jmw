/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class com_example_opengltest_TestGLSurfaceView */

#ifndef _Included_com_example_opengltest_TestGLSurfaceView
#define _Included_com_example_opengltest_TestGLSurfaceView
#ifdef __cplusplus
extern "C" {
#endif

/*
 * Class:     com_example_opengltest_TestGLSurfaceView
 * Method:    AppInit
 * Signature: ()V
 */
jboolean Java_com_example_opengltest_TestGLSurfaceView_AppInit(JNIEnv *, jobject, jint, jint);

/*
 * Class:     com_example_opengltest_TestGLSurfaceView
 * Method:    AppResize
 * Signature: (II)V
 */
void Java_com_example_opengltest_TestGLSurfaceView_AppResize(JNIEnv *, jobject, jint, jint);

/*
 * Class:     com_example_opengltest_TestGLSurfaceView
 * Method:    AppRun
 * Signature: ()V
 */
void Java_com_example_opengltest_TestGLSurfaceView_AppRun(JNIEnv *, jobject);

void Java_com_example_opengltest_TestGLSurfaceView_AppExit(JNIEnv *, jobject);

#ifdef __cplusplus
}
#endif

extern JavaVM *g_VM;
extern jclass  g_JavaClass;

#endif
