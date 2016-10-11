//
//  ASCollectionView+Undeprecated.h
//  AsyncDisplayKit
//
//  Created by Adlai Holler on 10/10/16.
//  Copyright © 2016 Facebook. All rights reserved.
//

#import <AsyncDisplayKit/AsyncDisplayKit.h>

NS_ASSUME_NONNULL_BEGIN

/**
 * Currently our public collection API is on @c ASCollectionNode and the @c ASCollectionView
 * API is deprecated, but the implementations still live in the view.
 *
 * This category lets us avoid deprecation warnings everywhere internally.
 * In the future, the @c ASCollectionView public API will be eliminated and so will this file.
 */
@interface ASCollectionView (Undeprecated)

/**
 * Initializes an ASCollectionView
 *
 * @discussion Initializes and returns a newly allocated collection view object with the specified layout.
 *
 * @param layout The layout object to use for organizing items. The collection view stores a strong reference to the specified object. Must not be nil.
 */
- (instancetype)initWithCollectionViewLayout:(UICollectionViewLayout *)layout;

/**
 * Initializes an ASCollectionView
 *
 * @discussion Initializes and returns a newly allocated collection view object with the specified frame and layout.
 *
 * @param frame The frame rectangle for the collection view, measured in points. The origin of the frame is relative to the superview in which you plan to add it. This frame is passed to the superclass during initialization.
 * @param layout The layout object to use for organizing items. The collection view stores a strong reference to the specified object. Must not be nil.
 */
- (instancetype)initWithFrame:(CGRect)frame collectionViewLayout:(UICollectionViewLayout *)layout;

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
- (ASRangeTuningParameters)tuningParametersForRangeType:(ASLayoutRangeType)rangeType AS_WARN_UNUSED_RESULT;

/**
 * Set the tuning parameters for a range type in full mode.
 *
 * @param tuningParameters The tuning parameters to store for a range type.
 * @param rangeType The range type to set the tuning parameters for.
 *
 * @see ASLayoutRangeMode
 * @see ASLayoutRangeType
 */
- (void)setTuningParameters:(ASRangeTuningParameters)tuningParameters forRangeType:(ASLayoutRangeType)rangeType;

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
- (ASRangeTuningParameters)tuningParametersForRangeMode:(ASLayoutRangeMode)rangeMode rangeType:(ASLayoutRangeType)rangeType AS_WARN_UNUSED_RESULT;

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
- (void)setTuningParameters:(ASRangeTuningParameters)tuningParameters forRangeMode:(ASLayoutRangeMode)rangeMode rangeType:(ASLayoutRangeType)rangeType;

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
- (void)performBatchAnimated:(BOOL)animated updates:(nullable __attribute((noescape)) void (^)())updates completion:(nullable void (^)(BOOL finished))completion;

/**
 *  Perform a batch of updates asynchronously.  This method must be called from the main thread.
 *  The asyncDataSource must be updated to reflect the changes before update block completes.
 *
 *  @param updates    The block that performs the relevant insert, delete, reload, or move operations.
 *  @param completion A completion handler block to execute when all of the operations are finished. This block takes a single
 *                    Boolean parameter that contains the value YES if all of the related animations completed successfully or
 *                    NO if they were interrupted. This parameter may be nil. If supplied, the block is run on the main thread.
 */
- (void)performBatchUpdates:(nullable __attribute((noescape)) void (^)())updates completion:(nullable void (^)(BOOL finished))completion;

/**
 * Reload everything from scratch, destroying the working range and all cached nodes.
 *
 * @param completion block to run on completion of asynchronous loading or nil. If supplied, the block is run on
 * the main thread.
 * @warning This method is substantially more expensive than UICollectionView's version.
 */
- (void)reloadDataWithCompletion:(nullable void (^)())completion;

/**
 * Reload everything from scratch, destroying the working range and all cached nodes.
 *
 * @warning This method is substantially more expensive than UICollectionView's version.
 */
- (void)reloadData;

/**
 * Reload everything from scratch entirely on the main thread, destroying the working range and all cached nodes.
 *
 * @warning This method is substantially more expensive than UICollectionView's version and will block the main thread
 * while all the cells load.
 */
- (void)reloadDataImmediately;

/**
 * Triggers a relayout of all nodes.
 *
 * @discussion This method invalidates and lays out every cell node in the collection.
 */
- (void)relayoutItems;

/**
 *  Blocks execution of the main thread until all section and row updates are committed. This method must be called from the main thread.
 */
- (void)waitUntilAllUpdatesAreCommitted;

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
- (void)registerSupplementaryNodeOfKind:(NSString *)elementKind;

/**
 * Inserts one or more sections.
 *
 * @param sections An index set that specifies the sections to insert.
 *
 * @discussion This method must be called from the main thread. The asyncDataSource must be updated to reflect the changes
 * before this method is called.
 */
- (void)insertSections:(NSIndexSet *)sections;

/**
 * Deletes one or more sections.
 *
 * @param sections An index set that specifies the sections to delete.
 *
 * @discussion This method must be called from the main thread. The asyncDataSource must be updated to reflect the changes
 * before this method is called.
 */
- (void)deleteSections:(NSIndexSet *)sections;

/**
 * Reloads the specified sections.
 *
 * @param sections An index set that specifies the sections to reload.
 *
 * @discussion This method must be called from the main thread. The asyncDataSource must be updated to reflect the changes
 * before this method is called.
 */
- (void)reloadSections:(NSIndexSet *)sections;

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
- (void)moveSection:(NSInteger)section toSection:(NSInteger)newSection;

/**
 * Inserts items at the locations identified by an array of index paths.
 *
 * @param indexPaths An array of NSIndexPath objects, each representing an item index and section index that together identify an item.
 *
 * @discussion This method must be called from the main thread. The asyncDataSource must be updated to reflect the changes
 * before this method is called.
 */
- (void)insertItemsAtIndexPaths:(NSArray<NSIndexPath *> *)indexPaths;

/**
 * Deletes the items specified by an array of index paths.
 *
 * @param indexPaths An array of NSIndexPath objects identifying the items to delete.
 *
 * @discussion This method must be called from the main thread. The asyncDataSource must be updated to reflect the changes
 * before this method is called.
 */
- (void)deleteItemsAtIndexPaths:(NSArray<NSIndexPath *> *)indexPaths;

/**
 * Reloads the specified items.
 *
 * @param indexPaths An array of NSIndexPath objects identifying the items to reload.
 *
 * @discussion This method must be called from the main thread. The asyncDataSource must be updated to reflect the changes
 * before this method is called.
 */
- (void)reloadItemsAtIndexPaths:(NSArray<NSIndexPath *> *)indexPaths;

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
- (void)moveItemAtIndexPath:(NSIndexPath *)indexPath toIndexPath:(NSIndexPath *)newIndexPath;

/**
 * Similar to -visibleCells.
 *
 * @return an array containing the nodes being displayed on screen.
 */
- (NSArray<__kindof ASCellNode *> *)visibleNodes AS_WARN_UNUSED_RESULT;

@end

NS_ASSUME_NONNULL_END
