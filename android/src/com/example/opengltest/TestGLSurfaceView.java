package com.example.opengltest;

import java.io.IOException;
import java.io.InputStream;
import java.util.Iterator;
import java.util.Map.Entry;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.content.Context;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;
import android.media.MediaPlayer;
import android.opengl.GLSurfaceView;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.Display;
import android.view.MotionEvent;
import android.view.Surface;
import android.view.WindowManager;

/**
 * Created by 임진승 on 2014-10-29.
 */
public class TestGLSurfaceView extends GLSurfaceView {

    private TestGLRenderer mRenderer;
    private static OpenGLTestActivity mContext;   
    
    private final int INVALID_POINTER_ID  = 0;
    public final static int MODE_TOUCH_PUSH = 0;
    public final static int MODE_TOUCH_POP = 1;
    public final static int MODE_TOUCH_MOVE = 2;
    
    public final String TAG = "_log_";


    public native boolean AppInit(int width, int height);
    public native void AppResize(int width, int height);
    public native void AppRun();
    public native void AppExit();
    
    public static native void HandleActionDown(int id, float x, float y);
    public static native void HandleActionUp(int id, float x, float y);
    public static native void HandleActionMove(int[] ids, float[] xs, float[] ys);


    public TestGLSurfaceView(Context context) {
        super(context);

        mContext = (OpenGLTestActivity)context;
        
        setDebugFlags(GLSurfaceView.DEBUG_CHECK_GL_ERROR | GLSurfaceView.DEBUG_LOG_GL_CALLS);
        
        mRenderer = new TestGLRenderer(this);
        setRenderer(mRenderer);
    }
    
    
	public static byte[] GetPackageName() {
		return mContext.getPackageName().getBytes();			
	}
	
	public static String GetApkPath()
	{
		
		ApplicationInfo appInfo = null;
		PackageManager packMgmr = mContext.getPackageManager();
		try {
		    appInfo = packMgmr.getApplicationInfo(mContext.getPackageName(), 0);
		} catch (NameNotFoundException e) {
		    e.printStackTrace();
		    throw new RuntimeException("Unable to locate APK...");
		}

		return appInfo.sourceDir;
	}
	
	
	public static byte[] GetResBytes(String fileName) {
		InputStream is=null;
		int size = 0;
		byte buf[]=null;
		
		try {
			is = mContext.getAssets().open(fileName);
			size = is.available();
			buf = new byte[size];
			is.read(buf);

		} catch (IOException e) {
			e.printStackTrace();
		} finally {
			try {
				if(is != null)
				{
					is.close();
					is = null;
				}
			}
			catch(IOException e){ e.printStackTrace(); }
		}
		
		return buf;			
	}
	
	public static int GetFileSize(String fileName) {
		InputStream is=null;
		int size = 0;
		
		try {
			is = mContext.getAssets().open(fileName);
			size = is.available();
		} catch (IOException e) {
			e.printStackTrace();
		} finally {
			try {
				if(is != null)
				{
					is.close();
					is = null;
				}
			}
			catch(IOException e){ e.printStackTrace(); }
		}
		
		return size;
	}
	
	
	public static int GetDPI(int type)
	{
		int ret;
		
		Display display = ((WindowManager)mContext.getSystemService(Context.WINDOW_SERVICE)).getDefaultDisplay();
		DisplayMetrics metrics = new DisplayMetrics();
		display.getMetrics(metrics);
		
		if(type == 0)
			ret = (int)metrics.xdpi;
		else
			ret = (int)metrics.ydpi;
		

		return ret;
		
	}
	
	
	public static float GetDensity()
	{
		Display display = ((WindowManager)mContext.getSystemService(Context.WINDOW_SERVICE)).getDefaultDisplay();
		DisplayMetrics metrics = new DisplayMetrics();
		display.getMetrics(metrics);
		
		return metrics.density;
	}
	
	
	
	@Override
	public void onResume() {
		super.onResume();
		
		this.setRenderMode(RENDERMODE_CONTINUOUSLY);
		
		this.queueEvent(new Runnable() {
			@Override
			public void run() {
				}
			});
	}
	
	
	@Override
	public void onPause() {
		this.queueEvent(new Runnable() {
			@Override
			public void run() {
				}
			});
		this.setRenderMode(RENDERMODE_WHEN_DIRTY);
		//super.onPause();
	}
	
    
    
    public boolean onTouchEvent(final MotionEvent pMotionEvent) {   	
    	
    	final int pointerNumber = pMotionEvent.getPointerCount();
		final int[] ids = new int[pointerNumber];
		final float[] xs = new float[pointerNumber];
		final float[] ys = new float[pointerNumber];

		for (int i = 0; i < pointerNumber; i++) {
			ids[i] = pMotionEvent.getPointerId(i);
			xs[i] = pMotionEvent.getX(i);
			ys[i] = pMotionEvent.getY(i);
		}

		switch (pMotionEvent.getAction() & MotionEvent.ACTION_MASK) {
			case MotionEvent.ACTION_POINTER_DOWN:  // 두개 이상 멀티 터치 발생
				final int indexPointerDown = pMotionEvent.getAction() >> MotionEvent.ACTION_POINTER_INDEX_SHIFT;
				final int idPointerDown = pMotionEvent.getPointerId(indexPointerDown);
				final float xPointerDown = pMotionEvent.getX(indexPointerDown);
				final float yPointerDown = pMotionEvent.getY(indexPointerDown);

				this.queueEvent(new Runnable() {
					@Override
					public void run() {
						TestGLSurfaceView.HandleActionDown(idPointerDown, xPointerDown, yPointerDown);
					}
				});
				break;

			case MotionEvent.ACTION_DOWN: // 터치가 하나 입력 된 경우
	
				final int idDown = pMotionEvent.getPointerId(0);
				final float xDown = xs[0];
				final float yDown = ys[0];

				this.queueEvent(new Runnable() {
					@Override
					public void run() {
						TestGLSurfaceView.HandleActionDown(idDown, xDown, yDown);
					}
				});
				break;

			case MotionEvent.ACTION_MOVE: // 드래그
				this.queueEvent(new Runnable() {
					@Override
					public void run() {
						//Cocos2dxGLSurfaceView.this.mCocos2dxRenderer.handleActionMove(ids, xs, ys);
					}
				});
				break;

				
			case MotionEvent.ACTION_POINTER_UP: // 두개 이상 터치중 하나는 떨어졌을때
				final int indexPointUp = pMotionEvent.getAction() >> MotionEvent.ACTION_POINTER_INDEX_SHIFT;
				final int idPointerUp = pMotionEvent.getPointerId(indexPointUp);
				final float xPointerUp = pMotionEvent.getX(indexPointUp);
				final float yPointerUp = pMotionEvent.getY(indexPointUp);

				this.queueEvent(new Runnable() {
					@Override
					public void run() {
						TestGLSurfaceView.HandleActionUp(idPointerUp, xPointerUp, yPointerUp);
					}
				});
				break;

			case MotionEvent.ACTION_UP: // 터치가 하나 만 떨어진경우
				// there are only one finger on the screen
				final int idUp = pMotionEvent.getPointerId(0);
				final float xUp = xs[0];
				final float yUp = ys[0];

				this.queueEvent(new Runnable() {
					@Override
					public void run() {
						TestGLSurfaceView.HandleActionUp(idUp, xUp, yUp);
					}
				});
				break;

			case MotionEvent.ACTION_CANCEL:
				this.queueEvent(new Runnable() {
					@Override
					public void run() {
						//Cocos2dxGLSurfaceView.this.mCocos2dxRenderer.handleActionCancel(ids, xs, ys);
					}
				});
				break;
		}
		
		if (BuildConfig.DEBUG) {
			dumpMotionEvent(pMotionEvent);
		}
    	
    	return true;
    }
    
    
    
	private void dumpMotionEvent(final MotionEvent event) {
		final String names[] = { "DOWN", "UP", "MOVE", "CANCEL", "OUTSIDE", "POINTER_DOWN", "POINTER_UP", "7?", "8?", "9?" };
		final StringBuilder sb = new StringBuilder();
		final int action = event.getAction();
		final int actionCode = action & MotionEvent.ACTION_MASK;
		sb.append("event ACTION_").append(names[actionCode]);
		if (actionCode == MotionEvent.ACTION_POINTER_DOWN || actionCode == MotionEvent.ACTION_POINTER_UP) {
			sb.append("(pid ").append(action >> MotionEvent.ACTION_POINTER_INDEX_SHIFT);
			sb.append(")");
		}
		sb.append("[");
		for (int i = 0; i < event.getPointerCount(); i++) {
			sb.append("#").append(i);
			sb.append("(pid ").append(event.getPointerId(i));
			sb.append(")=").append((int) event.getX(i));
			sb.append(",").append((int) event.getY(i));
			if (i + 1 < event.getPointerCount()) {
				sb.append(";");
			}
		}
		sb.append("]");
		Log.d(TAG, sb.toString());
	}
	
	


    public class TestGLRenderer implements GLSurfaceView.Renderer {

        public TestGLRenderer(TestGLSurfaceView view) {

        }

        @Override
        public void onDrawFrame(GL10 gl) {
            AppRun();               		
        }


        @Override
        public void onSurfaceChanged(GL10 gl, int width, int height) {
            AppResize(width, height);
        }

        @Override
        public void onSurfaceCreated(GL10 gl, EGLConfig config) {
            
        	
        	DisplayMetrics metrics = new DisplayMetrics();

        	mContext.getWindowManager().getDefaultDisplay().getMetrics(metrics);
        	
        	Display display = ((WindowManager)mContext.getSystemService(Context.WINDOW_SERVICE)).getDefaultDisplay();
        	if(display.getRotation() == Surface.ROTATION_0 || display.getRotation() == Surface.ROTATION_180)
        		AppInit(metrics.widthPixels, metrics.heightPixels);
        	else
        		AppInit(metrics.heightPixels, metrics.widthPixels);
        }

    }


}



