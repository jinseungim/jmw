#include "main.h"
#include "jmw.h"
#include "AppDelegate.h"



USING_NS_JMW;




JavaVM *g_VM;
jclass  g_JavaClass;

void ChangeDir(void) {
	JNIEnv* env;
	jmethodID jMethod;
	jbyteArray returnArray;
	jint _size;
	char changeDir[0xff];
	char packageName[0xff];

	g_VM->GetEnv((void**) &env, JNI_VERSION_1_6);
	jMethod = env->GetStaticMethodID(g_JavaClass, "GetPackageName", "()[B");
	returnArray = (jbyteArray)env->CallStaticObjectMethod(g_JavaClass, jMethod);
	_size = env->GetArrayLength(returnArray);
	if (_size > 2) {
		memset(changeDir, 0, sizeof(changeDir));
		memset(packageName, 0, sizeof(packageName));
		env->GetByteArrayRegion(returnArray, 0, _size, (jbyte*)packageName);

		sprintf(changeDir, "data/data/%s", packageName);

		chdir(changeDir);
		env->DeleteLocalRef(returnArray);
	}
}


//SevenZDec sz7("/sdcard/test.7z", "/sdcard/7zOhh");


AppDelegate app;

/*
 * Class:     com_example_opengltest_TestGLSurfaceView
 * Method:    AppInit
 * Signature: ()V
 */
jboolean Java_com_example_opengltest_TestGLSurfaceView_AppInit(JNIEnv *env, jobject obj, jint width, jint height)
{




	jobject jNativesCls;

	env->GetJavaVM(&g_VM);
	jNativesCls = env->FindClass("com/example/opengltest/TestGLSurfaceView");
	g_JavaClass = (jclass)env->NewGlobalRef(jNativesCls);
	env->DeleteLocalRef(jNativesCls);

	//ChangeDir();
	//getcwd( buff, 1024);

	//LOG("2. working dir: %s", buff);
	GetDirector().SetScreenSize(width, height);

	//sz7.Start(nullptr);
	LOG("AppInit()");
	return AppMain::GetInstance().Init(&app);




}

/*
 * Class:     com_example_opengltest_TestGLSurfaceView
 * Method:    AppResize
 * Signature: (II)V
 */
void Java_com_example_opengltest_TestGLSurfaceView_AppResize(JNIEnv *env, jobject obj, jint width, jint height)
{
	AppMain::GetInstance().Resize(width, height);

	LOG("Resize %d %d", width, height);

}

/*
 * Class:     com_example_opengltest_TestGLSurfaceView
 * Method:    AppRun
 * Signature: ()V
 */
void Java_com_example_opengltest_TestGLSurfaceView_AppRun(JNIEnv *env, jobject obj)
{
	AppMain::GetInstance().Run();
}


void Java_com_example_opengltest_TestGLSurfaceView_AppExit(JNIEnv *env, jobject obj)
{
	AppMain::GetInstance().ReleaseInstance();
}
