#include "pdfutil.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <setjmp.h>

#include "hpdf.h"
#include <QDebug>

jmp_buf env;

void error_handler (HPDF_STATUS   error_no,
               HPDF_STATUS   detail_no,
               void         *user_data)
{
    printf ("ERROR: error_no=%04X, detail_no=%u\n", (HPDF_UINT)error_no,
                (HPDF_UINT)detail_no);
    longjmp(env, 1);
}


void
show_stripe_pattern  (HPDF_Page   page,
                      HPDF_REAL   x,
                      HPDF_REAL   y)
{
    HPDF_UINT iy = 0;

    while (iy < 50) {
        HPDF_Page_SetRGBStroke (page, 0.0, 0.0, 0.5);
        HPDF_Page_SetLineWidth (page, 1);
        HPDF_Page_MoveTo (page, x, y + iy);
        HPDF_Page_LineTo (page, x + HPDF_Page_TextWidth (page, "ABCabc123"),
                    y + iy);
        HPDF_Page_Stroke (page);
        iy += 3;
    }

    HPDF_Page_SetLineWidth (page, 2.5);
}


void
show_description  (HPDF_Page          page,
                   HPDF_REAL          x,
                   HPDF_REAL          y,
                   const char   *text)
{
    float fsize = HPDF_Page_GetCurrentFontSize (page);
    HPDF_Font font = HPDF_Page_GetCurrentFont (page);
    HPDF_RGBColor c = HPDF_Page_GetRGBFill (page);

    HPDF_Page_BeginText (page);
    HPDF_Page_SetRGBFill (page, 0, 0, 0);
    HPDF_Page_SetTextRenderingMode (page, HPDF_FILL);
    HPDF_Page_SetFontAndSize (page, font, 10);
    HPDF_Page_TextOut (page, x, y - 12, text);
    HPDF_Page_EndText (page);

    HPDF_Page_SetFontAndSize (page, font, fsize);
    HPDF_Page_SetRGBFill (page, c.r, c.g, c.b);
}


void
draw_image (HPDF_Doc     pdf,
            const char  *filename,
            float        x,
            float        y,
            float       dpi,
            const char  *text)
{
#ifdef __WIN32__
    const char* FILE_SEPARATOR = "\\";
#else
    const char* FILE_SEPARATOR = "/";
#endif
    char filename1[255];
    char print_text[256];

    HPDF_Page page = HPDF_GetCurrentPage (pdf);
    HPDF_Image image;

    memset(filename1, 0, sizeof(char)*255);
    strcpy(filename1, "/Users/paipeng/Downloads/");
    strcat(filename1, filename);
    printf("filename1: %s\n", filename1);
    qDebug() << "filename1: " << filename1;

    if (strstr(filename, ".bmp")) {
        image = HPDF_LoadBmpImageFromFile(pdf, filename1);
    } else {
        image = HPDF_LoadPngImageFromFile(pdf, filename1);
    }
    qDebug() << "image loaded from file, image size: " << HPDF_Image_GetWidth(image) << "-" << HPDF_Image_GetHeight(image);


    snprintf(print_text, sizeof(char) * 256, "%s (%.0f dpi)", text, dpi);
    /* Draw image to the canvas. */
    if (strstr(filename, ".bmp")) {
        HPDF_Page_DrawImage(page, image, x, y, HPDF_Image_GetWidth(image)*72.0/dpi, HPDF_Image_GetHeight(image)*72.0/dpi);
    }
    else {
        HPDF_Page_DrawImage(page, image, x, y, HPDF_Image_GetWidth(image)*72.0/dpi, HPDF_Image_GetHeight(image)*72.0/dpi);
    }
    /* Print the text. */
    HPDF_Page_BeginText (page);
    HPDF_Page_SetTextLeading (page, 16);
    HPDF_Page_MoveTextPos (page, x, y);
    HPDF_Page_ShowTextNextLine (page, filename);
    HPDF_Page_ShowTextNextLine (page, print_text);
    HPDF_Page_EndText (page);
}


PDFUtil::PDFUtil()
{

}

int PDFUtil::generatePdf(QString data) {
    const char *page_title = "CI Label";

    HPDF_Doc  pdf;
    HPDF_Font font;
    HPDF_Page page;
    char fname[256];

    float tw;
    float fsize;
    int i;
    int len;

    float angle1;
    float angle2;
    float rad1;
    float rad2;

    float ypos;

    strcpy (fname, "qt_text_pdf_test");
    strcat (fname, ".pdf");

    pdf = HPDF_New (error_handler, NULL);
    if (!pdf) {
        printf ("error: cannot create PdfDoc object\n");
        return 1;
    }

    if (setjmp(env)) {
        HPDF_Free (pdf);
        return 1;
    }

    /* set compression mode */
    HPDF_SetCompressionMode (pdf, HPDF_COMP_ALL);

#if 0
    HPDF_UseCNTFonts(pdf);
    HPDF_UseCNTEncodings(pdf);
#endif

#if 1
    /* create default-font */
    /* create default-font */
    font = HPDF_GetFont (pdf, "Helvetica", NULL);

#else
    HPDF_UseCNSFonts(pdf);
    HPDF_UseCNSEncodings(pdf);

    HPDF_UseUTFEncodings(pdf);
    HPDF_SetCurrentEncoder(pdf, "UTF-8");
    const char* font_name = HPDF_LoadTTFontFromFile(pdf, "C:\\Users\\paipeng\\git\\libharu\\demo\\ttfont\\FZQTJW.TTF", HPDF_TRUE);
    font = HPDF_GetFont(pdf, font_name, "UTF-8");
#endif

    /* add a new page object. */
    page = HPDF_AddPage (pdf);
    //HPDF_Page_SetSize (page, HPDF_PAGE_SIZE_A5, HPDF_PAGE_LANDSCAPE);
    // 50x30mm 72ppi
    // 1inch = 25.4 mm
    float mm_inch = 0.0393701;
    HPDF_Page_SetWidth(page, 50 * mm_inch * 72);
    HPDF_Page_SetHeight(page, 30 * mm_inch * 72);

    /* draw grid to the page */
    //print_grid  (pdf, page);

    // print the lines of the page.
    HPDF_Page_SetLineWidth (page, 1);
    HPDF_Page_Rectangle (page, 5, 5, HPDF_Page_GetWidth(page) - 10,
                HPDF_Page_GetHeight (page) - 10);
    HPDF_Page_Stroke (page);


    /* print the title of the page (with positioning center). */
    HPDF_Page_SetFontAndSize (page, font, 12);
    tw = HPDF_Page_TextWidth (page, page_title);
    HPDF_Page_BeginText (page);
    HPDF_Page_TextOut (page, (HPDF_Page_GetWidth(page) - tw) / 2,
                HPDF_Page_GetHeight (page) - 20, page_title);
    HPDF_Page_EndText (page);

    HPDF_Page_BeginText (page);
    HPDF_Page_MoveTextPos (page, 60, HPDF_Page_GetHeight(page) - 60);

    /*
     * font size
     */
    fsize = 8;
    while (fsize < 9) {
        char buf[50];
        int len;

        /* set style and size of font. */
        HPDF_Page_SetFontAndSize(page, font, fsize);

        /* set the position of the text. */
        HPDF_Page_MoveTextPos (page, 0, -5 - fsize);

        /* measure the number of characters which included in the page. */
        const char *samp_text = data.toStdString().data();
        strcpy(buf, samp_text);
        len = HPDF_Page_MeasureText (page, samp_text,
                        HPDF_Page_GetWidth(page) - 120, HPDF_FALSE, NULL);

        /* truncate the text. */
        buf[len] = 0x00;

        HPDF_Page_ShowText (page, samp_text);
#if 0
        /* print the description. */
        HPDF_Page_MoveTextPos (page, 0, -10);
        HPDF_Page_SetFontAndSize(page, font, 8);
        #ifdef __WIN32__
        _snprintf(buf, 50, "Fontsize=%.0f", fsize);
        #else
        snprintf(buf, 50, "Fontsize=%.0f", fsize);
        #endif
        HPDF_Page_ShowText (page, buf);

#endif
        fsize *= 1.5;
    }

    HPDF_Page_EndText (page);

    draw_image (pdf, "ABCD1234.png", 50, HPDF_Page_GetHeight (page) - 150, 600,"1bit grayscale.");


    /* save the document to a file */
    HPDF_SaveToFile (pdf, fname);

    /* clean up */
    HPDF_Free (pdf);

    return 0;
}
