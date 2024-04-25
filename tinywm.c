#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <stdlib.h>
#include <stdio.h>
Display * dpy;
void map_request(XEvent *e) {
    Window w = e->xmaprequest.window;
    XSelectInput(dpy, w, StructureNotifyMask|EnterWindowMask);
    XMapWindow(dpy, w);
    XMoveResizeWindow(dpy, w, 0,0,1920,1080);
}
int main(void)
{

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

	if(!(dpy = XOpenDisplay(0x0))) return 1;
	XSelectInput(dpy,  DefaultRootWindow(dpy), SubstructureRedirectMask);

	XGrabKey(dpy, XKeysymToKeycode(dpy, XStringToKeysym("F1")), 0,
			DefaultRootWindow(dpy), True, GrabModeAsync, GrabModeAsync);
	XGrabKey(dpy, XKeysymToKeycode(dpy, XStringToKeysym("F2")), 0,
			DefaultRootWindow(dpy), True, GrabModeAsync, GrabModeAsync);
	XGrabKey(dpy, XKeysymToKeycode(dpy, XStringToKeysym("Print")), 0,
			DefaultRootWindow(dpy), True, GrabModeAsync, GrabModeAsync);
	XGrabKey(dpy, XKeysymToKeycode(dpy, XStringToKeysym("Tab")), Mod4Mask,
			DefaultRootWindow(dpy), True, GrabModeAsync, GrabModeAsync);

	start.subwindow = None;
	for(;;)
	{
		XNextEvent(dpy, &ev);
		if(ev.type == MapRequest) {
			map_request(&ev);
		}
		if(ev.type == KeyPress && ev.xkey.keycode == 67) {
			system("DISPLAY=:0 setxkbmap -layout us");
		}
		if(ev.type == KeyPress && ev.xkey.keycode == 68) {
			system("DISPLAY=:0 setxkbmap -layout ru");
		}
		if(ev.type == KeyPress && ev.xkey.keycode == 107)
			system("maim -s | xclip -selection clipboard -t image/png -i");
		if(ev.type == KeyPress && ev.xkey.keycode == 23) {
			XQueryTree( dpy,DefaultRootWindow(dpy), &root_return, &parent_return, &children_return, &nchildren_return);
			for(int i = 0; i < nchildren_return; i++) {
				XGetGeometry(dpy, children_return[i], &root_return, &x_return, &y_return, &width_return, &height_return, &border_width_return, &depth_return);
				if(width_return>500 && height_return > 500){
					XRaiseWindow(dpy,children_return[i]);
					XSetInputFocus(dpy, children_return[i], RevertToParent, CurrentTime);
					system("DISPLAY=:0 setxkbmap -layout us");
					break;
				}
			}
		}
	}
}

