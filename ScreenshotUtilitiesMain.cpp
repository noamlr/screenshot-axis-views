/*============================================================================

The Medical Imaging Interaction Toolkit (MITK)

Copyright (c) German Cancer Research Center (DKFZ)
All rights reserved.

Use of this source code is governed by a 3-clause BSD license that can be
found in the LICENSE file.

============================================================================*/

#include "QmitkRegisterClasses.h"
#include "ScreenshotUtilities.h"

#include "mitkDataStorage.h"

#include <QApplication>
#include <itksys/SystemTools.hxx>

/* BEGIN: Add for TIME DELAY  */
/*
#include <EGL/egl.h>

static const EGLint configAttribs[] = {
				EGL_SURFACE_TYPE, EGL_PBUFFER_BIT,
				EGL_BLUE_SIZE, 8,
				EGL_GREEN_SIZE, 8,
				EGL_RED_SIZE, 8,
				EGL_DEPTH_SIZE, 8,
				EGL_RENDERABLE_TYPE, EGL_OPENGL_BIT,
				EGL_NONE
};    

static const int pbufferWidth = 9;
static const int pbufferHeight = 9;

static const EGLint pbufferAttribs[] = {
				EGL_WIDTH, pbufferWidth,
        EGL_HEIGHT, pbufferHeight,
        EGL_NONE,
};
*/
/*END: Add for TIME DELAY  */


int main(int argc, char *argv[]){

	/* BEGIN: Add for TIME DELAY  */
	/*
	// 1. Initialize EGL
  EGLDisplay eglDpy = eglGetDisplay(EGL_DEFAULT_DISPLAY);
  EGLint major, minor;
  eglInitialize(eglDpy, &major, &minor);

  // 2. Select an appropriate configuration
  EGLint numConfigs;
  EGLConfig eglCfg;
  eglChooseConfig(eglDpy, configAttribs, &eglCfg, 1, &numConfigs);

  // 3. Create a surface
  EGLSurface eglSurf = eglCreatePbufferSurface(eglDpy, eglCfg, pbufferAttribs);

  // 4. Bind the API
  eglBindAPI(EGL_OPENGL_API);

  // 5. Create a context and make it current
  EGLContext eglCtx = eglCreateContext(eglDpy, eglCfg, EGL_NO_CONTEXT, NULL);
  eglMakeCurrent(eglDpy, eglSurf, eglSurf, eglCtx);

  // from now on use your OpenGL context
	*/
	/*END: Add for TIME DELAY  */

	QApplication qtapplication(argc, argv);

  if (argc < 2){
    fprintf(stderr, "Usage:   %s -i [filename1] [filename2] ... -tf [filename3]\n\n", itksys::SystemTools::GetFilenameName(argv[0]).c_str());
		
		/* BEGIN: Add for TIME DELAY  */
		/*	
    // 6. Terminate EGL when finished
  	eglTerminate(eglDpy);
		*/
		/*END: Add for TIME DELAY  */
    
		return 1;
  }

  // Register Qmitk-dependent global instances
  QmitkRegisterClasses();

  ScreenshotUtilities mainWidget(argc, argv);
  mainWidget.Initialize();

	/* BEGIN: Add for TIME DELAY  */
	// 6. Terminate EGL when finished
  /*
	eglTerminate(eglDpy);
	*/
	/*END: Add for TIME DELAY  */
  
	return 0;// qtapplication.exec();
}
