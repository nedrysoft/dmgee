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

#include "Font.h"

#import <AppKit/AppKit.h>

QString Nedrysoft::Font::systemFontName() {
    NSFont *font = [NSFont systemFontOfSize:12];

    return QString([[font fontName] cStringUsingEncoding:[NSString defaultCStringEncoding]]);
}

QString Nedrysoft::Font::getFilename(const QString& fontName) {
    NSFont *font = [NSFont fontWithName: [NSString stringWithCString:fontName.toLatin1().data() encoding:[NSString defaultCStringEncoding]] size:12];

    if (font) {
        CTFontDescriptorRef fontRef = CTFontDescriptorCreateWithNameAndSize((CFStringRef) [font fontName], [font pointSize]);

        auto url = static_cast<CFURLRef>(CTFontDescriptorCopyAttribute(fontRef, kCTFontURLAttribute));

        NSString *fontPath = [NSString stringWithString:[(NSURL *) CFBridgingRelease(url) path]];

        return QString([fontPath cStringUsingEncoding:[NSString defaultCStringEncoding]]);
    }

    return QString();
}
