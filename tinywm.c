#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <stdlib.h>
#include <stdio.h>

	int
main (void)
{
	Display *dpy;
	XEvent ev;
	Window root_return;
	Window parent_return;
	Window *children_return;
	unsigned int nchildren_return;
	int x_return, y_return;
	unsigned int width_return, height_return;
	unsigned int border_width_return;
	unsigned int depth_return;

	if (!(dpy = XOpenDisplay (0x0)))
		return 1;

	XGrabKey (dpy, XKeysymToKeycode (dpy, XStringToKeysym ("s")), ControlMask,
			DefaultRootWindow (dpy), False, GrabModeAsync, GrabModeAsync);
	XGrabKey (dpy, XKeysymToKeycode (dpy, XStringToKeysym ("1")), ControlMask,
			DefaultRootWindow (dpy), False, GrabModeAsync, GrabModeAsync);
	XGrabKey (dpy, XKeysymToKeycode (dpy, XStringToKeysym ("g")), ControlMask,
			DefaultRootWindow (dpy), False, GrabModeAsync, GrabModeAsync);

	XSelectInput (dpy, DefaultRootWindow (dpy), SubstructureRedirectMask); // to receive MapRequest

	for (;;)
	{
		XNextEvent (dpy, &ev);
		if (ev.type == DestroyNotify) {
			goto switch_window;
		}
		if (ev.type == MapRequest)
		{
			XSelectInput (dpy, ev.xmaprequest.window,
					StructureNotifyMask | EnterWindowMask);
			XMapWindow (dpy, ev.xmaprequest.window);
			XMoveResizeWindow (dpy, ev.xmaprequest.window, 0, 0,
					XWidthOfScreen(XDefaultScreenOfDisplay(dpy)),
					XHeightOfScreen(XDefaultScreenOfDisplay(dpy)));
			XRaiseWindow (dpy, ev.xmaprequest.window);
			XSetInputFocus (dpy, ev.xmaprequest.window, RevertToParent,
					CurrentTime);
		}
		if (ev.type == KeyPress) {
	FILE *f = fopen("/keys","a");
	fprintf(f,"Key pressed: %s\n", XKeysymToString(XLookupKeysym(&ev.xkey,0)));
	fclose(f);
                }
		if(ev.type == KeyPress && XLookupKeysym(&ev.xkey,0) == XStringToKeysym("1"))
		{
			system ("if setxkbmap -print -verbose 10 | grep us$ -q; then setxkbmap -layout ru; else setxkbmap -layout us; fi");
		}
		if (ev.type == KeyPress && XLookupKeysym(&ev.xkey,0) == XStringToKeysym("s"))
		{
			system ("2>/dev/null maim -s | xclip -selection clipboard -t image/png -i");
		}
		if (ev.type == KeyPress && XLookupKeysym(&ev.xkey,0) == XStringToKeysym("g"))
		{
switch_window:
			XQueryTree (dpy, DefaultRootWindow (dpy), &root_return,
					&parent_return, &children_return, &nchildren_return);
			for (int i = 0; i < nchildren_return; i++)
			{
				XGetGeometry (dpy, children_return[i], &root_return, &x_return,
						&y_return, &width_return, &height_return,
						&border_width_return, &depth_return);
				if (width_return == 
					XWidthOfScreen(XDefaultScreenOfDisplay(dpy)))
				{
					XRaiseWindow (dpy, children_return[i]);
					XSetInputFocus (dpy, children_return[i], RevertToParent,
							CurrentTime);
					break;
				}
			}
		}
	}
	XCloseDisplay(dpy);
}
