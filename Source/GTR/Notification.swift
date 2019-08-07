//
// Created by Mengyu Li on 2018-11-06.
// Copyright (c) 2018 limengyu.top. All rights reserved.
//

import Foundation

public struct Notification {
    private static let requestStart = Foundation.Notification.Name("top.limengyu.GTR.Request.Start")
    private static let requestComplete = Foundation.Notification.Name("top.limengyu.GTR.Request.Complete")
    private static let requestCanceled = Foundation.Notification.Name("top.limengyu.GTR.Request.Canceled")

    public static let userInfoMethodKey = Method.userInfoKey
    public static let userInfoTaskIDKey = "GTR.TaskID"

    public enum RequestStatusType {
        case start, complete, canceled

        public var notificationName: Foundation.Notification.Name {
            switch self {
            case .start:
                return requestStart
            case .complete:
                return requestComplete
            case .canceled:
                return requestCanceled
            }
        }
    }
}
