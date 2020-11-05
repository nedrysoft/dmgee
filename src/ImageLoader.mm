 /*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of dmgee
 *
 * Created by Adrian Carpenter on 30/10/2020.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "ImageLoader.h"
#import "AppKit/AppKit.h"

bool Nedrysoft::ImageLoader::load(QString &filename, char **data, unsigned int *length) {
    NSString *fileName = filename.toNSString();

    NSImage *loadedImage = [[NSImage alloc] initWithContentsOfFile:fileName];

    if (loadedImage.isValid) {
        NSData *tiffData = [loadedImage TIFFRepresentation];

        *data = (char *) malloc(tiffData.length);
        *length = static_cast<unsigned int>(tiffData.length);

        memcpy(*data, tiffData.bytes, *length);

        [loadedImage release];

        return true;
    }

    [loadedImage release];

    return false;
}

 bool Nedrysoft::ImageLoader::imageForFile(QString &filename, char **data, unsigned int *length, int width, int height) {
     NSString *fileName = filename.toNSString();

     NSImage *loadedImage = [[NSWorkspace sharedWorkspace] iconForFile:fileName];

     [loadedImage setSize:NSMakeSize(width,height)];

     if (loadedImage.isValid) {
         NSData *tiffData = [loadedImage TIFFRepresentation];

         *data = (char *) malloc(tiffData.length);
         *length = static_cast<unsigned int>(tiffData.length);

         memcpy(*data, tiffData.bytes, *length);

         [loadedImage release];

         return true;
     }

     [loadedImage release];

     return false;
 }