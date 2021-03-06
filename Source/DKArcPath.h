/**
 @author Contributions from the community; see CONTRIBUTORS.md
 @date 2005-2016
 @copyright MPL2; see LICENSE.txt
*/

#import <Cocoa/Cocoa.h>
#import "DKDrawablePath.h"

NS_ASSUME_NONNULL_BEGIN

//! shape types this class supports:
typedef NS_ENUM(NSInteger, DKArcPathType) {
	kDKArcPathOpenArc = 0,
	kDKArcPathWedge,
	kDKArcPathCircle
};

// the class:

@interface DKArcPath : DKDrawablePath <NSCopying, NSCoding> {
@private
	CGFloat mRadius;
	CGFloat mStartAngle;
	CGFloat mEndAngle;
	NSPoint mCentre;
	DKArcPathType mArcType;
}

/** @brief The radius of the arc.
 */
@property (nonatomic) CGFloat radius;

/** @brief The starting angle, which is the more anti-clockwise point on the arc.
 
 Angle is passed in DEGREES.
 */
@property CGFloat startAngle;

/** @brief The ending angle, which is the more clockwise point on the arc.
 
 Angle is passed in DEGREES.
 */
@property CGFloat endAngle;

/** @brief The arc type, which affects the path geometry.
 */
@property (nonatomic) DKArcPathType arcType;

- (IBAction)convertToPath:(nullable id)sender;

@end

// partcodes this class defines - note that the implicit partcodes used by DKDrawablePath are not used by this class,
// so we don't need to ensure these are out of range. The numbers here are entirely arbitrary, but the code does assume
// they are consecutive, continuous, and ordered thus:

enum {
	kDKArcPathRadiusPart = 2,
	kDKArcPathStartAnglePart,
	kDKArcPathEndAnglePart,
	kDKArcPathRotationKnobPart,
	kDKArcPathCentrePointPart,
};

// the simple creation mode can be set (rather than, say, kDKPathCreateModeArcSegment) to create arcs in a one-step process
// which simply drags out the radius of an arc 45 degrees centred on the horizontal axis. The arc is editable in
// exactly the same way afterwards so there is no functionality lost doing it this way. It might be found to be easier to use
// than the 2-stage arc creation process.

enum {
	kDKArcSimpleCreationMode = 7
};

NS_ASSUME_NONNULL_END
