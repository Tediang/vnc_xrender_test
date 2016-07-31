#include <X11/extensions/Xrender.h>
#include <X11/Xlib.h>

int main()
{
Display *d;
Window w, wa, wb;
XEvent event;
Font f;
GC gc;

int w_w = 800;
int w_h = 600;
int wa_w = 300;
int wa_h = 100;
int wb_w = 450;
int wb_h = 150;


d = XOpenDisplay( NULL );
w = XCreateSimpleWindow( d, RootWindow( d, 0 ),
                          0, 0,
                          w_w, w_h,
                          1, WhitePixel(d, 0), BlackPixel(d, 0) );
XSelectInput( d, w, StructureNotifyMask | ExposureMask|ButtonPressMask );
wa = XCreateSimpleWindow( d, w,
                           20, 20,
                           wa_w, wa_h,
                           1, WhitePixel(d, 0), BlackPixel(d, 0) );
XSelectInput( d, wa, StructureNotifyMask | ExposureMask|ButtonPressMask );
gc = XCreateGC( d, w, 0, 0 );
f = XLoadFont( d, "fixed" );
XSetFont( d, gc, f );
XMapWindow( d, w );
XMapWindow( d, wa );
XFlush( d );

while (1)
{
  XNextEvent(d, &event);
  switch(event.type)
  {
    case Expose:
    if(event.xany.window == w)
    {
        XSetForeground( d, gc, BlackPixel(d, 0) );
        XFillRectangle( d, w, gc, 0, 0, 260, 160 );

        XSetForeground( d, gc, WhitePixel(d, 0) );
        XDrawString( d, wa, gc, 50, 50, "TED", 3 ); 
    }
  }
}

}
