#include "../../src/svgren/render.hpp"

#include <utki/debug.hpp>

#include <papki/FSFile.hpp>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <X11/Xlib.h>


void processEvent(Display *display, Window window, XImage *ximage, int width, int height){
	XEvent ev;
	XNextEvent(display, &ev);
	switch(ev.type)
	{
	case Expose:
		XPutImage(display, window, DefaultGC(display, 0), ximage, 0, 0, 0, 0, width, height);
		break;
	case ButtonPress:
		exit(0);
	}
}

int main(int argc, char **argv){
	auto dom = svgdom::load(papki::FSFile("tiger.svg"));
	
	ASSERT_ALWAYS(dom)
	
	
	
	XImage *ximage;
	
	int width = 800, height=800;

	Display *display = XOpenDisplay(NULL);
	
	Visual *visual = DefaultVisual(display, 0);
	
	Window window = XCreateSimpleWindow(display, RootWindow(display, 0), 0, 0, width, height, 1, 0, 0);
	
	if(visual->c_class != TrueColor){
		TRACE_ALWAYS(<< "Cannot handle non true color visual ...\n" << std::endl)
		return 1;
	}

	unsigned imWidth = 0;
	unsigned imHeight = 0;
	auto img = svgren::render(*dom, imWidth, imHeight);
	
	ximage = XCreateImage(display, visual, 24, ZPixmap, 0, reinterpret_cast<char*>(&*img.begin()), imWidth, imHeight, 8, 0);
	
	XSelectInput(display, window, ButtonPressMask|ExposureMask);
	
	XMapWindow(display, window);
	
	while(true){
		processEvent(display, window, ximage, imWidth, imHeight);
	}

	TRACE_ALWAYS(<< "[PASSED]" << std::endl)
}
