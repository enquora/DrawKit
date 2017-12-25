/**
 @author Contributions from the community; see CONTRIBUTORS.md
 @date 2005-2016
 @copyright MPL2; see LICENSE.txt
*/

#import <Cocoa/Cocoa.h>
#import "GCObservableObject.h"

@class DKColorStop;

//! gradient type:
typedef NS_ENUM(NSInteger, DKGradientType) {
	kDKGradientTypeLinear = 0,
	kDKGradientTypeRadial = 1,
	kDKGradientSweptAngle = 3
};

//! gradient blending mode:
typedef NS_ENUM(NSInteger, DKGradientBlending) {
	kDKGradientRGBBlending = 0,
	kDKGradientHSBBlending = 1,
	kDKGradientAlphaBlending = 64
};

typedef NS_ENUM(NSInteger, DKGradientInterpolation) {
	kDKGradientInterpLinear = 0,
	kDKGradientInterpQuadratic = 2,
	kDKGradientInterpCubic = 3,
	kDKGradientInterpSinus = 4,
	kDKGradientInterpSinus2 = 5
};

/** @brief A DKGradient encapsulates gradient/shading drawing.
*/
@interface DKGradient : GCObservableObject <NSCoding, NSCopying> {
	NSMutableArray* m_colorStops; // color stops
	id m_extensionData; // additional supplementary data
	CGFloat m_gradAngle; // linear angle in radians
	DKGradientType m_gradType; // type
	DKGradientBlending m_blending; // method to blend colours
	DKGradientInterpolation m_interp; // interpolation function
}

// simple gradient convenience methods

/** @brief Returns an instance of the default gradient (simple linear black to white)
 @return autoreleased default gradient object
 */
+ (DKGradient*)defaultGradient;

/** @brief Returns a linear gradient from Color c1 to c2

 Gradient is linear and draws left to right c1 --> c2
 @param c1 the starting Color
 @param c2 the ending Color
 @return gradient object
 */
+ (DKGradient*)gradientWithStartingColor:(NSColor*)c1 endingColor:(NSColor*)c2;

/** @brief Returns a gradient from Color c1 to c2 with given type and angle
 @param c1 the starting Color
 @param c2 the ending Color
 @param gt the gradient's type (linear or radial, etc)
 @param degrees angle in degrees
 @return gradient object
 */
+ (DKGradient*)gradientWithStartingColor:(NSColor*)c1 endingColor:(NSColor*)c2 type:(NSInteger)gt angle:(CGFloat)degrees;

// modified copies:

/** @brief Creates a copy of the gradient but colorizies it by substituting the hue from <color>
 @param color donates its hue
 @return a new gradient, a copy of the receiver in every way except colourized by <color> 
 */
- (DKGradient*)gradientByColorizingWithColor:(NSColor*)color;

/** @brief Creates a copy of the gradient but sets the alpha vealue of all stop colours to <alpha>
 @param alpha the desired alpha
 @return a new gradient, a copy of the receiver with requested alpha 
 */
- (DKGradient*)gradientWithAlpha:(CGFloat)alpha;

// setting up the Color stops

- (DKColorStop*)addColor:(NSColor*)Color at:(CGFloat)pos;
- (void)addColorStop:(DKColorStop*)stop;

/** @brief Removes the last Color from he list of Colors
 */
- (void)removeLastColor;

/** @brief Removes a Color stop from the list of Colors
 @param stop the stop to remove
 */
- (void)removeColorStop:(DKColorStop*)stop;

/** @brief Removes all Colors from the list of Colors
 */
- (void)removeAllColors;

/** @brief Sets the list of Color stops in the gradient

 A gradient needs a minimum of two Colors to be a gradient, but will function with one.
 @param stops an array of DKColorStop objects
 */
- (void)setColorStops:(NSArray<DKColorStop*>*)stops;

/** @brief Returns the list of Color stops in the gradient

 A gradient needs a minimum of two Colors to be a gradient, but will function with one.
 @return the array of DKColorStop (color + position) objects in the gradient
 */
- (NSArray<DKColorStop*>*)colorStops;

@property (copy) NSArray<DKColorStop*> *colorStops;

/** @brief Sorts the Color stops into position order

 Stops are sorted in place
 */
- (void)sortColorStops;

/** @brief Reverses the order of all the Color stops so "inverting" the gradient

 Stop positions are changed, but Colors are not touched
 */
- (void)reverseColorStops;

// KVO compliant accessors:

/** @brief Returns the number of Color stops in the gradient

 This also makes the stops array KVC compliant
 @return an integer, the number of Colors used to compute the gradient
 */
- (NSUInteger)countOfColorStops;
@property (readonly) NSUInteger countOfColorStops;

/** @brief Returns the the indexed Color stop

 This also makes the stops array KVC compliant
 @param ix index number of the stop
 @return a Color stop
 */
- (DKColorStop*)objectInColorStopsAtIndex:(NSUInteger)ix;
- (void)insertObject:(DKColorStop*)stop inColorStopsAtIndex:(NSUInteger)ix;
- (void)removeObjectFromColorStopsAtIndex:(NSUInteger)ix;

// a variety of ways to fill a path

- (void)fillRect:(NSRect)rect;

/** @brief Fills the path using the gradient

 The fill will proceed as for a standard fill. A gradient that needs a starting point will assume
 the centre of the path's bounds as that point when using this method.
 @param path the bezier path to fill. 
 */
- (void)fillPath:(NSBezierPath*)path;

/** @brief Fills the path using the gradient
 @param path the bezier path to fill
 @param co displacement from the centre for the start of a radial fill
 */
- (void)fillPath:(NSBezierPath*)path centreOffset:(NSPoint)co;
/** @brief Fills the path using the gradient between two given points
 
 Radii are ignored for linear gradients. Angle is ignored by this method, if you call it directly
 (angle is used to calculate start and endpoints in other methods that call this)
 @param path the bezier path to fill
 @param sp the point where the gradient begins
 @param sr for radial fills, the radius of the start of the gradient
 @param ep the point where the gradient ends
 @param er for radial fills, the radius of the end of the gradient
 */
- (void)fillPath:(NSBezierPath*)path startingAtPoint:(NSPoint)sp
		startRadius:(CGFloat)sr
	  endingAtPoint:(NSPoint)ep
		  endRadius:(CGFloat)er;

- (void)fillContext:(CGContextRef)context startingAtPoint:(NSPoint)sp
		startRadius:(CGFloat)sr
	  endingAtPoint:(NSPoint)ep
		  endRadius:(CGFloat)er API_DEPRECATED_WITH_REPLACEMENT("fillStartingAtPoint:startRadius:endingAtPoint:endRadius:", macosx(10.0, 10.6));

- (void)fillStartingAtPoint:(NSPoint)sp
				startRadius:(CGFloat)sr
			  endingAtPoint:(NSPoint)ep
				  endRadius:(CGFloat)er;

/** @brief Returns the computed Color for the gradient ramp expressed as a value from 0 to 1.0
 
 While intended for internal use, this function can be called at any time if you wish
 the private version here is called internally. It does fewer checks and returns raw component
 values for performance. do not use from external code.
 @param val the proportion of the gradient ramp from start (0) to finish (1.0)
 @return the Color corresponding to that position
 */
- (NSColor*)colorAtValue:(CGFloat)val;

// setting the angle

/** @brief The gradient's current angle in radians.
 */
@property (nonatomic) CGFloat angle;

/** @brief The gradient's current angle in degrees.
 */
@property CGFloat angleInDegrees;

- (void)setAngleWithoutNotifying:(CGFloat)ang;

// setting gradient type, blending and interpolation settings

/** @brief The gradient's basic type.
 
 Valid types are: \c kDKGradientTypeLinear and \c kDKGradientTypeRadial
 */
@property (nonatomic) DKGradientType gradientType;

/** @brief The blending mode of the gradient.
 */
@property (nonatomic) DKGradientBlending gradientBlending;

/** @brief The interpolation algorithm of the gradient.
 */
@property (nonatomic) DKGradientInterpolation gradientInterpolation;

// swatch images

/** @brief Returns an image of the current gradient for use in a UI, etc.
 @param size the desired image size
 @param showBorder YES to draw a border around the image, NO for no border
 @return an NSIMage containing the current gradient
 */
- (NSImage*)swatchImageWithSize:(NSSize)size withBorder:(BOOL)showBorder;
- (NSImage*)standardSwatchImage;

@end

#define DKGradientSwatchSize (NSMakeSize(20, 20))

#pragma mark -

/** @brief Small object that links a Color with its relative position.
*/
@interface DKColorStop : NSObject <NSCoding, NSCopying> {
	NSColor* mColor;
	CGFloat position;
	DKGradient* __weak m_ownerRef;
@public
	CGFloat components[4]; // cached rgba values
}

- (instancetype)init UNAVAILABLE_ATTRIBUTE;
- (instancetype)initWithColor:(NSColor*)aColor at:(CGFloat)pos NS_DESIGNATED_INITIALIZER;
- (instancetype)initWithCoder:(NSCoder*)coder NS_DESIGNATED_INITIALIZER;

/** @brief The Color associated with this stop.
 
 Colors are converted to calibrated RGB to permit shading calculations
 */
@property (nonatomic, strong) NSColor *color;

/** @brief Set the alpha of the colour associated with this stop
 @param alpha the alpha to set
 */
- (void)setAlpha:(CGFloat)alpha;

/** @brief The stop's relative position.
 
 Value is constrained between 0.0 and 1.0.
 */
@property (nonatomic) CGFloat position;

@end

// notifications sent by DKGradient:

extern NSNotificationName kDKNotificationGradientWillAddColorStop;
extern NSNotificationName kDKNotificationGradientDidAddColorStop;
extern NSNotificationName kDKNotificationGradientWillRemoveColorStop;
extern NSNotificationName kDKNotificationGradientDidRemoveColorStop;
extern NSNotificationName kDKNotificationGradientWillChange;
extern NSNotificationName kDKNotificationGradientDidChange;
