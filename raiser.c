#include <X11/Xlib.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    Display * dpy;
    int root;
    if(!(dpy = XOpenDisplay(0x0))) return 1;
if(argc==2) root = atoi(argv[1]); else return 1;
XRaiseWindow(dpy,root);
}
