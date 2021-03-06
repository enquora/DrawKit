/**
 @author Contributions from the community; see CONTRIBUTORS.md
 @date 2005-2016
 @copyright MPL2; see LICENSE.txt
*/

#import <Cocoa/Cocoa.h>
#import "DKCommonTypes.h"

NS_ASSUME_NONNULL_BEGIN

@class DKQuartzCache;

/** @brief DKHandle is a base class for all handles, which are the knobs attached to shapes for interacting with them.

DKHandle is a base class for all handles, which are the knobs attached to shapes for interacting with them. This is an evolution of DKKnob
 which is still used as a central helper class for dispatching drawing to handles as needed.
 
 DKHandle is subclassed for each handle type, making it easier to customise and also add caching.
*/
@interface DKHandle : NSObject {
@private
	DKQuartzCache* mCache;
	NSSize mSize;
	NSColor* mColour;
}

@property (class, readonly) DKKnobType type;
+ (nullable DKHandle*)handleForType:(DKKnobType)type size:(NSSize)size colour:(nullable NSColor*)colour;
+ (void)setHandleClass:(Class)hClass forType:(DKKnobType)type;

@property (class, readonly, copy, nullable) NSColor* fillColour;
@property (class, readonly, copy, nullable) NSColor* strokeColour;
+ (NSBezierPath*)pathWithSize:(NSSize)size;
@property (class, readonly) CGFloat strokeWidth;
@property (class, readonly) CGFloat scaleFactor;

- (instancetype)init UNAVAILABLE_ATTRIBUTE;
- (instancetype)initWithSize:(NSSize)size;
- (instancetype)initWithSize:(NSSize)size colour:(nullable NSColor*)colour NS_DESIGNATED_INITIALIZER;
@property (readonly) NSSize size;

@property (strong, nullable) NSColor* colour;

- (void)drawAtPoint:(NSPoint)point;
- (void)drawAtPoint:(NSPoint)point angle:(CGFloat)radians;
- (BOOL)hitTestPoint:(NSPoint)point inHandleAtPoint:(NSPoint)hp;

@end

NS_ASSUME_NONNULL_END
