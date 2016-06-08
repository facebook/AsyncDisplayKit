/*
 *  Copyright (c) 2014-present, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree. An additional grant
 *  of patent rights can be found in the PATENTS file in the same directory.
 *
 */

#pragma once

#import <UIKit/UIKit.h>
#import <AsyncDisplayKit/ASAssert.h>
#import <AsyncDisplayKit/ASLayoutable.h>
#import <AsyncDisplayKit/ASDimension.h>

NS_ASSUME_NONNULL_BEGIN

extern CGPoint const CGPointNull;

extern BOOL CGPointIsNull(CGPoint point);

/**
 * A node in the layout tree that represents the size and position of the object that created it (ASLayoutable).
 */
@interface ASLayout : NSObject

/**
 * The underlying object described by this layout
 */
@property (nonatomic, weak, readonly) id<ASLayoutable> layoutableObject;

/**
 * Size of the current layout
 */
@property (nonatomic, readonly) CGSize size;

/**
 * Position in parent. Default to CGPointNull.
 * 
 * @discussion When being used as a sublayout, this property must not equal CGPointNull.
 */
@property (nonatomic, readwrite) CGPoint position;

/**
 * The size range that was use to determine the size of the layout.
 */
@property (nonatomic, readonly) ASSizeRange constrainedSizeRange;

/**
 * Array of ASLayouts. Each must have a valid non-null position.
 */
@property (nonatomic, readonly) NSArray<ASLayout *> *sublayouts;

/**
 * A list of sublayouts that were not already flattened.
 */
@property (nonatomic, readonly) NSArray<ASLayout *> *immediateSublayouts;

/**
 * Mark the layout dirty for future regeneration.
 */
@property (nonatomic, getter=isDirty) BOOL dirty;

/**
 * A boolean describing if the current layout has been flattened.
 */
@property (nonatomic, readonly, getter=isFlattened) BOOL flattened;

/**
 * @abstract This view is invisible, and it doesn't take any space for layout purposes.
 * Used when attached to a layout.
 */
@property (nonatomic, readonly, getter=isGone) BOOL gone;


/**
 * Initializer.
 *
 * @param layoutableObject The backing ASLayoutable object.
 *
 * @param size The size of this layout.
 *
 * @param position The position of this layout within its parent (if available).
 *
 * @param sublayouts Sublayouts belong to the new layout.
 */
+ (instancetype)layoutWithLayoutableObject:(id <ASLayoutable>)layoutableObject
                      constrainedSizeRange:(ASSizeRange)sizeRange
                                      size:(CGSize)size
                                  position:(CGPoint)position
                                sublayouts:(NSArray *)sublayouts
                                 flattened:(BOOL)flattened
                                      gone:(BOOL)gone;

/**
 * Convenience initializer that has CGPointNull position.
 * Best used by ASDisplayNode subclasses that are manually creating a layout for -calculateLayoutThatFits:,
 * or for ASLayoutSpec subclasses that are referencing the "self" level in the layout tree,
 * or for creating a sublayout of which the position is yet to be determined.
 *
 * @param layoutableObject The backing ASLayoutable object.
 *
 * @param size The size of this layout.
 *
 * @param sublayouts Sublayouts belong to the new layout.
 */
+ (instancetype)layoutWithLayoutableObject:(id<ASLayoutable>)layoutableObject
                      constrainedSizeRange:(ASSizeRange)sizeRange
                                      size:(CGSize)size
                                sublayouts:(nullable NSArray<ASLayout *> *)sublayouts;

/**
 * Convenience that has CGPointNull position and no sublayouts. 
 * Best used for creating a layout that has no sublayouts, and is either a root one
 * or a sublayout of which the position is yet to be determined.
 *
 * @param layoutableObject The backing ASLayoutable object.
 *
 * @param size The size of this layout.
 */
+ (instancetype)layoutWithLayoutableObject:(id<ASLayoutable>)layoutableObject
                      constrainedSizeRange:(ASSizeRange)sizeRange
                                      size:(CGSize)size;

/**
 * Convenience initializer that is flattened and has CGPointNull position.
 *
 * @param layoutableObject The backing ASLayoutable object.
 *
 * @param size The size of this layout.
 *
 * @param sublayouts Sublayouts belong to the new layout.
 */
+ (instancetype)flattenedLayoutWithLayoutableObject:(id<ASLayoutable>)layoutableObject
                               constrainedSizeRange:(ASSizeRange)sizeRange
                                               size:(CGSize)size
                                         sublayouts:(nullable NSArray<ASLayout *> *)sublayouts;

/**
 * @abstract Evaluates a given predicate block against each object in the receiving layout tree
 * and returns a new, 1-level deep layout containing the objects for which the predicate block returns true.
 *
 * @param predicateBlock The block is applied to a layout to be evaluated. 
 * The block takes 1 argument: evaluatedLayout - the layout to be evaluated.
 * The block returns YES if evaluatedLayout evaluates  to true, otherwise NO.
 *
 * @return A new, 1-level deep layout containing the layouts for which the predicate block returns true.
 */
- (ASLayout *)flattenedLayoutUsingPredicateBlock:(BOOL (^)(ASLayout *evaluatedLayout))predicateBlock;

/**
 * @abstract Returns a valid frame for the current layout computed with the size and position.
 * @discussion Clamps the layout's origin or position to 0 if any of the calculated values are infinite.
 */
- (CGRect)frame;

@end

NS_ASSUME_NONNULL_END
