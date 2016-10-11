//
//  ASCollectionView.h
//  AsyncDisplayKit
//
//  Copyright (c) 2014-present, Facebook, Inc.  All rights reserved.
//  This source code is licensed under the BSD-style license found in the
//  LICENSE file in the root directory of this source tree. An additional grant
//  of patent rights can be found in the PATENTS file in the same directory.
//

#import <UIKit/UIKit.h>

#import <AsyncDisplayKit/ASRangeController.h>
#import <AsyncDisplayKit/ASCollectionViewProtocols.h>
#import <AsyncDisplayKit/ASBaseDefines.h>
#import <AsyncDisplayKit/ASBatchContext.h>

@class ASCellNode;
@class ASCollectionNode;
@protocol ASCollectionDataSource;
@protocol ASCollectionDelegate;
@protocol ASCollectionViewLayoutInspecting;
@protocol ASSectionContext;

NS_ASSUME_NONNULL_BEGIN

/**
 * Asynchronous UICollectionView with Intelligent Preloading capabilities.
 *
 * @discussion ASCollectionView is a true subclass of UICollectionView, meaning it is pointer-compatible
 * with code that currently uses UICollectionView.
 *
 * The main difference is that asyncDataSource expects -nodeForItemAtIndexPath, an ASCellNode, and
 * the sizeForItemAtIndexPath: method is eliminated (as are the performance problems caused by it).
 * This is made possible because ASCellNodes can calculate their own size, and preload ahead of time.
 *
 * @note ASCollectionNode is strongly recommended over ASCollectionView.  This class exists for adoption convenience.
 */
@interface ASCollectionView : UICollectionView

/**
 * The object that acts as the asynchronous delegate of the collection view
 *
 * @discussion The delegate must adopt the ASCollectionDelegate protocol. The collection view maintains a weak reference to the delegate object.
 *
 * The delegate object is responsible for providing size constraints for nodes and indicating whether batch fetching should begin.
 */
@property (nonatomic, weak) id<ASCollectionDelegate>   asyncDelegate;

/**
 * The object that acts as the asynchronous data source of the collection view
 *
 * @discussion The datasource must adopt the ASCollectionDataSource protocol. The collection view maintains a weak reference to the datasource object.
 *
 * The datasource object is responsible for providing nodes or node creation blocks to the collection view.
 */
@property (nonatomic, weak) id<ASCollectionDataSource> asyncDataSource;

/**
 * Returns the corresponding ASCollectionNode
 *
 * @return collectionNode The corresponding ASCollectionNode, if one exists.
 */
@property (nonatomic, weak, readonly) ASCollectionNode *collectionNode;

/**
 * The number of screens left to scroll before the delegate -collectionView:beginBatchFetchingWithContext: is called.
 *
 * Defaults to two screenfuls.
 */
@property (nonatomic, assign) CGFloat leadingScreensForBatching;

/**
 * Optional introspection object for the collection view's layout.
 *
 * @discussion Since supplementary and decoration views are controlled by the collection view's layout, this object
 * is used as a bridge to provide information to the internal data controller about the existence of these views and
 * their associated index paths. For collection views using `UICollectionViewFlowLayout`, a default inspector
 * implementation `ASCollectionViewFlowLayoutInspector` is created and set on this property by default. Custom
 * collection view layout subclasses will need to provide their own implementation of an inspector object for their
 * supplementary views to be compatible with `ASCollectionView`'s supplementary node support.
 */
@property (nonatomic, weak) id<ASCollectionViewLayoutInspecting> layoutInspector;

/**
 * Retrieves the node for the item at the given index path.
 *
 * @param indexPath The index path of the requested node.
 * @return The node at the given index path, or @c nil if no item exists at the specified path.
 */
- (nullable ASCellNode *)nodeForItemAtIndexPath:(NSIndexPath *)indexPath AS_WARN_UNUSED_RESULT;

/**
 * Similar to -supplementaryViewForElementKind:atIndexPath:
 *
 * @param elementKind The kind of supplementary node to locate.
 * @param indexPath The index path of the requested supplementary node.
 *
 * @return The specified supplementary node or nil
 */
- (nullable ASCellNode *)supplementaryNodeForElementKind:(NSString *)elementKind atIndexPath:(NSIndexPath *)indexPath AS_WARN_UNUSED_RESULT;

/**
 * Similar to -indexPathForCell:.
 *
 * @param cellNode a cellNode in the collection view
 *
 * @return The index path for this cell node.
 *
 * @discussion This method will return @c nil for a node that is still being
 *   displayed in the collection view, if the data source has deleted the item.
 *   That is, the node is visible but it no longer corresponds
 *   to any item in the data source and will be removed soon.
 */
- (nullable NSIndexPath *)indexPathForNode:(ASCellNode *)cellNode AS_WARN_UNUSED_RESULT;

/**
 * Determines collection view's current scroll direction. Supports 2-axis collection views.
 *
 * @return a bitmask of ASScrollDirection values.
 */
@property (nonatomic, readonly) ASScrollDirection scrollDirection;

/**
 * Determines collection view's scrollable directions.
 *
 * @return a bitmask of ASScrollDirection values.
 */
@property (nonatomic, readonly) ASScrollDirection scrollableDirections;

/**
 * Forces the .contentInset to be UIEdgeInsetsZero.
 *
 * @discussion By default, UIKit sets the top inset to the navigation bar height, even for horizontally
 * scrolling views.  This can only be disabled by setting a property on the containing UIViewController,
 * automaticallyAdjustsScrollViewInsets, which may not be accessible.  ASPagerNode uses this to ensure
 * its flow layout behaves predictably and does not log undefined layout warnings.
 */
@property (nonatomic) BOOL zeroContentInsets;

@end

@interface ASCollectionView (Deprecated)

/**
 * Initializes an ASCollectionView
 *
 * @discussion Initializes and returns a newly allocated collection view object with the specified layout.
 *
 * @param layout The layout object to use for organizing items. The collection view stores a strong reference to the specified object. Must not be nil.
 */
- (instancetype)initWithCollectionViewLayout:(UICollectionViewLayout *)layout ASDISPLAYNODE_DEPRECATED;

/**
 * Initializes an ASCollectionView
 *
 * @discussion Initializes and returns a newly allocated collection view object with the specified frame and layout.
 *
 * @param frame The frame rectangle for the collection view, measured in points. The origin of the frame is relative to the superview in which you plan to add it. This frame is passed to the superclass during initialization.
 * @param layout The layout object to use for organizing items. The collection view stores a strong reference to the specified object. Must not be nil.
 */
- (instancetype)initWithFrame:(CGRect)frame collectionViewLayout:(UICollectionViewLayout *)layout ASDISPLAYNODE_DEPRECATED;

/**
 * Tuning parameters for a range type in full mode.
 *
 * @param rangeType The range type to get the tuning parameters for.
 *
 * @return A tuning parameter value for the given range type in full mode.
 *
 * @see ASLayoutRangeMode
 * @see ASLayoutRangeType
 */
- (ASRangeTuningParameters)tuningParametersForRangeType:(ASLayoutRangeType)rangeType AS_WARN_UNUSED_RESULT ASDISPLAYNODE_DEPRECATED;

/**
 * Set the tuning parameters for a range type in full mode.
 *
 * @param tuningParameters The tuning parameters to store for a range type.
 * @param rangeType The range type to set the tuning parameters for.
 *
 * @see ASLayoutRangeMode
 * @see ASLayoutRangeType
 */
- (void)setTuningParameters:(ASRangeTuningParameters)tuningParameters forRangeType:(ASLayoutRangeType)rangeType ASDISPLAYNODE_DEPRECATED;

/**
 * Tuning parameters for a range type in the specified mode.
 *
 * @param rangeMode The range mode to get the running parameters for.
 * @param rangeType The range type to get the tuning parameters for.
 *
 * @return A tuning parameter value for the given range type in the given mode.
 *
 * @see ASLayoutRangeMode
 * @see ASLayoutRangeType
 */
- (ASRangeTuningParameters)tuningParametersForRangeMode:(ASLayoutRangeMode)rangeMode rangeType:(ASLayoutRangeType)rangeType AS_WARN_UNUSED_RESULT ASDISPLAYNODE_DEPRECATED;

/**
 * Set the tuning parameters for a range type in the specified mode.
 *
 * @param tuningParameters The tuning parameters to store for a range type.
 * @param rangeMode The range mode to set the running parameters for.
 * @param rangeType The range type to set the tuning parameters for.
 *
 * @see ASLayoutRangeMode
 * @see ASLayoutRangeType
 */
- (void)setTuningParameters:(ASRangeTuningParameters)tuningParameters forRangeMode:(ASLayoutRangeMode)rangeMode rangeType:(ASLayoutRangeType)rangeType ASDISPLAYNODE_DEPRECATED;

/**
 *  Perform a batch of updates asynchronously, optionally disabling all animations in the batch. This method must be called from the main thread.
 *  The asyncDataSource must be updated to reflect the changes before the update block completes.
 *
 *  @param animated   NO to disable animations for this batch
 *  @param updates    The block that performs the relevant insert, delete, reload, or move operations.
 *  @param completion A completion handler block to execute when all of the operations are finished. This block takes a single
 *                    Boolean parameter that contains the value YES if all of the related animations completed successfully or
 *                    NO if they were interrupted. This parameter may be nil. If supplied, the block is run on the main thread.
 */
- (void)performBatchAnimated:(BOOL)animated updates:(nullable __attribute((noescape)) void (^)())updates completion:(nullable void (^)(BOOL finished))completion ASDISPLAYNODE_DEPRECATED;

/**
 *  Perform a batch of updates asynchronously.  This method must be called from the main thread.
 *  The asyncDataSource must be updated to reflect the changes before update block completes.
 *
 *  @param updates    The block that performs the relevant insert, delete, reload, or move operations.
 *  @param completion A completion handler block to execute when all of the operations are finished. This block takes a single
 *                    Boolean parameter that contains the value YES if all of the related animations completed successfully or
 *                    NO if they were interrupted. This parameter may be nil. If supplied, the block is run on the main thread.
 */
- (void)performBatchUpdates:(nullable __attribute((noescape)) void (^)())updates completion:(nullable void (^)(BOOL finished))completion ASDISPLAYNODE_DEPRECATED;

/**
 * Reload everything from scratch, destroying the working range and all cached nodes.
 *
 * @param completion block to run on completion of asynchronous loading or nil. If supplied, the block is run on
 * the main thread.
 * @warning This method is substantially more expensive than UICollectionView's version.
 */
- (void)reloadDataWithCompletion:(nullable void (^)())completion ASDISPLAYNODE_DEPRECATED;

/**
 * Reload everything from scratch, destroying the working range and all cached nodes.
 *
 * @warning This method is substantially more expensive than UICollectionView's version.
 */
- (void)reloadData ASDISPLAYNODE_DEPRECATED;

/**
 * Reload everything from scratch entirely on the main thread, destroying the working range and all cached nodes.
 *
 * @warning This method is substantially more expensive than UICollectionView's version and will block the main thread
 * while all the cells load.
 */
- (void)reloadDataImmediately ASDISPLAYNODE_DEPRECATED;

/**
 * Triggers a relayout of all nodes.
 *
 * @discussion This method invalidates and lays out every cell node in the collection.
 */
- (void)relayoutItems ASDISPLAYNODE_DEPRECATED;

/**
 *  Blocks execution of the main thread until all section and row updates are committed. This method must be called from the main thread.
 */
- (void)waitUntilAllUpdatesAreCommitted ASDISPLAYNODE_DEPRECATED;

/**
 * Registers the given kind of supplementary node for use in creating node-backed supplementary views.
 *
 * @param elementKind The kind of supplementary node that will be requested through the data source.
 *
 * @discussion Use this method to register support for the use of supplementary nodes in place of the default
 * `registerClass:forSupplementaryViewOfKind:withReuseIdentifier:` and `registerNib:forSupplementaryViewOfKind:withReuseIdentifier:`
 * methods. This method will register an internal backing view that will host the contents of the supplementary nodes
 * returned from the data source.
 */
- (void)registerSupplementaryNodeOfKind:(NSString *)elementKind ASDISPLAYNODE_DEPRECATED;

/**
 * Inserts one or more sections.
 *
 * @param sections An index set that specifies the sections to insert.
 *
 * @discussion This method must be called from the main thread. The asyncDataSource must be updated to reflect the changes
 * before this method is called.
 */
- (void)insertSections:(NSIndexSet *)sections ASDISPLAYNODE_DEPRECATED;

/**
 * Deletes one or more sections.
 *
 * @param sections An index set that specifies the sections to delete.
 *
 * @discussion This method must be called from the main thread. The asyncDataSource must be updated to reflect the changes
 * before this method is called.
 */
- (void)deleteSections:(NSIndexSet *)sections ASDISPLAYNODE_DEPRECATED;

/**
 * Reloads the specified sections.
 *
 * @param sections An index set that specifies the sections to reload.
 *
 * @discussion This method must be called from the main thread. The asyncDataSource must be updated to reflect the changes
 * before this method is called.
 */
- (void)reloadSections:(NSIndexSet *)sections ASDISPLAYNODE_DEPRECATED;

/**
 * Moves a section to a new location.
 *
 * @param section The index of the section to move.
 *
 * @param newSection The index that is the destination of the move for the section.
 *
 * @discussion This method must be called from the main thread. The asyncDataSource must be updated to reflect the changes
 * before this method is called.
 */
- (void)moveSection:(NSInteger)section toSection:(NSInteger)newSection ASDISPLAYNODE_DEPRECATED;

/**
 * Inserts items at the locations identified by an array of index paths.
 *
 * @param indexPaths An array of NSIndexPath objects, each representing an item index and section index that together identify an item.
 *
 * @discussion This method must be called from the main thread. The asyncDataSource must be updated to reflect the changes
 * before this method is called.
 */
- (void)insertItemsAtIndexPaths:(NSArray<NSIndexPath *> *)indexPaths ASDISPLAYNODE_DEPRECATED;

/**
 * Deletes the items specified by an array of index paths.
 *
 * @param indexPaths An array of NSIndexPath objects identifying the items to delete.
 *
 * @discussion This method must be called from the main thread. The asyncDataSource must be updated to reflect the changes
 * before this method is called.
 */
- (void)deleteItemsAtIndexPaths:(NSArray<NSIndexPath *> *)indexPaths ASDISPLAYNODE_DEPRECATED;

/**
 * Reloads the specified items.
 *
 * @param indexPaths An array of NSIndexPath objects identifying the items to reload.
 *
 * @discussion This method must be called from the main thread. The asyncDataSource must be updated to reflect the changes
 * before this method is called.
 */
- (void)reloadItemsAtIndexPaths:(NSArray<NSIndexPath *> *)indexPaths ASDISPLAYNODE_DEPRECATED;

/**
 * Moves the item at a specified location to a destination location.
 *
 * @param indexPath The index path identifying the item to move.
 *
 * @param newIndexPath The index path that is the destination of the move for the item.
 *
 * @discussion This method must be called from the main thread. The asyncDataSource must be updated to reflect the changes
 * before this method is called.
 */
- (void)moveItemAtIndexPath:(NSIndexPath *)indexPath toIndexPath:(NSIndexPath *)newIndexPath ASDISPLAYNODE_DEPRECATED;

/**
 * Query the sized node at @c indexPath for its calculatedSize.
 *
 * @param indexPath The index path for the node of interest.
 *
 * This method is deprecated. Call @c calculatedSize on the node of interest instead. First deprecated in version 2.0.
 */
- (CGSize)calculatedSizeForNodeAtIndexPath:(NSIndexPath *)indexPath ASDISPLAYNODE_DEPRECATED;

/**
 * Similar to -visibleCells.
 *
 * @return an array containing the nodes being displayed on screen.
 */
- (NSArray<__kindof ASCellNode *> *)visibleNodes AS_WARN_UNUSED_RESULT ASDISPLAYNODE_DEPRECATED;

@end


/**
 * This is a node-based UICollectionViewDataSource.
 */
#define ASCollectionViewDataSource ASCollectionDataSource
@protocol ASCollectionDataSource <ASCommonCollectionViewDataSource>

@optional

/**
 * Similar to -collectionView:cellForItemAtIndexPath:.
 *
 * @param collectionView The sender.
 *
 * @param indexPath The index path of the requested node.
 *
 * @return a node for display at this indexpath. This will be called on the main thread and should
 *   not implement reuse (it will be called once per row).  Unlike UICollectionView's version,
 *   this method is not called when the row is about to display.
 */
- (ASCellNode *)collectionView:(ASCollectionView *)collectionView nodeForItemAtIndexPath:(NSIndexPath *)indexPath;

/**
 * Similar to -collectionView:nodeForItemAtIndexPath:
 * This method takes precedence over collectionView:nodeForItemAtIndexPath: if implemented.
 *
 * @param collectionView The sender.
 *
 * @param indexPath The index path of the requested node.
 *
 * @return a block that creates the node for display at this indexpath.
 *   Must be thread-safe (can be called on the main thread or a background
 *   queue) and should not implement reuse (it will be called once per row).
 */
- (ASCellNodeBlock)collectionView:(ASCollectionView *)collectionView nodeBlockForItemAtIndexPath:(NSIndexPath *)indexPath;

/**
 * Asks the collection view to provide a supplementary node to display in the collection view.
 *
 * @param collectionView An object representing the collection view requesting this information.
 * @param kind           The kind of supplementary node to provide.
 * @param indexPath      The index path that specifies the location of the new supplementary node.
 */
- (ASCellNode *)collectionView:(ASCollectionView *)collectionView nodeForSupplementaryElementOfKind:(NSString *)kind atIndexPath:(NSIndexPath *)indexPath;

/**
 * TODO: Docs
 */
- (nullable id<ASSectionContext>)collectionView:(ASCollectionView *)collectionView contextForSection:(NSInteger)section;

/**
 * Indicator to lock the data source for data fetching in async mode.
 * We should not update the data source until the data source has been unlocked. Otherwise, it will incur data inconsistency or exception
 * due to the data access in async mode.
 *
 * @param collectionView The sender.
 * @deprecated The data source is always accessed on the main thread, and this method will not be called.
 */
- (void)collectionViewLockDataSource:(ASCollectionView *)collectionView ASDISPLAYNODE_DEPRECATED;

/**
 * Indicator to unlock the data source for data fetching in async mode.
 * We should not update the data source until the data source has been unlocked. Otherwise, it will incur data inconsistency or exception
 * due to the data access in async mode.
 *
 * @param collectionView The sender.
 * @deprecated The data source is always accessed on the main thread, and this method will not be called.
 */
- (void)collectionViewUnlockDataSource:(ASCollectionView *)collectionView ASDISPLAYNODE_DEPRECATED;

@end

ASDISPLAYNODE_DEPRECATED
@protocol ASCollectionViewDelegate <ASCollectionDelegate>
@end

/**
 * This is a node-based UICollectionViewDelegate.
 */
@protocol ASCollectionDelegate <ASCommonCollectionViewDelegate, NSObject>

@optional

/**
 * Provides the constrained size range for measuring the node at the index path.
 *
 * @param collectionView The sender.
 *
 * @param indexPath The index path of the node.
 *
 * @return A constrained size range for layout the node at this index path.
 */
- (ASSizeRange)collectionView:(ASCollectionView *)collectionView constrainedSizeForNodeAtIndexPath:(NSIndexPath *)indexPath;

/**
 * Informs the delegate that the collection view will add the given node
 * at the given index path to the view hierarchy.
 *
 * @param collectionView The sender.
 * @param node The node that will be displayed.
 * @param indexPath The index path of the item that will be displayed.
 *
 * @warning AsyncDisplayKit processes collection view edits asynchronously. The index path
 *   passed into this method may not correspond to the same item in your data source
 *   if your data source has been updated since the last edit was processed.
 */
- (void)collectionView:(ASCollectionView *)collectionView willDisplayNode:(ASCellNode *)node forItemAtIndexPath:(NSIndexPath *)indexPath;

/**
 * Informs the delegate that the collection view did remove the provided node from the view hierarchy.
 * This may be caused by the node scrolling out of view, or by deleting the item
 * or its containing section with @c deleteItemsAtIndexPaths: or @c deleteSections: .
 * 
 * @param collectionView The sender.
 * @param node The node which was removed from the view hierarchy.
 * @param indexPath The index path at which the node was located before it was removed.
 *
 * @warning AsyncDisplayKit processes collection view edits asynchronously. The index path
 *   passed into this method may not correspond to the same item in your data source
 *   if your data source has been updated since the last edit was processed.
 */
- (void)collectionView:(ASCollectionView *)collectionView didEndDisplayingNode:(ASCellNode *)node forItemAtIndexPath:(NSIndexPath *)indexPath;

/**
 * Receive a message that the collectionView is near the end of its data set and more data should be fetched if 
 * necessary.
 *
 * @param collectionView The sender.
 * @param context A context object that must be notified when the batch fetch is completed.
 *
 * @discussion You must eventually call -completeBatchFetching: with an argument of YES in order to receive future
 * notifications to do batch fetches. This method is called on a background queue.
 *
 * UICollectionView currently only supports batch events for tail loads. If you require a head load, consider
 * implementing a UIRefreshControl.
 */
- (void)collectionView:(ASCollectionView *)collectionView willBeginBatchFetchWithContext:(ASBatchContext *)context;

/**
 * Tell the collectionView if batch fetching should begin.
 *
 * @param collectionView The sender.
 *
 * @discussion Use this method to conditionally fetch batches. Example use cases are: limiting the total number of
 * objects that can be fetched or no network connection.
 *
 * If not implemented, the collectionView assumes that it should notify its asyncDelegate when batch fetching
 * should occur.
 */
- (BOOL)shouldBatchFetchForCollectionView:(ASCollectionView *)collectionView;

/**
 * Informs the delegate that the collection view will add the node
 * at the given index path to the view hierarchy.
 *
 * @param collectionView The sender.
 * @param indexPath The index path of the item that will be displayed.
 *
 * @warning AsyncDisplayKit processes collection view edits asynchronously. The index path
 *   passed into this method may not correspond to the same item in your data source
 *   if your data source has been updated since the last edit was processed.
 *
 * This method is deprecated. Use @c collectionView:willDisplayNode:forItemAtIndexPath: instead.
 */
- (void)collectionView:(ASCollectionView *)collectionView willDisplayNodeForItemAtIndexPath:(NSIndexPath *)indexPath ASDISPLAYNODE_DEPRECATED;

@end

/**
 * Defines methods that let you coordinate with a `UICollectionViewFlowLayout` in combination with an `ASCollectionView`.
 */
@protocol ASCollectionDelegateFlowLayout <ASCollectionDelegate>

@optional

/**
 * @discussion This method is deprecated and does nothing from 1.9.7 and up
 * Previously it applies the section inset to every cells within the corresponding section.
 * The expected behavior is to apply the section inset to the whole section rather than
 * shrinking each cell individually.
 * If you want this behavior, you can integrate your insets calculation into
 * `constrainedSizeForNodeAtIndexPath`
 * please file a github issue if you would like this to be restored.
 */
- (UIEdgeInsets)collectionView:(ASCollectionView *)collectionView layout:(UICollectionViewLayout *)collectionViewLayout insetForSectionAtIndex:(NSInteger)section __deprecated_msg("This method does nothing for 1.9.7+ due to incorrect implementation previously, see the header file for more information.");

/**
 * Asks the delegate for the size of the header in the specified section.
 */
- (CGSize)collectionView:(ASCollectionView *)collectionView layout:(UICollectionViewLayout *)collectionViewLayout referenceSizeForHeaderInSection:(NSInteger)section;

/**
 * Asks the delegate for the size of the footer in the specified section.
 */
- (CGSize)collectionView:(ASCollectionView *)collectionView layout:(UICollectionViewLayout *)collectionViewLayout referenceSizeForFooterInSection:(NSInteger)section;

@end

NS_ASSUME_NONNULL_END
