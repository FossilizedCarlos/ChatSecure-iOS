//
//  OTRBuddy.h
//  Off the Record
//
//  Created by David Chiles on 3/28/14.
//  Copyright (c) 2014 Chris Ballinger. All rights reserved.
//

#import "OTRYapDatabaseObject.h"
#import "OTRThreadOwner.h"
#import "OTRUserInfoProfile.h"
@import UIKit;

typedef NS_ENUM(NSUInteger, OTRChatState) {
    OTRChatStateUnknown   = 0,
    OTRChatStateActive    = 1,
    OTRChatStateComposing = 2,
    OTRChatStatePaused    = 3,
    OTRChatStateInactive  = 4,
    OTRChatStateGone      = 5
};

/** These are the preferences for a buddy on how to send a message. Related OTRMessageTransportSecurity*/
typedef NS_ENUM(NSUInteger, OTRSessionSecurity) {
    OTRSessionSecurityBestAvailable = 0,
    OTRSessionSecurityPlaintextOnly = 1,
    OTRSessionSecurityPlaintextWithOTR = 2,
    OTRSessionSecurityOTR = 3,
    OTRSessionSecurityOMEMO = 4,
    OTRSessionSecurityOMEMOandOTR = 5
};


@class OTRAccount, OTRMessage;


@interface OTRBuddy : OTRYapDatabaseObject <YapDatabaseRelationshipNode, OTRThreadOwner, OTRUserInfoProfile>

@property (nonatomic, strong, nonnull) NSString *username;
@property (nonatomic, strong, readwrite, nonnull) NSString *displayName;
@property (nonatomic, strong, nullable) NSString *composingMessageString;

// Dynamic properties backed by in-memory cache
// You don't have to save the object when setting these
// When setting these properties use OTRBuddyCache methods
@property (atomic, strong, nullable, readonly) NSString *statusMessage;
@property (atomic, readonly) OTRChatState chatState;
@property (atomic, readonly) OTRChatState lastSentChatState;
@property (atomic, readonly) OTRThreadStatus status;

/** uniqueId of last incoming or outgoing OTRMessage */
@property (nonatomic, strong, nullable) NSString *lastMessageId;

/** User can choose a preferred security method e.g. plaintext, OTR, OMEMO. If undefined, best available option should be chosen elsewhere. OMEMO > OTR > Plaintext */
@property (nonatomic, readwrite) OTRSessionSecurity preferredSecurity;

/**
 * Setting this value does a comparison of against the previously value
 * to invalidate the OTRImages cache.
 */
@property (nonatomic, strong, nullable) NSData *avatarData;

@property (nonatomic, strong, nonnull) NSString *accountUniqueId;

- (nullable id <OTRMessageProtocol>)lastMessageWithTransaction:(nonnull YapDatabaseReadTransaction *)transaction;
- (nullable OTRAccount*)accountWithTransaction:(nonnull YapDatabaseReadTransaction *)transaction;

- (void)bestTransportSecurityWithTransaction:(nonnull YapDatabaseReadTransaction *)transaction
                             completionBlock:(void (^_Nonnull)(OTRMessageTransportSecurity))block
                             completionQueue:(nonnull dispatch_queue_t)queue;


+ (nullable instancetype)fetchBuddyForUsername:(nonnull NSString *)username
                          accountName:(nonnull NSString *)accountName
                          transaction:(nonnull YapDatabaseReadTransaction *)transaction;

+ (nullable instancetype)fetchBuddyWithUsername:(nonnull NSString *)username
                   withAccountUniqueId:(nonnull NSString *)accountUniqueId
                           transaction:(nonnull YapDatabaseReadTransaction *)transaction;

/** Excluded properties for Mantle */
+ (nonnull NSSet<NSString*>*) excludedProperties;

@end
