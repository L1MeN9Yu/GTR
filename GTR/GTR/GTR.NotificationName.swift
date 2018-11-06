//
// Created by Mengyu Li on 2018-11-06.
// Copyright (c) 2018 limengyu.top. All rights reserved.
//

import Foundation

extension GTR {
    public struct Notification {
        public static let requestStart = Foundation.Notification.Name("top.limengyu.GTR.Request.Start")
        public static let requestComplete = Foundation.Notification.Name("top.limengyu.GTR.Request.Complete")
        public static let requestCanceled = Foundation.Notification.Name("top.limengyu.GTR.Request.Canceled")

        public static let userInfoMethodKey = GTR.Method.userInfoKey
        public static let userInfoTaskIDKey = "GTR.TaskID"
    }
}