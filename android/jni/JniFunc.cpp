#include "jmw.h"
#include "main.h"
#include "JniFunc.h"

USING_NS_JMW;

unsigned char* GetRes(const char *resName, size_t* size0)
{
	jmethodID jMethod;
	jbyteArray bytes;
	jbyte* buffer;

	JNIEnv* env;
	g_VM->GetEnv((void**) &env, JNI_VERSION_1_6);

	jMethod = env->GetStaticMethodID(g_JavaClass, "GetResBytes", "(Ljava/lang/String;)[B");
	jstring str=env->NewStringUTF(resName);
	bytes = (jbyteArray) env->CallStaticObjectMethod(g_JavaClass, jMethod, str);

	*size0 = env->GetArrayLength(bytes);
	env->DeleteLocalRef(str);

	if(*size0>0)
	{
		buffer = (jbyte*) malloc(*size0);
		env->GetByteArrayRegion(bytes, 0, *size0, buffer);
		env->DeleteLocalRef(bytes);

		return (unsigned char*)buffer;
	}


	return nullptr;

}



int	GetDPI(int type)
{
	jmethodID jMethod;
	JNIEnv* env;
	int ret=0;

	g_VM->GetEnv((void**) &env, JNI_VERSION_1_6);
	jMethod = env->GetStaticMethodID(g_JavaClass, "GetDPI", "(I)I");

	if(jMethod)
	{
		ret = env->CallStaticIntMethod(g_JavaClass, jMethod, type);
	}

	return ret;
}


float GetDensity(void)
{
	jmethodID jMethod;
	JNIEnv* env;
	jfloat density=1.0;

	g_VM->GetEnv((void**) &env, JNI_VERSION_1_6);
	jMethod = env->GetStaticMethodID(g_JavaClass, "GetDensity", "()F");

	if(jMethod)
	{
		density = env->CallStaticFloatMethod(g_JavaClass, jMethod);
	}

	return density;
}



void Java_com_example_opengltest_TestGLSurfaceView_HandleActionDown(JNIEnv *, jobject, jint id, jfloat x, jfloat y)
{
	y = GetDirector().GetDeviceHeight() - y;
	AppMain::GetInstance().HandleTouch(TOUCH_DOWN, id, x, y);
}

void Java_com_example_opengltest_TestGLSurfaceView_HandleActionUp(JNIEnv *, jobject, jint id, jfloat x, jfloat y)
{
	y = GetDirector().GetDeviceHeight() - y;
	AppMain::GetInstance().HandleTouch(TOUCH_UP, id, x, y);
}

void Java_com_example_opengltest_TestGLSurfaceView_HandleActionMove(JNIEnv *env, jobject, jintArray ids, jfloatArray xs, jfloatArray ys)
{
	int size = env->GetArrayLength(ids);
	jint id[size];
	jfloat x[size];
	jfloat y[size];

	env->GetIntArrayRegion(ids, 0, size, id);
	env->GetFloatArrayRegion(xs, 0, size, x);
	env->GetFloatArrayRegion(ys, 0, size, y);

	for(int i=0; i<size; i++)
	{
		jfloat y0 = GetDirector().GetDeviceHeight() - y[i];
		AppMain::GetInstance().HandleTouch(TOUCH_MOVE, id[i], x[i], y0);
	}
}

void Java_com_example_opengltest_TestGLSurfaceView_HandleActionCancel(JNIEnv *env, jobject, jintArray ids, jfloatArray xs, jfloatArray ys)
{
	int size = env->GetArrayLength(ids);
	jint id[size];
	jfloat x[size];
	jfloat y[size];

	env->GetIntArrayRegion(ids, 0, size, id);
	env->GetFloatArrayRegion(xs, 0, size, x);
	env->GetFloatArrayRegion(ys, 0, size, y);

	for(int i=0; i<size; i++)
	{
		jfloat y0 = GetDirector().GetDeviceHeight() - y[i];
		AppMain::GetInstance().HandleTouch(TOUCH_UP, id[i], x[i], y0);
	}
}
