//
//  ASRelativeSize.h
//  AsyncDisplayKit
//
//  Copyright (c) 2014-present, Facebook, Inc.  All rights reserved.
//  This source code is licensed under the BSD-style license found in the
//  LICENSE file in the root directory of this source tree. An additional grant
//  of patent rights can be found in the PATENTS file in the same directory.
//

#import <UIKit/UIKit.h>
#import <AsyncDisplayKit/ASBaseDefines.h>
#import <AsyncDisplayKit/ASDimension.h>

/** 
 Expresses a size with relative dimensions.
 Used by ASStaticLayoutSpec.
 */
typedef struct {
  ASRelativeDimension width;
  ASRelativeDimension height;
} ASRelativeSize;

ASDISPLAYNODE_EXTERN_C_BEGIN
NS_ASSUME_NONNULL_BEGIN

#pragma mark - ASRelativeSize

extern ASRelativeSize ASRelativeSizeMake(ASRelativeDimension width, ASRelativeDimension height);

/** Convenience constructor to provide size in points. */
extern ASRelativeSize ASRelativeSizeMakeWithCGSize(CGSize size);

/** Convenience constructor to provide size as a fraction. */
extern ASRelativeSize ASRelativeSizeMakeWithFraction(CGFloat fraction);

/** Resolve this relative size relative to a parent size. */
extern CGSize ASRelativeSizeResolveSize(ASRelativeSize relativeSize, CGSize parentSize, CGSize autoSize);

extern BOOL ASRelativeSizeEqualToRelativeSize(ASRelativeSize lhs, ASRelativeSize rhs);

extern NSString *NSStringFromASRelativeSize(ASRelativeSize size);

NS_ASSUME_NONNULL_END
ASDISPLAYNODE_EXTERN_C_END
