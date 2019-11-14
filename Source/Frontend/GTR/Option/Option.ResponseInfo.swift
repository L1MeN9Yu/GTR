//
// Created by Mengyu Li on 2019/11/14.
// Copyright (c) 2019 limengyu.top. All rights reserved.
//

import Foundation

public extension Option {
    struct ResponseInfo {
        let base: Bool
        let time: Bool
        let size: Bool
        let speed: Bool
        let ssl: Bool
        let socket: Bool
        let cookie: Bool

        public init(base: Bool, time: Bool, size: Bool, speed: Bool, ssl: Bool, socket: Bool, cookie: Bool) {
            self.base = base
            self.time = time
            self.size = size
            self.speed = speed
            self.ssl = ssl
            self.socket = socket
            self.cookie = cookie
        }
    }
}