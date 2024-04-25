/* TinyWM is written by Nick Welch <nick@incise.org> in 2005 & 2011.
 *
 * This software is in the public domain
 * and is provided AS IS, with NO WARRANTY. */

#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <stdlib.h>
#ifdef DEBUG
#include <stdio.h>
#endif


#define MAX(a, b) ((a) > (b) ? (a) : (b))

int main(void)
{
	Display * dpy;
	XWindowAttributes attr;
	XButtonEvent start;
	XEvent ev;
	Window root_return;
	Window parent_return;
	Window* children_return;
	unsigned int nchildren_return;
	int x_return, y_return;
	unsigned int width_return, height_return;
	unsigned int border_width_return;
	unsigned int depth_return;
#ifdef DEBUG
	FILE *fptr;
	char *name;
#endif

	if(!(dpy = XOpenDisplay(0x0))) return 1;

	XGrabKey(dpy, XKeysymToKeycode(dpy, XStringToKeysym("F1")), 0,
			DefaultRootWindow(dpy), True, GrabModeAsync, GrabModeAsync);
	XGrabKey(dpy, XKeysymToKeycode(dpy, XStringToKeysym("F2")), 0,
			DefaultRootWindow(dpy), True, GrabModeAsync, GrabModeAsync);
	XGrabKey(dpy, XKeysymToKeycode(dpy, XStringToKeysym("Print")), 0,
			DefaultRootWindow(dpy), True, GrabModeAsync, GrabModeAsync);
	XGrabKey(dpy, XKeysymToKeycode(dpy, XStringToKeysym("0")), Mod4Mask,
			DefaultRootWindow(dpy), True, GrabModeAsync, GrabModeAsync);
	// 23 keycode
	//XGrabKey(dpy, XKeysymToKeycode(dpy, XStringToKeysym("Tab")), Mod4Mask,
			//DefaultRootWindow(dpy), True, GrabModeAsync, GrabModeAsync);
	XGrabButton(dpy, 1, Mod4Mask, DefaultRootWindow(dpy), True,
			ButtonPressMask|ButtonReleaseMask|PointerMotionMask, GrabModeAsync, GrabModeAsync, None, None);
	XGrabButton(dpy, 3, Mod4Mask, DefaultRootWindow(dpy), True,
			ButtonPressMask|ButtonReleaseMask|PointerMotionMask, GrabModeAsync, GrabModeAsync, None, None);

	start.subwindow = None;
	for(;;)
	{
		XNextEvent(dpy, &ev);
#ifdef DEBUG
		fptr = fopen("/tmp/keys","a");
		fprintf(fptr,"key: %u\n",ev.xkey.keycode);
		fclose(fptr);
#endif
		if(ev.type == KeyPress && ev.xkey.keycode == 67) {
			system("DISPLAY=:0 setxkbmap -layout us");
		}
		if(ev.type == KeyPress && ev.xkey.keycode == 68) {
			system("DISPLAY=:0 setxkbmap -layout ru");
		}
		if(ev.type == KeyPress && ev.xkey.keycode == 107)
			system("maim -s | xclip -selection clipboard -t image/png -i");
		if(ev.type == KeyPress && ev.xkey.keycode == 19) {
			XQueryTree( dpy,DefaultRootWindow(dpy), &root_return, &parent_return, &children_return, &nchildren_return);
			for(int i = 0; i < nchildren_return; i++) {
				XGetGeometry(dpy, children_return[i], &root_return, &x_return, &y_return, &width_return, &height_return, &border_width_return, &depth_return);
				if(width_return>500 && height_return > 500){
					XRaiseWindow(dpy,children_return[i]);
					XSetInputFocus(dpy, children_return[i], RevertToParent, CurrentTime);
					system("DISPLAY=:0 setxkbmap -layout us");
#ifdef DEBUG
					XFetchName(dpy,children_return[i],&name);
					fptr = fopen("/tmp/keys","a");
					fprintf(fptr,"window: '%s'\n",name);
					fclose(fptr);
#endif
					break;
				}
			}
		}
		else if(ev.type == ButtonPress && ev.xbutton.subwindow != None)
		{
			XGetWindowAttributes(dpy, ev.xbutton.subwindow, &attr);
			start = ev.xbutton;
			XRaiseWindow(dpy,ev.xbutton.subwindow);
			XSetInputFocus(dpy, ev.xbutton.subwindow, RevertToParent, CurrentTime);
		}
		else if(ev.type == MotionNotify && start.subwindow != None)
		{
			int xdiff = ev.xbutton.x_root - start.x_root;
			int ydiff = ev.xbutton.y_root - start.y_root;
			XMoveResizeWindow(dpy, start.subwindow,
					attr.x + (start.button==1 ? xdiff : 0),
					attr.y + (start.button==1 ? ydiff : 0),
					MAX(1, attr.width + (start.button==3 ? xdiff : 0)),
					MAX(1, attr.height + (start.button==3 ? ydiff : 0)));
		}
		else if(ev.type == ButtonRelease)
			start.subwindow = None;
	}
}

