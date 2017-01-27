//
//  ASCollectionViewLayoutInspector.h
//  AsyncDisplayKit
//
//  Created by Garrett Moon on 11/19/16.
//  Copyright © 2016 Facebook. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <AsyncDisplayKit/ASDimension.h>
#import <AsyncDisplayKit/ASScrollDirection.h>

@class ASCollectionView;
@protocol ASCollectionDataSource;
@protocol ASCollectionDelegate;

NS_ASSUME_NONNULL_BEGIN

extern ASSizeRange NodeConstrainedSizeForScrollDirection(ASCollectionView *collectionView);

@protocol ASCollectionViewLayoutInspecting <NSObject>

/**
 * Asks the inspector to provide a constrained size range for the given collection view node.
 */
- (ASSizeRange)collectionView:(ASCollectionView *)collectionView constrainedSizeForNodeAtIndexPath:(NSIndexPath *)indexPath;

/**
 * Return the directions in which your collection view can scroll
 */
- (ASScrollDirection)scrollableDirections;

@optional

/**
 * Asks the inspector to provide a constrained size range for the given supplementary node.
 */
- (ASSizeRange)collectionView:(ASCollectionView *)collectionView constrainedSizeForSupplementaryNodeOfKind:(NSString *)kind atIndexPath:(NSIndexPath *)indexPath;

/**
 * Asks the inspector for the number of supplementary views for the given kind in the specified section.
 */
- (NSUInteger)collectionView:(ASCollectionView *)collectionView supplementaryNodesOfKind:(NSString *)kind inSection:(NSUInteger)section;

/**
 * Allow the inspector to respond to delegate changes.
 *
 * @discussion A great time to update perform selector caches!
 */
- (void)didChangeCollectionViewDelegate:(nullable id<ASCollectionDelegate>)delegate;

/**
 * Allow the inspector to respond to dataSource changes.
 *
 * @discussion A great time to update perform selector caches!
 */
- (void)didChangeCollectionViewDataSource:(nullable id<ASCollectionDataSource>)dataSource;

#pragma mark Deprecated Methods

/**
 * Asks the inspector for the number of supplementary sections in the collection view for the given kind.
 *
 * @deprecated This method will not be called, and it is only deprecated as a reminder to remove it.
 * Supplementary elements must exist in the same sections as regular collection view items i.e. -numberOfSectionsInCollectionView:
 */
- (NSUInteger)collectionView:(ASCollectionView *)collectionView numberOfSectionsForSupplementaryNodeOfKind:(NSString *)kind ASDISPLAYNODE_DEPRECATED_MSG("Use ASCollectionNode's method instead.");

@end

/**
 * A layout inspector for non-flow layouts that returns a constrained size to let the cells layout itself as
 * far as possible based on the scrollable direction of the collection view. It throws exceptions for delegate
 * methods that are related to supplementary node's management.
 */
@interface ASCollectionViewLayoutInspector : NSObject <ASCollectionViewLayoutInspecting>

- (instancetype)init NS_UNAVAILABLE;
- (instancetype)initWithCollectionView:(ASCollectionView *)collectionView NS_DESIGNATED_INITIALIZER;

@end

NS_ASSUME_NONNULL_END
