//
//  ASDisplayNodeLayoutTests.m
//  AsyncDisplayKit
//
//  Copyright (c) 2014-present, Facebook, Inc.  All rights reserved.
//  This source code is licensed under the BSD-style license found in the
//  LICENSE file in the root directory of this source tree. An additional grant
//  of patent rights can be found in the PATENTS file in the same directory.
//

#import "ASXCTExtensions.h"
#import <AsyncDisplayKit/AsyncDisplayKit.h>
#import "ASLayoutSpecSnapshotTestsHelper.h"
#import "ASDisplayNode+FrameworkPrivate.h"

@interface ASDisplayNodeLayoutTests : XCTestCase
@end

@implementation ASDisplayNodeLayoutTests

- (void)testMeasureOnLayoutIfNotHappenedBefore
{
  CGSize nodeSize = CGSizeMake(100, 100);
  
  ASStaticSizeDisplayNode *displayNode = [ASStaticSizeDisplayNode new];
  displayNode.staticSize  = nodeSize;
  
  // Use a button node in here as ASButtonNode uses layoutSpecThatFits:
  ASButtonNode *buttonNode = [ASButtonNode new];
  [displayNode addSubnode:buttonNode];
  
  displayNode.frame = {.size = nodeSize};
  buttonNode.frame = {.size = nodeSize};
  
  ASXCTAssertEqualSizes(displayNode.calculatedSize, CGSizeZero, @"Calculated size before measurement and layout should be 0");
  ASXCTAssertEqualSizes(buttonNode.calculatedSize, CGSizeZero, @"Calculated size before measurement and layout should be 0");
  
  // Trigger view creation and layout pass without a manual measure: call before so the automatic measurement
  // pass will trigger in the layout pass
  [displayNode.view layoutIfNeeded];
  
  ASXCTAssertEqualSizes(displayNode.calculatedSize, nodeSize, @"Automatic measurement pass should have happened in layout pass");
  ASXCTAssertEqualSizes(buttonNode.calculatedSize, nodeSize, @"Automatic measurement pass should have happened in layout pass");
}

- (void)testMeasureOnLayoutIfNotHappenedBeforeForRangeManagedNodes
{
  CGSize nodeSize = CGSizeMake(100, 100);
  
  ASStaticSizeDisplayNode *displayNode = [ASStaticSizeDisplayNode new];
  displayNode.staticSize  = nodeSize;
  
  ASButtonNode *buttonNode = [ASButtonNode new];
  [displayNode addSubnode:buttonNode];
  
  [displayNode enterHierarchyState:ASHierarchyStateRangeManaged];
  
  displayNode.frame = {.size = nodeSize};
  buttonNode.frame = {.size = nodeSize};
  
  ASXCTAssertEqualSizes(displayNode.calculatedSize, CGSizeZero, @"Calculated size before measurement and layout should be 0");
  ASXCTAssertEqualSizes(buttonNode.calculatedSize, CGSizeZero, @"Calculated size before measurement and layout should be 0");
  
  // Trigger layout pass without a maeasurment pass before
  [displayNode.view layoutIfNeeded];
  
  ASXCTAssertEqualSizes(displayNode.calculatedSize, nodeSize, @"Automatic measurement pass should have happened in layout pass");
  ASXCTAssertEqualSizes(buttonNode.calculatedSize, nodeSize, @"Automatic measurement pass should have happened in layout pass");
}

#if DEBUG
- (void)testNotAllowAddingSubnodesInLayoutSpecThatFits
{
  ASDisplayNode *displayNode = [ASDisplayNode new];
  ASDisplayNode *someOtherNode = [ASDisplayNode new];
  
  displayNode.layoutSpecBlock = ^(ASDisplayNode * _Nonnull node, ASSizeRange constrainedSize) {
    [node addSubnode:someOtherNode];
    return [ASInsetLayoutSpec insetLayoutSpecWithInsets:UIEdgeInsetsZero child:someOtherNode];
  };
  
  XCTAssertThrows([displayNode measure:CGSizeMake(100, 100)], @"Should throw if subnode was added in layoutSpecThatFits:");
}

- (void)testNotAllowModifyingSubnodesInLayoutSpecThatFits
{
  ASDisplayNode *displayNode = [ASDisplayNode new];
  ASDisplayNode *someOtherNode = [ASDisplayNode new];
  
  [displayNode addSubnode:someOtherNode];
  
  displayNode.layoutSpecBlock = ^(ASDisplayNode * _Nonnull node, ASSizeRange constrainedSize) {
    [someOtherNode removeFromSupernode];
    [node addSubnode:[ASDisplayNode new]];
    return [ASInsetLayoutSpec insetLayoutSpecWithInsets:UIEdgeInsetsZero child:someOtherNode];
  };
  
  XCTAssertThrows([displayNode measure:CGSizeMake(100, 100)], @"Should throw if subnodes where modified in layoutSpecThatFits:");
}
#endif

@end
