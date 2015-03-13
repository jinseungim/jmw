package com.example.opengltest;

import android.app.Activity;
import android.os.Bundle;


public class OpenGLTestActivity extends Activity {

	private TestGLSurfaceView mSurfaceView;
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		//setContentView(R.layout.activity_open_gltest);
		
		mSurfaceView = new TestGLSurfaceView(this);
        setContentView(mSurfaceView);
	}
	
	
    @Override
    protected void onPause() {
       
        super.onPause();
        mSurfaceView.onPause();
    }

    @Override
    protected void onResume() {
        
        super.onResume();
        mSurfaceView.onResume();
    }
    
    
	@Override
	protected void onDestroy() {
		
		super.onDestroy();
		
		mSurfaceView.queueEvent(new Runnable(){
			 @Override
			 public void run() {
				
				 mSurfaceView.AppExit();
			 }
		 });
	}
    
    
    static
    {
    	System.loadLibrary("AppMain");
    }
    
    
}
