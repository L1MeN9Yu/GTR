//
// Created by Mengyu Li on 2019/10/30.
// Copyright (c) 2019 limengyu.top. All rights reserved.
//

import Foundation

public typealias TaskOptions = Option.Race

public extension Option {
    struct Race {
        let isDebug: Bool

        let timeout: UInt32

        let maxRedirects: Int

        public init(isDebug: Bool, timeout: UInt32, maxRedirects: Int = -1) {
            self.isDebug = isDebug
            self.timeout = timeout
            self.maxRedirects = maxRedirects
        }
    }
}