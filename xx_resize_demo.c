#include <X11/extensions/Xrender.h>

#include <X11/Xlib.h>
#include <stdio.h>
#include <cairo.h>
#include <cairo-xlib.h>
#include <stdlib.h>

const int wa_w = 200;
const int wa_h = 100;

const int wa_w_scaled = 300;
const int wa_h_scaled = 150;

int gap = 1;

void draw_scale(Display *d, Visual *v, GC gc, Window win_src, Window win_dst, int w_dst, int h_dst)
{
// create src
    XRenderPictFormat* format = XRenderFindVisualFormat(d, v);
    XRenderPictureAttributes pa;
    pa.subwindow_mode = IncludeInferiors;  // Don't clip child widgets
    Picture src = XRenderCreatePicture(d,
                                       win_src,
                                       format,
                                       CPSubwindowMode,
                                       &pa);

// scale src
    double w_rate = 1.5;
    double h_rate = 1.5;

    XTransform xform = { {
            { XDoubleToFixed(1), XDoubleToFixed(0), XDoubleToFixed(0) },
            { XDoubleToFixed(0), XDoubleToFixed(1), XDoubleToFixed(0) },
            { XDoubleToFixed(0), XDoubleToFixed(0), XDoubleToFixed(1.1) }
        }
    };
    XRenderSetPictureTransform(d, src, &xform);
//    // Apply filter to smooth out the image.
//    XRenderSetPictureFilter(d, src, FilterBest, NULL, 0);
//
// create dst
    XImage* image_wa = XGetImage(d,
                                 win_src,
                                 0,
                                 0,
                                 300,
                                 100,
                                 AllPlanes, ZPixmap);

    Pixmap dst_pixmap = XCreatePixmap(d,
                                      win_dst,
                                      w_dst,
                                      h_dst,
                                      format->depth);
//
    Picture dst = XRenderCreatePicture(d, dst_pixmap, format, 0, NULL);
    XRenderColor transparent = { 0 };
    XRenderFillRectangle(d,
                         PictOpSrc,
                         dst,
                         &transparent,
                         0,
                         0,
                         w_dst,
                         h_dst);
//
// src --> dst
    XRenderComposite(d,
                     PictOpSrc,
                     src,
                     None,
                     dst,
                     0,
                     0,
                     0,
                     0,
                     0,
                     0,
                     w_dst,
                     h_dst);

    XImage* image_wa2 = XGetImage(d,
    win_src,
            0,
            0,
            300,
            100,
            AllPlanes, ZPixmap);

    XImage* image = XGetImage(d,
                              dst_pixmap,
                              0,
                              0,
                              w_dst,
                              h_dst,
                              AllPlanes, ZPixmap);
    XPutImage(d, win_dst, gc, image, 0,0,0,0,w_dst, h_dst);
}

void draw_scale_bk(Display *d, Visual *v, GC gc, Window win_src, Window win_dst, int w_dst, int h_dst)
{
// create src
    XRenderPictFormat* format = XRenderFindVisualFormat(d, v);
    XRenderPictureAttributes pa;
    pa.subwindow_mode = IncludeInferiors;  // Don't clip child widgets
//    Pixmap pix_left = XCreatePixmap(d, win_src, 88, 100, format->depth);
//    Picture src = XRenderCreatePicture(d,
//                                       pix_left,
//                                       format,
//                                       CPSubwindowMode,
//                                       &pa);
    Picture src = XRenderCreatePicture(d,
                                       win_src,
                                       format,
                                       CPSubwindowMode,
                                       &pa);

// scale src
    double w_rate = (double)wa_w_scaled/ wa_w;
    double h_rate = (double)wa_h_scaled/ wa_h;

//    w_rate = 1.0;
//    h_rate = 1.0;

    fprintf(stderr, "TED__w_rate(%lf)\n", w_rate);

    XTransform xform = { {
            { XDoubleToFixed(1/w_rate), XDoubleToFixed(0), XDoubleToFixed(0) },
            { XDoubleToFixed(0), XDoubleToFixed(1/h_rate), XDoubleToFixed(0) },
            { XDoubleToFixed(0), XDoubleToFixed(0), XDoubleToFixed(1) }
        }
    };
    XRenderSetPictureTransform(d, src, &xform);
    // Apply filter to smooth out the image.
    XRenderSetPictureFilter(d, src, FilterBest, NULL, 0); //FilterNearest FilterBest
//
// create dst
    Pixmap dst_pixmap = XCreatePixmap(d,
                                      win_dst,
                                      132,
                                      wa_h_scaled,
                                      format->depth);
//
    Picture dst = XRenderCreatePicture(d, dst_pixmap, format, 0, NULL);
    XRenderColor transparent = { 0 };
    XRenderFillRectangle(d,
                         PictOpSrc,
                         dst,
                         &transparent,
                         0,
                         0,
                         wa_w_scaled,
                         wa_h_scaled);
//
// src --> dst
    XRenderComposite(d,
                     PictOpSrc,
                     src,
                     None,
                     dst,
                     0,
                     0,
                     0,
                     0,
                     0,
                     0,
                     132,
                     150);

    XCopyArea(d, dst_pixmap, win_dst, gc, 0, 0, wa_w_scaled, wa_h_scaled, 0, 0);
    XRenderComposite(d,
                     PictOpSrc,
                     src,
                     None,
                     dst,
                     132,
                     0,
                     0,
                     0,
                     0,
                     0,
                     300,
                     150);
//    XCopyArea(d, dst_pixmap, win_dst, gc, 0, 0, wa_w_scaled, wa_h_scaled, 0, wa_h_scaled/gap);
   // XCopyArea(d, dst_pixmap, win_dst, gc, 0, 0, wa_w_scaled, wa_h_scaled, 132, 0);
//    fprintf(stderr, "TED__copy...1 at() ");
//    XCopyArea(d, dst_pixmap, win_dst, gc, 0, 0, wa_w_scaled, wa_h_scaled, wa_w_scaled/gap, wa_h_scaled/gap);
}

void draw_scale_normal(Display *d, Visual *v, GC gc, Window win_src, Window win_dst, int w_src, int h_src, int w_dst, int h_dst)
{
    XImage* xim_src = XGetImage(d,
                              win_src,
                              0,
                              0,
                              w_src,
                              h_src,
                              AllPlanes, ZPixmap);
    XPutImage(d, win_dst, gc, xim_src, 0, 0, 0, 0, w_dst, h_dst);
}

void draw_scale_cairo(Display *d, Visual *v, GC gc, Window win_src, Window win_dst, int w_dst, int h_dst)
{
    cairo_surface_t *dst_surface = cairo_xlib_surface_create(d, win_dst, v, w_dst, h_dst);
    //cairo_xlib_surface_set_size(dst_surface, w_dst, h_dst);

    cairo_surface_t *src_surface = cairo_xlib_surface_create(d, win_src, v, wa_w, wa_h);
    //cairo_xlib_surface_set_size(src_surface, 300, 100);
    cairo_xlib_surface_set_size(src_surface, 450, 150);

    cairo_t *cr = cairo_create(dst_surface);
    //cairo_translate(cr, .5*w_dst, .5*h_dst);
    //cairo_translate(cr, 0, 0);
    //cairo_xlib_surface_set_size(src_surface, 600, 300);
    //cairo_scale(cr, 2, 2);
    cairo_scale(cr, 1.5, 1.5);
    //cairo_xlib_surface_set_size(src_surface, 600, 300);
    cairo_set_source_surface(cr, src_surface, 0, 0);
    cairo_paint(cr);
    fprintf(stderr, "cairo image to wb...\n");
}

void draw_scale_cairo_png(Display *d, Visual *v, GC gc, Window win_dst, int w_dst, int h_dst)
{
    cairo_surface_t *dst_surface = cairo_xlib_surface_create(d, win_dst, v, w_dst, h_dst);
    //cairo_xlib_surface_set_size(dst_surface, w_dst, h_dst);

    //cairo_surface_t *src_surface = cairo_image_surface_create_from_png ("./lx.png");
    cairo_surface_t *src_surface = cairo_image_surface_create_from_png ("/root/real-vnc/xrender_xlib_resize_demo/lx.png");
    //cairo_xlib_surface_set_size(src_surface, 300, 100);
    //cairo_xlib_surface_set_size(src_surface, 450, 150);

    cairo_t *cr = cairo_create(dst_surface);
    cairo_translate(cr, .5*w_dst, .5*h_dst);
    //cairo_translate(cr, 0, 0);
    cairo_scale(cr, 450/600.0, 150/400.0);
    //cairo_scale(cr, 1.5, 1.5);
    //cairo_xlib_surface_set_size(src_surface, 450, 150);
    cairo_set_source_surface(cr, src_surface, 0, 0);
    cairo_paint(cr);
    fprintf(stderr, "cairo image to wb...\n");
}



int main(int argc, char **argv)
{
    Display *d;
    Window w, wa, wb;
    int s;
    Visual *v;
    XEvent event;
    Font f;
    GC gc;

    if(argc == 2)
    {
        fprintf(stderr, "TED__str_gap(%s)\n", argv[1]);
        gap = atoi(argv[1]);
        fprintf(stderr, "TED__gap(%d)\n", gap);
    }

    int w_w = 1600;
    int w_h = 800;

    int wb_w = 800;
    int wb_h = 600;

    int k = 0;

    int count_display = 1;

    d = XOpenDisplay( NULL );
    s = DefaultScreen(d);
    v = DefaultVisual(d, s);
    w = XCreateSimpleWindow( d, RootWindow( d, 0 ),
                             0, 0,
                             w_w, w_h,
                             1, WhitePixel(d, 0), BlackPixel(d, 0) );
    XSelectInput( d, w, StructureNotifyMask | ExposureMask );

    wa = XCreateSimpleWindow( d, w,
                              20, 20,
                              wa_w, wa_h,
                              1, BlackPixel(d, 0), WhitePixel(d, 0) );
    XSelectInput( d, wa, StructureNotifyMask );
    wb = XCreateSimpleWindow( d, w,
                              20, 320,
                              wb_w, wb_h,
                              1, WhitePixel(d, 0), BlackPixel(d, 0) );
    XSelectInput( d, wb, StructureNotifyMask );



    gc = XCreateGC( d, w, 0, 0 );
    int x , y , w_ , h ;
    Pixmap bitmap;
    //int rc = XReadBitmapFile(d, w, "/root/Desktop/xrender_xlib_resize_demo/lx2.bmp", &w_, &h, &bitmap, &x, &y);

    f = XLoadFont( d, "fixed" );
    XSetFont( d, gc, f );

    XMapWindow( d, w );
    XMapWindow( d, wa );
    XMapWindow( d, wb );
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
                XDrawString( d, wa, gc, 80, 80, "TED", 3 );


//                int rc = XReadBitmapFile(d, w, "/root/Desktop/xrender_xlib_resize_demo/2.bmp", &w, &h, &bitmap, &x, &y);
//                fprintf(stderr, "XReadBitmapFile...rc(%d)\n", rc);
//                XCopyPlane(d, bitmap, wa, gc, 0, 0, w, h, 0, 0, 1);
                //XSetForeground( d, gc, WhitePixel(d, 0) );
                //XDrawString( d, wb, gc, 50, 50, "SAU", 3 );

                // put wb
                //draw_scale(d, v, gc, wa, wb, wb_w, wb_h);
                draw_scale_bk(d, v, gc, wa, wb, wb_w, wb_h);
                //if (k++ == 0)
                //draw_scale_cairo(d, v, gc ,wa, wb, wb_w, wb_h);
                //draw_scale_cairo_png(d, v, gc, wb, wb_w, wb_h);
                //draw_scale_normal(d, v, gc, wa, wb, wa_w, wa_h, wb_w, wb_h);
                fprintf(stderr, "put image to wb...%d\n", count_display++);
            }
        }
    }
}
