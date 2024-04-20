#include <X11/Xlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

Display * dpy = 0;
Window prev = 0;
Window cur = 0;
enum states {
	SET_CUR_TO_ANY,
	SET_CUR_TO_ANY_BUT_NOT_CUR,
	SET_CUR_TO_ANY_BUT_NOT_CUR_AND_NOT_PREV
};
unsigned prevClick = 0;
void setCurrentWindow(int state);
int doubleClick();

void updateCurAndPreviousWindow() {
	if(0 == cur) {
		setCurrentWindow(SET_CUR_TO_ANY);
		return;
	}
	if(0 == prev) {
		prev = cur;
		setCurrentWindow(SET_CUR_TO_ANY_BUT_NOT_CUR);
		return;
	}
	if(0 == doubleClick()) {
		int tmp = cur;
		cur = prev;
		prev = tmp;
		return;
	}
	setCurrentWindow(SET_CUR_TO_ANY_BUT_NOT_CUR_AND_NOT_PREV);
}

void setCurrentWindow(int state) {
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
				return;
			}
			if(state==SET_CUR_TO_ANY_BUT_NOT_CUR) {
				if(children_return[i] != cur) {
					cur = children_return[i];
					return;
				}
			}
			if(state==SET_CUR_TO_ANY_BUT_NOT_CUR_AND_NOT_PREV) {
				if(children_return[i] != cur &&
						children_return[i] != prev) {
					cur = children_return[i];
					return;
				}
			}
					
		}
	}
}

int main(int argc,char *argv[]) {
l:
	if(!(dpy = XOpenDisplay(0x0))) return 1;
	updateCurAndPreviousWindow();
	XRaiseWindow(dpy,cur);
	XCloseDisplay(dpy);
	int i;scanf("%d",&i); goto l;

}

int doubleClick() {
	if(0 == prevClick) {
		prevClick = time(0);
		return 0;
	}
	if(2 > time(0)-prevClick) {
		prevClick = time(0);
		return 1;
	}
	prevClick = time(0);
	return 0;
}