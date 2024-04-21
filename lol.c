#include <X11/Xlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

Window prev = 0;
Window cur = 0;
enum states {
	SET_CUR_TO_ANY,
	SET_CUR_TO_ANY_BUT_NOT_CUR,
	SET_CUR_TO_ANY_BUT_NOT_CUR_AND_NOT_PREV
};
unsigned prevClick = 0;
int setCurrentWindow(int state, Display *dpy);
int doubleClick();

void checkIfWindowClosed(Window *w) {

}

void checkIfCurAndPrevWindowsClosed() {
if(cur) checkIfWindowClosed(&cur);
if(prev) checkIfWindowClosed(&prev);
}

void updateCurAndPreviousWindow(Display *dpy) {
	checkIfCurAndPrevWindowsClosed();
	if(0 == cur) {
		setCurrentWindow(SET_CUR_TO_ANY, dpy);
		return;
	}
	if(0 == prev) {
		prev = cur;
		if(0==setCurrentWindow(SET_CUR_TO_ANY_BUT_NOT_CUR,dpy))
			prev = 0;
		return;
	}
	if(0 == doubleClick()) {
		int tmp = cur;
		cur = prev;
		prev = tmp;
		return;
	}
	if(0==setCurrentWindow(SET_CUR_TO_ANY_BUT_NOT_CUR_AND_NOT_PREV,dpy)){
		int tmp = cur;
		cur = prev;
		prev = tmp;
	}
}

int setCurrentWindow(int state, Display *dpy) {
	Window root_return;
	Window parent_return;
	Window* children_return;
	unsigned int nchildren_return;
	XWindowAttributes attr;

	XQueryTree( dpy,DefaultRootWindow(dpy), &root_return, &parent_return, &children_return, &nchildren_return);
	for(int i = 0; i < nchildren_return; i++) {
		char * name = NULL;
		XFetchName( dpy, children_return[i], &name);
		XGetWindowAttributes( dpy, children_return[i], &attr);
		Window root_return;
		int x_return, y_return;
		unsigned int width_return, height_return;
		unsigned int border_width_return;
		unsigned int depth_return;
		XGetGeometry(dpy, children_return[i], &root_return, &x_return, &y_return, &width_return, &height_return, &border_width_return, &depth_return);
		if(width_return>500 && height_return > 500){
			printf("%s %ld %d %d\n",name, children_return[i], width_return, height_return);
			if(state==SET_CUR_TO_ANY) {
				cur = children_return[i];
				return 1;
			}
			if(state==SET_CUR_TO_ANY_BUT_NOT_CUR) {
				if(children_return[i] != cur) {
					cur = children_return[i];
					return 1;
				}
			}
			if(state==SET_CUR_TO_ANY_BUT_NOT_CUR_AND_NOT_PREV) {
				if(children_return[i] != cur &&
						children_return[i] != prev) {
					cur = children_return[i];
					return 1;
				}
			}
					
		}
	}
	return 0;
}

void simple(Display *dpy) {
	Window root_return;
	Window parent_return;
	Window* children_return;
	unsigned int nchildren_return;
	int x_return, y_return;
	unsigned int width_return, height_return;
	unsigned int border_width_return;
	unsigned int depth_return;

	XQueryTree( dpy,DefaultRootWindow(dpy), &root_return, &parent_return, &children_return, &nchildren_return);

	for(int i = 0; i < nchildren_return; i++) {
		XGetGeometry(dpy, children_return[i], &root_return, &x_return, &y_return, &width_return, &height_return, &border_width_return, &depth_return);
		if(width_return>500 && height_return > 500){
			Window cur = children_return[i];
			XRaiseWindow(dpy,cur);
			XSetInputFocus(dpy, cur, RevertToParent, CurrentTime);
			return;
		}
	}
}

void altTab(Display *dpy) {
	simple(dpy);
	return;
	updateCurAndPreviousWindow(dpy);
	if(0 == cur) return;
	XRaiseWindow(dpy,cur);
	XSetInputFocus(dpy, cur, RevertToParent, CurrentTime);
}

int interactive(int argc,char *argv[]) {
Display *dpy;
l:
	if(!(dpy = XOpenDisplay(0x0))) return 1;
	updateCurAndPreviousWindow(dpy);
	XRaiseWindow(dpy,cur);
	XSetInputFocus(dpy, cur, RevertToParent, CurrentTime);
	XCloseDisplay(dpy);
	int i;scanf("%d",&i); goto l;

}

int doubleClick() {
	if(0 == prevClick) {
		prevClick = time(0);
		return 0;
	}
	if(1 > time(0)-prevClick) {
		prevClick = time(0);
		return 1;
	}
	prevClick = time(0);
	return 0;
}
