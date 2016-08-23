//
//  ASViewController.mm
//  AsyncDisplayKit
//
//  Created by Huy Nguyen on 16/09/15.
//
//  Copyright (c) 2014-present, Facebook, Inc.  All rights reserved.
//  This source code is licensed under the BSD-style license found in the
//  LICENSE file in the root directory of this source tree. An additional grant
//  of patent rights can be found in the PATENTS file in the same directory.
//

#import "ASViewController.h"
#import "ASAssert.h"
#import "ASAvailability.h"
#import "ASDisplayNodeInternal.h"
#import "ASDisplayNode+FrameworkPrivate.h"
#import "ASTraitCollection.h"
#import "ASEnvironmentInternal.h"
#import "ASRangeControllerUpdateRangeProtocol+Beta.h"

#define AS_LOG_VISIBILITY_CHANGES 0

@implementation ASViewController
{
  BOOL _ensureDisplayed;
  BOOL _automaticallyAdjustRangeModeBasedOnViewEvents;
  BOOL _parentManagesVisibilityDepth;
  NSInteger _visibilityDepth;
  BOOL _selfConformsToRangeModeProtocol;
  BOOL _nodeConformsToRangeModeProtocol;
  BOOL _didCheckRangeModeProtocolConformance;
}

- (instancetype)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
  ASDisplayNodeAssert(NO, @"ASViewController requires using -initWithNode:");
  return [self initWithNode:[[ASDisplayNode alloc] init]];
}

- (instancetype)initWithCoder:(NSCoder *)aDecoder
{
  ASDisplayNodeAssert(NO, @"ASViewController requires using -initWithNode:");
  return [self initWithNode:[[ASDisplayNode alloc] init]];
}

- (instancetype)initWithNode:(ASDisplayNode *)node
{
  if (!(self = [super initWithNibName:nil bundle:nil])) {
    return nil;
  }
  
  ASDisplayNodeAssertNotNil(node, @"Node must not be nil");
  ASDisplayNodeAssertTrue(!node.layerBacked);
  _node = node;

  _automaticallyAdjustRangeModeBasedOnViewEvents = NO;

  return self;
}

- (void)loadView
{
  ASDisplayNodeAssertTrue(!_node.layerBacked);
  
  // Apple applies a frame and autoresizing masks we need.  Allocating a view is not
  // nearly as expensive as adding and removing it from a hierarchy, and fortunately
  // we can avoid that here.  Enabling layerBacking on a single node in the hierarchy
  // will have a greater performance benefit than the impact of this transient view.
  [super loadView];
  UIView *view = self.view;
  CGRect frame = view.frame;
  UIViewAutoresizing autoresizingMask = view.autoresizingMask;
  
  // We have what we need, so now create and assign the view we actually want.
  view = _node.view;
  _node.frame = frame;
  _node.autoresizingMask = autoresizingMask;
  self.view = view;
  
  // ensure that self.node has a valid trait collection before a subclass's implementation of viewDidLoad.
  // Any subnodes added in viewDidLoad will then inherit the proper environment.
  if (AS_AT_LEAST_IOS8) {
    ASEnvironmentTraitCollection traitCollection = [self environmentTraitCollectionForUITraitCollection:self.traitCollection];
    [self progagateNewEnvironmentTraitCollection:traitCollection];
  } else {
    ASEnvironmentTraitCollection traitCollection = ASEnvironmentTraitCollectionMakeDefault();
    traitCollection.containerSize = self.view.bounds.size;
    [self progagateNewEnvironmentTraitCollection:traitCollection];
  }
}

- (void)viewWillLayoutSubviews
{
  [super viewWillLayoutSubviews];
  [_node measureWithSizeRange:[self nodeConstrainedSize]];
  
  if (!AS_AT_LEAST_IOS9) {
    [self _legacyHandleViewDidLayoutSubviews];
  }
}

- (void)viewDidLayoutSubviews
{
  if (_ensureDisplayed && self.neverShowPlaceholders) {
    _ensureDisplayed = NO;
    [self.node recursivelyEnsureDisplaySynchronously:YES];
  }
  [super viewDidLayoutSubviews];
}

ASVisibilityDidMoveToParentViewController;

- (void)viewWillAppear:(BOOL)animated
{
  [super viewWillAppear:animated];
  _ensureDisplayed = YES;
  [_node measureWithSizeRange:[self nodeConstrainedSize]];
  [_node recursivelyFetchData];
  
  if (_parentManagesVisibilityDepth == NO) {
    [self setVisibilityDepth:0];
  }
}

ASVisibilitySetVisibilityDepth;

ASVisibilityViewDidDisappearImplementation;

ASVisibilityDepthImplementation;

- (void)visibilityDepthDidChange
{
  ASLayoutRangeMode rangeMode = ASLayoutRangeModeForVisibilityDepth(self.visibilityDepth);
#if AS_LOG_VISIBILITY_CHANGES
  NSString *rangeModeString;
  switch (rangeMode) {
    case ASLayoutRangeModeMinimum:
      rangeModeString = @"Minimum";
      break;
      
    case ASLayoutRangeModeFull:
      rangeModeString = @"Full";
      break;
      
    case ASLayoutRangeModeVisibleOnly:
      rangeModeString = @"Visible Only";
      break;
      
    case ASLayoutRangeModeLowMemory:
      rangeModeString = @"Low Memory";
      break;
      
    default:
      break;
  }
  NSLog(@"Updating visibility of:%@ to: %@ (visibility depth: %d)", self, rangeModeString, self.visibilityDepth);
#endif
  [self updateCurrentRangeModeWithModeIfPossible:rangeMode];
}

#pragma mark - Automatic range mode

- (BOOL)automaticallyAdjustRangeModeBasedOnViewEvents
{
  return _automaticallyAdjustRangeModeBasedOnViewEvents;
}

- (void)setAutomaticallyAdjustRangeModeBasedOnViewEvents:(BOOL)automaticallyAdjustRangeModeBasedOnViewEvents
{
  _automaticallyAdjustRangeModeBasedOnViewEvents = automaticallyAdjustRangeModeBasedOnViewEvents;
}

- (void)updateCurrentRangeModeWithModeIfPossible:(ASLayoutRangeMode)rangeMode
{
  if (!_automaticallyAdjustRangeModeBasedOnViewEvents) { return; }
  
  if (!_didCheckRangeModeProtocolConformance) {
    _selfConformsToRangeModeProtocol = [self conformsToProtocol:@protocol(ASRangeControllerUpdateRangeProtocol)];
    _nodeConformsToRangeModeProtocol = [_node conformsToProtocol:@protocol(ASRangeControllerUpdateRangeProtocol)];
    _didCheckRangeModeProtocolConformance = YES;
    if (!_selfConformsToRangeModeProtocol && !_nodeConformsToRangeModeProtocol) {
      NSLog(@"Warning: automaticallyAdjustRangeModeBasedOnViewEvents set to YES in %@, but range mode updating is not possible because neither view controller nor node %@ conform to ASRangeControllerUpdateRangeProtocol.", self, _node);
    }
  }
  
  if (_selfConformsToRangeModeProtocol) {
    id<ASRangeControllerUpdateRangeProtocol> rangeUpdater = (id<ASRangeControllerUpdateRangeProtocol>)self;
    [rangeUpdater updateCurrentRangeWithMode:rangeMode];
  }
  
  if (_nodeConformsToRangeModeProtocol) {
    id<ASRangeControllerUpdateRangeProtocol> rangeUpdater = (id<ASRangeControllerUpdateRangeProtocol>)_node;
    [rangeUpdater updateCurrentRangeWithMode:rangeMode];
  }
}

#pragma mark - Layout Helpers

- (ASSizeRange)nodeConstrainedSize
{
  ASDisplayNodeAssertMainThread();
  
  CGSize viewSize;
  if (AS_AT_LEAST_IOS9) {
    viewSize = self.view.bounds.size;
  } else {
    viewSize = [self _legacyConstrainedSize];
  }
  
  //handle UIViewController insets
  if ((self.edgesForExtendedLayout & UIRectEdgeTop) != UIRectEdgeTop) {
    //Sadly, it appears as if topLayoutGuide.length is zero when edgesForExtendedLayout doesn't include
    //the top, so we need to cheat and see what our origin was set to.
    viewSize.height -= self.view.frame.origin.y;
  }
  
  return ASSizeRangeMake(viewSize, viewSize);
}

- (ASInterfaceState)interfaceState
{
  return _node.interfaceState;
}

#pragma mark - Legacy Layout Handling

- (BOOL)_shouldLayoutTheLegacyWay
{
  BOOL isModalViewController = (self.presentingViewController != nil && self.presentedViewController == nil);
  BOOL hasNavigationController = (self.navigationController != nil);
  BOOL hasParentViewController = (self.parentViewController != nil);
  if (isModalViewController && !hasNavigationController && !hasParentViewController) {
    return YES;
  }
  
  // Check if the view controller is a root view controller
  BOOL isRootViewController = self.view.window.rootViewController == self;
  if (isRootViewController) {
    return YES;
  }
  
  return NO;
}

- (CGSize)_legacyConstrainedSize
{
  // In modal presentation the view does not have the right bounds in iOS7 and iOS8. As workaround using the superviews
  // view bounds
  UIView *view = self.view;
  CGSize viewSize = view.bounds.size;
  if ([self _shouldLayoutTheLegacyWay]) {
    UIView *superview = view.superview;
    if (superview != nil) {
      viewSize = superview.bounds.size;
    }
  }
  return viewSize;
}

- (void)_legacyHandleViewDidLayoutSubviews
{
  // In modal presentation or as root viw controller the view does not automatic resize in iOS7 and iOS8.
  // As workaround we adjust the frame of the view manually
  if ([self _shouldLayoutTheLegacyWay]) {
    CGSize maxConstrainedSize = [self nodeConstrainedSize].max;
    _node.frame = (CGRect){.origin = CGPointZero, .size = maxConstrainedSize};
  }
}

#pragma mark - ASEnvironmentTraitCollection

- (ASEnvironmentTraitCollection)environmentTraitCollectionForUITraitCollection:(UITraitCollection *)traitCollection
{
  if (self.overrideDisplayTraitsWithTraitCollection) {
    ASTraitCollection *asyncTraitCollection = self.overrideDisplayTraitsWithTraitCollection(traitCollection);
    return [asyncTraitCollection environmentTraitCollection];
  }
  
  ASDisplayNodeAssertMainThread();
  ASEnvironmentTraitCollection asyncTraitCollection = ASEnvironmentTraitCollectionFromUITraitCollection(traitCollection);
  asyncTraitCollection.containerSize = self.view.frame.size;
  return asyncTraitCollection;
}

- (ASEnvironmentTraitCollection)environmentTraitCollectionForWindowSize:(CGSize)windowSize
{
  if (self.overrideDisplayTraitsWithWindowSize) {
    ASTraitCollection *traitCollection = self.overrideDisplayTraitsWithWindowSize(windowSize);
    return [traitCollection environmentTraitCollection];
  }
  ASEnvironmentTraitCollection traitCollection = self.node.environmentTraitCollection;
  traitCollection.containerSize = windowSize;
  return traitCollection;
}

- (void)progagateNewEnvironmentTraitCollection:(ASEnvironmentTraitCollection)environmentTraitCollection
{
  ASEnvironmentState environmentState = self.node.environmentState;
  ASEnvironmentTraitCollection oldEnvironmentTraitCollection = environmentState.environmentTraitCollection;
  
  if (ASEnvironmentTraitCollectionIsEqualToASEnvironmentTraitCollection(environmentTraitCollection, oldEnvironmentTraitCollection) == NO) {
    environmentState.environmentTraitCollection = environmentTraitCollection;
    self.node.environmentState = environmentState;
    [self.node setNeedsLayout];
    
    NSArray<id<ASEnvironment>> *children = [self.node children];
    for (id<ASEnvironment> child in children) {
      ASEnvironmentStatePropagateDown(child, environmentState.environmentTraitCollection);
    }
  }
}

- (void)traitCollectionDidChange:(UITraitCollection *)previousTraitCollection
{
  [super traitCollectionDidChange:previousTraitCollection];
  
  ASEnvironmentTraitCollection environmentTraitCollection = [self environmentTraitCollectionForUITraitCollection:self.traitCollection];
  environmentTraitCollection.containerSize = self.view.bounds.size;
  [self progagateNewEnvironmentTraitCollection:environmentTraitCollection];
}

- (void)willTransitionToTraitCollection:(UITraitCollection *)newCollection withTransitionCoordinator:(id<UIViewControllerTransitionCoordinator>)coordinator
{
  [super willTransitionToTraitCollection:newCollection withTransitionCoordinator:coordinator];
  
  ASEnvironmentTraitCollection environmentTraitCollection = ASEnvironmentTraitCollectionFromUITraitCollection(newCollection);
  // A call to willTransitionToTraitCollection:withTransitionCoordinator: is always followed by a call to viewWillTransitionToSize:withTransitionCoordinator:.
  // However, it is not immediate and we may need the new trait collection before viewWillTransitionToSize:withTransitionCoordinator: is called. Our view already has the
  // new bounds, so go ahead and set the containerSize and propagate the traits here. As long as nothing else in the traits changes (which it shouldn't)
  // then we won't we will skip the propagation in viewWillTransitionToSize:withTransitionCoordinator:.
  environmentTraitCollection.containerSize = self.view.bounds.size;
  [self progagateNewEnvironmentTraitCollection:environmentTraitCollection];
}

- (void)viewWillTransitionToSize:(CGSize)size withTransitionCoordinator:(id<UIViewControllerTransitionCoordinator>)coordinator
{
  [super viewWillTransitionToSize:size withTransitionCoordinator:coordinator];
  
  ASEnvironmentTraitCollection environmentTraitCollection = [self environmentTraitCollectionForWindowSize:size];
  [self progagateNewEnvironmentTraitCollection:environmentTraitCollection];
}

- (void)didRotateFromInterfaceOrientation:(UIInterfaceOrientation)fromInterfaceOrientation
{
  [super didRotateFromInterfaceOrientation:fromInterfaceOrientation];
  
  ASEnvironmentTraitCollection traitCollection = self.node.environmentTraitCollection;
  traitCollection.containerSize = self.view.bounds.size;
  [self progagateNewEnvironmentTraitCollection:traitCollection];
}

@end
