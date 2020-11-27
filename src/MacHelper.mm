/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of dmgee
 *
 * Created by Adrian Carpenter on 21/11/2020.
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

#include "MacHelper.h"

#import <Appkit/AppKit.h>

QPixmap Nedrysoft::MacHelper::standardImage(StandardImage::StandardImageName standardImage, QSize imageSize) {
    NSBitmapImageRep *bitmapRepresentation = [[NSBitmapImageRep alloc]
        initWithBitmapDataPlanes: nullptr
        pixelsWide: imageSize.width()
        pixelsHigh: imageSize.height()
        bitsPerSample: 8
        samplesPerPixel: 4
        hasAlpha: YES
        isPlanar: NO
        colorSpaceName: NSDeviceRGBColorSpace
        bitmapFormat: NSBitmapFormatAlphaFirst
        bytesPerRow: 0
        bitsPerPixel: 0
    ];

    NSString *nativeImageName = nullptr;

    switch(standardImage) {
        case Nedrysoft::StandardImage::NSImageNamePreferencesGeneral: {
            nativeImageName = NSImageNamePreferencesGeneral;
            break;
        }

        case Nedrysoft::StandardImage::NSImageNameUserAccounts: {
            nativeImageName = NSImageNameUserAccounts;
            break;
        }

        case Nedrysoft::StandardImage::NSImageNameAdvanced: {
            nativeImageName = NSImageNameAdvanced;
            break;
        }
    }

    NSImage *nsImage = [NSImage imageNamed: nativeImageName];

    [NSGraphicsContext saveGraphicsState];

    [NSGraphicsContext setCurrentContext: [NSGraphicsContext graphicsContextWithBitmapImageRep: bitmapRepresentation]];

    [nsImage drawInRect: NSMakeRect(0, 0, imageSize.width(), imageSize.height()) fromRect: NSZeroRect operation: NSCompositingOperationSourceOver fraction: 1];

    [NSGraphicsContext restoreGraphicsState];

    QPixmap pixmap = QtMac::fromCGImageRef([bitmapRepresentation CGImage]);

    [bitmapRepresentation release];

    return pixmap;
}

Nedrysoft::AlertButton::AlertButtonResult Nedrysoft::MacHelper::nativeAlert(QWidget *parent, QString messageText, QString informativeText, QStringList buttons) {
    NSAlert *alert = [[NSAlert alloc] init];

    for (auto button : buttons) {
        [alert addButtonWithTitle:button.toNSString()];
    }

    [alert setMessageText:messageText.toNSString()];
    [alert setInformativeText:informativeText.toNSString()];
    [alert setAlertStyle:NSAlertStyleInformational];

    auto result = static_cast<Nedrysoft::AlertButton::AlertButtonResult>([alert runModal]);

    [alert release];

    return result;
}

bool Nedrysoft::MacHelper::loadImage(QString &filename, char **data, unsigned int *length) {
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

bool Nedrysoft::MacHelper::imageForFile(QString &filename, char **data, unsigned int *length, int width, int height) {
    auto loadedImage = [[NSWorkspace sharedWorkspace] iconForFile:filename.toNSString()];

    [loadedImage setSize:NSMakeSize(width,height)];

    if (loadedImage.isValid) {
        NSData *tiffData = [loadedImage TIFFRepresentation];

        *data = (char *) malloc(tiffData.length);
        *length = static_cast<unsigned int>(tiffData.length);

        memcpy(*data, tiffData.bytes, *length);

        return true;
    }

    return false;
}

QString Nedrysoft::MacHelper::systemFontName() {
    NSFont *font = [NSFont systemFontOfSize:12];

    return QString([[font fontName] cStringUsingEncoding:[NSString defaultCStringEncoding]]);
}

QString Nedrysoft::MacHelper::fontFilename(const QString& fontName) {
    NSFont *font = [NSFont fontWithName: [NSString stringWithCString:fontName.toLatin1().data() encoding:[NSString defaultCStringEncoding]] size:12];

    if (font) {
        CTFontDescriptorRef fontRef = CTFontDescriptorCreateWithNameAndSize((CFStringRef) [font fontName], [font pointSize]);

        auto url = static_cast<CFURLRef>(CTFontDescriptorCopyAttribute(fontRef, kCTFontURLAttribute));

        NSString *fontPath = [NSString stringWithString:[(NSURL *) CFBridgingRelease(url) path]];

        return QString([fontPath cStringUsingEncoding:[NSString defaultCStringEncoding]]);
    }

    return QString();
}
