//
//  ASImageNodeSnapshotTests.m
//  AsyncDisplayKit
//
//  Copyright (c) 2014-present, Facebook, Inc.  All rights reserved.
//  This source code is licensed under the BSD-style license found in the
//  LICENSE file in the root directory of this source tree. An additional grant
//  of patent rights can be found in the PATENTS file in the same directory.
//

#import "ASSnapshotTestCase.h"

#import <AsyncDisplayKit/AsyncDisplayKit.h>

@interface ASImageNodeSnapshotTests : ASSnapshotTestCase
@end

@implementation ASImageNodeSnapshotTests

- (UIImage *)testImage
{
  NSString *path = [[NSBundle bundleForClass:[self class]] pathForResource:@"logo-square"
                                                                    ofType:@"png"
                                                               inDirectory:@"TestResources"];
  return [UIImage imageWithContentsOfFile:path];
}

- (void)testRenderLogoSquare
{
  // trivial test case to ensure ASSnapshotTestCase works
  ASImageNode *imageNode = [[ASImageNode alloc] init];
  imageNode.image = [self testImage];
  ASLayout *l = [imageNode layoutThatFits:ASSizeRangeMake(CGSizeZero, CGSizeMake(100, 100))];
  imageNode.frame = (CGRect){.size = l.size};
  [imageNode layoutIfNeeded];

  ASSnapshotVerifyNode(imageNode, nil);
}

- (void)testForcedScaling
{
  CGSize forcedImageSize = CGSizeMake(100, 100);
  
  ASImageNode *imageNode = [[ASImageNode alloc] init];
  imageNode.forcedSize = forcedImageSize;
  imageNode.image = [self testImage];
  
  // Snapshot testing requires that node is formally laid out.
  imageNode.style.width = ASDimensionMake(forcedImageSize.width);
  imageNode.style.height = ASDimensionMake(forcedImageSize.height);
  ASLayout *l = [imageNode layoutThatFits:ASSizeRangeMake(CGSizeZero, forcedImageSize)];
  imageNode.frame = (CGRect){.size = l.size};
  [imageNode layoutIfNeeded];
  ASSnapshotVerifyNode(imageNode, @"first");
  
  imageNode.style.width = ASDimensionMake(200);
  imageNode.style.height = ASDimensionMake(200);
  l = [imageNode layoutThatFits:ASSizeRangeMake(CGSizeZero, CGSizeMake(200, 200))];
  imageNode.frame = (CGRect){.size = l.size};
  [imageNode layoutIfNeeded];
  
  ASSnapshotVerifyNode(imageNode, @"second");
  
  XCTAssert(CGImageGetWidth((CGImageRef)imageNode.contents) == forcedImageSize.width * imageNode.contentsScale &&
            CGImageGetHeight((CGImageRef)imageNode.contents) == forcedImageSize.height * imageNode.contentsScale,
            @"Contents should be 100 x 100 by contents scale.");
}

- (void)testTintColorBlock
{
  UIImage *test = [self testImage];
  UIImage *tinted = ASImageNodeTintColorModificationBlock([UIColor redColor])(test);
  ASImageNode *node = [[ASImageNode alloc] init];
  node.image = tinted;
  ASLayout *l = [node layoutThatFits:ASSizeRangeMake(test.size)];
  node.frame = (CGRect){.size = l.size};
  [node layoutIfNeeded];
  
  ASSnapshotVerifyNode(node, nil);
}

- (void)testRoundedCornerBlock
{
  UIGraphicsBeginImageContext(CGSizeMake(100, 100));
  [[UIColor blueColor] setFill];
  UIRectFill(CGRectMake(0, 0, 100, 100));
  UIImage *result = UIGraphicsGetImageFromCurrentImageContext();
  UIGraphicsEndImageContext();
  UIImage *rounded = ASImageNodeRoundBorderModificationBlock(2, [UIColor redColor])(result);
  ASImageNode *node = [[ASImageNode alloc] init];
  node.image = rounded;
  ASLayout *l = [node layoutThatFits:ASSizeRangeMake(rounded.size)];
  node.frame = (CGRect){.size = l.size};
  [node layoutIfNeeded];
  
  ASSnapshotVerifyNode(node, nil);
}

@end
