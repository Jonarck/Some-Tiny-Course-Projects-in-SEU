
/************************************************************************************
** Function Name : lcd_ili932x_pixel_draw                                          **
** Description   : Draws a pixel on the screen.                                    **
**				   This function sets the given pixel to a particular color.  The  **
**                 coordinates of the pixel are assumed to be within the extents of**
**                 the display.                                                    **
** Arguments     : pvDisplayData -is a pointer to the driver-specific data for this**
**                                display driver.                                  **
**				   lX - is the X coordinate of the pixel.                          **
**				   lY - is the Y coordinate of the pixel.		                   **
**                 ulValue - is the color of the pixel.							   **
** Return        : None                                                            **
** Output        :                                                                 **
=====================================================================================
** Author        : Triton.zhang                                                    **
** Data          : 2011-10-20                                                      **
** Notes         :                                                                 **
************************************************************************************/
static void lcd_ili932x_pixel_draw(void *pvDisplayData, long lX, long lY, unsigned long ulValue)
{
    // Set the X address of the display cursor.
    lcd_write_command(0x20);
#ifdef PORTRAIT
    lcd_write_data(lX);
#endif
#ifdef LANDSCAPE
    lcd_write_data(LCD_VERTICAL_MAX - 1 - lY);
#endif
#ifdef PORTRAIT_FLIP
    lcd_write_data(LCD_VERTICAL_MAX - 1 - lX);
#endif
#ifdef LANDSCAPE_FLIP
    lcd_write_data(lY);
#endif

    // Set the Y address of the display cursor.
    lcd_write_command(0x21);
#ifdef PORTRAIT
    lcd_write_data(lY);
#endif
#ifdef LANDSCAPE
    lcd_write_data(lX);
#endif
#ifdef PORTRAIT_FLIP
    lcd_write_data(LCD_HORIZONTAL_MAX - 1 - lY);
#endif
#ifdef LANDSCAPE_FLIP
    lcd_write_data(LCD_HORIZONTAL_MAX - 1 - lX);
#endif

    // Write the pixel value.
    lcd_write_command(0x22);
    lcd_write_data(ulValue);
}


/************************************************************************************
** Function Name : lcd_ili932x_pixel_draw_multiple                                 **
** Description   : Draws a horizontal sequence of pixels on the screen.            **
**				   This function draws a horizontal sequence of pixels on the      **
**                 screen, using the supplied palette.  For 1 bit per pixel format,**
**                 the palette contains pre-translated colors; for 4 and 8 bit per **
**                 pixel formats, the palette contains 24-bit RGB values that must **
**                 be translated before being written to the display.              **
** Arguments     : pvDisplayData -is a pointer to the driver-specific data for this**
**                                display driver.                                  **
**				   lX - is the X coordinate of the pixel.                          **
**				   lY - is the Y coordinate of the pixel.		                   **
**                 lX0 - is sub-pixel offset within the pixel data, which is valid **
**                       for 1 or 4 bit per pixel formats.					       **
**                 lCount - is the number of pixels to draw.                       **
**                 lBPP - is the number of bits per pixel; must be 1, 4, or 8.     **
**                 pucData - is a pointer to the pixel data.  For 1 and 4 bit per  **
**                           pixel formats, the most significant bit(s) represent  **
**                           the left-most pixel.                                  **
**                 pucPalette - is a pointer to the palette used to draw the pixels**
** Return        : None                                                            **
** Output        :                                                                 **
=====================================================================================
** Author        : Triton.zhang                                                    **
** Data          : 2011-10-20                                                      **
** Notes         :                                                                 **
************************************************************************************/
static void lcd_ili932x_pixel_draw_multiple(void *pvDisplayData, long lX,
                                           long lY, long lX0, long lCount,
                                           long lBPP,
                                           const unsigned char *pucData,
                                           const unsigned char *pucPalette)
{
    unsigned long ulByte;

    // Set the cursor increment to left to right, followed by top to bottom.
    lcd_write_command(0x03);
#ifdef PORTRAIT
    lcd_write_data(0x0030);
#endif
#ifdef LANDSCAPE
    lcd_write_data(0x0028);
#endif
#ifdef PORTRAIT_FLIP
    lcd_write_data(0x0000);
#endif
#ifdef LANDSCAPE_FLIP
    lcd_write_data(0x0018);
#endif

    // Set the starting X address of the display cursor.
    lcd_write_command(0x20);
#ifdef PORTRAIT
    lcd_write_data(lX);
#endif
#ifdef LANDSCAPE
    lcd_write_data(LCD_VERTICAL_MAX - 1 - lY);
#endif
#ifdef PORTRAIT_FLIP
    lcd_write_data(LCD_VERTICAL_MAX - 1 - lX);
#endif
#ifdef LANDSCAPE_FLIP
    lcd_write_data(lY);
#endif

    // Set the Y address of the display cursor.
    lcd_write_command(0x21);
#ifdef PORTRAIT
    lcd_write_data(lY);
#endif
#ifdef LANDSCAPE
    lcd_write_data(lX);
#endif
#ifdef PORTRAIT_FLIP
    lcd_write_data(LCD_HORIZONTAL_MAX - 1 - lY);
#endif
#ifdef LANDSCAPE_FLIP
    lcd_write_data(LCD_HORIZONTAL_MAX - 1 - lX);
#endif

    // Write the data RAM write command.
    lcd_write_command(0x22);

    // Determine how to interpret the pixel data based on the number of bits per pixel.
    switch (lBPP)
    {
        // The pixel data is in 1 bit per pixel format.
        case 1:
        {
            // Loop while there are more pixels to draw.
            while (lCount)
            {
                // Get the next byte of image data.
                ulByte = *pucData++;

                // Loop through the pixels in this byte of image data.
                for(; (lX0 < 8) && lCount; lX0++, lCount--)
                {
                    // Draw this pixel in the appropriate color.
                    lcd_write_data(((unsigned long *)pucPalette)[(ulByte >> (7 - lX0)) & 1]);
                }

                // Start at the beginning of the next byte of image data.
                lX0 = 0;
            }

            // The image data has been drawn.
            break;
        }

        // The pixel data is in 4 bit per pixel format.
        case 4:
        {
            // Loop while there are more pixels to draw.  "Duff's device" is
            // used to jump into the middle of the loop if the first nibble of
            // the pixel data should not be used.  Duff's device makes use of
            // the fact that a case statement is legal anywhere within a
            // sub-block of a switch statement.  See
            // http://en.wikipedia.org/wiki/Duff's_device for detailed
            // information about Duff's device.
            switch (lX0 & 1)
            {
                case 0:
                    while(lCount)
                    {
                        // Get the upper nibble of the next byte of pixel data
                        // and extract the corresponding entry from the
                        // palette.
                        ulByte = (*pucData >> 4) * 3;
                        ulByte = (*(unsigned long *)(pucPalette + ulByte) & 0x00ffffff);

                        // Translate this palette entry and write it to the screen.
                        lcd_write_data(DPYCOLORTRANSLATE(ulByte));

                        // Decrement the count of pixels to draw.
                        lCount--;

                        // See if there is another pixel to draw.
                        if (lCount)
						{
						    case 1:
                            // Get the lower nibble of the next byte of pixel
                            // data and extract the corresponding entry from
                            // the palette.
                            ulByte = (*pucData++ & 15) * 3;
                            ulByte = (*(unsigned long *)(pucPalette + ulByte) &	0x00ffffff);

                            // Translate this palette entry and write it to the
                            // screen.
                            lcd_write_data(DPYCOLORTRANSLATE(ulByte));

                            // Decrement the count of pixels to draw.
                            lCount--;
                        }
                    }
            }

            // The image data has been drawn.
            break;
        }

        // The pixel data is in 8 bit per pixel format.
        case 8:
        {
            // Loop while there are more pixels to draw.
            while(lCount--)
            {
                // Get the next byte of pixel data and extract the
                // corresponding entry from the palette.
                ulByte = *pucData++ * 3;
                ulByte = *(unsigned long *)(pucPalette + ulByte) & 0x00ffffff;

                // Translate this palette entry and write it to the screen.
                lcd_write_data(DPYCOLORTRANSLATE(ulByte));
            }

            // The image data has been drawn.
            break;
        }
    }
}

/************************************************************************************
** Function Name : lcd_ili932x_line_draw_hor                                       **
** Description   : Draws a horizontal line.                                        **
**				   This function draws a horizontal line on the display.  The      **
**                 coordinates of the line are assumed to be within the extents of **
**                 the display.                                                    **
** Arguments     : pvDisplayData -is a pointer to the driver-specific data for this**
**                                display driver.                                  **
**				   lX - is the X coordinate of the start of pixel.                 **
**				   lX2 - is the X coordinate of the end of the line.               **
**                 lY - is the Y coordinate of the line.       				       **
**                 ulValue - is the color of the line.                             **
** Return        : None                                                            **
** Output        :                                                                 **
=====================================================================================
** Author        : Triton.zhang                                                    **
** Data          : 2011-10-20                                                      **
** Notes         :                                                                 **
************************************************************************************/
static void lcd_ili932x_line_draw_hor(void *pvDisplayData, long lX1, long lX2,
                                   long lY, unsigned long ulValue)
{
    // Set the cursor increment to left to right, followed by top to bottom.
    lcd_write_command(0x03);
#ifdef PORTRAIT
    lcd_write_data(0x0030);
#endif
#ifdef LANDSCAPE
    lcd_write_data(0x0028);
#endif
#ifdef PORTRAIT_FLIP
    lcd_write_data(0x0000);
#endif
#ifdef LANDSCAPE_FLIP
    lcd_write_data(0x0018);
#endif

    // Set the starting X address of the display cursor.
    lcd_write_command(0x20);
#ifdef PORTRAIT
    lcd_write_data(lX1);
#endif
#ifdef LANDSCAPE
    lcd_write_data(LCD_VERTICAL_MAX - 1 - lY);
#endif
#ifdef PORTRAIT_FLIP
    lcd_write_data(LCD_VERTICAL_MAX - 1 - lX1);
#endif
#ifdef LANDSCAPE_FLIP
    lcd_write_data(lY);
#endif

    // Set the Y address of the display cursor.
    lcd_write_command(0x21);
#ifdef PORTRAIT
    lcd_write_data(lY);
#endif
#ifdef LANDSCAPE
    lcd_write_data(lX1);
#endif
#ifdef PORTRAIT_FLIP
    lcd_write_data(LCD_HORIZONTAL_MAX - 1 - lY);
#endif
#ifdef LANDSCAPE_FLIP
    lcd_write_data(LCD_HORIZONTAL_MAX - 1 - lX1);
#endif

    // Write the data RAM write command.
    lcd_write_command(0x22);

    // Loop through the pixels of this horizontal line.
    while (lX1++ <= lX2)
    {
        // Write the pixel value.
        lcd_write_data(ulValue);
    }
}

/************************************************************************************
** Function Name : lcd_ili932x_line_draw_ver                                       **
** Description   : Draws a vertical line.                                          **
**				   This function draws a vertical line on the display.  The        **
**                 coordinates of the line are assumed to be within the extents of **
**                 the display.                                                    **
** Arguments     : pvDisplayData -is a pointer to the driver-specific data for this**
**                                display driver.                                  **
**				   lX - is the X coordinate of the start of pixel.                 **
**				   lY1 is the Y coordinate of the start of the line.               **
**                 lY2 is the Y coordinate of the end of the line.   		       **
**                 ulValue - is the color of the line.                             **
** Return        : None                                                            **
** Output        :                                                                 **
=====================================================================================
** Author        : Triton.zhang                                                    **
** Data          : 2011-10-20                                                      **
** Notes         :                                                                 **
************************************************************************************/
static void lcd_ili932x_line_draw_ver(void *pvDisplayData, long lX, long lY1,
                                   long lY2, unsigned long ulValue)
{
    // Set the cursor increment to top to bottom, followed by left to right.
    lcd_write_command(0x03);
#ifdef PORTRAIT
    lcd_write_data(0x0038);
#endif
#ifdef LANDSCAPE
    lcd_write_data(0x0020);
#endif
#ifdef PORTRAIT_FLIP
    lcd_write_data(0x0008);
#endif
#ifdef LANDSCAPE_FLIP
    lcd_write_data(0x0010);
#endif

    // Set the X address of the display cursor.
    lcd_write_command(0x20);
#ifdef PORTRAIT
    lcd_write_data(lX);
#endif
#ifdef LANDSCAPE
    lcd_write_data(LCD_VERTICAL_MAX - 1 - lY1);
#endif
#ifdef PORTRAIT_FLIP
    lcd_write_data(LCD_VERTICAL_MAX - 1 - lX);
#endif
#ifdef LANDSCAPE_FLIP
    lcd_write_data(lY1);
#endif

    // Set the starting Y address of the display cursor.
    lcd_write_command(0x21);
#ifdef PORTRAIT
    lcd_write_data(lY1);
#endif
#ifdef LANDSCAPE
    lcd_write_data(lX);
#endif
#ifdef PORTRAIT_FLIP
    lcd_write_data(319 - lY1);
#endif
#ifdef LANDSCAPE_FLIP
    lcd_write_data(319 - lX);
#endif

    // Write the data RAM write command.
    lcd_write_command(0x22);

    // Loop through the pixels of this vertical line.
    while(lY1++ <= lY2)
    {
        // Write the pixel value.
        lcd_write_data(ulValue);
    }
}

/************************************************************************************
** Function Name : lcd_ili932x_rect_fill                                           **
** Description   : Draws a vertical line.                                          **
**				   This function fills a rectangle on the display.  The coordinates**
**                 of the rectangle are assumed to be within the extents of the    **
**                 display, and the rectangle specification is fully inclusive (in **
**                 other words, both sXMin and sXMax are drawn, along with sYMin   **
**                 and sYMax).                                                     **
** Arguments     : pvDisplayData -is a pointer to the driver-specific data for this**
**                                display driver.                                  **
**				   pRect - is a pointer to the structure describing the rectangle. **
**                 ulValue - is the color of the rectangle.                        **
** Return        : None                                                            **
** Output        :                                                                 **
=====================================================================================
** Author        : Triton.zhang                                                    **
** Data          : 2011-10-20                                                      **
** Notes         :                                                                 **
************************************************************************************/
static void lcd_ili932x_rect_fill(void *pvDisplayData, const tRectangle *pRect, unsigned long ulValue)
{
    long lCount;

    // Write the X extents of the rectangle.
    lcd_write_command(0x50);
#ifdef PORTRAIT
    lcd_write_data(pRect->sXMin);
#endif
#ifdef LANDSCAPE
    lcd_write_data((LCD_VERTICAL_MAX - 1) - pRect->sYMax);
#endif
#ifdef PORTRAIT_FLIP
    lcd_write_data((LCD_VERTICAL_MAX - 1) - pRect->sXMax);
#endif
#ifdef LANDSCAPE_FLIP
    lcd_write_data(pRect->sYMin);
#endif
    lcd_write_command(0x51);
#ifdef PORTRAIT
    lcd_write_data(pRect->sXMax);
#endif
#ifdef LANDSCAPE
    lcd_write_data((LCD_VERTICAL_MAX - 1) - pRect->sYMin);
#endif
#ifdef PORTRAIT_FLIP
    lcd_write_data((LCD_VERTICAL_MAX - 1) - pRect->sXMin);
#endif
#ifdef LANDSCAPE_FLIP
    lcd_write_data(pRect->sYMax);
#endif

    // Write the Y extents of the rectangle.
    lcd_write_command(0x52);
#ifdef PORTRAIT
    lcd_write_data(pRect->sYMin);
#endif
#ifdef LANDSCAPE
    lcd_write_data(pRect->sXMin);
#endif
#ifdef PORTRAIT_FLIP
    lcd_write_data((LCD_HORIZONTAL_MAX - 1) - pRect->sYMax);
#endif
#ifdef LANDSCAPE_FLIP
    lcd_write_data((LCD_HORIZONTAL_MAX - 1) - pRect->sXMax);
#endif
    lcd_write_command(0x53);
#ifdef PORTRAIT
    lcd_write_data(pRect->sYMax);
#endif
#ifdef LANDSCAPE
    lcd_write_data(pRect->sXMax);
#endif
#ifdef PORTRAIT_FLIP
    lcd_write_data((LCD_HORIZONTAL_MAX - 1) - pRect->sYMin);
#endif
#ifdef LANDSCAPE_FLIP
    lcd_write_data((LCD_HORIZONTAL_MAX - 1) - pRect->sXMin);
#endif

    // Set the display cursor to the upper left of the rectangle.
    lcd_write_command(0x20);
#ifdef PORTRAIT
    lcd_write_data(pRect->sXMin);
#endif
#ifdef LANDSCAPE
    lcd_write_data((LCD_VERTICAL_MAX - 1) - pRect->sYMin);
#endif
#ifdef PORTRAIT_FLIP
    lcd_write_data((LCD_VERTICAL_MAX - 1) - pRect->sXMin);
#endif
#ifdef LANDSCAPE_FLIP
    lcd_write_data(pRect->sYMin);
#endif

    lcd_write_command(0x21);
#ifdef PORTRAIT
    lcd_write_data(pRect->sYMin);
#endif
#ifdef LANDSCAPE
    lcd_write_data(pRect->sXMin);
#endif
#ifdef PORTRAIT_FLIP
    lcd_write_data((LCD_HORIZONTAL_MAX - 1) - pRect->sYMax);
#endif
#ifdef LANDSCAPE_FLIP
    lcd_write_data((LCD_HORIZONTAL_MAX - 1) - pRect->sXMax);
#endif

    // Write the data RAM write command.
    lcd_write_command(0x22);

    // Loop through the pixels of this filled rectangle.
    for (lCount = ((pRect->sXMax - pRect->sXMin + 1) * (pRect->sYMax - pRect->sYMin + 1)); 
	               lCount >= 0; 
				   lCount--)
    {
        // Write the pixel value.
        lcd_write_data(ulValue);
    }

    // Reset the X extents to the entire screen.
    lcd_write_command(0x50);
    lcd_write_data(0x0000);
    lcd_write_command(0x51);
    lcd_write_data(0x00ef);

    // Reset the Y extents to the entire screen.
    lcd_write_command(0x52);
    lcd_write_data(0x0000);
    lcd_write_command(0x53);
    lcd_write_data(0x013f);
}

/************************************************************************************
** Function Name : lcd_ili932x_color_translate                                     **
** Description   : Translates a 24-bit RGB color to a display driver-specific color**
**				   This function translates a 24-bit RGB color into a value that   **
**                 can be written into the display's frame buffer in order to      **
**                 reproduce that color, or the closest possible approximation of  **
**                 that color.                                                     **
** Arguments     : pvDisplayData -is a pointer to the driver-specific data for this**
**                                display driver.                                  **
**				   ulValue - is the 24-bit RGB color.  The least-significant byte  **
**                           is the blue channel, the next byte is the green       **
**                           channel, and the third byte is the red channel.       **
** Return        : None                                                            **
** Output        :                                                                 **
=====================================================================================
** Author        : Triton.zhang                                                    **
** Data          : 2011-10-20                                                      **
** Notes         :                                                                 **
************************************************************************************/
static unsigned long lcd_ili932x_color_translate(void *pvDisplayData, unsigned long ulValue)
{
    // Translate from a 24-bit RGB color to a 5-6-5 RGB color.
    return(DPYCOLORTRANSLATE(ulValue));
	//return 0;
}

/************************************************************************************
** Function Name : lcd_ili932x_flush                                               **
** Description   : Flushes any cached drawing operations.                          **
**				   This functions flushes any cached drawing operations to the     **
**                 display.  This is useful when a local frame buffer is used for  **
**                 drawing operations, and the flush would copy the local frame    **
**                 buffer to the display.  For the ILI932x driver, the flush is a  **
**                 no operation.                                                   **
** Arguments     : pvDisplayData -is a pointer to the driver-specific data for this**
**                                display driver.                                  **
**				   ulValue - is the 24-bit RGB color.  The least-significant byte  **
**                           is the blue channel, the next byte is the green       **
**                           channel, and the third byte is the red channel.       **
** Return        : None                                                            **
** Output        :                                                                 **
=====================================================================================
** Author        : Triton.zhang                                                    **
** Data          : 2011-10-20                                                      **
** Notes         :                                                                 **
************************************************************************************/
void lcd_ili932x_flush(void *pvDisplayData)
{
    // There is nothing to be done.
	LCD_WriteRegister(0x07, 0x0133);

}

/************************************************************************************
** Function Name : lcd_ili932x_close                                               **
** Description   : Flushes any cached drawing operations.                          **
**				   This functions flushes any cached drawing operations to the     **
**                 display.  This is useful when a local frame buffer is used for  **
**                 drawing operations, and the flush would copy the local frame    **
**                 buffer to the display.  For the ILI932x driver, the flush is a  **
**                 no operation.                                                   **
** Arguments     : pvDisplayData -is a pointer to the driver-specific data for this**
**                                display driver.                                  **
**				   ulValue - is the 24-bit RGB color.  The least-significant byte  **
**                           is the blue channel, the next byte is the green       **
**                           channel, and the third byte is the red channel.       **
** Return        : None                                                            **
** Output        :                                                                 **
=====================================================================================
** Author        : Triton.zhang                                                    **
** Data          : 2011-10-20                                                      **
** Notes         :                                                                 **
************************************************************************************/
void lcd_ili932x_close(void *pvDisplayData)
{
	LCD_WriteRegister(0x07, 0x0000);
}
//=====================================================================================
// The graphics library display structure that describes the driver for the
// F02, F03 or F05 variants of the Formike Electronic KWH028Q02 TFT panel with
// ILI932x controllers.
//
//=====================================================================================
const tDisplay g_slcd_320x240_ili932x =
{
    sizeof(tDisplay),
    0,
#if defined(PORTRAIT) || defined(PORTRAIT_FLIP)
    LCD_VERTICAL_MAX,
    LCD_HORIZONTAL_MAX,
#else
    LCD_HORIZONTAL_MAX,
    LCD_VERTICAL_MAX,
#endif
    lcd_ili932x_pixel_draw,
    lcd_ili932x_pixel_draw_multiple,
    lcd_ili932x_line_draw_hor,
    lcd_ili932x_line_draw_ver,
    lcd_ili932x_rect_fill,
    lcd_ili932x_color_translate,
    lcd_ili932x_flush
};


//=======================================================================================
//=======================================================================================
